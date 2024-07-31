// Copyright 2020 Fancapital Inc.  All rights reserved.
#include "./config.h"
#include "../src/daapi_server.h"
#include <boost/algorithm/string.hpp>

namespace co {
    DaapiServer::DaapiServer() {
    }

    DaapiServer::~DaapiServer() {
        if (md_spi) {
            delete md_spi;
            md_spi = nullptr;
        }
    }

    void DaapiServer::Run() {
        QOptionsPtr opt = Singleton<Config>::GetInstance()->opt();
        QServer::Instance()->Init(opt);
        QServer::Instance()->Start();
        string market = "HKEX";
        md_spi = new MDCallBack(market);
        md_spi->Init();
        __info << "Succeed to init Zhida MD interface.";
        QServer::Instance()->Wait();
        QServer::Instance()->Stop();
    }
}  // namespace co

