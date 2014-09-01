// MarketHandler.cpp : Defines the entry point for the console application.
//
#include<stdio.h>
#include<stdlib.h>

#include "event.h"
#include "../KSMarketDataAPI/KSMarketDataAPI.h"
#include "MarketApi.h"
#include "MarketHandler.h"

#include <iostream>
#ifdef WIN32
#include "windows.h"
#pragma comment(lib, "../KSMarketDataAPI/KSMarketDataAPI.lib")
#pragma message("Automatically linking with KSMarketDataAPI.lib")
#else
#include<unistd.h>
#include<string.h>
#endif

	// constructor，which need a valid pointer to a CThostFtdcMduserApi instance 
	MarketHandler::MarketHandler(CThostFtdcMdApi *pUserApi) : m_pUserApi(pUserApi) {}

	MarketHandler::~MarketHandler() {}

	// After making a succeed connection with the CTP server, the client should send the login request to the CTP server.
    void MarketHandler::OnFrontConnected()
    {
        printf("OnFrontConnected:\n");

        CThostFtdcReqUserLoginField reqUserLogin;
        memset(&reqUserLogin, 0, sizeof(reqUserLogin));
        // set BrokerID
        printf("BrokerID:%s\n", m_chBrokerID);
        strcpy(reqUserLogin. BrokerID, m_chBrokerID);

        // set user id
        printf("userid:%s\n", m_chUserID);
        strcpy(reqUserLogin.UserID, m_chUserID);

        // set password
        printf("password:%s\n",m_chPassword);
        strcpy(reqUserLogin.Password, m_chPassword);

        // send the login request
        m_pUserApi->ReqUserLogin(&reqUserLogin, m_nRequestID++ );
    }


	//When the connection between client and the CTP server	disconnected,the follwing function will be called.
	void MarketHandler::OnFrontDisconnected(int nReason)
	{ 
		//  Inthis  case,  API  willreconnect，the  client  application can ignore this.
		printf("OnFrontDisconnected.\n");
	} 

	// After receiving the login request from  the client，the CTP server will send the following response to notify the client whether the login success or not.
	void MarketHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
        printf("OnRspUserLogin:");
        if (pRspUserLogin != NULL)
        {
            printf("%s|%s|%s|%s|%s|%s|%s|%s|%s|%d|%d|%s|", 
                pRspUserLogin->BrokerID,					// 经纪公司代码
                pRspUserLogin->UserID,						// 用户代码
                pRspUserLogin->TradingDay,					// 交易日
                pRspUserLogin->SystemName,					// 交易系统名称
                pRspUserLogin->LoginTime,					// 登陆成功时间
                pRspUserLogin->SHFETime,					// 上期所时间
                pRspUserLogin->DCETime,						// 大商所时间
                pRspUserLogin->CZCETime,					// 郑商所时间
                pRspUserLogin->FFEXTime,					// 中金所时间
                pRspUserLogin->FrontID,						// frond id
                pRspUserLogin->SessionID,					// session id
                pRspUserLogin->MaxOrderRef					// 最大报单引用
                );
        }
        printf("\n");
        printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
        if (pRspInfo->ErrorID != 0)
		{
			// in case any login failure, the client should handle this error.
			printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
            return;
		}

		// get trading day
		printf("获取当前交易日 = %s\n",m_pUserApi->GetTradingDay());

		// 行情订阅列表
		//char *ppInstrumentID[] = {"IF1203"};
        char *ppInstrumentID[1024];
        ppInstrumentID[0] = new char[31];
        strcpy (ppInstrumentID[0], m_chContract);
		// 行情订阅个数
		int iInstrumentID = 1;
        // 	订阅
		m_pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
        // 释放内存
        delete ppInstrumentID[0];
	}

	///RspSubMarketData return
	void MarketHandler::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspSubMarketData:%s\n", pSpecificInstrument->InstrumentID);
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);

