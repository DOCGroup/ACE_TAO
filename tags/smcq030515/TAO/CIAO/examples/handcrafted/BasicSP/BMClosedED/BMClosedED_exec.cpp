// $Id$

#include "BMClosedED_exec.h"

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
MyImpl::BMClosedED_exec_i::get_dataout (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return BasicSP::CCM_ReadData::_duplicate (this);
}

void
MyImpl::BMClosedED_exec_i::push_in_avail (BasicSP::DataAvailable *
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  ACE_DEBUG ((LM_DEBUG,
              "BMClosedED - Doing computations \n"));

  ACE_DEBUG ((LM_DEBUG,
              "BMClosedED - Doing data fetch \n"));

  // Refresh position
  BasicSP::ReadData_var dat
    = this->context_->get_connection_datain (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (dat.in ()))
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  this->str_ =
    dat->get_data ();

  ACE_DEBUG ((LM_DEBUG,
              "BMDisplay - Display data is [%s] \n",
              this->str_.in ()));

  // Nitify others
  BasicSP::DataAvailable_var event =
    new OBV_BasicSP::DataAvailable;

  this->context_->push_out_avail (event
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// Operations from HUDisplay::position

char *
MyImpl::BMClosedED_exec_i::get_data (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->str_.inout ();
}

// Operations from Components::SessionComponent
void
MyImpl::BMClosedED_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMClosedED_exec_i::set_session_context\n"));

  this->context_ =
    BasicSP::CCM_BMClosedED_Context::_narrow (ctx
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::BMClosedED_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMClosedED_exec_i::ccm_activate\n"));
}

void
MyImpl::BMClosedED_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMClosedED_exec_i::ccm_passivate\n"));
}

void
MyImpl::BMClosedED_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMClosedED_exec_i::ccm_remove\n"));
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
MyImpl::BMClosedEDHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::BMClosedED_exec_i;
}


extern "C" BMCLOSEDED_EXEC_Export ::Components::HomeExecutorBase_ptr
createBMClosedEDHome_Impl (void)
{
  return new MyImpl::BMClosedEDHome_exec_i;
}
