// -*- C++ -*-
// $Id$
// The run_test.pl perl script will check for the expected output.

#include "ace/config.h"
#include "tao/IFR_Client/IFR_ComponentsC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ifr_ior= ACE_TEXT("file://ifr.ior");
const ACE_TCHAR *member_type_id= ACE_TEXT("IDL:arrayOfStruct:1.0");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        ifr_ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var the_orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var obj = the_orb->string_to_object (ACE_TEXT_ALWAYS_CHAR(ifr_ior));
      if (CORBA::is_nil (obj.in()))
        {
          ACE_DEBUG ((LM_DEBUG, "Obtained nil reference to Object for interface repository\n" ));
          return 1;
        }

      CORBA::ComponentIR::Repository_var ifr;

      try
        {
          ifr = CORBA::ComponentIR::Repository::_narrow (obj.in());
        }
      catch (CORBA::SystemException &)
        {
          ACE_DEBUG ((LM_DEBUG, "exception catched .. narrowing IFR object\n" ));
          return 1;
        }

      if (CORBA::is_nil (ifr.in()))
        {
          ACE_DEBUG ((LM_DEBUG, "Can't narrow to IFR\n" ));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "Looking up %C", member_type_id ));
      CORBA::Contained_var
        dependency = ifr->lookup_id (ACE_TEXT_ALWAYS_CHAR(member_type_id));
      if (CORBA::is_nil (dependency.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, " *** failed ***\n" ));
          return 1;
        }

      CORBA::AliasDef_var
          the_alias = CORBA::AliasDef::_narrow (dependency.in ());
      if (CORBA::is_nil (the_alias.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, " *** not an alias ***\n" ));
          return 1;
        }

      CORBA::IDLType_var
        orig_type = the_alias->original_type_def ();
      CORBA::TypeCode_var
        tc_content = orig_type->type ();
      const CORBA::ULong
        length= tc_content->length ();
      CORBA::TypeCode_var
        cont_type = tc_content->content_type ();
      const char
        *name= cont_type->name ();
      ACE_DEBUG ((LM_DEBUG,
                  " = %C [%u]\n",
                  name,
                  length ));

      if (2u == length)
        {
          ACE_DEBUG ((LM_DEBUG, "Correct result, test passed\n" ));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Error: Should have been %C [2]\n", name));
          return 2;
        }
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      return 1;
    }
  return 0;
}
