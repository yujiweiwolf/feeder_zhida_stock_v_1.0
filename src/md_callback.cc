// Copyright 2020 Fancapital Inc.  All rights reserved.
#include <memory>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>
#include "./config.h"
#include "../src/md_callback.h"
#include "instrument_mgr.h"
namespace fs = std::filesystem;

#define MINPRICE 0.000001
#define MAXPIRCE 999999999999.99

namespace co {
    MDCallBack::MDCallBack(string market) : CDAMarketDataSpi(), market_(market) {
        req_id_ = 0;
        is_login_ = false;
        trading_day_ = x::RawDate();
    }

    MDCallBack::~MDCallBack() {
        if (md_api_) {
            if (is_login_) {
                is_login_ = false;
                x::Sleep(1000);
            }

            md_api_->Release();
            md_api_ = NULL;
        }
    }

    void MDCallBack::Init() {
        string file = market_ + "_Market.log";
        md_api_ =  CDAMarketDataApi::CreateMarketApi(true, file.c_str());
        if (md_api_) {
            LOG_INFO << "API Version: " << md_api_->GetVersion();
            md_api_->RegisterSpi(this);
            md_api_->RegisterNameServer(Singleton<Config>::GetInstance()->serveraddress().c_str());
            md_api_->SetHeartBeatTimeout(Singleton<Config>::GetInstance()->heartbeat());
            md_api_->Init();
        }
    }

    bool MDCallBack::IsLogin() {
        return is_login_;
    }

    void MDCallBack::OnFrontConnected() {
        LOG_INFO << "MdApi OnFrontConnected";
        CMarketReqUserLoginField field;
        memset(&field, 0, sizeof(field));
        string userid = Singleton<Config>::GetInstance()->userid();
        strncpy(field.UserId, userid.c_str(), userid.length());

        string password = Singleton<Config>::GetInstance()->password();
        strncpy(field.UserPwd, password.c_str(), password.length());

        string authorcode = Singleton<Config>::GetInstance()->authorcode();
        strncpy(field.AuthorCode, authorcode.c_str(), authorcode.length());

        string macdddress = Singleton<Config>::GetInstance()->macdddress();
        strncpy(field.MacAddress, macdddress.c_str(), macdddress.length());

        string computername = Singleton<Config>::GetInstance()->computername();
        strncpy(field.ComputerName, computername.c_str(), computername.length());

        string softwareversion = Singleton<Config>::GetInstance()->softwareversion();
        strncpy(field.SoftwareName, softwareversion.c_str(), softwareversion.length());

        string softwarename = Singleton<Config>::GetInstance()->softwarename();
        strncpy(field.SoftwareVersion, softwarename.c_str(), softwarename.length());

        string pricebrokerid = Singleton<Config>::GetInstance()->pricebrokerid();
        strncpy(field.BrokerIDForMarketPrice, pricebrokerid.c_str(), pricebrokerid.length());

        bool ret = md_api_->ReqUserLogin(&field, req_id_++);
        if (!ret) {
            LOG_ERROR << __FUNCTION__ << " " << x::GBKToUTF8(field.ErrorDescription);
        }
    }

