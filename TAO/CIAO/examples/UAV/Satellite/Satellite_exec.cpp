// $Id$

#include "CIAO_common.h"
#include "Satellite_exec.h"
#include "ace/High_Res_Timer.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::Satellite_exec_i::Satellite_exec_i (void)
  : str_ ("BM DEVICE DATA")
{

}

/// Default destructor.
MyImpl::Satellite_exec_i::~Satellite_exec_i ()
{
}

// Operations from supported interface(s)

void
MyImpl::Satellite_exec_i::alert (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  BBN_UAV::TargetLocated_var ev = new OBV_BBN_UAV::TargetLocated ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Satellite, pushing TargetLocated!\n")));

  
  ACE_hrtime_t start = ACE_OS::gethrtime ();
  this->context_->push_target_located (ev.in ()
                                       ACE_ENV_ARG_PARAMETER);
  ACE_hrtime_t end = ACE_OS::gethrtime ();

  ACE_DEBUG ((LM_DEBUG, "RTT = %i\n", end - start));
}

// Operations from Components::SessionComponent
void
MyImpl::Satellite_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Satellite_exec_i::set_session_context\n"));

  this->context_ =
    BBN_UAV::CCM_Satellite_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::Satellite_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Satellite_exec_i::ccm_activate\n"));
  char *argv[1] = { "Satellite_exec"};

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

}

void
MyImpl::Satellite_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Satellite_exec_i::ccm_passivate\n"));
}

void
MyImpl::Satellite_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Satellite_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::SatelliteHome_exec_i::SatelliteHome_exec_i ()
{
}

/// Default dtor.
MyImpl::SatelliteHome_exec_i::~SatelliteHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::SatelliteHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::Satellite_exec_i;
}


extern "C" SATELLITE_EXEC_Export ::Components::HomeExecutorBase_ptr
createSatelliteHome_Impl (void)
{
  return new MyImpl::SatelliteHome_exec_i;
}
