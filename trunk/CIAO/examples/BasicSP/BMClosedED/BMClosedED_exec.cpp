// $Id$

#include "ciao/CIAO_common.h"
#include "BMClosedED_exec.h"
#include "ace/SString.h"

#include "ace/OS_NS_string.h"


#define DISPLACEMENT 256

/// Default constructor.
MyImpl::BMClosedED_exec_i::BMClosedED_exec_i ()
{
}

/// Default destructor.
MyImpl::BMClosedED_exec_i::~BMClosedED_exec_i ()
{
}

// Operations from HUDisplay::BMClosedED

BasicSP::CCM_ReadData_ptr
MyImpl::BMClosedED_exec_i::get_dataout (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return BasicSP::CCM_ReadData::_duplicate (this);
}

void
MyImpl::BMClosedED_exec_i::push_in_avail (BasicSP::DataAvailable *
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "BMClosedED - Doing computations \n"));

      ACE_DEBUG ((LM_DEBUG,
                  "BMClosedED - Doing data fetch \n"));
    }

  // Refresh position
  BasicSP::ReadData_var dat
    = this->context_->get_connection_datain ();

  if (CORBA::is_nil (dat.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "BMClosedED - got nil from get_connection \n"));

      ACE_THROW (CORBA::BAD_INV_ORDER ());
    }

  CORBA::String_var str =
    dat->get_data ();

  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "BMClosedED - Display data is [%s] \n",
                  str.in ()));
    }

  if (ACE_OS::strcmp (str.in (), "BM DEVICE DATA") == 0)
    {
      this->str_ = CORBA::string_dup ("BM CLOSED ED DATA");
    }

  // Notify others.
  BasicSP::DataAvailable_var event =
    new OBV_BasicSP::DataAvailable;

  this->context_->push_out_avail (event
                                  ACE_ENV_ARG_PARAMETER);
}

// Operations from HUDisplay::position

char *
MyImpl::BMClosedED_exec_i::get_data (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->str_.inout ());
}

// Operations from Components::SessionComponent
void
MyImpl::BMClosedED_exec_i::set_session_context (
    Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::BMClosedED_exec_i::set_session_context\n"));
    }

  this->context_ =
    BasicSP::CCM_BMClosedED_Context::_narrow (ctx
                                              ACE_ENV_ARG_PARAMETER);

  if (CORBA::is_nil (this->context_.in ()))
    {
      ACE_THROW (CORBA::INTERNAL ());
    }
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::BMClosedED_exec_i::ciao_preactivate (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::BMClosedED_exec_i::ccm_activate (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "MyImpl::BMClosedED_exec_i::ccm_activate\n"));
    }

}

void
MyImpl::BMClosedED_exec_i::ciao_postactivate (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
}

void
MyImpl::BMClosedED_exec_i::ccm_passivate (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  //  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG,
                "MyImpl::BMClosedED_exec_i::ccm_passivate\n"));
}

void
MyImpl::BMClosedED_exec_i::ccm_remove (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  //  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG,
                "MyImpl::BMClosedED_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::BMClosedEDHome_exec_i::BMClosedEDHome_exec_i ()
{
}

/// Default dtor.
MyImpl::BMClosedEDHome_exec_i::~BMClosedEDHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::BMClosedEDHome_exec_i::create (void)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  Components::EnterpriseComponent_ptr tmp =
    Components::EnterpriseComponent::_nil ();
  ACE_NEW_THROW_EX (tmp,
		                MyImpl::BMClosedED_exec_i,
		                CORBA::NO_MEMORY ());
  return tmp;
}


extern "C" BMCLOSEDED_EXEC_Export ::Components::HomeExecutorBase_ptr
createBMClosedEDHome_Impl (void)
{
  return new MyImpl::BMClosedEDHome_exec_i;
}
