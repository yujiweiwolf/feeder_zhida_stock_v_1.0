// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "feeder/feeder.h"
#include "x/x.h"
#include "config.h"

namespace co {
class InstrumentMgr {
 public:
    void Init() {
        try {
            Login();
            ReadInstrument();
            LOG_INFO << "get instrument size: " << instruments_.size();
        } catch (std::exception& e) {
            LOG_ERROR << "get instrument faild, " << e.what();
        }
    }

    std::set<std::string>& GetInstrument() {
        return instruments_;
    }

 private:
    void Login() {
        // 客户端直接执行
        // curl -H "Content-Type:application/json" -X POST -d '{"userID":"MK00000022","password":"pftz123123"}' https://con_sh1.directaccess.cn/account/login
        Singleton<Config>::Instance();
        Singleton<Config>::GetInstance()->Init();
        co::HTTPOptions opt;
        opt.method = co::kHTTPMethodPost;
        opt.url = Singleton<Config>::GetInstance()->login_address();
        opt.headers.insert(std::make_pair("Content-Type", "application/json"));
        string userid = Singleton<Config>::GetInstance()->userid();
        string password = Singleton<Config>::GetInstance()->password();
        opt.data = "{\"userID\":\""
                   + userid +
                   "\",\"password\":\""
                   + password +
                   "\"}";
        for (int i = 0; i < 10; i++) {
            auto result = co::SendHTTP(opt);
            LOG_INFO <<std::endl <<  result << std::endl;
            std::stringstream str_stream(result);
            boost::property_tree::ptree root;
            boost::property_tree::read_json(str_stream, root);
            boost::property_tree::ptree::iterator root_it = root.begin();
            for (; root_it != root.end(); ++root_it) {
                string key = root_it->first;
                if ("data" == key) {
                    boost::property_tree::ptree tkt_node = root.get_child(key);
                    boost::property_tree::ptree::iterator tkt_node_it = tkt_node.begin();
                    for (; tkt_node_it != tkt_node.end(); ++tkt_node_it) {
                        string tkt_key = tkt_node_it->first;
                        string tkt_val = tkt_node.get<string>(tkt_key);
                        if (tkt_key == "token") {
                            token_ = tkt_val;
                            LOG_INFO << "token: " << token_;
                            return;
                        } else if (tkt_key == "message") {
                            LOG_INFO << tkt_key;
                        }
                    }
                }
            }
            x::Sleep(5000);
        }
    }

    void ReadInstrument() {
        int index = 0;
        for (int i = 0; i < 100; i++) {
            x::Sleep(10);
            char buffer[102400] = {0};
            string cmd_line = "curl -H \"Content-Type:application/x-www-form-urlencoded\" -H \"token:"
                              "\"" + token_ + "\""
                              "\" -X POST --data-urlencode 'jsonText={\"size\":\"200\",\"pageNo\":\""
                              + std::to_string(i) +
                              "\",\"exCode\":\"HKEX\",\"prdCode\":\"\",\"exMarketType\":\"S\"}' "
                              "\"" + Singleton<Config>::GetInstance()->query_addres() + "\"";
            LOG_INFO << cmd_line;
            FILE *fp = popen(cmd_line.c_str(), "r");
            if (NULL != fp) {
                fread(buffer, 1, sizeof (buffer), fp);
                // LOG_INFO <<std::endl <<  buffer << std::endl;
                string str_json = buffer;
                std::stringstream str_stream(str_json);
                boost::property_tree::ptree root;
                boost::property_tree::read_json(str_stream, root);
                boost::property_tree::ptree::iterator root_it = root.begin();
                for (; root_it != root.end(); ++root_it) {
                    string key = root_it->first;
                    if ("data" == key) {
                        boost::property_tree::ptree tkt_node = root.get_child(key);
                        boost::property_tree::ptree::iterator tkt_node_it = tkt_node.begin();
                        for (; tkt_node_it != tkt_node.end(); ++tkt_node_it) {
                            string tkt_key = tkt_node_it->first;
                            string tkt_val = tkt_node.get<string>(tkt_key);
                            if ("records" == tkt_key) {
                                boost::property_tree::ptree items = tkt_node.get_child(tkt_key);
                                for (auto it = items.begin(); it != items.end(); ++it) {
                                    boost::property_tree::ptree cpn = it->second;
                                    boost::property_tree::ptree::iterator cpn_it = cpn.begin();
                                    for (; cpn_it != cpn.end(); ++cpn_it) {
                                        string cpn_key = cpn_it->first;
                                        string val = cpn.get<string>(cpn_key);
                                        if (cpn_key == "contractCode" && val < "10000.HK") {
                                            LOG_INFO << "records, " << cpn_key << ", " << val << ", index:" << ++index;
                                            instruments_.insert(val);
                                        }
                                    }
                                }
                            } else if (tkt_key == "pages") {
                                // cout <<"data: " << tkt_key << ":" << tkt_val << endl;
                                // pages 是一个固定值
                                int page = atoi(tkt_val.c_str());
                                if (i > page) {
                                    return;
                                }
                            }
                        }
                    }
                }
                pclose(fp);
            }
        }
    }

 private:
    std::string token_;
    std::set<std::string> instruments_;
};
}  // namespace co