// $Id$

/**
 * @file Broker_i.h
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Douglas C. Schmidt <schmidt@dre.vanderbilt.edu>
 */

#ifndef BROKERI_H_
#define BROKERI_H_

// local headers
#include "StockNameConsumer_i.h"
#include "BrokerS.h"
#include "DistributorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Stock_StockBroker_i
 * @brief This class defined the Stock Broker client.
 */
class  Stock_StockBroker_i
  : public virtual POA_Stock::StockBroker
{
public:
  /**
   * Constructor.
   *
   * @param orb
   * @param stock_name The name of the stock that the Stock Broker client is interested in.
   * @param priority The CORBA priority of this Stock Broker client.
   */
  Stock_StockBroker_i (CORBA::ORB_ptr orb,
                       Stock::StockDistributor_ptr dist,
                       const char *stock_name);

  /// Destructor
  virtual ~Stock_StockBroker_i (void);

  /**
   * Return the StockNameConsumer object created by the Constructor.
   *
   * @return Returns a StockNameConsumer object reference.
   */
  virtual ::Stock::StockNameConsumer_ptr get_consumer_notifier ();

  /**
   * Duplicate a StockQuoter object using the StockQuoter object reference "c" in the argument.
   *
   * @param c A StockQuoter object reference.
   */
  virtual void connect_quoter_info (::Stock::StockQuoter_ptr c);

  /**
   * Destroy the StockQuoter object and return it.
   *
   * @return Returns the destroyed StockQuoter object reference.
   */
  virtual ::Stock::StockQuoter_ptr disconnect_quoter_info ();

  /**
   * Return the StockQuoter object.
   *
   * @return Returns the StockQuoter object reference that has been
   *         created by connect_quoter_info ().
   */
  virtual ::Stock::StockQuoter_ptr get_connection_quoter_info ();

  /**
   * Shutdown the object and destroy the application.
   */
  virtual void shutdown ();

private:
  // Cached ORB pointer
  CORBA::ORB_var orb_;

  /// A StockQuoter object reference that is used to get detailed
  /// stock information.
  Stock::StockQuoter_var quoter_;

  /// A StockNameConsumer servant that is used to get notification of
  /// updates.
  Stock_StockNameConsumer_i *consumer_;

  /// The distributor that we are registered with, useful for shutdown.
  Stock::StockDistributor_var distributor_;
};

/**
 * @class Stock_StockBrokerHome_i
 * @brief This class defined the Stock Broker home.
 */
class  Stock_StockBrokerHome_i
  : public virtual POA_Stock::StockBrokerHome,
    public ACE_Event_Handler
{
public:
  /**
   * Constructor.
   * Register the necessary factories and mappings with the specified orb and
   * Create a new instance of the StockBroker object.
   *
   * @param orb
   */
  Stock_StockBrokerHome_i (CORBA::ORB_ptr orb);

  /// Destructor
  virtual ~Stock_StockBrokerHome_i (void);

  /**
   * Return the StockBroker object created by the Constructor.
   *
   * @return The StockBroker object created by the Constructor.
   */
  virtual ::Stock::StockBroker_ptr create (Stock::StockDistributor_ptr dist,
                                           const char *stock_name);

  virtual int handle_signal (int signum,
                             siginfo_t * = 0,
                             ucontext_t * = 0);

private:
  /// The StockDistributor object created by its home.
  Stock::StockBroker_var broker_;

  /// Cache a reference to the ORB.
  CORBA::ORB_var orb_;
};

#endif /* BROKERI_H_  */
