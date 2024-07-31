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

/// ����������˶���
#define MAX_SUB_COUNT				50

/////////////////////////////////////////////////////////////////////////
/// ������������
/////////////////////////////////////////////////////////////////////////

/// ��Ʊ
#define DAF_TYPE_Stock				'S'
/// �ڻ�
#define DAF_TYPE_Future				'D'
/// δ֪
#define DAF_TYPE_Unknown			 0

/////////////////////////////////////////////////////////////////////////
/// ��������
/////////////////////////////////////////////////////////////////////////

/// ׷�Ӷ���
#define DAF_SUB_Append				'1'
/// �˶�
#define DAF_SUB_Unsubcribe			'2'
/// �˶�����
#define DAF_SUB_UnsubcribeAll		'3'

/////////////////////////////////////////////////////////////////////////
/// ������������
/////////////////////////////////////////////////////////////////////////

/// ������������
#define DAF_Market_Full				'Z'
/// �ɽ���������
#define DAF_Market_Fill				'Y'


/// ���ַ��� empty std::string
#define EMPTY_STRING				""


/////////////////////////////////////////////////////////////////////////
//// Derivative define start
/////////////////////////////////////////////////////////////////////////
/// ������:		DERIVATIVE_BID=��,  DERIVATIVE_ASK=��
/// Bid or ask:		DERIVATIVE_BID=bid, DERIVATIVE_ASK=ask
#define DERIVATIVE_BID						"1"
#define DERIVATIVE_ASK						"2"

///���ֻ���ƽ��
///open/close position flag
#define DERIVATIVE_OPEN_POS_FLAG			"1"
#define DERIVATIVE_CLOSE_POS_FLAG			"2"

// ������:		STOCK_BID=��,  STOCK_ASK=��
// Bid or ask:		STOCK_BID=bid, STOCK_ASK=ask
#define STOCK_BID							"1"
#define STOCK_ASK							"2"

// ���ֻ���ƽ��:
		// STOCK_OPEN_POS_FLAG=����,STOCK_OPEN_POS_FLAG=ƽ��
// open/close position flag:
		// STOCK_OPEN_POS_FLAG=open position,STOCK_CLOSE_POS_FLAG=close position
#define STOCK_OPEN_POS_FLAG					"1"
#define STOCK_CLOSE_POS_FLAG				"2"

// �Ƿ�ģ���û���STOCK_IS_SIMULATED_USER=�ǣ�STOCK_IS_NOT_SIMULATED_USER or other������
//Simulated user:
//		STOCK_IS_SIMULATED_USER=is a simulated user
//		STOCK_IS_NOT_SIMULATED_USER or other = is not a simulated user
#define STOCK_IS_SIMULATED_USER				"1"
#define STOCK_IS_NOT_SIMULATED_USER			"0"

/////////////////////////////////////////////////////////////////////////
////  End of Stock define
/////////////////////////////////////////////////////////////////////////



}