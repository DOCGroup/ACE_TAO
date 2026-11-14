#include "amh_ami_pch.h"
#include "ace/Get_Opt.h"
#include "middleC.h"

#include <iostream>

struct Client_Options
{
  ACE_TString ior_file {ACE_TEXT ("file://middle.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option;

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior_file> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
    {
      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));
      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object (orb->string_to_object (
        ACE_TEXT_ALWAYS_CHAR (options.ior_file.c_str ())));
      if (CORBA::is_nil (object.in ()))
        {
          std::cerr << "Could not get middle IOR." << std::endl;
          return 1;
        }

      Middle_var const middle (Middle::_narrow (object.in ()));
      if (CORBA::is_nil (middle.in ()))
        {
          std::cerr << "IOR was not a Middle object reference." << std::endl;
          return 1;
        }

      CORBA::String_var const question (CORBA::string_dup (
        "How much wood would a woodchuck chuck, if a woodchuck could chuck wood?"));
      std::cout << "Question is: " << question.in () << std::endl;

      CORBA::String_var const answer (middle->get_the_answer (question.in ()));
      std::cout << "Answer is: " << answer.in () << std::endl;
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
