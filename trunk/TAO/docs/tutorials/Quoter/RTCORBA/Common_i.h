// $Id$

/**
 * @file Common_i.h
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 */

#ifndef COMMONI_H_
#define COMMONI_H_

// local headers
#include "CommonS.h"
#include "BrokerS.h"

// TAO headers
#include <tao/RTCORBA/RTCORBA.h>

// STL headers
#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Stock_StockNameConsumer_i
 * @brief This class is used as a callback interface used by the 
 *        distributor to notify brokers of updates.
 */
class  Stock_StockNameConsumer_i
  : public virtual POA_Stock::StockNameConsumer,
    public virtual PortableServer::RefCountServantBase
{
public:
  /**
   * Constructor.
   *
   * @param parent The parent StockBroker object reference of the StockNameConsumer object.
   * @param stock_name The name of the stock that the parent StockBroker object need to listen to.
   */
  Stock_StockNameConsumer_i (Stock::StockBroker_ptr parent,
                             const char *stock_name);

  /// Destructor.
  virtual ~Stock_StockNameConsumer_i (void);

  /**
   * Push an event to the consumer.
   *
   * @param the_stockname The name of the stock that will be provided to the parent StockBroker object.
   */
  virtual void push_StockName (::Stock::StockName *the_stockname)
    throw (::CORBA::SystemException);

private:
  /// Parent of the StockNameConsumer object.
  Stock::StockBroker_var parent_;

  /// Name of the stock to listen to.
  CORBA::String_var stock_name_;
};

/**
 * @class Stock_StockQuoter_i
 * @brief This class is used by brokers to get detailed stock information.
 */
class  Stock_StockQuoter_i
  : public virtual POA_Stock::StockQuoter,
    public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor.
  Stock_StockQuoter_i (void);

  /// Destructor.
  virtual ~Stock_StockQuoter_i (void);

  /**
   * Get detailed stock information.
   *
   * @param stock_name The name of the stock whose detailed info is needed.
   * @throw ::Stock::Invalid_Stock
   */
  virtual ::Stock::StockInfo * get_stock_info (const char *stock_name)
    throw (::CORBA::SystemException, ::Stock::Invalid_Stock);
};

#endif /* COMMONI_H_  */
