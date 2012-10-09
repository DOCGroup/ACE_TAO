// $Id$
#include "Reply_Handler.h"

Reply_Handler::Reply_Handler (CORBA::ORB_ptr o)
  : orb_ (CORBA::ORB::_duplicate (o))
{
}

void
Reply_Handler::next_prime (CORBA::ULong)
{
  return;
}

void
Reply_Handler::next_prime_excep (
    ::Messaging::ExceptionHolder *ex)
{

  try
    {
      ex->raise_exception ();
    }
  catch (const CORBA::COMM_FAILURE& )
    {
      /*ACE_DEBUG ((LM_DEBUG,
                  "Callback method <next_prime_excep> called:\n"));
                  */

      ACE_Time_Value tv (10, 0);
      this->orb_->run (tv);

      this->orb_->shutdown (0);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
    }
}
