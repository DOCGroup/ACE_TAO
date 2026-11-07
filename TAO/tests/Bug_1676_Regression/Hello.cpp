#include "Hello.h"
#include <memory>
#include <string>

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

#if 0
char *
Hello::get_string ()
{
  return CORBA::string_dup ("Hello there!");
}
#endif

::Test::StringList *
Hello::get_stringList ()
{
  std::unique_ptr<Test::StringList> seq = std::make_unique<Test::StringList> (10);
  seq->length (10);
  for (CORBA::ULong i {}; i < seq->length (); ++i)
    {
      std::string const value = "Hello World " + std::to_string (i);
      (*seq)[i] = CORBA::string_dup (value.c_str ());
    }

  // The IDL return type transfers the allocated sequence to the caller.
  return seq.release ();
}

void
Hello::get_stringList2 (::CORBA::Boolean initialize,
                        ::Test::StringList_out osl)
{
  // CORBA::String_var the_string = osl->length();
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Entering get_stringList2(%.4d)\n", osl));

  if (initialize)
    {
      std::unique_ptr<Test::StringList> seq = std::make_unique<Test::StringList> (10);

      seq->length (5);
      for (CORBA::ULong i {}; i < seq->length (); ++i)
        {
          std::string const value = "Hello Again " + std::to_string (i);
          (*seq)[i] = CORBA::string_dup (value.c_str ());
        }
      // The out parameter takes ownership of the completed sequence.
      osl = seq.release ();
    }
}

void
Hello::mod_stringList (::Test::StringList & iosl)
{
  // osl->length(10);
  for (CORBA::ULong i {}; i < iosl.length (); ++i)
    {
      if (i % 2)
        {
          std::string const value = "Hello Client " + std::to_string (i);
          iosl[i] = CORBA::string_dup (value.c_str ());
        }
    }
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
