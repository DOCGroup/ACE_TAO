// -*- C++ -*-
// The run_test.pl perl script will check for the expected output.

#include "ace/config.h"
#include "tao/IFR_Client/IFR_ComponentsC.h"
#include "ace/Get_Opt.h"

static int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const*& ifr_ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        ifr_ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -i <ior>\n"),
                           argv [0]),
                          -1);
      }

  if (ifr_ior == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("usage: %s [-i <ior>]\n"),
                       argv [0]),
                      -1);

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_TCHAR const* ifr_ior {ACE_TEXT ("file://ifr.ior")};
      ACE_TCHAR const* const member_type_id {ACE_TEXT ("IDL:arrayOfStruct:1.0")};
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ifr_ior) != 0)
        return 1;

      CORBA::Object_var const obj = orb->string_to_object (ifr_ior);
      if (CORBA::is_nil (obj.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Obtained nil reference to Object for interface repository\n")));
          return 1;
        }

      CORBA::ComponentIR::Repository_var ifr {};

      try
        {
          ifr = CORBA::ComponentIR::Repository::_narrow (obj.in ());
        }
      catch (CORBA::SystemException const&)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("exception caught while narrowing IFR object\n")));
          return 1;
        }

      if (CORBA::is_nil (ifr.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Can't narrow to IFR\n")));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Looking up %s"), member_type_id));
      CORBA::Contained_var const dependency =
        ifr->lookup_id (ACE_TEXT_ALWAYS_CHAR (member_type_id));
      if (CORBA::is_nil (dependency.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" *** failed ***\n")));
          return 1;
        }

      CORBA::AliasDef_var const the_alias = CORBA::AliasDef::_narrow (dependency.in ());
      if (CORBA::is_nil (the_alias.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" *** not an alias ***\n")));
          return 1;
        }

      CORBA::IDLType_var const orig_type = the_alias->original_type_def ();
      CORBA::TypeCode_var const tc_content = orig_type->type ();
      CORBA::ULong const length = tc_content->length ();
      CORBA::TypeCode_var const cont_type = tc_content->content_type ();
      char const* const name = cont_type->name ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT (" = %C [%u]\n"),
                  name,
                  length ));

      if (length == 2u)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Correct result, test passed\n")));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Error: Should have been %C [2]\n"), name));
          return 2;
        }
    }
  catch (::CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("ERROR : unexpected CORBA exception caught:");
      return 1;
    }
  return 0;
}
