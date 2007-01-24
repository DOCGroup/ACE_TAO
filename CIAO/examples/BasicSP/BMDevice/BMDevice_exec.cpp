// $Id$

#include "ciao/CIAO_common.h"
#include "BMDevice_exec.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::BMDevice_exec_i::BMDevice_exec_i (void)
  : str_ ("BM DEVICE DATA")
{

}

/// Default destructor.
MyImpl::BMDevice_exec_i::~BMDevice_exec_i ()
{
}

BasicSP::CCM_ReadData_ptr
MyImpl::BMDevice_exec_i::get_data_read (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return BasicSP::CCM_ReadData::_duplicate (this);
}

void
MyImpl::BMDevice_exec_i::push_timeout (BasicSP::TimeOut *
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Nitify others
  BasicSP::DataAvailable_var event = new OBV_BasicSP::DataAvailable;

  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "BMDevice, received a timeout from EC \n"));
    }

  this->context_->push_data_available (event
                                       ACE_ENV_ARG_PARAMETER);
}

char *
MyImpl::BMDevice_exec_i::data_read (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->str_);
}

char *
MyImpl::BMDevice_exec_i::get_data (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->data_read ();
}

// Operations from Components::SessionComponent
void
MyImpl::BMDevice_exec_i::set_session_context (
    Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::BMDevice_exec_i::set_session_context\n"));
    }

  this->context_ =
    BasicSP::CCM_BMDevice_Context::_narrow (ctx
                                            ACE_ENV_ARG_PARAMETER);

  if (CORBA::is_nil (this->context_.in ()))
    {
      ACE_THROW (CORBA::INTERNAL ());
    }
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::BMDevice_exec_i::ciao_preactivate (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::BMDevice_exec_i::ccm_activate (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::BMDevice_exec_i::ccm_activate\n"));
    }

}

void
MyImpl::BMDevice_exec_i::ciao_postactivate (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::BMDevice_exec_i::ccm_passivate (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::BMDevice_exec_i::ccm_passivate\n"));
    }
}

void
MyImpl::BMDevice_exec_i::ccm_remove (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::BMDevice_exec_i::ccm_remove\n"));
    }
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
MyImpl::BMDeviceHome_exec_i::create (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  Components::EnterpriseComponent_ptr tmp= 0;
  ACE_NEW_THROW_EX (tmp,
                    MyImpl::BMDevice_exec_i,
		                CORBA::NO_MEMORY ());

  return tmp;
}


extern "C" BMDEVICE_EXEC_Export ::Components::HomeExecutorBase_ptr
createBMDeviceHome_Impl (void)
{
  return new MyImpl::BMDeviceHome_exec_i;
}
