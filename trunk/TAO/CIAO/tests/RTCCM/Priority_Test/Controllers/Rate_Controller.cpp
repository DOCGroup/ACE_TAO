// $Id$

#include "Rate_Controller.h"
#include "CIAO_common.h"

/// Allocate sample history to take up to 5 minutes of samples.
const int MyImpl::Rate_Controller_i::sample_time_ = 5;

/// Default constructor.
MyImpl::Rate_Controller_i::Rate_Controller_i (CORBA::Long hertz)
  : rate_ (hertz),
    pulser_ (this),
    sampler_ (0)
{
}

/// Default destructor.
MyImpl::Rate_Controller_i::~Rate_Controller_i ()
{
  delete this->sampler_;
}

// Operations from Priority_Test::Controller
void
MyImpl::Rate_Controller_i::start (CORBA::Long arg
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // This simply performs some measurements and print out the result.
  ACE_DEBUG ((LM_DEBUG, "PERFORM TEST (%dhz)\n", this->rate_));

  if (this->pulser_.active())
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  this->work_ = arg;
  this->start_time_ = ACE_OS::gethrtime ();
  delete this->sampler_;
#if defined (COLLECT_AND_DUMP_SAMPLE_HISTORY)
  size_t niterations = this->rate_ * 60 * MyImpl::Rate_Controller_i::sample_time_;
  this->sampler_ = new SAMPLER (niterations);
#else
  this->sampler_ = new SAMPLER;
#endif /* COLLECT_AND_DUMP_SAMPLE_HISTORY */

  // @@ Start the rate generator
  this->pulser_.start (this->rate_);
}

void
MyImpl::Rate_Controller_i::stop (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (! this->pulser_.active ())
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  // @@ stop the rate generator
  this->pulser_.stop ();
  ACE_hrtime_t stop_test = ACE_OS::gethrtime ();

  ACE_DEBUG ((LM_DEBUG, "test finished (%dhz)\n", this->rate_));

  const int len = 1024;
  char title [len];

  ACE_OS::snprintf (title, len, "Total (work=%4d @ %2dhz)",
                    this->work_,
                    this->rate_);
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();

  size_t count = 0;

#if defined (COLLECT_AND_DUMP_SAMPLE_HISTORY)
  this->sampler_->dump_samples (title, gsf);

  ACE_Basic_Stats stats;
  this->sampler_->collect_basic_stats (stats);
  stats.dump_results (title, gsf);
  count = stats.samples_count ();
#else
  this->sampler_->dump_results (title, gsf);
  count = this->sampler_->samples_count ();
#endif /* COLLECT_BASIC_STATS */

  ACE_Throughput_Stats::dump_throughput (title, gsf,
                                         stop_test - this->start_time_,
                                         count);
}

// Operations from Components::SessionComponent
void
MyImpl::Rate_Controller_i::set_session_context (Components::SessionContext_ptr ctx
                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::Rate_Controller_i::set_session_context (%dhz)\n",
                this->rate_));

  this->context_ =
    Priority_Test::CCM_Controller_Context::_narrow (ctx
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::Rate_Controller_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::Rate_Controller_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::Rate_Controller_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::Rate_Controller_i::ccm_activate (%dhz)\n",
                this->rate_));

  this->pulser_.open ();
}

void
MyImpl::Rate_Controller_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::Rate_Controller_i::ccm_passivate(%dhz)\n",
                this->rate_));

  this->pulser_.close ();
}

void
MyImpl::Rate_Controller_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::Rate_Controller_i::ccm_remove (%dhz)\n",
                this->rate_));
}

void
MyImpl::Rate_Controller_i::pulse (void)
{
  ACE_TRY_NEW_ENV
    {
      if (CIAO::debug_level () > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Pulse (%dhz)!\n",
                    this->rate_));

      ACE_hrtime_t start = ACE_OS::gethrtime ();

      Priority_Test::Common_Ops_var device =
        this->context_->get_connection_worker (ACE_ENV_SINGLE_ARG_PARAMETER);

      (void) device->do_work (this->work_, 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->sampler_->sample (now - start);
    }
  ACE_CATCHANY
    {
      // >= 0 meaning always print out the exception.
      if (CIAO::debug_level () >= 0)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "Rate_Controller pulse exception\n");
    }
  ACE_ENDTRY;
}

/// Default ctor.
MyImpl::Rate_Controller_Home_i::Rate_Controller_Home_i (CORBA::Long hertz)
  : hertz_ (hertz)
{
}

/// Default dtor.
MyImpl::Rate_Controller_Home_i::~Rate_Controller_Home_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::Rate_Controller_Home_i::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::Rate_Controller_i (this->hertz_);
}


extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
createControllerHome25_Impl (void)
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "extern \"C\" ::createControllerHome_Impl (25hz)\n"));
  return new MyImpl::Rate_Controller_Home_i (25);
}

extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
createControllerHome50_Impl (void)
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "extern \"C\" ::createControllerHome_Impl (50hz)\n"));
  return new MyImpl::Rate_Controller_Home_i (50);
}

extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
createControllerHome75_Impl (void)
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "extern \"C\" ::createControllerHome_Impl (75hz)\n"));
  return new MyImpl::Rate_Controller_Home_i (75);
}
