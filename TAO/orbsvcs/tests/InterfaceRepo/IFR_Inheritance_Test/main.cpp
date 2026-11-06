// -*- C++ -*-
// The run_test.pl perl script will check for the expected output.

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"
#include <cstdlib>

static void
printContents (CORBA::ContainedSeq const& cont)
{
  try
    {
      for (CORBA::ULong i = 0; i < cont.length (); ++i)
        {
          CORBA::Contained::Description_var const description = cont[i]->describe ();
          if (description->kind == CORBA::dk_Interface)
            {
              CORBA::InterfaceDef_var const interface_def =
                CORBA::InterfaceDef::_narrow (cont[i]);

              CORBA::InterfaceDef::FullInterfaceDescription_var const full_description =
                interface_def->describe_interface ();

              for (CORBA::ULong operation = 0;
                   operation < full_description->operations.length ();
                   ++operation)
                ACE_DEBUG ((LM_DEBUG,
                            "operation %C::%C\n",
                            full_description->name.in (),
                            full_description->operations[operation].name.in ()));

              for (CORBA::ULong attribute = 0;
                   attribute < full_description->attributes.length ();
                   ++attribute)
                ACE_DEBUG ((LM_DEBUG,
                            "attribute %C::%C\n",
                            full_description->name.in (),
                            full_description->attributes[attribute].name.in ()));
            }
          else if (description->kind == CORBA::dk_Module)
            {
              CORBA::ModuleDef_var const module =
                CORBA::ModuleDef::_narrow (cont[i]);

              CORBA::ContainedSeq_var const contents = module->contents (CORBA::dk_all, 1);
              printContents (contents.in ());
            }
        }
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("printContents");
      std::exit (EXIT_FAILURE);
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const object = orb->resolve_initial_references ("InterfaceRepository");

      if (CORBA::is_nil (object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Null objref from resolve_initial_references\n")),
                          -1);

      CORBA::Repository_var const repository = CORBA::Repository::_narrow (object.in ());

      if (CORBA::is_nil (repository.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("CORBA::Repository::_narrow failed\n")),
                          -1);

      CORBA::ContainedSeq_var const contents = repository->contents (CORBA::dk_all, 0);

      printContents (contents.in ());

      CORBA::Contained_var const contained =
        repository->lookup_id ("IDL:IFR_Test/AbstractChild:1.0");
      CORBA::AbstractInterfaceDef_var const abstract_child =
        CORBA::AbstractInterfaceDef::_narrow (contained.in ());

      if (CORBA::is_nil (abstract_child.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("AbstractChild was not found in the IFR\n")),
                          -1);

      CORBA::InterfaceDefSeq_var abstract_bases = abstract_child->base_interfaces ();

      if (abstract_bases->length () != 1
          || CORBA::is_nil (abstract_bases[0]))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("AbstractChild does not have its expected base\n")),
                          -1);

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("main");
      return -1;
    }

  return 0;
}
