// $Id$

#ifndef DISTRIBUTORI_H_
#define DISTRIBUTORI_H_

// local headers
#include "Common_i.h"
#include "DistributorS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @@ Shanshan - Please remove the ACE exception macros from this
// code, ie the ACE_ENV_..., and replace the ACE_THROW_SPEC (( with
// throw (.

// @@ Shanshan - Please document this class using doxygen style
// comments.

class  Stock_StockDistributor_i
  : public virtual POA_Stock::StockDistributor,
    public virtual PortableServer::RefCountServantBase
{
public:
  // Constructor
  Stock_StockDistributor_i (CORBA::ORB_ptr orb, CORBA::PolicyList &policy_list);

  // Destructor
  virtual ~Stock_StockDistributor_i (void);

  virtual
  ::Stock::Cookie * subscribe_notifier (
      ::Stock::StockNameConsumer_ptr c,
      ::RTCORBA::Priority priority
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

  virtual
  ::Stock::StockNameConsumer_ptr unsubscribe_notifier (
      ::Stock::Cookie * ck
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

  virtual
  ::Stock::StockQuoter_ptr provide_quoter_info (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

  virtual
  ::CORBA::Long notification_rate (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

  virtual
  void notification_rate (
      ::CORBA::Long notification_rate
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

  virtual
  void start (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

  virtual
  void stop (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

private:
  /// Notification rate for distributor in milliseconds.
  CORBA::Long rate_;

  typedef std::map <Stock::Cookie *,
                    std::pair <Stock::StockNameConsumer_ptr,
                               RTCORBA::Priority> > CookieMap;

  CookieMap subscribers_list_;

  ACE_hthread_t _thread;

  ACE_thread_t _thread_id;

  RTCORBA::Current_var current_;

  Stock_StockQuoter_i *quoter_;

  RTCORBA::Mutex_var list_mutex_;

  bool active_;

  RTCORBA::RTORB_var rt_orb_;

  // @@ Shanshan - Please use ACE_Task instead of doing threading manually
  static ACE_THR_FUNC_RETURN _thread_func (void *param);
};

class  Stock_StockDistributorHome_i
  : public virtual POA_Stock::StockDistributorHome,
    public virtual PortableServer::RefCountServantBase
{
public:
  // Constructor
  Stock_StockDistributorHome_i (CORBA::ORB_ptr orb);

  // Destructor
  virtual ~Stock_StockDistributorHome_i (void);

  virtual
  ::Stock::StockDistributor_ptr create (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

private:
  Stock_StockDistributor_i *distributor_;
};

#endif /* DISTRIBUTORI_H_  */
