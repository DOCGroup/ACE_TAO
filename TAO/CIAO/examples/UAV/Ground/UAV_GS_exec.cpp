// $Id$

#include "CIAO_common.h"
#include "UAV_GS_exec.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::UAV_GS_exec_i::UAV_GS_exec_i (void)
  : str_ ("BM DEVICE DATA")
{

}

/// Default destructor.
MyImpl::UAV_GS_exec_i::~UAV_GS_exec_i ()
{
}

void
MyImpl::UAV_GS_exec_i::push_prepare_capture (BBN_UAV::PrepareCapture *ev
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Notify others
  BBN_UAV::StartCapture_var event = new OBV_BBN_UAV::StartCapture;

  ACE_DEBUG ((LM_DEBUG,
              "UAV_GS, received a PrepareCapture from CAOC \n"));

  this->context_->push_start_capture (event
                                      ACE_ENV_ARG_PARAMETER);

}

// Operations from Components::SessionComponent
void
MyImpl::UAV_GS_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UAV_GS_exec_i::set_session_context\n"));

  this->context_ =
    BBN_UAV::CCM_UAV_GS_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::UAV_GS_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UAV_GS_exec_i::ccm_activate\n"));
  char *argv[1] = { "UAV_GS_exec"};

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), BBN_UAV::PrepareCapture_init,
                               BBN_UAV::PrepareCapture);
}

void
MyImpl::UAV_GS_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UAV_GS_exec_i::ccm_passivate\n"));
}

void
MyImpl::UAV_GS_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UAV_GS_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::UAV_GSHome_exec_i::UAV_GSHome_exec_i ()
{
}

/// Default dtor.
MyImpl::UAV_GSHome_exec_i::~UAV_GSHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::UAV_GSHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::UAV_GS_exec_i;
}


extern "C" UAV_GS_EXEC_Export ::Components::HomeExecutorBase_ptr
createUAV_GSHome_Impl (void)
{
  return new MyImpl::UAV_GSHome_exec_i;
}
