// $Id$

#include "CIAO_common.h"
#include "BMDisplay_exec.h"

/// Default constructor.
MyImpl::BMDisplay_exec_impl::BMDisplay_exec_impl ()
{
}

/// Default destructor.
MyImpl::BMDisplay_exec_impl::~BMDisplay_exec_impl ()
{
}

// Operations from BasicSP::BMDisplay

void
MyImpl::BMDisplay_exec_impl::push_data_ready (
    BasicSP::DataAvailable *ev
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ev);

  ACE_DEBUG ((LM_DEBUG,
              "BMDisplay - Doing computations \n"));

  ACE_DEBUG ((LM_DEBUG,
              "BMDisplay - Doing data fetch \n"));

  // Refresh position
  BasicSP::ReadData_var dat
    = this->context_->get_connection_comp_data (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (dat.in ()))
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  char *str =
    dat->get_data (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "BMDisplay - Display data is [%s] \n",
              str));

}

// Operations from Components::SessionComponent
void
MyImpl::BMDisplay_exec_impl::set_session_context (Components::SessionContext_ptr ctx
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMDisplay_exec_impl::set_session_context\n"));

  this->context_ =
    BasicSP::CCM_BMDisplay_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::BMDisplay_exec_impl::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMDisplay_exec_impl::ccm_activate\n"));

  // @@ This hack work around a missing feature in CIAO's assembly
  // mechanism where a Softpkg descriptor can specify it's dependency
  // to a valuetype factory and instruct the deployment framework to
  // initialize and register the corresponding valuefactory in the
  // component server.  Here, we are registering the valuefactory
  // explicitly to work around this problem.

  char *argv[1] = { "BMDisplay_exec"};

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), BasicSP::DataAvailable_init,
                               BasicSP::DataAvailable);
}

void
MyImpl::BMDisplay_exec_impl::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMDisplay_exec_impl::ccm_passivate\n"));
}

void
MyImpl::BMDisplay_exec_impl::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::BMDisplay_exec_impl::ccm_remove\n"));
}

/// Default ctor.
MyImpl::BMDisplayHome_exec_impl::BMDisplayHome_exec_impl ()
{
}

/// Default dtor.
MyImpl::BMDisplayHome_exec_impl::~BMDisplayHome_exec_impl ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::BMDisplayHome_exec_impl::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::BMDisplay_exec_impl;
}


extern "C" BMDISPLAY_EXEC_Export ::Components::HomeExecutorBase_ptr
createBMDisplayHome_Impl (void)
{
  return new MyImpl::BMDisplayHome_exec_impl;
}
