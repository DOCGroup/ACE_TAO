// $Id$

#include "Producer_exec.h"

#include <fstream>
#include "ace/High_Res_Timer.h"
//=================================================================

Producer_Impl::Producer_exec_i::Producer_exec_i ()
{
}

Producer_Impl::Producer_exec_i::~Producer_exec_i ()
{
}

// Operations from supported interface(s)

void
Producer_Impl::Producer_exec_i::start (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  EC_Benchmark::TimeOut_var ev = new OBV_EC_Benchmark::TimeOut ();

  ////ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Producer pushing timeout event!\n")));

  ACE_hrtime_t start = ACE_OS::gethrtime ();
    
  this->context_->push_timeout (ev.in ()
                                ACE_ENV_ARG_PARAMETER);  
  
  std::ofstream producer_out;

  producer_out.open ("begin.dat", ios::app);

  producer_out << start << "\n";
}

// Operations from Components::SessionComponent

void
Producer_Impl::Producer_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Producer_Impl::EC_exec_i::set_session_context\n"));

  this->context_ =
    Producer_Impl::Producer_Exec_Context::_narrow (ctx
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.

}

void
Producer_Impl::Producer_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Producer_Impl::Producer_exec_i::ciao_preactivate\n"));
}

void
Producer_Impl::Producer_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Producer_Impl::Producer_exec_i::ccm_activate\n"));
}

void
Producer_Impl::Producer_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Producer_Impl::Producer_exec_i::ciao_postactivate\n"));
}

void
Producer_Impl::Producer_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Producer_Impl::Producer_exec_i::ccm_passivate\n"));
}

void
Producer_Impl::Producer_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Producer_Impl::Producer_exec_i::ccm_remove\n"));
}

//========================================================

Producer_Impl::ProducerHome_exec_i::ProducerHome_exec_i ()
{
}

Producer_Impl::ProducerHome_exec_i::~ProducerHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
Producer_Impl::ProducerHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  Components::EnterpriseComponent_ptr tmp= 0;
  ACE_NEW_THROW_EX (tmp,
                    Producer_Impl::Producer_exec_i,
		                CORBA::NO_MEMORY ());

  return tmp;
}


extern "C" PRODUCER_EXEC_Export ::Components::HomeExecutorBase_ptr
createProducerHome_Impl (void)
{
  return new Producer_Impl::ProducerHome_exec_i ();
}
