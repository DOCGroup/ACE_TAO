// $Id$

#include "Consumer_exec.h"

#include <fstream>
#include <iostream>

/// Default constructor.
Consumer_Impl::Consumer_exec_i::Consumer_exec_i (void)
  : event_count_ (0)
{
}

/// Default destructor.
Consumer_Impl::Consumer_exec_i::~Consumer_exec_i ()
{
}

void
Consumer_Impl::Consumer_exec_i::push_timeout (EC_Benchmark::TimeOut *
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ////ACE_DEBUG ((LM_DEBUG, "==============Consumer, received a timeout from Producer \n"));

  // Benchmarking: record the timestamp
  ACE_hrtime_t end = ACE_OS::gethrtime ();
  
  this->end_time_[this->event_count_] = end; // cache the end time

  std::cerr << end << std::endl;

  this->event_count_++;

  //std::cerr << this->event_count_ << std::endl;

  if (this->event_count_ >= niterations)
    std::cerr << "Benchmarking finished\n";
}

// Operations from Components::SessionComponent
void
Consumer_Impl::Consumer_exec_i::set_session_context (
    Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Consumer_Impl::Consumer_exec_i::set_session_context\n"));

  this->context_ =
    Consumer_Impl::Consumer_Exec_Context::_narrow (ctx
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
      ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
Consumer_Impl::Consumer_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Consumer_Impl::Consumer_exec_i::ciao_preactivate\n"));
}

void
Consumer_Impl::Consumer_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Consumer_Impl::Consumer_exec_i::ccm_activate\n"));
}

void
Consumer_Impl::Consumer_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Consumer_Impl::Consumer_exec_i::ciao_postactivate\n"));
}

void
Consumer_Impl::Consumer_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "done\n"));
  std::cerr << "global scale factor is: " << gsf << std::endl;

  std::ifstream producer_in;
  producer_in.open ("begin.dat");

  std::ofstream consumer_out;
  consumer_out.open ("end.dat");

  std::ofstream delta_out;
  delta_out.open ("delta.dat");

  for (int i = 0; i < niterations; ++i)
    {
      ACE_hrtime_t begin;
      producer_in >> begin;
      consumer_out << this->end_time_[i] << "\n";
      delta_out << (this->end_time_[i] -begin)/gsf << "\n";
    }

  producer_in.close ();
  consumer_out.close ();
  delta_out.close ();

  ACE_DEBUG ((LM_DEBUG, "Consumer_Impl::Consumer_exec_i::ccm_passivate\n"));
}

void
Consumer_Impl::Consumer_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Consumer_Impl::Consumer_exec_i::ccm_remove\n"));
}

//========================================================

/// Default ctor.
Consumer_Impl::ConsumerHome_exec_i::ConsumerHome_exec_i ()
{
}

/// Default dtor.
Consumer_Impl::ConsumerHome_exec_i::~ConsumerHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
Consumer_Impl::ConsumerHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  Components::EnterpriseComponent_ptr tmp= 0;
  ACE_NEW_THROW_EX (tmp,
                    Consumer_Impl::Consumer_exec_i,
		                CORBA::NO_MEMORY ());

  return tmp;
}


extern "C" CONSUMER_EXEC_Export ::Components::HomeExecutorBase_ptr
createConsumerHome_Impl (void)
{
  return new Consumer_Impl::ConsumerHome_exec_i;
}
