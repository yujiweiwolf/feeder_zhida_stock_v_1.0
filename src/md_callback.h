// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#include <string>
#include <vector>
#include "feeder/feeder.h"
#include "DAMarketApi.h"
using namespace DAMARKET;

namespace co {
class MDCallBack : public CDAMarketDataSpi {
 public:
    explicit MDCallBack(string market);

    virtual ~MDCallBack();

    void Init();

    bool IsLogin();

 private:
    virtual void OnFrontConnected();

    virtual void OnFrontDisconnected(int iReason);

    virtual void OnHeartBeatWarning(int iTimeLapse);

    virtual void OnRspUserLogin(CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast);

    virtual void OnRspMarketData(CMarketRspMarketDataField* pRspMarketDataField, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast);

    virtual void OnRspError(CMarketRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    virtual void OnRtnBrokerData(CMarketRtnBrokerDataField *pRtnBrokerData, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast);

    virtual void OnRtnMarketData(CMarketRtnMarketDataField *pRtnMarketData);

    virtual void OnRtnTradeDate(CMarketRtnTradeDateField *pRtnTradeDate, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast);

    virtual void OnRtnUserLogout(CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast);

     void SubscribeMarket();

 private:
    int req_id_;
    bool is_login_;
    CDAMarketDataApi* md_api_;
    string market_;
    bool first_flag_ = false;
    int64_t trading_day_ = 0;
};
}  // namespace co



