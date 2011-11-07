// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"
#include "tao/DynamicAny/DynamicAny.h"
#include "tao/ORB.h"
#include "tao/IFR_Client/IFR_ComponentsC.h"

namespace
{
  const ACE_TCHAR *ifr_ior_file = 0;
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:s:"));
  const unsigned char full_success = 0x01;
  unsigned char success = 0;

  while (true)
    {
      int c = get_opts ();
      if (success == full_success)
        {
          break;
        }

      if (c == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "usage:  %s"
                            " -i <ifr_ior>"
                            "\n",
                            argv [0]),
                            -1);
        }

      switch (c)
        {
        case 'i':
          ifr_ior_file = get_opts.opt_arg ();
          success |= 0x01;
          break;
        }
    }

  // Indicates successful parsing of the command line
  return 0;
}

// ----------------------------------------------------------------------

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result= 0;
  ACE_DEBUG (( LM_DEBUG, "Start\n" ));

  try
    {
      // init orb
      CORBA::ORB_var the_orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == -1)
          return -1;

      ACE_DEBUG (( LM_DEBUG, "Get IFR\n" ));
      CORBA::Object_var objref =
        the_orb->string_to_object (ifr_ior_file);
      if (objref.in () == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "The received objref is nil\n"),
                            -1);
        }

      ACE_DEBUG (( LM_DEBUG, "Narrow IFR\n" ));
      CORBA::ComponentIR::Repository_var the_repo_ref;
      the_repo_ref = CORBA::ComponentIR::Repository::_narrow (objref.in ());

      ACE_DEBUG (( LM_DEBUG, "Obtaining DynamicAny\n" ));
      CORBA::Object_var factory_obj =
        the_orb->resolve_initial_references ("DynAnyFactory");
      DynamicAny::DynAnyFactory_var dynanyfactory =
        DynamicAny::DynAnyFactory::_narrow (factory_obj.in ());

      ACE_DEBUG (( LM_DEBUG, "\nLook up c2\n" ));
      CORBA::Contained_var c2 =
        the_repo_ref->lookup_id ("IDL:m1/c2:1.0");
      if (CORBA::is_nil (c2.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "Can't look up the const m1/c2\n"),
                            -1);
        }
      CORBA::ConstantDef_var c2_def = CORBA::ConstantDef::_narrow (c2.in ());
      CORBA::String_var c2_name= c2_def->absolute_name ();
      CORBA::TypeCode_var c2_tc = c2_def->type ();
      CORBA::String_var c2_id = c2_tc->id ();
      ACE_DEBUG (( LM_DEBUG, "constant \"%C\" is type \"%C\"", c2_name.in (), c2_id.in () ));
      CORBA::TCKind c2_tckind = c2_tc->kind ();
      ACE_DEBUG (( LM_DEBUG, ", tkkind %d", c2_tckind ));
      if (CORBA::tk_enum == c2_tckind)
        {
          ACE_DEBUG (( LM_DEBUG, " (CORBA::tk_enum)\n" ));
          CORBA::Any_var the_value = c2_def->value ();

          DynamicAny::DynAny_var dany =
            dynanyfactory->create_dyn_any (the_value.in ());
          DynamicAny::DynEnum_var denum =
            DynamicAny::DynEnum::_narrow (dany.in ());
          CORBA::String_var the_strValue = denum->get_as_string ();
          CORBA::ULong the_intValue = denum->get_as_ulong ();

          ACE_DEBUG ((LM_DEBUG, "Whose value is \"%C\" which has an integer value of %d\n",
                     the_strValue.in (), the_intValue ));

          if (0 == ACE_OS::strcmp( "e1_2", the_strValue.in () ))
            {
              ACE_DEBUG ((LM_DEBUG, "The string value is correct\n" ));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, "ERROR: The string value should be \"e1_2\"\n" ));
              result = -1;
            }
          if (1 == the_intValue )
            {
              ACE_DEBUG ((LM_DEBUG, "The corresponding integer value is correct\n" ));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, "ERROR: The corresponding integer value should be 1\n" ));
              result = -1;
            }
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "\nERROR: Wrong tkkind for m1::c2, should be %d\n", CORBA::tk_enum));
          result= -1;
        }
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("ERROR: CORBA Exception");
      result= -1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, "ERROR: UNKNOWN Excetion\n"));
      result= -1;
    }

  ACE_DEBUG (( LM_DEBUG, "\nDone\n" ));
  return result;
}
