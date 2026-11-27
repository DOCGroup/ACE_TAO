#include "test_i.h"

#include <cstring>
#include <cstdlib>
#include <string>

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb, char const * key)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , _key (CORBA::string_dup (key))
{
}

char *
Simple_Server_i::remote_call ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SERVER(%C)> executing remote_call()\n"), _key.in ()));

  std::string result ("remote_call() completed by ");
  result += this->_key.in ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SERVER(%C)> returning '%C'\n"), _key.in (), result.c_str ()));
  return CORBA::string_dup (result.c_str ());
}

char *
Simple_Server_i::shutdown (char const * key)
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SERVER(%C)> executing shutdown()\n"), _key.in ()));
  //FUZZ: enable check_for_lack_ACE_OS

  bool still_alive {true};
  if (std::strcmp (_key.in (), key) == 0)
    {
      this->orb_->shutdown (false);
      still_alive = false;
    }

  std::string result ("shutdown() completed by ");
  result += this->_key.in ();
  result += ", still_alive=";
  result += std::to_string (still_alive);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SERVER(%C)> returning '%C'\n"), _key.in (), result.c_str ()));
  return CORBA::string_dup (result.c_str ());
}

char *
Simple_Server_i::abort (char const * key)
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SERVER(%C)> executing abort()\n"), _key.in ()));
  //FUZZ: enable check_for_lack_ACE_OS

  bool still_alive {true};
  if (std::strcmp (_key.in (), key) == 0)
    {
      std::abort ();
      still_alive = false;
    }

  std::string result ("abort() completed by ");
  result += this->_key.in ();
  result += ", still_alive=";
  result += std::to_string (still_alive);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SERVER(%C)> returning '%C'\n"), _key.in (), result.c_str ()));
  return CORBA::string_dup (result.c_str ());
}
