// $Id$

#include "ace/Get_Opt.h"
#include "tao/IFR_Client/IFR_BasicC.h"
// Must include this header file and link to TAO_IFR_Client.lib
// to dynamically load this necessary library.
#include "tao/IFR_Client/IFR_Client_Adapter_Impl.h"




const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      CORBA::InterfaceDef_var ifd = tmp->_get_interface();

      if ( ! CORBA::is_nil (ifd.in()))
      {
        CORBA::InterfaceDef::FullInterfaceDescription *id = ifd->describe_interface();

        CORBA::TypeCode_var paramType (id->operations[0].parameters[0].type.in());
        if (paramType->member_count () != 2)
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            ACE_TEXT("ERROR Incorrect parameter member count %d expected 2.\n"),
                            paramType->member_count ()),
                            1);
        }
        CORBA::TypeCode_var exceptionType (id->operations[0].exceptions[0].type.in());
        if (exceptionType->member_count () != 1)
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            ACE_TEXT("ERROR Incorrect exception member count %d expected 1.\n"),
                            exceptionType->member_count ()),
                            1);
        }
      }
      else
      {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("ERROR failed to get interface for object.\n")),
                          1);
      }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
