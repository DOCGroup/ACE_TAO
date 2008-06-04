// -*- C++ -*-
// $Id$

// The run_test.pl perl script will check for the expected output.

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"

ACE_RCSID (IFR_Inheritance_Test,
           main,
           "$Id$")

void printContents( const CORBA::ContainedSeq& cont )
{
  try
    {
      for( unsigned int i=0; i<cont.length(); i++ )
        {
          if( cont[i]->describe()->kind == CORBA::dk_Interface )
            {
              CORBA::InterfaceDef_var intDef =
                CORBA::InterfaceDef::_narrow (cont[i]
 );

              CORBA::InterfaceDef::FullInterfaceDescription* desc =
                intDef->describe_interface ();

              //printf( "-- %s:\n", (const char*)(desc->name) );

              for( unsigned int j1=0; j1 < desc->operations.length (); j1++ )
                ACE_OS::printf( "operation %s::%s\n",
                                (const char*)(desc->name),
                                (const char*)((desc->operations[j1]).name) );

              for( unsigned int j2=0; j2 < desc->attributes.length (); j2++ )
                ACE_OS::printf( "attribute %s::%s\n",
                                (const char*)(desc->name),
                                (const char*)((desc->attributes[j2]).name) );
            }
          else if( cont[i]->describe ()->kind == CORBA::dk_Module )
            {
              CORBA::ModuleDef_var moduleDef =
                CORBA::ModuleDef::_narrow (cont[i]
 );

              CORBA::ContainedSeq_var moduleContents =
                moduleDef->contents (CORBA::dk_all,1
 );
              printContents (moduleContents.in ());
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("printContents");
      ACE_OS::exit(-1); // the test has failed!
    }
}


int  main(int argc, char** argv)
{
  try
    {
      CORBA::ORB_var      orb = CORBA::ORB_init (argc,
                                    argv,
                                    0);
      CORBA::Object_var object =
        orb->resolve_initial_references ("InterfaceRepository");

      if (CORBA::is_nil (object.in ()))
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "Null objref from resolve_initial_references\n"
            ),
            -1
          );
        }

      CORBA::Repository_var  ifr =
        CORBA::Repository::_narrow (object.in ());

      if (CORBA::is_nil (ifr.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "CORBA::Repository::_narrow failed\n"),
                            -1);
        }

     CORBA::ContainedSeq_var  cont = ifr->contents (CORBA::dk_all, 0);

     printContents (cont.in ());

     orb->shutdown ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("main");
      return -1;
    }

  return 0;
}
