// -*- C++ -*-
// $Id$

// The run_test.pl perl script will check for the expected output.

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"

void printContents (const CORBA::ContainedSeq& cont)
{
  try
    {
      for (CORBA::ULong i = 0; i < cont.length(); ++i)
        {
          CORBA::Contained::Description_var topdesc = cont[i]->describe ();
          if (topdesc->kind == CORBA::dk_Interface)
            {
              CORBA::InterfaceDef_var intDef =
                CORBA::InterfaceDef::_narrow (cont[i]);

              CORBA::InterfaceDef::FullInterfaceDescription_var desc =
                intDef->describe_interface ();

              //printf ("-- %s:\n", desc->name.in ());

              for (CORBA::ULong j1 = 0; j1 < desc->operations.length (); ++j1)
                ACE_DEBUG ((LM_DEBUG,
                            "operation %C::%C\n",
                            desc->name.in (),
                            desc->operations[j1].name.in ()));

              for (CORBA::ULong j2 = 0; j2 < desc->attributes.length (); ++j2)
                ACE_DEBUG ((LM_DEBUG,
                            "attribute %C::%C\n",
                            desc->name.in (),
                            desc->attributes[j2].name.in ()));
            }
          else if (topdesc->kind == CORBA::dk_Module)
            {
              CORBA::ModuleDef_var moduleDef =
                CORBA::ModuleDef::_narrow (cont[i]);

              CORBA::ContainedSeq_var moduleContents =
                moduleDef->contents (CORBA::dk_all, 1);
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

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var object =
        orb->resolve_initial_references ("InterfaceRepository");

      if (CORBA::is_nil (object.in ()))
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "Null objref from resolve_initial_references\n"),
             -1);
        }

      CORBA::Repository_var  ifr =
        CORBA::Repository::_narrow (object.in ());

      if (CORBA::is_nil (ifr.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "CORBA::Repository::_narrow failed\n"),
                            -1);
        }

      CORBA::ContainedSeq_var cont = ifr->contents (CORBA::dk_all, 0);

      printContents (cont.in ());

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("main");
      return -1;
    }

  return 0;
}
