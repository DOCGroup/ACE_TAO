// $Id$

#include "CIAO_common.h"
#include "Engagement_exec.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::Engagement_exec_i::Engagement_exec_i (void)
  : str_ ("BM DEVICE DATA")
{

}

/// Default destructor.
MyImpl::Engagement_exec_i::~Engagement_exec_i ()
{
}

void
MyImpl::Engagement_exec_i::push_prepare_engage (BBN_UAV::PrepareEngage *ev
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Notify others
  BBN_UAV::StartEngage_var event1 = new OBV_BBN_UAV::StartEngage;
  BBN_UAV::PrepareMove_var event2 = new OBV_BBN_UAV::PrepareMove;

  ACE_DEBUG ((LM_DEBUG,
              "Engagement, received a PrepareEngage from CAOC \n"));
  
  this->context_->push_start_engage (event1
                                     ACE_ENV_ARG_PARAMETER);

  this->context_->push_prepare_move (event2
                                     ACE_ENV_ARG_PARAMETER);
}

// Operations from Components::SessionComponent
void
MyImpl::Engagement_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Engagement_exec_i::set_session_context\n"));

  this->context_ =
    BBN_UAV::CCM_Engagement_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::Engagement_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Engagement_exec_i::ccm_activate\n"));
  char *argv[1] = { "Engagement_exec"};

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), BBN_UAV::PrepareEngage_init,
                               BBN_UAV::PrepareEngage);
}

void
MyImpl::Engagement_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Engagement_exec_i::ccm_passivate\n"));
}

void
MyImpl::Engagement_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Engagement_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::EngagementHome_exec_i::EngagementHome_exec_i ()
{
}

/// Default dtor.
MyImpl::EngagementHome_exec_i::~EngagementHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::EngagementHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::Engagement_exec_i;
}


extern "C" ENGAGEMENT_EXEC_Export ::Components::HomeExecutorBase_ptr
createEngagementHome_Impl (void)
{
  return new MyImpl::EngagementHome_exec_i;
}
