// $Id$

/**
 * @file Distributor_i.h
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Douglas C. Schmidt <schmidt@dre.vanderbilt.edu>
 */

#ifndef DISTRIBUTORI_H_
#define DISTRIBUTORI_H_

// local headers
#include "DistributorS.h"
#include "Stock_Database.h"

// ACE headers
#include "ace/Task.h"
#include "ace/Event_Handler.h"
#include "tao/RTPortableServer/RTPortableServer.h"

// STL headers
#include <map>
#include <vector>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class StockDistributor_i
 * @brief This class defined the Stock Distributor server.
 */
class  StockDistributor_i
  : public virtual POA_Stock::StockDistributor,
    public virtual ACE_Task_Base
{
public:
  /**
   * Constructor.
   *
   * @param orb
   * @param policy_list The policy list use to create StockQuoter_POA.
   */
  StockDistributor_i (RTPortableServer::POA_ptr poa);

  /// Destructor
  virtual ~StockDistributor_i (void);

  /**
   * Insert StockNameConsumer object and its priority into the StockDistributor
   * object's subscribers' map.
   *
   * @param c The StockNameConsumer object to be inserted into the StockDistributor
   * object's subscribers' map.
   * @param priority The priority of the StockNameConsumer object.
   * @return A Cookie object that records this subscription.
   */
  virtual ::Stock::Cookie * subscribe_notifier (::Stock::StockNameConsumer_ptr c,
                                                ::RTCORBA::Priority priority);

  /**
   * Erase the StockNameConsumer object from the StockDistributor object's subscribers' map.
   *
   * @param ck The Cookie object that records the subscription.
   * @param The reference of the StockNameConsumer object that has been erased.
   */
  virtual ::Stock::StockNameConsumer_ptr unsubscribe_notifier (::Stock::Cookie *ck);

  /**
   * Return the StockQuoter object created by the Constructor.
   *
   * @return A StockQuoter object created by this StockDistributor object.
   */
  virtual ::Stock::StockQuoter_ptr provide_quoter_info (::Stock::Cookie *ck);

  /**
   * Get the notification_rate attribute.
   *
   * @return The notification_rate attribute.
   */
  virtual ::CORBA::Long notification_rate ();

  /**
   * Set the notification_rate attribute.
   *
   * @param notification_rate The value of notification_rate attribute that need to be set.
   */
  virtual void notification_rate (::CORBA::Long notification_rate);

  /**
   * Set the active state of the StockDistributor object to true and create a thread
   * to publish the stock information to the Stock Broker clients.
   */
  virtual void start (void);

  /**
   * Set the active state of StockDistributor object to false.
   */
  virtual void stop (void);

  virtual void shutdown ();

  /// Callback for stock database.
  void operator () (std::vector <std::string> &stocks);

  int svn (void);

  /// The map that stores the subscribed StockNameConsumer object.
  typedef std::map <std::string,
                    std::pair <Stock::StockNameConsumer_var,
                               RTCORBA::Priority> > CookieMap;

private:
  /// Notification rate for distributor in seconds.
  CORBA::Long rate_;

  CookieMap subscribers_list_;

  /// This lock serializes access to the subscriber list.
  ACE_RW_Thread_Mutex lock_;

  /// The active state of the  StockDistributor object.
  bool active_;

  /// Cache a copy of the RTCORBA::POA.
  RTPortableServer::POA_var rt_poa_;

  /// Cache a copy of the CORBA::ORB.
  CORBA::ORB_var orb_;
};

/**
 * @class StockQuoter_i
 * @brief This class is used by brokers to get detailed stock information.
 */
class  StockQuoter_i
  : public virtual POA_Stock::StockQuoter
{
public:
  /// Constructor.
  StockQuoter_i (void);

  /// Destructor.
  virtual ~StockQuoter_i (void);

  /**
   * Get detailed stock information.
   *
   * @param stock_name The name of the stock whose detailed info is needed.
   * @throw ::Stock::Invalid_Stock
   */
  virtual ::Stock::StockInfo *get_stock_info (const char *stock_name);
};

/**
 * @class StockDistributorHome_i
 * @brief This servant defines the Stock Distributor home.
 */
class  StockDistributorHome_i
  : public virtual POA_Stock::StockDistributorHome,
    public ACE_Event_Handler
{
public:
  /**
   * Constructor.
   * Register the necessary factories and mappings with the specified orb and
   * Create a new instance of the StockDistributor object.
   *
   * @param orb
   */
  StockDistributorHome_i (CORBA::ORB_ptr orb);

  /// Destructor
  virtual ~StockDistributorHome_i (void);

  /**
   * Return the StockDistributor object created by the Constructor.
   *
   * @return The StockDistributor object created by the Constructor.
   */
  virtual ::Stock::StockDistributor_ptr create ();

  virtual int handle_signal (int signum,
                             siginfo_t * = 0,
                             ucontext_t * = 0);
private:
  void create_distributor (void);

  /// Cached ORB used to shut down the system
  CORBA::ORB_var orb_;

  /// Cached POA created during object construction
  RTPortableServer::POA_var rt_poa_;

  /// The StockDistributor servant created by its home.
  PortableServer::ObjectId_var dist_id_;
};


// Define typedefs for our version of the stock database.
typedef ACE_Singleton<Stock_Database<StockDistributor_i>, TAO_SYNCH_MUTEX> Stock_Database_Singleton;
#define STOCK_DATABASE Stock_Database_Singleton::instance()

#endif /* DISTRIBUTORI_H_  */
