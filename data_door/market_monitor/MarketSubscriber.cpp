// MarketSubscriber.cpp : Defines the entry point for the console application.
//
#include "MarketSubscriber.h"
#include "MarketHandler.h"
#include "event.h"
#include "../KSMarketDataAPI/KSMarketDataAPI.h"
#include<stdlib.h>
#include<stdio.h>
#include <iostream>
using namespace std;
#ifdef WIN32
#include "windows.h"
#pragma comment(lib, "../KSMarketDataAPI/KSMarketDataAPI.lib")
#pragma message("Automatically linking with KSMarketDataAPI.lib")
#else
#include<unistd.h>
#include<string.h>
#endif

using namespace KingstarAPI;

MarketSubscriber::MarketSubscriber()
{
	init();
}

MarketSubscriber::~MarketSubscriber()
{
	release();
}

void MarketSubscriber::init()
{
        //create a CThostFtdcMdApi instance
        marketApi = CThostFtdcMdApi::CreateFtdcMdApi();

        //create an event handler instance
        handler = new MarketHandler(marketApi);

        //Create a manual reset event with no signal
        handler->m_hEvent = event_create(true, false);

        //set spi's broker, user, passwd
        strcpy (handler->m_chBrokerID, "3748FD77");	// Nanhua Mechantile Broker ID 
#ifdef WIN32
	_snprintf(handler->m_chUserID, sizeof(handler->m_chUserID)-1, "8023901");
#else
        snprintf(handler->m_chUserID, sizeof(handler->m_chUserID), "8023901");
#endif
        strcpy (handler->m_chPassword, "2525");

        strcpy (handler->m_chContract, "al1412");

        //register an event handler instance
        marketApi->RegisterSpi(handler);

        //register the kingstar front address and port
	marketApi->RegisterFront("tcp://124.160.44.166:17159");		// Nanhua Mechantile API 
        //make the connection between client and CTP server
        marketApi->Init();
}

void MarketSubscriber::release()
{
        // logout
        CThostFtdcUserLogoutField UserLogout;
        memset(&UserLogout, 0, sizeof(UserLogout));
        // broker id 
        strcpy(UserLogout.BrokerID, handler->m_chBrokerID);
        // investor ID 
        strcpy(UserLogout.UserID, handler->m_chUserID);
        marketApi->ReqUserLogout(&UserLogout, handler->m_nRequestID++ );

        // waiting for quit event
	event_timedwait((event_handle)handler->m_hEvent, 3000/*INFINITE*/);  

        // release the API instance
        marketApi->Release();

	delete handler;
}

  void MarketSubscriber::subscribe( char* contract)
{
        //ÐÐÇé¶©ÔÄÁÐ±í
        //char *ppInstrumentID[] = {"IF1203"};
        char *ppInstrumentID[1024];
        ppInstrumentID[0] = new char[31];
        strcpy (ppInstrumentID[0], contract);

        //ÐÐÇé¶©ÔÄ¸öÊý
        int iInstrumentID = 1;

        //¶©ÔÄ
        marketApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	printf("Subscribed market data for :%s\n", ppInstrumentID[0]);

        //ÊÍ·ÅÄÚ´æ
        delete ppInstrumentID[0];
}

