#include "test_i.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb, const char* key)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , _key (CORBA::string_dup (key))
{
}

Simple_Server_i::Simple_Server_i ()
  : orb_ (0)
{
}

char*
Simple_Server_i::remote_call ()
{
  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> executing remote_call()\n", _key.in()));

  char buf[256];
  ACE_OS::sprintf (buf, "remote_call() completed by %s", _key.in());

  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> returning '%C'\n", _key.in(), buf));
  return CORBA::string_dup (buf);
}


char*
Simple_Server_i::shutdown (const char* key)
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> executing shutdown()\n", _key.in()));
  //FUZZ: enable check_for_lack_ACE_OS

  int still_alive = 1;
  if (!ACE_OS::strcmp (_key.in(), key))
    {
       this->orb_->shutdown (false);
       still_alive = 0;
    }

  char buf[256];
  ACE_OS::sprintf (buf, "shutdown() completed by %s, still_alive=%d",
           _key.in(), still_alive);
  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> returning '%C'\n", _key.in(), buf));
  return CORBA::string_dup (buf);
}

char*
Simple_Server_i::abort (const char* key)
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> executing abort()\n", _key.in()));
  //FUZZ: enable check_for_lack_ACE_OS

  int still_alive = 1;
  if (!ACE_OS::strcmp (_key.in(), key))
    {
       ACE_OS::abort();
       still_alive = 0;
    }

  char buf[256];
  ACE_OS::sprintf (buf, "abort() completed by %s, still_alive=%d",
           _key.in(), still_alive);
  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> returning '%C'\n", _key.in(), buf));
  return CORBA::string_dup (buf);
}
