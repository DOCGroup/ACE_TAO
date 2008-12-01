// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/IFR_Client/IFR_BasicC.h"
// Must include this header file and link to TAO_IFR_Client.lib
// to dynamically load this necessary library.
#include "tao/IFR_Client/IFR_Client_Adapter_Impl.h"

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
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
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "");

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      CORBA::InterfaceDef_var ifd = tmp->_get_interface();
      if ( ! CORBA::is_nil (ifd.in()))
      {
        CORBA::InterfaceDef::FullInterfaceDescription *id = ifd->describe_interface();
      }
      Test::GoodDay_var goodday =
        Test::GoodDay::_narrow(tmp.in ());

      if (CORBA::is_nil (goodday.in ()))
        {
          ACE_ERROR ((LM_DEBUG,
                             "Nil Test::GoodDay reference <%s>\n",
                             ior));
        }
      else
      {

        CORBA::String_var the_string =
          goodday->get_string ();

        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
                    the_string.in ()));
      }

      goodday->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
  return 0;
}
