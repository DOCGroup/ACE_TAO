// $Id$

#include "ciao/CIAO_common.h"
#include "BMDevice_exec.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::BMDevice_exec_i::BMDevice_exec_i (void)
  : data_read_ (new ReadData_Impl("BM DEVICE DATA"))
{

}

/// Default destructor.
MyImpl::BMDevice_exec_i::~BMDevice_exec_i ()
{
  delete this->data_read_;
}

BasicSP::CCM_ReadData_ptr
MyImpl::BMDevice_exec_i::get_data_read ()
{
  return BasicSP::CCM_ReadData::_duplicate (this->data_read_);
}

void
MyImpl::BMDevice_exec_i::push_timeout (BasicSP::TimeOut *)
{
  // Notify others
  BasicSP::DataAvailable_var event = new OBV_BasicSP::DataAvailable;

  ACE_DEBUG ((LM_DEBUG,
              "BMDevice, received a timeout from EC \n"));

  this->context_->push_data_available (event);
}

// Operations from Components::SessionComponent
void
MyImpl::BMDevice_exec_i:: set_session_context (Components::SessionContext_ptr ctx)
{
  ACE_DEBUG ((LM_DEBUG,
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
MyImpl::BMDevice_exec_i::configuration_complete ()
{
}

void
MyImpl::BMDevice_exec_i::ccm_activate ()
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::BMDevice_exec_i::ccm_activate\n"));
}

void
MyImpl::BMDevice_exec_i::ccm_passivate ()
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::BMDevice_exec_i::ccm_passivate\n"));
}

void
MyImpl::BMDevice_exec_i::ccm_remove ()
{
  ACE_DEBUG ((LM_DEBUG,
              "MyImpl::BMDevice_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::BMDeviceHome_exec_i::BMDeviceHome_exec_i ()
{
}

/// Default dtor.
MyImpl::BMDeviceHome_exec_i::~BMDeviceHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::BMDeviceHome_exec_i::create ()
{
  ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

  ACE_NEW_THROW_EX (
                    retval,
                    MyImpl::BMDevice_exec_i,
                    ::CORBA::NO_MEMORY ());

  return retval;
}


extern "C" BMDEVICE_EXEC_Export ::Components::HomeExecutorBase_ptr
create_BasicSP_BMDeviceHome_Impl (void)
{
  ::Components::HomeExecutorBase_ptr retval =
    ::Components::HomeExecutorBase::_nil ();

  ACE_NEW_RETURN (
                  retval,
                  MyImpl::BMDeviceHome_exec_i,
                  ::Components::HomeExecutorBase::_nil ());

  return retval;
}

