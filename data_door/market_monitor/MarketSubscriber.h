#ifndef __MARKET_SUBSCRIBER_H__
#define __MARKET_SUBSCRIBER_H__

#include <string>
#include "../KSMarketDataAPI/KSMarketDataAPI.h"
#include "MarketHandler.h"

using namespace KingstarAPI;

class MarketSubscriber 
{
 public:
  MarketSubscriber();
  virtual ~MarketSubscriber();

  void init();
  void release();
  // subscribe 
  void subscribe( const std::string );

 public:
  char contract[80];
  CThostFtdcMdApi *marketApi;
  MarketHandler *handler;

};

#endif
