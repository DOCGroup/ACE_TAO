// $Id$

#include "CIAO_common.h"
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
MyImpl::BMDevice_exec_i::get_data_read (ACE_ENV_SINGLE_ARG_DECL)
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

  ACE_DEBUG ((LM_DEBUG,
              "BMDevice, received a timeout from EC \n"));

  this->context_->push_data_available (event
                                       ACE_ENV_ARG_PARAMETER);
}

char *
MyImpl::BMDevice_exec_i::data_read (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->str_;
}

char *
MyImpl::BMDevice_exec_i::get_data (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->data_read (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// Operations from Components::SessionComponent
void
MyImpl::BMDevice_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMDevice_exec_i::set_session_context\n"));

  this->context_ =
    BasicSP::CCM_BMDevice_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::BMDevice_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMDevice_exec_i::ccm_activate\n"));
  char *argv[1] = { "BMDevice_exec"};

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), BasicSP::TimeOut_init,
                               BasicSP::TimeOut);
}

void
MyImpl::BMDevice_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMDevice_exec_i::ccm_passivate\n"));
}

void
MyImpl::BMDevice_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMDevice_exec_i::ccm_remove\n"));
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
MyImpl::BMDeviceHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::BMDevice_exec_i;
}


extern "C" BMDEVICE_EXEC_Export ::Components::HomeExecutorBase_ptr
createBMDeviceHome_Impl (void)
{
  return new MyImpl::BMDeviceHome_exec_i;
}
