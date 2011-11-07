//
// $Id$
//
#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Received call back !!!\n"));
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}

void
Hello::request_callback (Test::Hello_ptr call_me)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Making call back !!!\n"));
  try
    {
      CORBA::String_var result = call_me->get_string ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Exception attempting to callback client obj ref:\n");
      throw;
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Call back succeeded !!!\n"));
}
