// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/CodeSets/simple
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//   A simple client to demonstrate the use of codeset translation
//
// = AUTHORS
//      Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================
// IDL generated headers
#include "simpleC.h"

// ------------------------------------------------------------
// Client
// ------------------------------------------------------------
int main(int argc, char *argv[])
{
  char buf[1000];

  ACE_TRY_NEW_ENV
    {
      // Init the orb
      CORBA::ORB_var orb= CORBA::ORB_init (argc, argv);
      ACE_TRY_CHECK;

      // Get IOR from command line (or file)
      if(argc != 2)
        {
          ifstream fstr;
          fstr.open("server.ior");
          if (fstr.bad())
            {
              cout << "Cannot open server.ior and no IOR argument!" << endl;
              exit(1);
            }
          else
            {
              fstr >> buf;
            }
        }
      else
        {
          strcpy(buf, argv[1]);
        }

      // The first arg should be the IOR
      CORBA::Object_var object= orb->string_to_object(buf
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

         // Get the server
      CORBA::Environment env;
      simple_var server= simple::_narrow (object.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // invoke the call
      CORBA::String_var reply =
        server->op1("Hello World" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      cout << "sent Hello World, got " << reply << endl;

      reply = server->op1("Goodbye World" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      cout << "sent Goodby World, got " << reply << endl;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

}

