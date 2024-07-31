// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#include "./md_callback.h"
// #include "./trade_callback.h"

namespace co {
class DaapiServer {
 public:
    DaapiServer();
    ~DaapiServer();
    void Run();

 private:
    MDCallBack* md_spi = nullptr;
    // TradeCallBack* trade_spi_ = nullptr;
};
}  // namespace co
