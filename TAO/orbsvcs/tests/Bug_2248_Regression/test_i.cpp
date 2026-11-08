#include "test_i.h"

#include "ace/OS_NS_stdlib.h"

#include <cstring>
#include <string>

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb, char const * key)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , _key (CORBA::string_dup (key))
{
}

Simple_Server_i::Simple_Server_i ()
  : orb_ (nullptr)
{
}

char *
Simple_Server_i::remote_call ()
{
  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> executing remote_call()\n", _key.in()));

  std::string message = "remote_call() completed by ";
  message += this->_key.in ();

  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> returning '%C'\n", _key.in (), message.c_str ()));
  return CORBA::string_dup (message.c_str ());
}


char *
Simple_Server_i::shutdown (char const * key)
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> executing shutdown()\n", _key.in()));
  //FUZZ: enable check_for_lack_ACE_OS

  int still_alive = 1;
  if (std::strcmp (this->_key.in (), key) == 0)
    {
       this->orb_->shutdown (false);
       still_alive = 0;
    }

  std::string message = "shutdown() completed by ";
  message += this->_key.in ();
  message += ", still_alive=";
  message += std::to_string (still_alive);
  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> returning '%C'\n", _key.in (), message.c_str ()));
  return CORBA::string_dup (message.c_str ());
}

char *
Simple_Server_i::abort (char const * key)
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> executing abort()\n", _key.in()));
  //FUZZ: enable check_for_lack_ACE_OS

  int still_alive = 1;
  if (std::strcmp (this->_key.in (), key) == 0)
    {
       ACE_OS::abort();
       still_alive = 0;
    }

  std::string message = "abort() completed by ";
  message += this->_key.in ();
  message += ", still_alive=";
  message += std::to_string (still_alive);
  ACE_DEBUG ((LM_DEBUG, "SERVER(%C)> returning '%C'\n", _key.in (), message.c_str ()));
  return CORBA::string_dup (message.c_str ());
}
