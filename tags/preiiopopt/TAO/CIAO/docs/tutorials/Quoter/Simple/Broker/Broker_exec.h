//$Id$
/*
 * @file Broker_exec.h
 *
 * @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */


#ifndef CIAO_BROKER_EXEC_H
#define CIAO_BROKER_EXEC_H

#include /**/ "ace/pre.h"

#include "Broker_svnt.h"
#include "BrokerEC.h"
#include "Broker_exec_export.h"

#include <set>
#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIDL_StockBroker_Impl
{

  /**
   * @class StockBroker_exec_i
   *
   * @brief Executor implementation
   *
   * This class implements Stock::StockBroker component
   */
  class BROKER_EXEC_Export StockBroker_exec_i
  : public virtual StockBroker_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
    public:
    StockBroker_exec_i (void);
    virtual ~StockBroker_exec_i (void);


    // Supported or inherited operations.

    virtual void
    stock_subscribe (
    const char * stock_name
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Stock::Invalid_Stock));

    virtual void
    stock_unsubscribe (
    const char * stock_name
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Stock::Invalid_Stock));

    virtual void
    push_notify_in (
    ::Stock::StockName *ev
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (
    ::Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ciao_preactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ciao_postactivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ccm_activate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ccm_passivate (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    virtual void
    ccm_remove (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));

    protected:
    StockBroker_Context *context_;

    private:
    std::set<std::string> subscribed_stock_list_;
  };


  /**
   * @class StockBrokerHome_exec_i
   *
   * @brief Executor implementation
   *
   * This class implements Stock::StockBrokerHome component
   */
  class BROKER_EXEC_Export StockBrokerHome_exec_i
  : public virtual StockBrokerHome_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
    public:
    StockBrokerHome_exec_i (void);
    virtual ~StockBrokerHome_exec_i (void);

    virtual ::Components::EnterpriseComponent_ptr
    create (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
    ::CORBA::SystemException,
    ::Components::CCMException));
  };

  extern "C" BROKER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createStockBrokerHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_BROKER_EXEC_H */

