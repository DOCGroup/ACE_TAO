// $Id$

#include "Controller_exec.h"
#include "CIAO_common.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "ace/Env_Value_T.h"

/// Default constructor.
MyImpl::Controller_exec_i::Controller_exec_i ()
{
}

/// Default destructor.
MyImpl::Controller_exec_i::~Controller_exec_i ()
{
}

// Operations from Priority_Test::Controller
void
MyImpl::Controller_exec_i::start (CORBA::Long arg
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // This simply performs some measurements and print out the result.

  ACE_DEBUG ((LM_DEBUG, "PERFORM TEST\n"));

  ACE_UNUSED_ARG (arg);

  Priority_Test::Common_Ops_var device =
    this->context_->get_connection_worker (ACE_ENV_SINGLE_ARG_PARAMETER);

  const int niterations = 1000;
  const CORBA::Long work = arg;
  ACE_Sample_History history (niterations);

  ACE_hrtime_t test_start = ACE_OS::gethrtime ();
  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) device->do_work (work, 0 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      history.sample (now - start);
    }
  ACE_hrtime_t test_end = ACE_OS::gethrtime ();

  ACE_DEBUG ((LM_DEBUG, "test finished\n"));

  const int len = 1024;
  char title [len];

  ACE_OS::snprintf (title, len, "Total (work=%4d)", work);

  ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "done\n"));

  ACE_Env_Value<int> envar ("CIAO_DUMP_SAMPLE_HISTORY", 0);
  if (envar != 0)
    {
      history.dump_samples ("HISTORY", gsf);
    }

  ACE_Basic_Stats stats;
  history.collect_basic_stats (stats);
  stats.dump_results (title, gsf);

  ACE_Throughput_Stats::dump_throughput (title, gsf,
                                         test_end - test_start,
                                         stats.samples_count ());
}

void
MyImpl::Controller_exec_i::stop (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// Operations from Components::SessionComponent
void
MyImpl::Controller_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::Controller_exec_i::set_session_context\n"));

  this->context_ =
    Priority_Test::CCM_Controller_Context::_narrow (ctx
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::Controller_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::Controller_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::Controller_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::Controller_exec_i::ccm_activate\n"));
}

void
MyImpl::Controller_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::Controller_exec_i::ccm_passivate\n"));
}

void
MyImpl::Controller_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::Controller_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::Controller_Home_exec_i::Controller_Home_exec_i ()
{
}

/// Default dtor.
MyImpl::Controller_Home_exec_i::~Controller_Home_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::Controller_Home_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::Controller_exec_i;
}


extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
createControllerHome_Impl (void)
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "extern \"C\" ::createControllerHome_Impl\n"));
  return new MyImpl::Controller_Home_exec_i;
}
