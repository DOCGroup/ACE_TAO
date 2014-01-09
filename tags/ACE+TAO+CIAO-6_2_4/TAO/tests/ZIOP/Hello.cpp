//
// $Id$
//
#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : length_ (40000u)
  , orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (const char * mystring)
{
  ACE_DEBUG ((LM_DEBUG, "Received <%s>\n", mystring));
  return CORBA::string_dup ("Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!");
}

Test::Octet_Seq *
Hello::get_big_reply ()
{
  Test::Octet_Seq_var reply_mesg =
    new Test::Octet_Seq (this->length_);

  reply_mesg->length (this->length_);

  for (unsigned int i = 0u; i < this->length_; ++i)
    {
      reply_mesg[i]= static_cast<CORBA::Octet> (i & 0xff);
    }
  return reply_mesg._retn ();
}

void
Hello::big_request (const ::Test::Octet_Seq & octet_in)
{
  if (octet_in.length () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("Server side BLOB received\n")));
    }
  else
    {
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT("Error recieving BLOB on server\n")));
    }
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
