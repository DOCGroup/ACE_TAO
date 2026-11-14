#include "TestC.h"
#include "ace/Get_Opt.h"
#include <string>

struct Options
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
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
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return -1;

      CORBA::Object_var const tmp = orb->string_to_object (options.ior.c_str ());
      Server_var const server = Server::_narrow (tmp.in ());

      // This sequence-growth pattern is part of the regression scenario.
      ServerSequence_var vServers {};
      ServerSequence_var vAddition {};
      ACE_NEW_RETURN (vServers,
                      ServerSequence (),
                      -1);

      CORBA::ULong size = 1;
      CORBA::ULong step = 1;
      while (size <= 50000)
        {
          CORBA::ULong const old_size = vServers->length ();
          CORBA::ULong const add_size = size - old_size;
          server->CreateExtra (add_size, vAddition.out ());

          vServers->length (size);
          for (CORBA::ULong count {}; count < add_size; ++count)
            vServers[old_size + count] = Server::_duplicate (vAddition[count]);

          vAddition = nullptr;

          if (step * 5 > size)
            size += step;
          else
            {
              step *= 10;
              size = step;
            }
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Call delete on the server\n"));
      server->DeleteExtra (vServers.in ());
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Calling shutdown\n"));
      server->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("");
    }
  return 0;
}
