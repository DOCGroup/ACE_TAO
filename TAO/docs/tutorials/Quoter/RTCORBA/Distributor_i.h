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
  explicit StockDistributor_i (RTPortableServer::POA_ptr poa);

  StockDistributor_i (StockDistributor_i const &) = delete;
  StockDistributor_i (StockDistributor_i &&) = delete;
  StockDistributor_i &operator= (StockDistributor_i const &) = delete;
  StockDistributor_i &operator= (StockDistributor_i &&) = delete;

  /// Destructor
  ~StockDistributor_i () override;

  /**
   * Insert StockNameConsumer object and its priority into the StockDistributor
   * object's subscribers' map.
   *
   * @param c The StockNameConsumer object to be inserted into the StockDistributor
   * object's subscribers' map.
   * @param priority The priority of the StockNameConsumer object.
   * @return A Cookie object that records this subscription.
   */
  ::Stock::Cookie * subscribe_notifier (::Stock::StockNameConsumer_ptr c,
                                        ::RTCORBA::Priority priority) override;

  /**
   * Erase the StockNameConsumer object from the StockDistributor object's subscribers' map.
   *
   * @param ck The Cookie object that records the subscription.
   * @param The reference of the StockNameConsumer object that has been erased.
   */
  ::Stock::StockNameConsumer_ptr unsubscribe_notifier (::Stock::Cookie *ck) override;

  /**
   * Return the StockQuoter object created by the Constructor.
   *
   * @return A StockQuoter object created by this StockDistributor object.
   */
  ::Stock::StockQuoter_ptr provide_quoter_info (::Stock::Cookie *ck) override;

  /**
   * Get the notification_rate attribute.
   *
   * @return The notification_rate attribute.
   */
  ::CORBA::Long notification_rate () override;

  /**
   * Set the notification_rate attribute.
   *
   * @param notification_rate The value of notification_rate attribute that need to be set.
   */
  void notification_rate (::CORBA::Long notification_rate) override;

  /**
   * Set the active state of the StockDistributor object to true and create a thread
   * to publish the stock information to the Stock Broker clients.
   */
  void start () override;

  /**
   * Set the active state of StockDistributor object to false.
   */
  void stop () override;

  void shutdown () override;

  /// Callback for stock database.
  void operator () (std::vector <std::string> &stocks);

  int svn ();

  /// The map that stores the subscribed StockNameConsumer object.
  using CookieMap = std::map<std::string,
                             std::pair<Stock::StockNameConsumer_var,
                                       RTCORBA::Priority>>;

private:
  /// Notification rate for distributor in seconds.
  CORBA::Long rate_;

  CookieMap subscribers_list_;

  /// This lock serializes access to the subscriber list.
  ACE_RW_Thread_Mutex lock_;

  /// The active state of the  StockDistributor object.
  bool active_ {};

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
  StockQuoter_i () = default;

  /// Destructor.
  ~StockQuoter_i () override = default;

  StockQuoter_i (StockQuoter_i const &) = delete;
  StockQuoter_i (StockQuoter_i &&) = delete;
  StockQuoter_i &operator= (StockQuoter_i const &) = delete;
  StockQuoter_i &operator= (StockQuoter_i &&) = delete;

  /**
   * Get detailed stock information.
   *
   * @param stock_name The name of the stock whose detailed info is needed.
   * @throw ::Stock::Invalid_Stock
   */
  ::Stock::StockInfo *get_stock_info (const char *stock_name) override;
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
  explicit StockDistributorHome_i (CORBA::ORB_ptr orb);

  StockDistributorHome_i (StockDistributorHome_i const &) = delete;
  StockDistributorHome_i (StockDistributorHome_i &&) = delete;
  StockDistributorHome_i &operator= (StockDistributorHome_i const &) = delete;
  StockDistributorHome_i &operator= (StockDistributorHome_i &&) = delete;

  /// Destructor
  ~StockDistributorHome_i () override;

  /**
   * Return the StockDistributor object created by the Constructor.
   *
   * @return The StockDistributor object created by the Constructor.
   */
  ::Stock::StockDistributor_ptr create () override;

  int handle_signal (int signum,
                     siginfo_t * = nullptr,
                     ucontext_t * = nullptr) override;
private:
  void create_distributor ();

  /// Cached ORB used to shut down the system
  CORBA::ORB_var orb_;

  /// Cached POA created during object construction
  RTPortableServer::POA_var rt_poa_;

  /// The StockDistributor servant created by its home.
  PortableServer::ObjectId_var dist_id_;
};


// Define typedefs for our version of the stock database.
using Stock_Database_Singleton = ACE_Singleton<Stock_Database<StockDistributor_i>,
                                               TAO_SYNCH_MUTEX>;
#define STOCK_DATABASE Stock_Database_Singleton::instance()

#endif /* DISTRIBUTORI_H_  */