    void MDCallBack::SubscribeMarket() {
        std::set<string> csv_code;
        string file = Singleton<Config>::GetInstance()->code_file();
        if (!std::filesystem::exists(file)) {
            LOG_ERROR << file << "not exit ";
            return;
        }
        std::fstream infile;
        infile.open(file, std::ios::in);
        if (!infile.is_open()) {
            LOG_ERROR << "open file " << file << " failed! ";
            return;
        }
        std::string s;
        while (std::getline(infile, s)) {
            if (s.find("HK") != s.npos) {
                csv_code.insert(x::Trim(s));
            }
        }
        LOG_INFO <<  "csv code size: " << csv_code.size();
        std::set<string>& tmp_code = Singleton<co::InstrumentMgr>::GetInstance()->GetInstrument();
        std::vector<string> all_code;
        for (auto& it : tmp_code) {
            auto item = csv_code;
            all_code.push_back(it);
        }
        for (auto& it : csv_code) {
            if (auto item = tmp_code.find(it); item == tmp_code.end()) {
                LOG_INFO << "csv have, but remote server not have, code: " << it;
                all_code.push_back(it);
            }
        }
        int total_index = 0;
        int total_num = ceil(all_code.size() * 1.0 / MAX_SUB_COUNT);
        for (int i = 0; i < total_num; ++i) {
            CMarketReqMarketDataField field;
            memset(&field, 0, sizeof(field));
            field.SubscMode = DAF_SUB_Append;
            field.MarketType = DAF_TYPE_Stock;
            strncpy(field.ExchangeCode, "HKEX", strlen("HKEX"));
            int now_index = 0;
            for (int j = 0; j < MAX_SUB_COUNT; ++j) {
                string code = all_code[total_index];
                LOG_INFO << "Sub code: " << code << ", index: " << total_index;
                strncpy(field.MarketTrcode[now_index], code.c_str(), code.length());
                now_index++;
                total_index++;
                if (total_index >= static_cast<int>(all_code.size())) {
                    break;
                }
            }
            field.MarketCount = now_index;
            int ret = md_api_->ReqMarketData(&field, ++req_id_);
            if (ret) {
                LOG_ERROR << "Sub data faild, ret: " << ret << ", " << x::GBKToUTF8(field.ErrorDescription);
            } else {
                LOG_INFO << "Sub data succeed";
            }
            x::Sleep(1000);
        }

//        CMarketReqMarketDataField field;
//        memset(&field, 0, sizeof(field));
//        field.SubscMode = DAF_SUB_Append;
//        field.MarketType = DAF_TYPE_Stock;
//        strcpy(field.ExchangeCode, "HKEX");
//
//        field.MarketCount = 1;
//        strcpy(field.MarketTrcode[0], "09988.HK");
//        strcpy(field.MarketTrcode[1], "00006.HK");
//        strcpy(field.MarketTrcode[2], "00001.HK");
//        strcpy(field.MarketTrcode[3], "00002.HK");
//        strcpy(field.MarketTrcode[4], "00003.HK");
//
//        bool ret = md_api_->ReqMarketData(&field, ++req_id_);
//        if (!ret) {
//            LOG_ERROR << __FUNCTION__ << " " << x::GBKToUTF8(field.ErrorDescription);
//        }
    }

    void MDCallBack::OnFrontDisconnected(int nReason) {
        is_login_ = false;
        LOG_ERROR << "OnFrontDisconnected nReason = " << nReason;
    }

    void MDCallBack::OnHeartBeatWarning(int iTimeLapse) {
        LOG_INFO << " OnHeartBeatWarning " << iTimeLapse;
    }

    void MDCallBack::OnRspUserLogin(CMarketRspInfoField* pRspInfo, int iRequestID, bool bIsLast) {
        if (pRspInfo->ErrorID == 0) {
            is_login_ = true;
            LOG_INFO << "OnRspUserLogin Succeed";
            SubscribeMarket();
        } else {
            LOG_ERROR << "OnRspUserLogin ErrorID = " << pRspInfo->ErrorID << ", ErrorMsg = " << pRspInfo->ErrorMsg;
        }
    }

    void MDCallBack::OnRtnUserLogout(CMarketRspInfoField* pRspInfo, int iRequestID, bool bIsLast) {
        if (pRspInfo->ErrorID == 0) {
            is_login_ = false;
            LOG_INFO << "OnRspUserLogout";
        } else {
            LOG_ERROR << "OnRspUserLogout ErrorID = " << pRspInfo->ErrorID << ", ErrorMsg = " << pRspInfo->ErrorMsg;
        }
    }

