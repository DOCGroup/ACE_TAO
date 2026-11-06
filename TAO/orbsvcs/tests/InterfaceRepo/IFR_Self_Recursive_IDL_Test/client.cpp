#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/IFR_Client/IFR_BasicC.h"
// Must include this header file and link to TAO_IFR_Client.lib
// to dynamically load this necessary library.
#include "tao/IFR_Client/IFR_Client_Adapter_Impl.h"

static int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const*& ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
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
                           ACE_TEXT("usage: %s [-k <ior>]\n"),
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
      ACE_TCHAR const* ior {ACE_TEXT("file://test.ior")};
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (ior);

      CORBA::InterfaceDef_var const interface_def = object->_get_interface ();
      if (!CORBA::is_nil (interface_def.in ()))
        {
          CORBA::InterfaceDef::FullInterfaceDescription * const description =
            interface_def->describe_interface ();
          if (description == nullptr)
            ACE_ERROR ((LM_ERROR, ACE_TEXT("Null interface description returned\n")));
          CORBA::InterfaceDef::FullInterfaceDescription_var const description_owner {
            description
          };
        }

      Test::GoodDay_var const goodday = Test::GoodDay::_narrow (object.in ());

      if (CORBA::is_nil (goodday.in ()))
        ACE_ERROR ((LM_DEBUG,
                    ACE_TEXT("Nil Test::GoodDay reference <%s>\n"),
                    ior));
      else
        {
          CORBA::String_var const the_string = goodday->get_string ();

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("(%P|%t) - string returned <%s>\n"),
                      the_string.in ()));
        }

      goodday->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
  return 0;
}
