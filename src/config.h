// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include "feeder/feeder.h"

template <typename T>
class Singleton {
 public:
    template<typename... Args>
    static T* Instance(Args&&... args) {
        if (m_pInstance == nullptr)
            m_pInstance = new T(std::forward<Args>(args)...);

        return m_pInstance;
    }

    static T* GetInstance() {
        if (m_pInstance == nullptr)
            throw std::logic_error("please initialize the instance first");

        return m_pInstance;
    }

    static void DestroyInstance() {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

 private:
    Singleton(void) {}
    virtual ~Singleton(void) {}
    Singleton(const Singleton&) {}
    Singleton& operator=(const Singleton&) {}

 private:
    static T* m_pInstance;
};

template <class T> T* Singleton<T>::m_pInstance = nullptr;


namespace co {
class Config {
 public:
    inline string serveraddress() {
        return serveraddress_;
    }
    inline string userid() {
        return userid_;
    }
    inline string password() {
        return password_;
    }
    inline int heartbeat() {
        return heartbeat_;
    }
    inline string authorcode() {
        return authorcode_;
    }
    inline string macdddress() {
        return macdddress_;
    }
    inline string computername() {
        return computername_;
    }
    inline string softwareversion() {
        return softwareversion_;
    }
    inline string softwarename() {
        return softwarename_;
    }
    inline string pricebrokerid() {
        return pricebrokerid_;
    }
    inline string code_file() {
        return code_file_;
    }
    inline string login_address() {
        return login_address_;
    }
    inline string query_addres() {
        return query_addres_;
    }
    inline QOptionsPtr opt() {
        return opt_;
    }
    void Init();

 private:
    QOptionsPtr opt_;
    string serveraddress_;
    string userid_;
    string password_;
    int heartbeat_;
    string authorcode_;
    string macdddress_;
    string computername_;
    string softwareversion_;
    string softwarename_;
    string pricebrokerid_;
    string code_file_;
    string login_address_;
    string query_addres_;
};
}  // namespace co
