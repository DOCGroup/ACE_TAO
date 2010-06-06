// $Id$

#include "BMDevice_exec.h"

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
MyImpl::BMDevice_exec_i::BMDevice_exec_i (void)
  : data_read_ (new ReadData_Impl ("BM DEVICE DATA"))
{
}

/// Default destructor.
MyImpl::BMDevice_exec_i::~BMDevice_exec_i (void)
{
  delete this->data_read_;
}

BasicSP::CCM_ReadData_ptr
MyImpl::BMDevice_exec_i::get_data_read (void)
{
  return
    BasicSP::CCM_ReadData::_duplicate (this->data_read_);
}

void
MyImpl::BMDevice_exec_i::push_timeout_value (BasicSP::TimeOut *)
{
  // Notify others
  BasicSP::DataAvailable_var event =
    new OBV_BasicSP::DataAvailable;

  ACE_DEBUG ((LM_EMERGENCY,
              "BMDevice, received a timeout from EC\n"));

  this->context_->push_data_available (event);
}

// Operations from Components::SessionComponent
void
MyImpl::BMDevice_exec_i:: set_session_context (
  Components::SessionContext_ptr ctx)
{
  ACE_DEBUG ((LM_EMERGENCY,
              "MyImpl::BMDevice_exec_i::set_session_context\n"));

  this->context_ =
    BasicSP::CCM_BMDevice_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    {
      throw CORBA::INTERNAL ();
    }
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::BMDevice_exec_i::configuration_complete (void)
{
}

void
MyImpl::BMDevice_exec_i::ccm_activate (void)
{
  ACE_DEBUG ((LM_EMERGENCY,
              "MyImpl::BMDevice_exec_i::ccm_activate\n"));
}

void
MyImpl::BMDevice_exec_i::ccm_passivate (void)
{
  ACE_DEBUG ((LM_EMERGENCY,
              "MyImpl::BMDevice_exec_i::ccm_passivate\n"));
}

void
MyImpl::BMDevice_exec_i::ccm_remove (void)
{
  ACE_DEBUG ((LM_EMERGENCY,
              "MyImpl::BMDevice_exec_i::ccm_remove\n"));
}

extern "C" BMDEVICE_EXEC_Export ::Components::EnterpriseComponent_ptr
create_BasicSP_BMDevice_Impl (void)
{
  ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

  ACE_NEW_RETURN (retval,
                  MyImpl::BMDevice_exec_i,
                  ::Components::EnterpriseComponent::_nil ());

  return retval;
}

/// Default ctor.
MyImpl::BMDeviceHome_exec_i::BMDeviceHome_exec_i (void)
{
}

/// Default dtor.
MyImpl::BMDeviceHome_exec_i::~BMDeviceHome_exec_i (void)
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::BMDeviceHome_exec_i::create (void)
{
  ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

  ACE_NEW_THROW_EX (retval,
                    MyImpl::BMDevice_exec_i,
                    ::CORBA::NO_MEMORY ());

  return retval;
}


extern "C" BMDEVICE_EXEC_Export ::Components::HomeExecutorBase_ptr
create_BasicSP_BMDeviceHome_Impl (void)
{
  ::Components::HomeExecutorBase_ptr retval =
    ::Components::HomeExecutorBase::_nil ();

  ACE_NEW_RETURN (retval,
                  MyImpl::BMDeviceHome_exec_i,
                  ::Components::HomeExecutorBase::_nil ());

  return retval;
}

