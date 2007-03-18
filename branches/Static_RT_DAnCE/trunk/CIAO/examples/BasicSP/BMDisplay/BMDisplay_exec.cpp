// $Id$

#include "ciao/CIAO_common.h"
#include "BMDisplay_exec.h"

/// Default constructor.
MyImpl::BMDisplay_exec_impl::BMDisplay_exec_impl ()
{
}

/// Default destructor.
MyImpl::BMDisplay_exec_impl::~BMDisplay_exec_impl ()
{
}

// Operations from BasicSP::BMDisplay

void
MyImpl::BMDisplay_exec_impl::push_data_ready (
    BasicSP::DataAvailable *ev
  )
{
  ACE_UNUSED_ARG (ev);

  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "BMDisplay - Doing computations \n"));

      ACE_DEBUG ((LM_DEBUG,
                  "BMDisplay - Doing data fetch \n"));
    }

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
MyImpl::BMDisplay_exec_impl::set_session_context (
    Components::SessionContext_ptr ctx
  )
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::BMDisplay_exec_impl::set_session_context\n"));
    }

  this->context_ =
    BasicSP::CCM_BMDisplay_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    throw CORBA::INTERNAL ();
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::BMDisplay_exec_impl::ciao_preactivate (
  )
{
}

void
MyImpl::BMDisplay_exec_impl::ccm_activate ()
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::BMDisplay_exec_impl::ccm_activate\n"));
    }

}

void
MyImpl::BMDisplay_exec_impl::ciao_postactivate (
  )
{
}

void
MyImpl::BMDisplay_exec_impl::ccm_passivate (
  )
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::BMDisplay_exec_impl::ccm_passivate\n"));
    }
}

void
MyImpl::BMDisplay_exec_impl::ccm_remove ()
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::BMDisplay_exec_impl::ccm_remove\n"));
    }
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
  return new MyImpl::BMDisplay_exec_impl;
}


extern "C" BMDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
createBMDisplayHome_Impl (void)
{
  return new MyImpl::BMDisplayHome_exec_impl;
}
