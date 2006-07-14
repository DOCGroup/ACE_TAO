// $Id$

#ifndef BROKERI_H_
#define BROKERI_H_

// local headers
#include "Common_i.h"
#include "BrokerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @@ Shanshan - Please document this code in doxygen style
// @@ Shanshan - Please remove ACE exception macros.

class  Stock_StockBroker_i
  : public virtual POA_Stock::StockBroker,
    public virtual PortableServer::RefCountServantBase
{
public:
  // Constructor
  Stock_StockBroker_i (CORBA::ORB_ptr orb,
                       const char *stock_name,
                       RTCORBA::Priority priority);

  // Destructor
  virtual ~Stock_StockBroker_i (void);

  virtual
  ::Stock::StockNameConsumer_ptr get_consumer_notifier (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

  virtual
  void connect_quoter_info (
      ::Stock::StockQuoter_ptr c
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

  virtual
  ::Stock::StockQuoter_ptr disconnect_quoter_info (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

  virtual
  ::Stock::StockQuoter_ptr get_connection_quoter_info (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

private:
  Stock::StockQuoter_var quoter_;

  // @@ Shanshan - Please use an auto_ptr here to manage the memory for you.
  Stock_StockNameConsumer_i *consumer_;
};

class  Stock_StockBrokerHome_i
  : public virtual POA_Stock::StockBrokerHome,
    public virtual PortableServer::RefCountServantBase
{
public:
  // Constructor
  Stock_StockBrokerHome_i (CORBA::ORB_ptr orb,
                           const char *stock_name,
                           RTCORBA::Priority priority);

  // Destructor
  virtual ~Stock_StockBrokerHome_i (void);

  virtual
  ::Stock::StockBroker_ptr create (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

private:
  // @@ Shanshan - Please use an auto_ptr here to manage the memory for you
  Stock_StockBroker_i *broker_;
};

#endif /* BROKERI_H_  */
