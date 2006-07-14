// $Id$

#ifndef COMMONI_H_
#define COMMONI_H_

// local headers
#include "CommonS.h"
#include "BrokerS.h"

// ACE headers
#include <ace/Thread.h>

// TAO headers
#include <tao/RTCORBA/RTCORBA.h>

// STL headers
#include <map>
#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @@ Shanshan - Please document this code in doxygen style
// @@ Shanshan - Please remove ACE exception  macros.

class  Stock_Trigger_i
  : public virtual POA_Stock::Trigger,
    public virtual PortableServer::RefCountServantBase
{
public:
  // Constructor
  Stock_Trigger_i (void);

  // Destructor
  virtual ~Stock_Trigger_i (void);

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
};

class  Stock_StockNameConsumer_i
  : public virtual POA_Stock::StockNameConsumer,
    public virtual PortableServer::RefCountServantBase
{
public:
  // Constructor
  Stock_StockNameConsumer_i (Stock::StockBroker_ptr parent, const char *stock_name);

  // Destructor
  virtual ~Stock_StockNameConsumer_i (void);

  virtual
  void push_StockName (
      ::Stock::StockName * the_stockname
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException
    ));

private:
  // @@  Shanshan - We probably want to use a _var here instead of a
  // pointer, so the memory is managed.
  // Parent of the StockNameConsumer object.
  Stock::StockBroker_ptr parent_;

  // Name of the stock to listen.
  CORBA::String_var stock_name_;
};

class  Stock_StockQuoter_i
  : public virtual POA_Stock::StockQuoter,
    public virtual PortableServer::RefCountServantBase
{
public:
  // Constructor
  Stock_StockQuoter_i (void);

  // Destructor
  virtual ~Stock_StockQuoter_i (void);

  virtual
  ::Stock::StockInfo * get_stock_info (
      const char * stock_name
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      ::CORBA::SystemException,
      ::Stock::Invalid_Stock
    ));
};

#endif /* COMMONI_H_  */
