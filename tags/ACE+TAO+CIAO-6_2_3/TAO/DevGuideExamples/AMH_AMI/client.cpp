// $Id$

#include "amh_ami_pch.h"
#include "ace/Get_Opt.h"
#include "middleC.h"

#include <iostream>

const ACE_TCHAR *ior_file = ACE_TEXT ("file://middle.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
          ior_file = get_opts.opt_arg ();
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
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
        return 1;

    // Read and destringify the Asynch_Except_Demo object's IOR.
    CORBA::Object_var obj = orb->string_to_object(ACE_TEXT_ALWAYS_CHAR(ior_file));
    if( CORBA::is_nil( obj.in() ) ) {
      std::cerr << "Could not get middle IOR." << std::endl;
      return 1;
    }

    // Narrow the IOR to a Asycnh_Except_Demo object reference.
    Middle_var mid =  Middle::_narrow( obj.in() );
    if( CORBA::is_nil( mid.in() ) ) {
      std::cerr << "IOR was not an middle object reference." << std::endl;
      return 1;
    }

    CORBA::String_var question =
      CORBA::string_dup ("How much wood would a woodchuck chuck, if a woodchuck could chuck wood?");

    std::cout << "Question is: " << question.in() << std::endl;


    //  trigger the exception via AMI call
    CORBA::String_var answer =
      mid->get_the_answer (question.in());

    std::cout << "Answer is: " << answer.in() << std::endl;

    return 0;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "CORBA exception: " << ex << std::endl;
  }

  return 1;
}
