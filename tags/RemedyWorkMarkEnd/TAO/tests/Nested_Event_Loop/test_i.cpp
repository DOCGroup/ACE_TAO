// $Id$

#include "test_i.h"
#include "ace/OS_NS_string.h"

server_i::server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
server_i::loop (client_ptr remote_partner,
                CORBA::ULong event_loop_depth,
                CORBA::ULong event_loop_iterations)
{
  ACE_DEBUG ((LM_DEBUG,
              "server_i::loop: event_loop_depth = %2.2d; event_loop_iterations = %2.2d\n",
              event_loop_depth,
              event_loop_iterations));

  this->run_no_ops (remote_partner,
                    event_loop_iterations / 2);

  if (--event_loop_depth != 0)
    {
      remote_partner->loop (event_loop_depth,
                            event_loop_iterations);
    }

  this->run_no_ops (remote_partner,
                    event_loop_iterations / 2);
}

void
server_i::run_no_ops (client_ptr remote_partner,
                      CORBA::ULong iterations)
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
                                    act_for_flag);

      while (!got_reply)
        {
          this->orb_->perform_work ();
        }
    }
}

void
server_i::no_op (client_ptr remote_partner,
                 const act &act_for_iterations,
                 const act &act_for_flag)
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

  remote_partner->twoway_no_op ();
}

void
server_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

client_i::client_i (server_ptr remote_partner)
  : remote_partner_ (server::_duplicate (remote_partner))
{
#if defined (CORBA_E_COMPACT) || defined (CORBA_E_MICRO)
  PortableServer::POA_var poa = this->_default_POA ();
  PortableServer::ObjectId_var id = poa->activate_object (this);
  CORBA::Object_var object = poa->id_to_reference (id.in ());
  self_ = client::_unchecked_narrow (object.in ());
#else
  self_ = this->_this ();
#endif /* CORBA_E_COMPACT || CORBA_E_MICRO */
}

void
client_i::loop (CORBA::ULong event_loop_depth,
                CORBA::ULong event_loop_iterations)
{
  ACE_DEBUG ((LM_DEBUG,
              "client_i::loop: event_loop_depth = %2.2d; event_loop_iterations = %2.2d\n",
              event_loop_depth,
              event_loop_iterations));


  try
    {
      this->remote_partner_->loop (self_.in (),
                                   event_loop_depth,
                                   event_loop_iterations);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client_i::loop:");
    }
}

void
client_i::oneway_no_op (const act &act_for_iterations,
                        const act &act_for_flag)
{

  try
    {
      this->remote_partner_->no_op (self_.in (),
                                    act_for_iterations,
                                    act_for_flag);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client_i::oneway_no_op:");
    }
}

void
client_i::twoway_no_op (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "client_i::twoway_no_op\n"));
}
