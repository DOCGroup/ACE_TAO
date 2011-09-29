// $Id$

#include "tao/IFR_Client/IFR_ComponentsC.h"
#include "ace/Get_Opt.h"
#include "tao/ORB.h"

namespace
{
  const ACE_TCHAR *ifr_ior_file = 0;
  const ACE_TCHAR *idl_value = 0;
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:s:"));
  const unsigned char full_success = 0x03;
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
                              " -s <idl_valuetype>"
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
        case 's':
          idl_value = get_opts.opt_arg ();
          success |= 0x02;
          break;
        }
    }

  // Indicates successful parsing of the command line
  return 0;
}

// ----------------------------------------------------------------------

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // init orb
      CORBA::ORB_var the_orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == -1)
        {
          return -1;
        }

      // get IFR
      CORBA::Object_var objref =
        the_orb->string_to_object (ifr_ior_file);
      if (objref.in () == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "The received objref is nil\n"),
                            -1);
        }

      CORBA::ComponentIR::Repository_var the_repo_ref;
      try
        {
          the_repo_ref = CORBA::ComponentIR::Repository::_narrow (objref.in ());
        }
      catch (CORBA::Exception &ex)
        {
          ex._tao_print_exception ("Can't narrow the IFR:");
          return 1;
        }

      // search in repository
      CORBA::Contained_var current_contained =
        the_repo_ref->lookup_id (ACE_TEXT_ALWAYS_CHAR (idl_value));
      if (CORBA::is_nil(current_contained.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "Can't look up the valuetype\n"),
                            -1);
        }

      // get value type definition
      CORBA::ExtValueDef_var value_def =
        CORBA::ExtValueDef::_narrow (current_contained.in ());
      CORBA::ExtValueDef::ExtFullValueDescription_var value_descr;
      try
        {
          value_descr = value_def->describe_ext_value ();
        }
      catch (CORBA::Exception &ex)
        {
          ex._tao_print_exception ("Can't describe_ext_value:");
          return 1;
        }

      CORBA::ValueMemberSeq& the_value_members =
        value_descr->members;
      for (CORBA::ULong ct = 0; ct < the_value_members.length (); ++ct)
      {
        const CORBA::ValueMember& current_member =
          the_value_members [ct];
        ACE_DEBUG ((LM_DEBUG,
                    "value type member '%C'\n",
                    current_member.name.in ()));
      }
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("MAIN: Unexpected CORBA exception caught:");
      return 1;
    }
  return 0;
}
