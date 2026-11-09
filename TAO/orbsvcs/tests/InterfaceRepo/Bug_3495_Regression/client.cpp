#include "ace/Get_Opt.h"
#include "tao/IFR_Client/IFR_BasicC.h"
// Must include this header file and link to TAO_IFR_Client.lib
// to dynamically load this necessary library.
#include "tao/IFR_Client/IFR_Client_Adapter_Impl.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const*& ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-k <ior>]\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_TCHAR const* ior {ACE_TEXT ("file://test.ior")};
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (ior);

      CORBA::InterfaceDef_var const interface_def = object->_get_interface ();

      if (!CORBA::is_nil (interface_def.in ()))
        {
          CORBA::InterfaceDef::FullInterfaceDescription_var const description {
            interface_def->describe_interface ()
          };

          CORBA::TypeCode_ptr const parameter_type =
            description->operations[0].parameters[0].type.in ();
          if (parameter_type->member_count () != 2)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("ERROR Incorrect parameter member count %d expected 2.\n"),
                               parameter_type->member_count ()),
                              1);

          CORBA::TypeCode_ptr const exception_type =
            description->operations[0].exceptions[0].type.in ();
          if (exception_type->member_count () != 1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("ERROR Incorrect exception member count %d expected 1.\n"),
                               exception_type->member_count ()),
                              1);
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR failed to get interface for object.\n")),
                          1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
