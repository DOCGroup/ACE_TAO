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
                CORBA::ULong event_loop_iterations,
                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "server_i::loop: event_loop_depth = %2.2d; event_loop_iterations = %2.2d\n",
              event_loop_depth,
              event_loop_iterations));

  this->run_no_ops (remote_partner,
                    event_loop_iterations / 2,
                    ACE_TRY_ENV);
  ACE_CHECK;

  if (--event_loop_depth != 0)
    {
      remote_partner->loop (event_loop_depth,
                            event_loop_iterations,
                            ACE_TRY_ENV);
      ACE_CHECK;
    }

  this->run_no_ops (remote_partner,
                    event_loop_iterations / 2,
                    ACE_TRY_ENV);
  ACE_CHECK;
}

void
server_i::run_no_ops (client_ptr remote_partner,
                      CORBA::ULong iterations,
                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  while (iterations != 0)
    {
      void *temp = &iterations;
      CORBA::ULong act_for_iterations =
        ACE_reinterpret_cast (CORBA::ULong, tmp);

      CORBA::ULong got_reply = 0;

      CORBA::ULong act_for_flag =
        ACE_reinterpret_cast (CORBA::ULong, &got_reply);

      remote_partner->oneway_no_op (act_for_iterations,
                                    act_for_flag,
                                    ACE_TRY_ENV);
      ACE_CHECK;

      while (!got_reply)
        {
          this->orb_->perform_work (ACE_TRY_ENV);
          ACE_CHECK;
        }
    }
}

void
server_i::no_op (client_ptr remote_partner,
                 CORBA::ULong act_for_iterations,
                 CORBA::ULong act_for_flag,
                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong *iterations =
    ACE_reinterpret_cast (CORBA::ULong *, act_for_iterations);

  CORBA::ULong *flag =
    ACE_reinterpret_cast (CORBA::ULong *, act_for_flag);

  ACE_DEBUG ((LM_DEBUG,
              "server_i::no_op: iterations = %d\n",
              *iterations));

  --(*iterations);

  *flag = 1;

  remote_partner->twoway_no_op (ACE_TRY_ENV);
  ACE_CHECK;
}

void
server_i::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0,
                        ACE_TRY_ENV);
}

client_i::client_i (server_ptr remote_partner)
  : remote_partner_ (server::_duplicate (remote_partner))
{
}

void
client_i::loop (CORBA::ULong event_loop_depth,
                CORBA::ULong event_loop_iterations,
                CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  ACE_DEBUG ((LM_DEBUG,
              "client_i::loop: event_loop_depth = %2.2d; event_loop_iterations = %2.2d\n",
              event_loop_depth,
              event_loop_iterations));

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      client_var self =
        this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->remote_partner_->loop (self.in (),
                                   event_loop_depth,
                                   event_loop_iterations,
                                   ACE_TRY_ENV);
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
client_i::oneway_no_op (CORBA::ULong act_for_iterations,
                        CORBA::ULong act_for_flag,
                        CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      client_var self =
        this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->remote_partner_->no_op (self.in (),
                                    act_for_iterations,
                                    act_for_flag,
                                    ACE_TRY_ENV);
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
client_i::twoway_no_op (CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  ACE_DEBUG ((LM_DEBUG,
              "client_i::twoway_no_op\n"));
}
