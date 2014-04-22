// $Id$

#include "ace/SString.h"
#include "ace/OS_NS_string.h"
#include "BMClosedED_exec.h"

#define DISPLACEMENT 256

MyImpl::ReadData_Impl::ReadData_Impl (const char* name)
  : str_ (name)
{
}

MyImpl::ReadData_Impl::~ReadData_Impl (void)
{
}

void
MyImpl::ReadData_Impl::set_name (const char* name)
{
  this->str_ = name;
}

char *
MyImpl::ReadData_Impl::get_data (void)
{
  return CORBA::string_dup (this->str_.in());
}

//=================================================

/// Default constructor.
MyImpl::BMClosedED_exec_i::BMClosedED_exec_i (void)
  : dataout_ (new ReadData_Impl(""))
{
}

/// Default destructor.
MyImpl::BMClosedED_exec_i::~BMClosedED_exec_i (void)
{
  delete this->dataout_;
}

// Operations from HUDisplay::BMClosedED

BasicSP::CCM_ReadData_ptr
MyImpl::BMClosedED_exec_i::get_dataout (void)
{
  return BasicSP::CCM_ReadData::_duplicate (this->dataout_);
}

void
MyImpl::BMClosedED_exec_i::push_in_avail (BasicSP::DataAvailable *)
{

  ACE_DEBUG ((LM_EMERGENCY,
              "BMClosedED - Doing computations\n"));

  ACE_DEBUG ((LM_EMERGENCY,
              "BMClosedED - Doing data fetch\n"));

  // Refresh position
  BasicSP::ReadData_var dat
    = this->context_->get_connection_datain ();

  if (CORBA::is_nil (dat.in ()))
    {
      ACE_DEBUG ((LM_EMERGENCY,
                  "BMClosedED - got nil from get_connection\n"));

      throw CORBA::BAD_INV_ORDER ();
    }

  CORBA::String_var str =
    dat->get_data ();

  ACE_DEBUG ((LM_EMERGENCY,
              "BMClosedED - Display data is [%s]\n",
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
    Components::SessionContext_ptr ctx)
{
  ACE_DEBUG ((LM_EMERGENCY,
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
MyImpl::BMClosedED_exec_i::configuration_complete (void)
{
}

void
MyImpl::BMClosedED_exec_i::ccm_activate (void)
{
  ACE_DEBUG ((LM_EMERGENCY,
              "MyImpl::BMClosedED_exec_i::ccm_activate\n"));
}

void
MyImpl::BMClosedED_exec_i::ccm_passivate (void)
{
  //  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_EMERGENCY,
                "MyImpl::BMClosedED_exec_i::ccm_passivate\n"));
}

void
MyImpl::BMClosedED_exec_i::ccm_remove (void)
{
  //  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_EMERGENCY,
                "MyImpl::BMClosedED_exec_i::ccm_remove\n"));
}

extern "C" BMCLOSEDED_EXEC_Export ::Components::EnterpriseComponent_ptr
create_BasicSP_BMClosedED_Impl (void)
{
  ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

  ACE_NEW_RETURN (retval,
                  MyImpl::BMClosedED_exec_i,
                  ::Components::EnterpriseComponent::_nil ());

  return retval;
}

/// Default ctor.
MyImpl::BMClosedEDHome_exec_i::BMClosedEDHome_exec_i (void)
{
}

/// Default dtor.
MyImpl::BMClosedEDHome_exec_i::~BMClosedEDHome_exec_i (void)
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::BMClosedEDHome_exec_i::create (void)
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