    void MDCallBack::OnRtnBrokerData(CMarketRtnBrokerDataField *pRtnBrokerData, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) {
        if (pRspInfo->ErrorID) {
            LOG_ERROR << "ErrorID: " << pRspInfo->ErrorID << ", ErrorMsg = " << pRspInfo->ErrorMsg;
        } else {
            LOG_INFO << "BrokerData: " << pRtnBrokerData->BrokerData;
        }
    }

    void MDCallBack::OnRspMarketData(CMarketRspMarketDataField* pReqMarketDataField, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) {
        if (pRspInfo->ErrorID) {
            LOG_ERROR << "ErrorID: " << pRspInfo->ErrorID << ", ErrorMsg = " << pRspInfo->ErrorMsg;
        } else {
            LOG_INFO << "订阅请求应答成功.";
        }
    }

    void MDCallBack::OnRspError(CMarketRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        if (pRspInfo->ErrorID) {
            LOG_ERROR << "ErrorID: " << pRspInfo->ErrorID << ", ErrorMsg = " << pRspInfo->ErrorMsg;
        }
    }

    void MDCallBack::OnRtnTradeDate(CMarketRtnTradeDateField *pRtnTradeDate, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) {
        if (pRspInfo->ErrorID) {
            LOG_ERROR << "ErrorID: " << pRspInfo->ErrorID << ", ErrorMsg = " << pRspInfo->ErrorMsg;
        } else {
            LOG_INFO << "TradeDate: " <<  pRtnTradeDate->TradeDate << ", TradeProduct: " << pRtnTradeDate->TradeProduct;
        }
    }

