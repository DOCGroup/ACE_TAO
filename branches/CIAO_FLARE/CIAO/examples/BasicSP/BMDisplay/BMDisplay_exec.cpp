// $Id$

#include "ciao/CIAO_common.h"
#include "BMDisplay_exec.h"

/// Default constructor.
MyImpl::BMDisplay_exec_i::BMDisplay_exec_i ()
{
}

/// Default destructor.
MyImpl::BMDisplay_exec_i::~BMDisplay_exec_i ()
{
}

// Operations from BasicSP::BMDisplay

void
MyImpl::BMDisplay_exec_i::push_data_ready (
    BasicSP::DataAvailable *ev
  )
{
  ACE_UNUSED_ARG (ev);

  ACE_DEBUG ((LM_DEBUG,
              "BMDisplay - Doing computations \n"));
  
  ACE_DEBUG ((LM_DEBUG,
              "BMDisplay - Doing data fetch \n"));
  // Refresh position
  BasicSP::ReadData_var dat
    = this->context_->get_connection_comp_data ();

  if (CORBA::is_nil (dat.in ()))
    throw CORBA::BAD_INV_ORDER ();

  CORBA::String_var str =
    dat->get_data ();

  ACE_DEBUG ((LM_DEBUG,
              "BMDisplay - Display data is [%s] \n",
              str.in ()));

}

// Operations from Components::SessionComponent
void
MyImpl::BMDisplay_exec_i::set_session_context (
    Components::SessionContext_ptr ctx
  )
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::BMDisplay_exec_i::set_session_context\n"));
  this->context_ =
    BasicSP::CCM_BMDisplay_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    throw CORBA::INTERNAL ();
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::BMDisplay_exec_i::configuration_complete (
  )
{
}

void
MyImpl::BMDisplay_exec_i::ccm_activate ()
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::BMDisplay_exec_i::ccm_activate\n"));
}

void
MyImpl::BMDisplay_exec_i::ccm_passivate (
  )
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::BMDisplay_exec_i::ccm_passivate\n"));
}

void
MyImpl::BMDisplay_exec_i::ccm_remove ()
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::BMDisplay_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::BMDisplayHome_exec_impl::BMDisplayHome_exec_impl ()
{
}

/// Default dtor.
MyImpl::BMDisplayHome_exec_impl::~BMDisplayHome_exec_impl ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::BMDisplayHome_exec_impl::create ()
{
  ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

  ACE_NEW_THROW_EX (
                    retval,
                    MyImpl::BMDisplay_exec_i,
                    ::CORBA::NO_MEMORY ());

  return retval;
}

extern "C" BMDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
create_BasicSP_BMDisplayHome_Impl (void)
{
  ::Components::HomeExecutorBase_ptr retval =
    ::Components::HomeExecutorBase::_nil ();

  ACE_NEW_RETURN (
                  retval,
                  MyImpl::BMDisplayHome_exec_impl,
                  ::Components::HomeExecutorBase::_nil ());

  return retval;
}
