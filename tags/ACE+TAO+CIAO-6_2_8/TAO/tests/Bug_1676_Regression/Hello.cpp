//
// $Id$
//
#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

#if 0
char *
Hello::get_string (void)
{
  return CORBA::string_dup ("Hello there!");
}
#endif

::Test::StringList *
Hello::get_stringList ()
{
  Test::StringList * seq;
  ACE_NEW_RETURN (seq,
                  Test::StringList(10),
                  0);
  seq->length(10);
  for (CORBA::ULong i = 0; i<seq->length(); i++)
    {
      char tmp[255] = {0};
      ACE_OS::sprintf(tmp, "Hello World %d", i);
      (*seq)[i] = CORBA::string_dup(tmp);
    }

  return seq;
}

void
Hello::get_stringList2 (::CORBA::Boolean initialize,
                        ::Test::StringList_out osl)
{
  // CORBA::String_var the_string = osl->length();
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Entering get_stringList2(%.4d)\n", osl));

  if (initialize)
    {
      ACE_NEW (osl,
               Test::StringList(10));

      osl->length(5);
      for (CORBA::ULong i = 0; i<osl->length(); i++)
        {
          char tmp[255] = {0};
          ACE_OS::sprintf(tmp, "Hello Again %d", i);
          (*osl)[i] = CORBA::string_dup(tmp);
        }
    }
}

void
Hello::mod_stringList (::Test::StringList & iosl)
{
  // osl->length(10);
  for (CORBA::ULong i = 0; i<iosl.length(); i++)
    {
      char tmp[255] = {0};
      ACE_OS::sprintf(tmp, "Hello Client %d", i);
      if (i%2)
        {
          iosl[i] = CORBA::string_dup(tmp);
        }
    }
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