    void MDCallBack::OnRtnMarketData(CMarketRtnMarketDataField *pRtnMarketData) {
//        if (strcmp(pRtnMarketData->TreatyCode, "09988.HK") == 0) {
//            LOG_INFO << pRtnMarketData->TreatyCode
//                     << ", " << pRtnMarketData->ExchangeCode
//                     << ", Open: " << pRtnMarketData->Open
//                     << ", CurrPrice: " << pRtnMarketData->CurrPrice
//                     << ", CurrNumber: " << pRtnMarketData->CurrNumber
//                     << ", FilledNum: " << pRtnMarketData->FilledNum
//                     << ", BuyPrice: " << pRtnMarketData->BuyPrice
//                     << ", BuyNumber: " << pRtnMarketData->BuyNumber
//                     << ", SalePrice: " << pRtnMarketData->SalePrice
//                     << ", SaleNumber: " << pRtnMarketData->SaleNumber
//                     << ", IntradaySettlePrice: " << pRtnMarketData->IntradaySettlePrice
//                     << ", TradeDay: " << pRtnMarketData->TradeDay
//                     << ", Time: " << pRtnMarketData->Time;
//        }

        string code = x::Trim(pRtnMarketData->TreatyCode);
        QContextPtr ctx = QServer::Instance()->GetContext(code);
        if (!ctx) {
            ctx = QServer::Instance()->NewContext(code, code);
        }
        co::fbs::QTickT& m = ctx->PrepareQTick();
        if (m.dtype <= 0) {
            m.dtype = kDTypeStock;
            m.src = kSrcQTickLevel2;
            m.market = kMarketHK;
        }

        string str_time;
        for (int Index = 0; Index < static_cast<int>(strlen(pRtnMarketData->Time)); ++Index) {
            if (pRtnMarketData->Time[Index] >= '0' && pRtnMarketData->Time[Index] <= '9') {
                str_time += pRtnMarketData->Time[Index];
            }
        }
        int64_t timestamp = atoll(str_time.c_str()) * 1000LL;
        int64_t code_date = timestamp / 1000000000LL;
        if (code_date < trading_day_) {
            LOG_INFO << "not valid timestamp, " << pRtnMarketData->TreatyCode
                     << ", " << pRtnMarketData->ExchangeCode
                     << ", FilledNum: " << pRtnMarketData->FilledNum
                     << ", Time: " << pRtnMarketData->Time;
            return;
        }
        int64_t sum_volume = atoll(pRtnMarketData->FilledNum);
        int64_t pre_sum_volume = m.sum_volume;
        double pre_sum_amount = m.sum_amount;
        int64_t new_volume = sum_volume - pre_sum_volume;
        if (new_volume < 0) {
            if (strcmp(pRtnMarketData->TreatyCode, "09988.HK") == 0) {
                LOG_INFO << code << ", " << pRtnMarketData->Time << ", sum_volume: " << sum_volume
                         << ", change small, pre_sum_volume: " << pre_sum_volume;
            }
            return;
        }

        double new_price = atof(pRtnMarketData->CurrPrice);
        m.timestamp = timestamp;
        m.new_price = new_price;
        m.sum_volume = sum_volume;
        m.new_volume = new_volume;
        m.new_amount = new_price * new_volume;
        m.sum_amount = pre_sum_amount + m.new_amount;

        double temp_price = atof(pRtnMarketData->PreSettlementPrice);
        if (temp_price < MAXPIRCE) {
            m.pre_close = temp_price;
        }

        temp_price = 0;
        temp_price = atof(pRtnMarketData->Open);
        if (temp_price < MAXPIRCE) {
            m.open = temp_price;
        }

        temp_price = 0;
        temp_price = atof(pRtnMarketData->High);
        if (temp_price < MAXPIRCE) {
            m.high = temp_price;
        }

        temp_price = 0;
        temp_price = atof(pRtnMarketData->Low);
        if (temp_price < MAXPIRCE) {
            m.low = temp_price;
        }

        temp_price = 0;
        temp_price = atof(pRtnMarketData->Close);
        if (temp_price < MAXPIRCE) {
            m.close = temp_price;
        }

        temp_price = 0;
        temp_price = atof(pRtnMarketData->LimitUpPrice);
        if (temp_price < MAXPIRCE) {
            m.upper_limit = temp_price;
        }

        temp_price = 0;
        temp_price = atof(pRtnMarketData->LimitDownPrice);
        if (temp_price < MAXPIRCE) {
            m.lower_limit = temp_price;
        }

        double price = 0.0;
        int64_t volume = 0;
        price = atof(pRtnMarketData->BuyPrice);
        volume = atoll(pRtnMarketData->BuyNumber);
        if (price > MINPRICE && volume > 0) {
            m.bp.push_back(price);
            m.bv.push_back(volume);
            price = atof(pRtnMarketData->BuyPrice2);
            volume = atoll(pRtnMarketData->BuyNumber2);
            if (price > MINPRICE && volume > 0) {
                m.bp.push_back(price);
                m.bv.push_back(volume);
                price = atof(pRtnMarketData->BuyPrice3);
                volume = atoll(pRtnMarketData->BuyNumber3);
                if (price > MINPRICE && volume > 0) {
                    m.bp.push_back(price);
                    m.bv.push_back(volume);
                    price = atof(pRtnMarketData->BuyPrice4);
                    volume = atoll(pRtnMarketData->BuyNumber4);
                    if (price > MINPRICE && volume > 0) {
                        m.bp.push_back(price);
                        m.bv.push_back(volume);
                        price = atof(pRtnMarketData->BuyPrice5);
                        volume = atoll(pRtnMarketData->BuyNumber5);
                        if (price > MINPRICE && volume > 0) {
                            m.bp.push_back(price);
                            m.bv.push_back(volume);
                            price = atof(pRtnMarketData->BuyPrice6);
                            volume = atoll(pRtnMarketData->BuyNumber6);
                            if (price > MINPRICE && volume > 0) {
                                m.bp.push_back(price);
                                m.bv.push_back(volume);
                                price = atof(pRtnMarketData->BuyPrice7);
                                volume = atoll(pRtnMarketData->BuyNumber7);
                                if (price > MINPRICE && volume > 0) {
                                    m.bp.push_back(price);
                                    m.bv.push_back(volume);
                                    price = atof(pRtnMarketData->BuyPrice8);
                                    volume = atoll(pRtnMarketData->BuyNumber8);
                                    if (price > MINPRICE && volume > 0) {
                                        m.bp.push_back(price);
                                        m.bv.push_back(volume);
                                        price = atof(pRtnMarketData->BuyPrice9);
                                        volume = atoll(pRtnMarketData->BuyNumber9);
                                        if (price > MINPRICE && volume > 0) {
                                            m.bp.push_back(price);
                                            m.bv.push_back(volume);
                                            price = atof(pRtnMarketData->BuyPrice10);
                                            volume = atoll(pRtnMarketData->BuyNumber10);
                                            if (price > MINPRICE && volume > 0) {
                                                m.bp.push_back(price);
                                                m.bv.push_back(volume);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        price = atof(pRtnMarketData->SalePrice);
        volume = atoll(pRtnMarketData->SaleNumber);
        if (price > MINPRICE && volume > 0) {
            m.ap.push_back(price);
            m.av.push_back(volume);
            price = atof(pRtnMarketData->SalePrice2);
            volume = atoll(pRtnMarketData->SaleNumber2);
            if (price > MINPRICE && volume > 0) {
                m.ap.push_back(price);
                m.av.push_back(volume);
                price = atof(pRtnMarketData->SalePrice3);
                volume = atoll(pRtnMarketData->SaleNumber3);
                if (price > MINPRICE && volume > 0) {
                    m.ap.push_back(price);
                    m.av.push_back(volume);
                    price = atof(pRtnMarketData->SalePrice4);
                    volume = atoll(pRtnMarketData->SaleNumber4);
                    if (price > MINPRICE && volume > 0) {
                        m.ap.push_back(price);
                        m.av.push_back(volume);
                        price = atof(pRtnMarketData->SalePrice5);
                        volume = atoll(pRtnMarketData->SaleNumber5);
                        if (price > MINPRICE && volume > 0) {
                            m.ap.push_back(price);
                            m.av.push_back(volume);
                            price = atof(pRtnMarketData->SalePrice6);
                            volume = atoll(pRtnMarketData->SaleNumber6);
                            if (price > MINPRICE && volume > 0) {
                                m.ap.push_back(price);
                                m.av.push_back(volume);
                                price = atof(pRtnMarketData->SalePrice7);
                                volume = atoll(pRtnMarketData->SaleNumber7);
                                if (price > MINPRICE && volume > 0) {
                                    m.ap.push_back(price);
                                    m.av.push_back(volume);
                                    price = atof(pRtnMarketData->SalePrice8);
                                    volume = atoll(pRtnMarketData->SaleNumber8);
                                    if (price > MINPRICE && volume > 0) {
                                        m.ap.push_back(price);
                                        m.av.push_back(volume);
                                        price = atof(pRtnMarketData->SalePrice9);
                                        volume = atoll(pRtnMarketData->SaleNumber9);
                                        if (price > MINPRICE && volume > 0) {
                                            m.ap.push_back(price);
                                            m.av.push_back(volume);
                                            price = atof(pRtnMarketData->SalePrice10);
                                            volume = atoll(pRtnMarketData->SaleNumber10);
                                            if (price > MINPRICE && volume > 0) {
                                                m.ap.push_back(price);
                                                m.av.push_back(volume);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        m.status = kStateOK;
        string _line = ctx->FinishQTick();
//        if (strcmp(pRtnMarketData->TreatyCode, "09988.HK") == 0) {
//            LOG_INFO << pRtnMarketData->TreatyCode
//                     << ", new_volume: " << m.new_volume
//                     << ", sum_volume: " << m.sum_volume
//                     << ", new_price: " << m.new_price
//                     << ", new_amount: " << m.new_amount
//                     << ", sum_amount: " << m.sum_amount;
//        }
        QServer::Instance()->PushQTick(_line);
    }
}  // namespace co
