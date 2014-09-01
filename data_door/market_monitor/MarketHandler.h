#ifndef __MARKET_HANDLER_H__
#define __MARKET_HANDLER_H__

#include <string>
#include "event.h"
#include "../KSMarketDataAPI/KSMarketDataAPI.h"

using namespace std;
using namespace KingstarAPI;

class MarketHandler : public CThostFtdcMdSpi
{
public: 
	// constructor£¬which need a valid pointer to a CThostFtdcMduserApi instance 
	MarketHandler(CThostFtdcMdApi *pUserApi);
	~MarketHandler();

    	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

public:
    // participant ID
    TThostFtdcBrokerIDType m_chBrokerID;

    // user id
    TThostFtdcUserIDType m_chUserID;

    // user password
    TThostFtdcPasswordType m_chPassword;

    // contract
    char m_chContract[80];

    // request id
    int m_nRequestID;

    // finish event
    event_handle m_hEvent;

private: 
     // a pointer of CThostFtdcMduserApi instance
     CThostFtdcMdApi *m_pUserApi;

};

#endif
