// $Id$

#include "CIAO_common.h"
#include "CAOC_exec.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::CAOC_exec_i::CAOC_exec_i (void)
  : str_ ("BM DEVICE DATA")
{

}

/// Default destructor.
MyImpl::CAOC_exec_i::~CAOC_exec_i ()
{
}

void
MyImpl::CAOC_exec_i::push_target_located (BBN_UAV::TargetLocated *ev
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Notify others
  BBN_UAV::PrepareCapture_var event = new OBV_BBN_UAV::PrepareCapture;

  ACE_DEBUG ((LM_DEBUG,
              "CAOC, received a TargetLocated from Satellite \n"));

  this->context_->push_prepare_capture (event
                                       ACE_ENV_ARG_PARAMETER);
}

// Operations from Components::SessionComponent
void
MyImpl::CAOC_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::CAOC_exec_i::set_session_context\n"));

  this->context_ =
    BBN_UAV::CCM_CAOC_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::CAOC_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::CAOC_exec_i::ccm_activate\n"));
  char *argv[1] = { "CAOC_exec"};

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), BBN_UAV::TargetLocated_init,
                               BBN_UAV::TargetLocated);
}

void
MyImpl::CAOC_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::CAOC_exec_i::ccm_passivate\n"));
}

void
MyImpl::CAOC_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::CAOC_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::CAOCHome_exec_i::CAOCHome_exec_i ()
{
}

/// Default dtor.
MyImpl::CAOCHome_exec_i::~CAOCHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::CAOCHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::CAOC_exec_i;
}


extern "C" CAOC_EXEC_Export ::Components::HomeExecutorBase_ptr
createCAOCHome_Impl (void)
{
  return new MyImpl::CAOCHome_exec_i;
}
