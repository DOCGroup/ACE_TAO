#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (const char * mystring)
{
  ACE_DEBUG ((LM_DEBUG, "Received <%s>\n", mystring));
  return CORBA::string_dup ("Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!");
}

Test::Octet_Seq *
Hello::get_big_reply (CORBA::ULong size)
{
  Test::Octet_Seq_var reply_mesg =
    new Test::Octet_Seq (size);

  reply_mesg->length (size);

  for (CORBA::ULong i = 0u; i < size; ++i)
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
                  ACE_TEXT("Server side BLOB received len = %d\n"),
                  octet_in.length ()));
    }
  else
    {
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT("Error receiving BLOB on server\n")));
    }
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
