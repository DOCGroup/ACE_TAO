#include "ace/Get_Opt.h"
#include "tao/DynamicAny/DynamicAny.h"
#include "tao/ORB.h"
#include "tao/IFR_Client/IFR_ComponentsC.h"
#include <cstring>

static int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const*& ifr_ior_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'i':
          ifr_ior_file = get_opts.opt_arg ();
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -i <ifr_ior>\n"),
                             argv [0]),
                            -1);
        }
    }

  if (ifr_ior_file == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("usage: %s -i <ifr_ior>\n"),
                       argv [0]),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result {};
  ACE_TCHAR const* ifr_ior_file {};
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Start\n")));

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ifr_ior_file) == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Get IFR\n")));
      CORBA::Object_var const objref = orb->string_to_object (ifr_ior_file);
      if (CORBA::is_nil (objref.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("The received objref is nil\n")), -1);

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Narrow IFR\n")));
      CORBA::ComponentIR::Repository_var const repository =
        CORBA::ComponentIR::Repository::_narrow (objref.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Obtaining DynamicAny\n")));
      CORBA::Object_var const factory_obj = orb->resolve_initial_references ("DynAnyFactory");
      DynamicAny::DynAnyFactory_var const dynanyfactory =
        DynamicAny::DynAnyFactory::_narrow (factory_obj.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nLook up c2\n")));
      CORBA::Contained_var const c2 = repository->lookup_id ("IDL:m1/c2:1.0");
      if (CORBA::is_nil (c2.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Can't look up the const m1/c2\n")),
                          -1);

      CORBA::ConstantDef_var const c2_def = CORBA::ConstantDef::_narrow (c2.in ());
      CORBA::String_var const c2_name = c2_def->absolute_name ();
      CORBA::TypeCode_var const c2_tc = c2_def->type ();
      CORBA::String_var const c2_id = c2_tc->id ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("constant \"%C\" is type \"%C\""),
                  c2_name.in (), c2_id.in ()));
      CORBA::TCKind const c2_tckind = c2_tc->kind ();
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT (", tkkind %d"), c2_tckind));
      if (CORBA::tk_enum == c2_tckind)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (CORBA::tk_enum)\n")));
          CORBA::Any_var const the_value = c2_def->value ();

          DynamicAny::DynAny_var const dany =
            dynanyfactory->create_dyn_any (the_value.in ());
          DynamicAny::DynEnum_var const denum =
            DynamicAny::DynEnum::_narrow (dany.in ());
          CORBA::String_var const the_str_value = denum->get_as_string ();
          CORBA::ULong const the_int_value = denum->get_as_ulong ();

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Whose value is \"%C\" which has an integer value of %d\n"),
                      the_str_value.in (), the_int_value));

          if (std::strcmp ("e1_2", the_str_value.in ()) == 0)
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The string value is correct\n")));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ERROR: The string value should be \"e1_2\"\n")));
              result = -1;
            }
          if (the_int_value == 1)
            {
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The corresponding integer value is correct\n")));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("ERROR: The corresponding integer value should be 1\n")));
              result = -1;
            }
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\nERROR: Wrong tkkind for m1::c2, should be %d\n"),
                      CORBA::tk_enum));
          result = -1;
        }
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("ERROR: CORBA Exception");
      result = -1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ERROR: UNKNOWN Excetion\n")));
      result = -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nDone\n")));
  return result;
}
