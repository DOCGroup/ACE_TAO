// -*- C++ -*-
// $Id$

// The run_test.pl perl script will check for the expected output.

#include <tao/IFR_Client/IFR_BasicC.h>

ACE_RCSID (IFR_Inheritance_Test,
           main,
           "$Id$")

void printContents( const CORBA::ContainedSeq& cont )
{
  for( unsigned int i=0; i<cont.length(); i++ )
  {
    if( cont[i]->describe()->kind == CORBA::dk_Interface )
    {
      CORBA::InterfaceDef_var intDef = CORBA::InterfaceDef::_narrow( cont[i].in() );
      CORBA::InterfaceDef::FullInterfaceDescription* desc = intDef->describe_interface();

      //printf( "-- %s:\n", (const char*)(desc->name) );

      for( unsigned int j=0; j < desc->operations.length(); j++ )
        printf( "operation %s::%s\n",
                (const char*)(desc->name),
                (const char*)((desc->operations[j]).name) );

      for( unsigned int j=0; j < desc->attributes.length(); j++ )
        printf( "attribute %s::%s\n",
                (const char*)(desc->name),
                (const char*)((desc->attributes[j]).name) );
    }
    else if( cont[i]->describe()->kind == CORBA::dk_Module )
    {
      CORBA::ModuleDef_var moduleDef = CORBA::ModuleDef::_narrow(cont[i].in());
      CORBA::ContainedSeq_var moduleContents = moduleDef->contents(CORBA::dk_all,1);
      printContents( moduleContents.in() );
    }
  }
}


int  main(int argc, char** argv)
{
  CORBA::ORB_var      orb = CORBA::ORB_init(argc, argv, "IFR_Test ORB");;
  CORBA::Object_var    obj = orb->resolve_initial_references( "InterfaceRepository" );
  CORBA::Repository_var  ifr = CORBA::Repository::_narrow( obj.in() );

  if( CORBA::is_nil(ifr.in()) )
  {
    printf( "Unable to locate the Interface Repository.\n" );
    return -1;
  }

  CORBA::ContainedSeq_var  cont = ifr->contents( CORBA::dk_all, 0 );

  printContents( cont.in() );

  orb->shutdown();

  return 0;
}
