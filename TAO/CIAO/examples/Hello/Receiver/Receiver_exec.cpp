//$Id$
/*
 * @file Receiver_exec.cpp
 *
 * @author Tao Lu <lu@dre.vanderbilt.edu>
 */

#include "Receiver_exec.h"
#include "CIAO_common.h"

Receiver_Impl::Receiver_exec_i::Receiver_exec_i ()
{
}

Receiver_Impl::Receiver_exec_i::~Receiver_exec_i ()
{
}

void
Receiver_Impl::Receiver_exec_i::push_click_in (Hello::timeout *
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //Get the message from the Sender first.
  ACE_DEBUG ((LM_DEBUG,
              "Receiver - Informed by the Sender \n"));

  Hello::message_var rev
    = this->context_->get_connection_read_message (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (rev.in ()))
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  CORBA::String_var str =
    rev->get_message (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Receiver - Got message from the server [%s] \n",
              str.in () ));
}

// Operations from Components::SessionComponen
void
Receiver_Impl::Receiver_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Receiver_Impl::Receiver_exec_i::set_session_context\n"));

  this->context_ =
    Receiver_Impl::Receiver_Exec_Context::_narrow (ctx
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
Receiver_Impl::Receiver_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Receiver_Impl::Receiver_exec_i::ccm_activate\n"));

  // @@ This hack work around a missing feature in CIAO's assembly
  // mechanism where a Softpkg descriptor can specify it's dependency
  // to a valuetype factory and instruct the deployment framework to
  // initialize and register the corresponding valuefactory in the
  // component server.  Here, we are registering the valuefactory
  // explicitly to work around this problem.
  char *argv[1] = { "Receiver_exec"};
  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(),
			       Hello::timeout_init,
                               Hello::timeout);
}

void
Receiver_Impl::Receiver_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Receiver_Impl::Receiver_exec_i::ccm_passivate\n"));
}

void
Receiver_Impl::Receiver_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "Receiver_Impl::Receiver_exec_i::ccm_remove\n"));
}


Receiver_Impl::ReceiverHome_exec_i::ReceiverHome_exec_i ()
{
}

Receiver_Impl::ReceiverHome_exec_i::~ReceiverHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
Receiver_Impl::ReceiverHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new Receiver_Impl::Receiver_exec_i;
}


extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
createReceiverHome_Impl (void)
{
  return new Receiver_Impl::ReceiverHome_exec_i ();
}