/*		if (bIsLast == true)
		{
			// 行情退订列表
			char *ppInstrumentID[] = {"CF1109"};			
			// 行情退订个数
			int iInstrumentID = 1;
			m_pUserApi->UnSubscribeMarketData(ppInstrumentID,iInstrumentID);
		}
*/	}


	///OnRspUnSubMarketData return
	void MarketHandler::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("OnRspUnSubMarketData:\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);

		// logout
/*		CThostFtdcUserLogoutField UserLogout;
		memset(&UserLogout, 0, sizeof(UserLogout));
		// broker id 
		strcpy(UserLogout.BrokerID, g_chBrokerID); 
		// investor ID 
		strcpy(UserLogout.UserID, g_chUserID);
		
		m_pUserApi->ReqUserLogout(&UserLogout,3);
*/	}

	///OnRtnDepthMarketData
	void MarketHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
	{
        printf("OnRtnDepthMarketData:");
        if(pDepthMarketData != NULL)
        {
            printf("%s|%s|%.04f|%.04f|%.04f|%.04f|%.04f|%d|%.04f|%.04f|%.04f|%d|%d|%.04f|%.04f|%.04f|%.04f|%.04f|%s|%d|%.04f|%d|%.04f|%d|%.04f|%d|%.04f|%d|%.04f|%d|%.04f|%d|%.04f|%d|%.04f|",
                pDepthMarketData->ExchangeID,					// 交易所代码
                pDepthMarketData->InstrumentID,					// 合约代码
                pDepthMarketData->PreClosePrice,				// 昨收盘
                pDepthMarketData->OpenPrice,					// 今开盘
                pDepthMarketData->HighestPrice,					// 最高价
                pDepthMarketData->LowestPrice,					// 最低价
                pDepthMarketData->LastPrice,					// 最新价
                pDepthMarketData->Volume,						// 数量
                pDepthMarketData->Turnover,						// 成交金额
                pDepthMarketData->BidPrice1,					// 申买价一
                pDepthMarketData->AskPrice1,					// 申卖价一
                pDepthMarketData->BidVolume1,					// 申买量一
                pDepthMarketData->AskVolume1,					// 申卖量一
                pDepthMarketData->UpperLimitPrice,				// 涨停板价
                pDepthMarketData->LowerLimitPrice,				// 跌停板价
                pDepthMarketData->PreSettlementPrice,			// 上次结算价
                pDepthMarketData->SettlementPrice,				// 本次结算价
                pDepthMarketData->OpenInterest,					// 持仓量
                pDepthMarketData->TradingDay,					// 交易日
                pDepthMarketData->BidVolume2,					// 申买量二
                pDepthMarketData->BidPrice2,					// 申买价二
                pDepthMarketData->BidVolume3,					// 申买量三
                pDepthMarketData->BidPrice3,					// 申买价三
                pDepthMarketData->BidVolume4,					// 申买量四
                pDepthMarketData->BidPrice4,					// 申买价四
                pDepthMarketData->BidVolume5,					// 申买量五
                pDepthMarketData->BidPrice5,					// 申买价五
                pDepthMarketData->AskVolume2,					// 申卖量二
                pDepthMarketData->AskPrice2,					// 申卖价二
                pDepthMarketData->AskVolume3,					// 申卖量三
                pDepthMarketData->AskPrice3,					// 申卖价三
                pDepthMarketData->AskVolume4,					// 申卖量四
                pDepthMarketData->AskPrice4,					// 申卖价四
                pDepthMarketData->AskVolume5,					// 申卖量五
                pDepthMarketData->AskPrice5						// 申卖价五
                );
        }
        printf("\n");
	}

	// logout return
    void MarketHandler::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
        printf("OnRspUserLogout:");
        if (NULL != pUserLogout)
        {
            printf("%s",pUserLogout->UserID);
        }
        printf("\n");
        printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
        printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

        // inform the main thread order transaction is over 
        //SetEvent(m_hEvent); 
	event_set((event_handle)m_hEvent);

	return;
	}

