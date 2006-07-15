// $Id$

/**
 * @file Distributor_i.h
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 */

#ifndef DISTRIBUTORI_H_
#define DISTRIBUTORI_H_

// ACE headers
#include "ace/Task.h"

// local headers
#include "Common_i.h"
#include "DistributorS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Stock_StockDistributor_i
 * @brief This class defined the Stock Distributor server.
 */
class  Stock_StockDistributor_i
  : public ACE_Task_Base,
    public virtual POA_Stock::StockDistributor,
    public virtual PortableServer::RefCountServantBase
{
public:
  /**
   * Constructor.
   *
   * @param orb
   * @param policy_list The policy list use to create StockQuoter_POA.
   */
  Stock_StockDistributor_i (CORBA::ORB_ptr orb, CORBA::PolicyList &policy_list);

  /// Destructor
  virtual ~Stock_StockDistributor_i (void);

  /**
   * Insert StockNameConsumer object and its priority into the StockDistributor 
   * object's subscribers' map.
   *
   * @param c The StockNameConsumer object to be inserted into the StockDistributor 
             object's subscribers' map.
   * @param priority The priority of the StockNameConsumer object.
   * @return A Cookie object that records this subscription.
   */
  virtual ::Stock::Cookie * subscribe_notifier (::Stock::StockNameConsumer_ptr c, ::RTCORBA::Priority priority)
    throw (::CORBA::SystemException);

  /**
   * Erase the StockNameConsumer object from the StockDistributor object's subscribers' map.
   * 
   * @param ck The Cookie object that records the subscription.
   * @param The reference of the StockNameConsumer object that has been erased.
   */
  virtual ::Stock::StockNameConsumer_ptr unsubscribe_notifier (::Stock::Cookie *ck)
    throw (::CORBA::SystemException);

  /**
   * Return the StockQuoter object created by the Constructor.
   *
   * @return A StockQuoter object created by this StockDistributor object.
   */
  virtual ::Stock::StockQuoter_ptr provide_quoter_info ()
    throw (::CORBA::SystemException);

  /**
   * Get the notification_rate attribute.
   *
   * @return The notification_rate attribute.
   */
  virtual ::CORBA::Long notification_rate ()
    throw (::CORBA::SystemException);

  /**
   * Set the notification_rate attribute.
   *
   * @param notification_rate The value of notification_rate attribute that need to be set.
   */
  virtual void notification_rate (::CORBA::Long notification_rate)
    throw (::CORBA::SystemException);

  /**
   * Set the active state of the StockDistributor object to true and create a thread 
   * to publish the stock information to the Stock Broker clients.
   */
  virtual void start ()
    throw (::CORBA::SystemException);

  /**
   * Set the active state of StockDistributor object to false.
   */
  virtual void stop ()
    throw (::CORBA::SystemException);

private:
  virtual int svc (void);

  /// Notification rate for distributor in milliseconds.
  CORBA::Long rate_;

  /// The map that stores the subscribed StockNameConsumer object.
  typedef std::map <Stock::Cookie *,
                    std::pair <Stock::StockNameConsumer_ptr,
                               RTCORBA::Priority> > CookieMap;
  CookieMap subscribers_list_;

  /// The StockQuoter object created by this StockDistributor object.
  Stock_StockQuoter_i *quoter_;

  /// RTCORBA mutex.
  ACE_Thread_Mutex lock_;

  /// The active state of the  StockDistributor object.
  bool active_;
};

/**
 * @class Stock_StockDistributorHome_i
 * @brief This class defined the Stock Distributor home.
 */
class  Stock_StockDistributorHome_i
  : public virtual POA_Stock::StockDistributorHome,
    public virtual PortableServer::RefCountServantBase
{
public:
  /**
   * Constructor.
   * Register the necessary factories and mappings with the specified orb and 
   * Create a new instance of the StockDistributor object.
   *
   * @param orb
   */
  Stock_StockDistributorHome_i (CORBA::ORB_ptr orb);

  /// Destructor
  virtual ~Stock_StockDistributorHome_i (void);

  /**
   * Return the StockDistributor object created by the Constructor.
   * 
   * @return The StockDistributor object created by the Constructor.
   */
  virtual ::Stock::StockDistributor_ptr create ()
    throw (::CORBA::SystemException);

private:
  /// The StockDistributor object created by its home.
  Stock_StockDistributor_i *distributor_;
};

#endif /* DISTRIBUTORI_H_  */
