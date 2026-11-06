#include "tao/IFR_Client/IFR_ComponentsC.h"
#include "ace/Get_Opt.h"
#include "tao/ORB.h"

struct Test_Options
{
  ACE_TCHAR const* ifr_ior_file {};
  ACE_TCHAR const* idl_value {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Test_Options& options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:s:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'i':
          options.ifr_ior_file = get_opts.opt_arg ();
          break;
        case 's':
          options.idl_value = get_opts.opt_arg ();
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -i <ifr_ior> -s <idl_valuetype>\n"),
                             argv [0]),
                            -1);
        }
    }

  if (options.ifr_ior_file == nullptr || options.idl_value == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("usage: %s -i <ifr_ior> -s <idl_valuetype>\n"),
                       argv [0]),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Test_Options options;

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) == -1)
        return -1;

      CORBA::Object_var objref =
        orb->string_to_object (options.ifr_ior_file);
      if (CORBA::is_nil (objref.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("The received objref is nil\n")), -1);

      CORBA::ComponentIR::Repository_var the_repo_ref;
      try
        {
          the_repo_ref = CORBA::ComponentIR::Repository::_narrow (objref.in ());
        }
      catch (CORBA::Exception const& ex)
        {
          ex._tao_print_exception ("Can't narrow the IFR:");
          return 1;
        }

      CORBA::Contained_var current_contained =
        the_repo_ref->lookup_id (ACE_TEXT_ALWAYS_CHAR (options.idl_value));
      if (CORBA::is_nil (current_contained.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Can't look up the valuetype\n")), -1);

      CORBA::ExtValueDef_var value_def =
        CORBA::ExtValueDef::_narrow (current_contained.in ());
      CORBA::ExtValueDef::ExtFullValueDescription_var value_descr;
      try
        {
          value_descr = value_def->describe_ext_value ();
        }
      catch (CORBA::Exception const& ex)
        {
          ex._tao_print_exception ("Can't describe_ext_value:");
          return 1;
        }

      CORBA::ValueMemberSeq const& the_value_members =
        value_descr->members;
      for (CORBA::ULong ct {}; ct < the_value_members.length (); ++ct)
        {
          CORBA::ValueMember const& current_member = the_value_members [ct];
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("value type member '%C'\n"),
                      current_member.name.in ()));
        }
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("MAIN: Unexpected CORBA exception caught:");
      return 1;
    }
  return 0;
}
