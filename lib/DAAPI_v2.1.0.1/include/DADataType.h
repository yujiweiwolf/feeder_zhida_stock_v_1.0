/////////////////////////////////////////////////////////////////////////
/// DriectAccess Trade Engine
/// Copyright (C) Shanghai DirectAccess Technology Co., Ltd.
/// Last Modify 2022/01/11
/// Define DataType For API
/// Author (c) LiKan
/////////////////////////////////////////////////////////////////////////

#pragma once

namespace DAMARKET {

#define DA_STR_SIZE					256
#define TRADE_MAX_NUM_ONE_PAGE		(5000)	
#define MAX_BROKER					4096

#define DAAPI_VERSION				"2.1.0.1"

#ifdef _WIN32 
#ifdef LIB_DA_API_EXPORT
#define DA_API_EXPORT				__declspec(dllexport)
#else
#define DA_API_EXPORT				__declspec(dllimport)
#endif
#else
#define DA_API_EXPORT
#endif

typedef char						TDACharType;
typedef char						TDAStringType[DA_STR_SIZE];
typedef char						TDABrokerType[MAX_BROKER];

typedef int							TDAIntType;
typedef int							TDAVolumeType;

typedef double						TDADoubleType;
typedef double						TDAPriceType;
typedef double						TDAMoneyType;

/// 单次最大订阅退订数
#define MAX_SUB_COUNT				50

/////////////////////////////////////////////////////////////////////////
/// 订阅行情种类
/////////////////////////////////////////////////////////////////////////

/// 股票
#define DAF_TYPE_Stock				'S'
/// 期货
#define DAF_TYPE_Future				'D'
/// 未知
#define DAF_TYPE_Unknown			 0

/////////////////////////////////////////////////////////////////////////
/// 订阅类型
/////////////////////////////////////////////////////////////////////////

/// 追加订阅
#define DAF_SUB_Append				'1'
/// 退订
#define DAF_SUB_Unsubcribe			'2'
/// 退订所有
#define DAF_SUB_UnsubcribeAll		'3'

/////////////////////////////////////////////////////////////////////////
/// 行情数据类型
/////////////////////////////////////////////////////////////////////////

/// 完整行情数据
#define DAF_Market_Full				'Z'
/// 成交行情数据
#define DAF_Market_Fill				'Y'


/// 空字符串 empty std::string
#define EMPTY_STRING				""


/////////////////////////////////////////////////////////////////////////
//// Derivative define start
/////////////////////////////////////////////////////////////////////////
/// 买还是卖:		DERIVATIVE_BID=买,  DERIVATIVE_ASK=卖
/// Bid or ask:		DERIVATIVE_BID=bid, DERIVATIVE_ASK=ask
#define DERIVATIVE_BID						"1"
#define DERIVATIVE_ASK						"2"

///开仓还是平仓
///open/close position flag
#define DERIVATIVE_OPEN_POS_FLAG			"1"
#define DERIVATIVE_CLOSE_POS_FLAG			"2"

// 买还是卖:		STOCK_BID=买,  STOCK_ASK=卖
// Bid or ask:		STOCK_BID=bid, STOCK_ASK=ask
#define STOCK_BID							"1"
#define STOCK_ASK							"2"

// 开仓还是平仓:
		// STOCK_OPEN_POS_FLAG=开仓,STOCK_OPEN_POS_FLAG=平仓
// open/close position flag:
		// STOCK_OPEN_POS_FLAG=open position,STOCK_CLOSE_POS_FLAG=close position
#define STOCK_OPEN_POS_FLAG					"1"
#define STOCK_CLOSE_POS_FLAG				"2"

// 是否模拟用户：STOCK_IS_SIMULATED_USER=是；STOCK_IS_NOT_SIMULATED_USER or other：不是
//Simulated user:
//		STOCK_IS_SIMULATED_USER=is a simulated user
//		STOCK_IS_NOT_SIMULATED_USER or other = is not a simulated user
#define STOCK_IS_SIMULATED_USER				"1"
#define STOCK_IS_NOT_SIMULATED_USER			"0"

/////////////////////////////////////////////////////////////////////////
////  End of Stock define
/////////////////////////////////////////////////////////////////////////



}