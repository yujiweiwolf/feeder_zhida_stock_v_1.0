/////////////////////////////////////////////////////////////////////////
/// DriectAccess Trade Engine
/// Copyright (C) Shanghai DirectAccess Technology Co., Ltd.
/// Last Modify 2022/01/11
/// Define DataType For API
/// Author (c) LiKan
/////////////////////////////////////////////////////////////////////////

#pragma once
#include "DAMarketStruct.h"

namespace DAMARKET {

	class CDAMarketDataSpi
	{
		//状态函数
	public:
		virtual void OnFrontConnected() {};
		virtual void OnFrontDisconnected(int iReason) {};
		virtual void OnHeartBeatWarning(int iTimeLapse) {};

		//请求函数
	public:
		virtual void OnRspUserLogin(CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) {};				
		virtual void OnRspMarketData(CMarketRspMarketDataField* pRspMarketDataField, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) {}
		virtual void OnRspError(CMarketRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
		virtual void OnRtnBrokerData(CMarketRtnBrokerDataField *pRtnBrokerData, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) {}
		virtual void OnRtnMarketData(CMarketRtnMarketDataField *pRtnMarketData) {}
		virtual void OnRtnTradeDate(CMarketRtnTradeDateField *pRtnTradeDate, CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) {}
		virtual void OnRtnUserLogout(CMarketRspInfoField *pRspInfo, int iRequestID, bool bIsLast) {}				
	};

	class DA_API_EXPORT CDAMarketDataApi
	{
		// 静态函数
	public:
		static const char *GetVersion();
		static CDAMarketDataApi *CreateMarketApi(bool bRecordLog, const char *pszFlowPath = "");

		// 构造函数
	public:
		CDAMarketDataApi() {};
		virtual ~CDAMarketDataApi() {};

		// 初始化函数
	public:
		virtual bool Init() = 0;
		virtual void Release() = 0;
		virtual void SetHeartBeatTimeout(int iTimeout,bool bIsStrictCheck = true) = 0;
		virtual void RegisterSpi(CDAMarketDataSpi *pIMarketEvent) = 0;
		virtual void RegisterNameServer(const char *pszNsAddress) = 0;

		// 请求函数
	public:
		virtual bool ReqUserLogin(CMarketReqUserLoginField *pReqUserLoginField, int iRequestID) = 0;
		virtual bool ReqBrokerData(CMarketReqBrokerDataField* pReqBrokerDataField, int iRequestID) = 0;
		virtual int  ReqMarketData(CMarketReqMarketDataField* pReqMarketDataField, int iRequestID) = 0;
	};

}