// $Id$

#include "ace/SString.h"
#include "ace/OS_NS_string.h"
#include "ciao/CIAO_common.h"
#include "BMClosedED_exec.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::BMClosedED_exec_i::BMClosedED_exec_i ()
  : dataout_ (new ReadData_Impl(""))
{
}

/// Default destructor.
MyImpl::BMClosedED_exec_i::~BMClosedED_exec_i ()
{
  delete this->dataout_;
}

// Operations from HUDisplay::BMClosedED

BasicSP::CCM_ReadData_ptr
MyImpl::BMClosedED_exec_i::get_dataout ()
{
  return BasicSP::CCM_ReadData::_duplicate (this->dataout_);
}

void
MyImpl::BMClosedED_exec_i::push_in_avail (BasicSP::DataAvailable *)
{

  ACE_DEBUG ((LM_DEBUG,
              "BMClosedED - Doing computations \n"));
  
  ACE_DEBUG ((LM_DEBUG,
              "BMClosedED - Doing data fetch \n"));

  // Refresh position
  BasicSP::ReadData_var dat
    = this->context_->get_connection_datain ();

  if (CORBA::is_nil (dat.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "BMClosedED - got nil from get_connection \n"));

      throw CORBA::BAD_INV_ORDER ();
    }

  CORBA::String_var str =
    dat->get_data ();

  ACE_DEBUG ((LM_DEBUG,
              "BMClosedED - Display data is [%s] \n",
              str.in ()));

  if (ACE_OS::strcmp (str.in (), "BM DEVICE DATA") == 0)
    {
      this->dataout_->set_name ("BM CLOSED ED DATA");
    }

  // Notify others.
  BasicSP::DataAvailable_var event =
    new OBV_BasicSP::DataAvailable;

  this->context_->push_out_avail (event);
}

// Operations from Components::SessionComponent
void
MyImpl::BMClosedED_exec_i::set_session_context (
    Components::SessionContext_ptr ctx
  )
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::BMClosedED_exec_i::set_session_context\n"));

  this->context_ =
    BasicSP::CCM_BMClosedED_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    {
      throw CORBA::INTERNAL ();
    }
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::BMClosedED_exec_i::configuration_complete ()
{
}

void
MyImpl::BMClosedED_exec_i::ccm_activate ()
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::BMClosedED_exec_i::ccm_activate\n"));
}

void
MyImpl::BMClosedED_exec_i::ccm_passivate ()
{
  //  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG,
                "MyImpl::BMClosedED_exec_i::ccm_passivate\n"));
}

void
MyImpl::BMClosedED_exec_i::ccm_remove ()
{
  //  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG,
                "MyImpl::BMClosedED_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::BMClosedEDHome_exec_i::BMClosedEDHome_exec_i ()
{
}

/// Default dtor.
MyImpl::BMClosedEDHome_exec_i::~BMClosedEDHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::BMClosedEDHome_exec_i::create ()
{
  ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

  ACE_NEW_THROW_EX (
                    retval,
                    MyImpl::BMClosedED_exec_i,
                    ::CORBA::NO_MEMORY ());

  return retval;
}

extern "C" BMCLOSEDED_EXEC_Export ::Components::HomeExecutorBase_ptr
create_BasicSP_BMClosedEDHome_Impl (void)
{
  ::Components::HomeExecutorBase_ptr retval =
    ::Components::HomeExecutorBase::_nil ();

  ACE_NEW_RETURN (
                  retval,
                  MyImpl::BMClosedEDHome_exec_i,
                  ::Components::HomeExecutorBase::_nil ());

  return retval;
}
