// $Id$

#include "test_i.h"

ACE_RCSID(Nested_Event_Loop, test_i, "$Id$")

server_i::server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
server_i::loop (client_ptr remote_partner,
                CORBA::ULong event_loop_depth,
                CORBA::ULong event_loop_iterations
                TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "server_i::loop: event_loop_depth = %2.2d; event_loop_iterations = %2.2d\n",
              event_loop_depth,
              event_loop_iterations));

  this->run_no_ops (remote_partner,
                    event_loop_iterations / 2
                    TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (--event_loop_depth != 0)
    {
      remote_partner->loop (event_loop_depth,
                            event_loop_iterations
                            TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  this->run_no_ops (remote_partner,
                    event_loop_iterations / 2
                    TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
server_i::run_no_ops (client_ptr remote_partner,
                      CORBA::ULong iterations
                      TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  while (iterations != 0)
    {
      CORBA::ULong *pointer_to_iterations = &iterations;
      size_t sizeof_pointer_to_iterations = sizeof (pointer_to_iterations);

      act act_for_iterations (sizeof_pointer_to_iterations);
      act_for_iterations.length (sizeof_pointer_to_iterations);

      ACE_OS::memcpy (act_for_iterations.get_buffer (),
                      &pointer_to_iterations,
                      sizeof_pointer_to_iterations);

      CORBA::ULong got_reply = 0;

      CORBA::ULong *pointer_to_flag = &got_reply;
      size_t sizeof_pointer_to_flag = sizeof (pointer_to_flag);

      act act_for_flag (sizeof_pointer_to_flag);
      act_for_flag.length (sizeof_pointer_to_flag);

      ACE_OS::memcpy (act_for_flag.get_buffer (),
                      &pointer_to_flag,
                      sizeof_pointer_to_flag);

      remote_partner->oneway_no_op (act_for_iterations,
                                    act_for_flag
                                    TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      while (!got_reply)
        {
          this->orb_->perform_work (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void
server_i::no_op (client_ptr remote_partner,
                 const act &act_for_iterations,
                 const act &act_for_flag
                 TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong *pointer_to_iterations = 0;

  ACE_OS::memcpy (&pointer_to_iterations,
                  act_for_iterations.get_buffer (),
                  act_for_iterations.length ());

  CORBA::ULong *pointer_to_flag = 0;

  ACE_OS::memcpy (&pointer_to_flag,
                  act_for_flag.get_buffer (),
                  act_for_flag.length ());

  ACE_DEBUG ((LM_DEBUG,
              "server_i::no_op: iterations = %d\n",
              *pointer_to_iterations));

  --(*pointer_to_iterations);

  *pointer_to_flag = 1;

  remote_partner->twoway_no_op (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
server_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0
                        TAO_ENV_ARG_PARAMETER);
}

client_i::client_i (server_ptr remote_partner)
  : remote_partner_ (server::_duplicate (remote_partner))
{
}

void
client_i::loop (CORBA::ULong event_loop_depth,
                CORBA::ULong event_loop_iterations
                TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "client_i::loop: event_loop_depth = %2.2d; event_loop_iterations = %2.2d\n",
              event_loop_depth,
              event_loop_iterations));

  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      client_var self =
        this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->remote_partner_->loop (self.in (),
                                   event_loop_depth,
                                   event_loop_iterations
                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught in client_i::loop:");
    }
  ACE_ENDTRY;
}

void
client_i::oneway_no_op (const act &act_for_iterations,
                        const act &act_for_flag
                        TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      client_var self =
        this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->remote_partner_->no_op (self.in (),
                                    act_for_iterations,
                                    act_for_flag
                                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught in client_i::no_op:");
    }
  ACE_ENDTRY;
}

void
client_i::twoway_no_op (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "client_i::twoway_no_op\n"));
}
