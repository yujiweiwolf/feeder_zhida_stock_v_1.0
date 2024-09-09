// Copyright 2020 Fancapital Inc.  All rights reserved.
#include <boost/program_options.hpp>
#include "x/x.h"
#include "./config.h"
#include "./daapi_server.h"
#include "./instrument_mgr.h"
using co::Config;
using co::DaapiServer;
namespace po = boost::program_options;

const char kVersion[] = "v1.0.11";

// 测试用
void test_code() {
//    {
//        // 测试环境，可以使用
//        co::HTTPOptions opt;
//        opt.method = co::kHTTPMethodPost;
//        opt.url = "http://222.73.105.170:19999/contract/getContract";
//        opt.headers["Content-Type"] = "application/x-www-form-urlencoded";
//        opt.headers["token"] = "eyJ0eXAiOiJKV1QiLCJ0eXBlIjoiSldUIiwiYWxnIjoiSFMyNTYifQ.eyJleHAiOjE2ODM4ODY3NjgsInVzZXJJRCI6Ik1OMDAwMDAwMDEiLCJpYXQiOjE2ODM4NzU5Njh9.hz73kIpBT-8Sh8JFeoti9_RKrlQH7_oZf6FzBYOPifU";
//        std::string body = "jsonText={\"size\":\"200\",\"pageNo\":\"0\",\"exCode\":\"HKEX\",\"prdCode\":\"\",\"exMarketType\":\"S\"}";
//        opt.data = body;
//        auto result = co::SendHTTP(opt);
//        LOG_INFO << "result: " << result;
//
//        string str_json = result;
//        std::stringstream str_stream(str_json);
//        boost::property_tree::ptree root;
//        boost::property_tree::read_json(str_stream, root);
//        boost::property_tree::ptree::iterator root_it = root.begin();
//        for (; root_it != root.end(); ++root_it) {
//            string key = root_it->first;
//            if ("data" == key) {
//                boost::property_tree::ptree tkt_node = root.get_child(key);
//                boost::property_tree::ptree::iterator tkt_node_it = tkt_node.begin();
//                for (; tkt_node_it != tkt_node.end(); ++tkt_node_it) {
//                    string tkt_key = tkt_node_it->first;
//                    string tkt_val = tkt_node.get<string>(tkt_key);
//                    if ("records" == tkt_key) {
//                        boost::property_tree::ptree items = tkt_node.get_child(tkt_key);
//                        for (auto it = items.begin(); it != items.end(); ++it) {
//                            boost::property_tree::ptree cpn = it->second;
//                            boost::property_tree::ptree::iterator cpn_it = cpn.begin();
//                            int index = 0;
//                            for (; cpn_it != cpn.end(); ++cpn_it) {
//                                string cpn_key = cpn_it->first;
//                                string val = cpn.get<string>(cpn_key);
//                                if (cpn_key == "contractCode" && val < "10000.HK") {
//                                    cout << "   records " << cpn_key << " : " << val << ", index:" << ++index << endl;
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }

    string token;
    {
        co::HTTPOptions opt;
        opt.method = co::kHTTPMethodPost;
        opt.url = "https://con_sh1.directaccess.cn/account/login";
        opt.headers.insert(std::make_pair("Content-Type", "application/json"));
        string userid = "MK00000022";
        string password = "pftz123123";
        opt.data = "{\"userID\":\""
                   + userid +
                   "\",\"password\":\""
                   + password +
                   "\"}";
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
                    if (tkt_key == "message") {
                        LOG_INFO << tkt_key;
                    }
                    if (tkt_key == "token") {
                        cout << tkt_key << " :" << tkt_val << endl;
                        token = tkt_val;
                        LOG_INFO << "token: " << token;
                    }
                }
            }
        }
    }

    {
        int index = 0;
        for (int i = 0; i < 100; i++) {
            char buffer[102400] = {0};
            string cmd_line = "curl -H \"Content-Type:application/x-www-form-urlencoded\" -H \"token:"
                              //"eyJ0eXAiOiJKV1QiLCJ0eXBlIjoiSldUIiwiYWxnIjoiSFMyNTYifQ.eyJleHAiOjE2ODM4ODY3NjgsInVzZXJJRCI6Ik1OMDAwMDAwMDEiLCJpYXQiOjE2ODM4NzU5Njh9.hz73kIpBT-8Sh8JFeoti9_RKrlQH7_oZf6FzBYOPifU"
                              "\"" + token + "\""
                              "\" -X POST --data-urlencode 'jsonText={\"size\":\"200\",\"pageNo\":\""
                              + std::to_string(i) +
                              "\",\"exCode\":\"HKEX\",\"prdCode\":\"\",\"exMarketType\":\"S\"}' "
                              "https://con_sh1.directaccess.cn/contract/getContract";
            // LOG_INFO << cmd_line;
            FILE *fp = popen(cmd_line.c_str(), "r");
            if (NULL != fp) {
                fread(buffer, 1, sizeof (buffer), fp);
                LOG_INFO <<std::endl <<  buffer << std::endl;
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
}

int main(int argc, char* argv[]) {
    try {
        po::options_description desc("[daapi_feeder] Usage");
        desc.add_options()
            ("help,h", "show help message")
            ("version,v", "show version information")
            ("passwd", po::value<std::string>(), "encode plain password");
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        if (vm.count("passwd")) {
            cout << co::EncodePassword(vm["passwd"].as<std::string>()) << endl;
            return 0;
        } else if (vm.count("help")) {
            cout << desc << endl;
            return 0;
        } else if (vm.count("version")) {
            cout << kVersion << endl;
            return 0;
        }
        LOG_INFO << "start zhida staock, version = " << kVersion;
        Singleton<Config>::Instance();
        Singleton<Config>::GetInstance()->Init();
        Singleton<co::InstrumentMgr>::Instance();
        Singleton<co::InstrumentMgr>::GetInstance()->Init();
        DaapiServer server;
        server.Run();
        LOG_INFO << "server is stopped.";
    } catch (x::Exception& e) {
        LOG_ERROR << "server is crashed, " << e.what();
        return 1;
    } catch (std::exception& e) {
        LOG_ERROR << "server is crashed, " << e.what();
        return 2;
    } catch (...) {
        LOG_ERROR << "server is crashed, unknown reason";
        return 3;
    }
    return 0;
}

