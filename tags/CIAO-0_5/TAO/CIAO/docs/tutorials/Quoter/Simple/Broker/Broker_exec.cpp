//$Id$
/*
 * @file Broker_exec.cpp
 *
 * @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */

#include "Broker_exec.h"
#include "ciao/CIAO_common.h"


namespace CIDL_StockBroker_Impl
{
  //==================================================================
  // Component Executor Implementation Class:   StockBroker_exec_i
  //==================================================================

  StockBroker_exec_i::StockBroker_exec_i (void)
  {
  }

  StockBroker_exec_i::~StockBroker_exec_i (void)
  {
  }


  // Supported or inherited operations.

  void
  StockBroker_exec_i::stock_subscribe (
  const char * stock_name
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Stock::Invalid_Stock))
  {
    if ((strcmp (stock_name, "MSFT") == 0) || (strcmp (stock_name, "IBM") == 0))
    {
      std::set<std::string>::iterator iter = this->subscribed_stock_list_.find (stock_name);
      if (iter == this->subscribed_stock_list_.end ())
      {
        this->subscribed_stock_list_.insert (stock_name);
      }
    }
    else
    {
      ACE_THROW (Stock::Invalid_Stock ());
    }
  }

  void
  StockBroker_exec_i::stock_unsubscribe (
  const char * stock_name
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Stock::Invalid_Stock))
  {
    if ((strcmp (stock_name, "MSFT") == 0) || (strcmp (stock_name, "IBM") == 0))
    {
      std::set<std::string>::iterator iter = this->subscribed_stock_list_.find (stock_name);
      if (iter != this->subscribed_stock_list_.end ())
      {
        this->subscribed_stock_list_.erase (iter);
      }
    }
    else
    {
      ACE_THROW (Stock::Invalid_Stock ());
    }

  }


  void
  StockBroker_exec_i::push_notify_in (
  Stock::StockName *ev
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {

    ACE_DEBUG ((LM_INFO,
                "Broker - Got message from Distributor\n"));

    CORBA::String_var stock_name = CORBA::string_dup (ev->name ());

    // Retrieve stock information if the stock name is in the subscribed_stock_list
    if (this->subscribed_stock_list_.find (stock_name.in ()) != this->subscribed_stock_list_.end ())
    {
      Stock::StockQuoter_var quoter_obj = this->context_->get_connection_read_quoter (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (CORBA::is_nil (quoter_obj.in ()))
      {
        ACE_THROW (CORBA::BAD_PARAM ());
      }

      Stock::StockInfo_var info = quoter_obj->get_stock_info (stock_name.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Quoter - Current value of %s is %d\n",
                             stock_name.in (),
                             info->last));
    }
  }

  // Operations from Components::SessionComponent

  void
  StockBroker_exec_i::set_session_context (
  ::Components::SessionContext_ptr ctx
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
    this->context_ = StockBroker_Context::_narrow (ctx
                                                   ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (0 == this->context_)
    {
      ACE_THROW (CORBA::INTERNAL ());
    }
  }

  void
  StockBroker_exec_i::ciao_preactivate (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
  }

  void
  StockBroker_exec_i::ciao_postactivate (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
  }

  void
  StockBroker_exec_i::ccm_activate (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
  }

  void
  StockBroker_exec_i::ccm_passivate (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
  }

  void
  StockBroker_exec_i::ccm_remove (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
  }

  //==================================================================
  // Home Executor Implementation Class:   StockBrokerHome_exec_i
  //==================================================================

  StockBrokerHome_exec_i::StockBrokerHome_exec_i (void)
  {
  }

  StockBrokerHome_exec_i::~StockBrokerHome_exec_i (void)
  {
  }

  ::Components::EnterpriseComponent_ptr
  StockBrokerHome_exec_i::create (
  ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
  ::CORBA::SystemException,
  ::Components::CCMException))
  {
    ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
    retval,
    StockBroker_exec_i,
    CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

    return retval;
  }

  extern "C" BROKER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createStockBrokerHome_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
    ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
    retval,
    StockBrokerHome_exec_i,
    ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

