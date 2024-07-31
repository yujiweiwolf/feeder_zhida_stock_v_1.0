// Copyright 2020 Fancapital Inc.  All rights reserved.
#include <x/x.h>
#include "../src/config.h"
#include "yaml-cpp/yaml.h"

namespace co {
    void Config::Init() {
        auto getStr = [&](const YAML::Node& node, const std::string& name) {
            try {
                return node[name] && !node[name].IsNull() ? node[name].as<std::string>() : "";
            } catch (std::exception& e) {
                LOG_ERROR << "load configuration failed: name = " << name << ", error = " << e.what();
                throw std::runtime_error(e.what());
            }
        };
        auto getInt = [&](const YAML::Node& node, const std::string& name, const int64_t& default_value = 0) {
            try {
                return node[name] && !node[name].IsNull() ? node[name].as<int64_t>() : default_value;
            } catch (std::exception& e) {
                LOG_ERROR << "load configuration failed: name = " << name << ", error = " << e.what();
                throw std::runtime_error(e.what());
            }
        };

        auto filename = x::FindFile("feeder.yaml");
        YAML::Node root = YAML::LoadFile(filename);
        opt_ = QOptions::Load(filename);
        auto feeder = root["daapi"];

        serveraddress_ = getStr(feeder, "serveraddress");
        userid_ = getStr(feeder, "userid");
        std::string _cfg_password = getStr(feeder, "password");
        password_ = DecodePassword(_cfg_password);

        heartbeat_ = getInt(feeder, "heartbeat");
        authorcode_ = getStr(feeder, "authorcode");
        macdddress_ = getStr(feeder, "macdddress");
        computername_ = getStr(feeder, "computername");
        softwareversion_ = getStr(feeder, "softwareversion");
        softwarename_ = getStr(feeder, "softwarename");
        pricebrokerid_ = getStr(feeder, "pricebrokerid");
        code_file_ = getStr(feeder, "code_file");
        login_address_ = getStr(feeder, "login_address");
        query_addres_ = getStr(feeder, "query_addres");

        stringstream ss;
        ss << "+-------------------- configuration begin --------------------+" << endl;
        ss << opt_->ToString() << endl;
        ss << endl;
        ss << "daapi:" << endl
            << "  serveraddress: " << serveraddress_ << endl
            << "  userid: " << userid_ << endl
            << "  password: " << string(password_.size(), '*') << endl
            << "  heartbeat: " << heartbeat_ << endl
            << "  authorcode: " << authorcode_ << endl
            << "  macdddress_: " << macdddress_ << endl
            << "  computername: " << computername_ << endl
            << "  softwareversion: " << softwareversion_ << endl
            << "  softwarename: " << softwarename_ << endl
            << "  pricebrokerid: " << pricebrokerid_ << endl
            << "  code_file: " << code_file_ << endl
            << "  login_address: " << login_address_ << endl
            << "  query_addres: " << query_addres_ << endl;
        ss << "+-------------------- configuration end   --------------------+";
        __info << endl << ss.str();
    }
}  // namespace co
