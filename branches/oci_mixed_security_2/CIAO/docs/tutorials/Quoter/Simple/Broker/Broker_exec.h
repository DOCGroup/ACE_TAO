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
    const char * stock_name);

    virtual void
    stock_unsubscribe (
    const char * stock_name);

    virtual void
    push_notify_in (
    ::Stock::StockName *ev);

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (
    ::Components::SessionContext_ptr ctx);

    virtual void
    ciao_preactivate ();

    virtual void
    ciao_postactivate ();

    virtual void
    ccm_activate ();

    virtual void
    ccm_passivate ();

    virtual void
    ccm_remove ();

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
    create ();
  };

  extern "C" BROKER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createStockBrokerHome_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_BROKER_EXEC_H */

