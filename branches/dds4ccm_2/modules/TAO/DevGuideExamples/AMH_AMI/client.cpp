// $Id$

#include "amh_ami_pch.h"

#include "middleC.h"

#include <iostream>
int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    // Read and destringify the Asynch_Except_Demo object's IOR.
    CORBA::Object_var obj = orb->string_to_object( "file://middle.ior" );
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
