//
// $Id$
//
#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
  length_ = 4000000;
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

  for (unsigned int i = 0; i < this->length_; ++i)
    {
      int size = 128;
      for (int ch = 0; ch < size && i < this->length_ - 1; ++ch)
        reply_mesg[++i] = ACE_OS::rand() % size--;
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
