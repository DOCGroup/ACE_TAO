// $Id$

/**
 * @file Broker_i.h
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 */

#ifndef BROKERI_H_
#define BROKERI_H_

// local headers
#include "Common_i.h"
#include "BrokerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Stock_StockBroker_i
 * @brief This class defined the Stock Broker client.
 */
class  Stock_StockBroker_i
  : public virtual POA_Stock::StockBroker,
    public virtual PortableServer::RefCountServantBase
{
public:
  /**
   * Constructor.
   *
   * @param orb
   * @param stock_name The name of the stock that the Stock Broker client is interested in.
   * @param priority The priority of this  Stock Broker client.
   */
  Stock_StockBroker_i (CORBA::ORB_ptr orb,
                       const char *stock_name,
                       RTCORBA::Priority priority);

  /// Destructor
  virtual ~Stock_StockBroker_i (void);

  /**
   * Return the StockNameConsumer object created by the Constructor.
   *
   * @return Returns a StockNameConsumer object reference.
   */
  virtual ::Stock::StockNameConsumer_ptr get_consumer_notifier ()
    throw (::CORBA::SystemException);

  /**
   * Duplicate a StockQuoter object using the StockQuoter object reference "c" in the argument.
   *
   * @param c A StockQuoter object reference.
   */
  virtual void connect_quoter_info (::Stock::StockQuoter_ptr c)
    throw (::CORBA::SystemException);

  /**
   * Destroy the StockQuoter object and return it.
   *
   * @return Returns the destroyed StockQuoter object reference.
   */
  virtual ::Stock::StockQuoter_ptr disconnect_quoter_info ()
    throw (::CORBA::SystemException);

  /**
   * Return the StockQuoter object.
   *
   * @return Returns the StockQuoter object reference that has been created by connect_quoter_info ().
   */
  virtual ::Stock::StockQuoter_ptr get_connection_quoter_info ()
    throw (::CORBA::SystemException);

private:
  /// A StockQuoter object reference that is used to get detailed stock information. 
  Stock::StockQuoter_var quoter_;

  /// A StockNameConsumer object reference that is used to get notification of updates..
 Stock_StockNameConsumer_i *consumer_;
};

/**
 * @class Stock_StockBrokerHome_i
 * @brief This class defined the Stock Broker home.
 */
class  Stock_StockBrokerHome_i
  : public virtual POA_Stock::StockBrokerHome,
    public virtual PortableServer::RefCountServantBase
{
public:
  /**
   * Constructor.
   * Register the necessary factories and mappings with the specified orb and 
   * Create a new instance of the StockBroker object.
   *
   * @param orb
   */
  Stock_StockBrokerHome_i (CORBA::ORB_ptr orb,
                           const char *stock_name,
                           RTCORBA::Priority priority);

  /// Destructor
  virtual ~Stock_StockBrokerHome_i (void);

  /**
   * Return the StockBroker object created by the Constructor.
   *
   * @return The StockBroker object created by the Constructor.
   */
  virtual ::Stock::StockBroker_ptr create ()
    throw (::CORBA::SystemException);

private:
  /// The StockDistributor object created by its home.
  Stock_StockBroker_i *broker_;
};

#endif /* BROKERI_H_  */
