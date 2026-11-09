#include "TestC.h"
#include "ace/Get_Opt.h"

struct Client_Options
{
  ACE_TCHAR const *ior { ACE_TEXT ("file://server.ior") };
};

constexpr ACE_TCHAR uipmc_ior[] =
  ACE_TEXT ("corbaloc:miop:1.0@1.0-domain-1/127.0.0.1:23232");

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                                     "-k <ior> "
                                     "\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result {};
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var obj = orb->string_to_object (uipmc_ior);

      CORBA::Object_var tmp = orb->string_to_object (options.ior);
      server_var server = server::_narrow (tmp.in ());

      try
        {
          server->method (obj.in ());

          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test passed !!\n")));
        }
      catch (CORBA::SystemException const &marshal)
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Test Failed - Regression. "
                                          "Sending UIPMC object ref to server failed with:\n")));
          marshal._tao_print_exception ("Exception :\n");
          result = 1;
        }

      server->shutdown ();
    }
  catch (CORBA::SystemException const &e)
    {
      e._tao_print_exception ("Unexpected exception - not a regression.\n");
      result = 1;
    }
  return result;
}
