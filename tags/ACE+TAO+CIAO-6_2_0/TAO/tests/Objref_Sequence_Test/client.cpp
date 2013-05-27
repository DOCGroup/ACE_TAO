//$Id$
#include "TestC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
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
  // Indicates successful parsing of the command line
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{



  try
    {
      CORBA::ORB_var orb  =
        CORBA::ORB_init (argc,
                         argv);

      if (parse_args (argc, argv) == -1)
        return -1;

      // Connect to the server
      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Server_var server = Server::_narrow (tmp.in ());

      // Create object instances
      // This portion of the test was given by Petr Tuma and am just
      // leaving it alone -- Bala
      ServerSequence_var      vServers;
      ServerSequence_var      vAddition;

      CORBA::ULong iStep;
      CORBA::ULong iSize, iOldSize, iAddSize;
      CORBA::ULong iCount;

      ACE_NEW_RETURN (vServers,
                      ServerSequence (),
                      -1);

      iSize = 1;
      iStep = 1;
      while (iSize <= 50000)
        {
          iOldSize = vServers->length ();
          iAddSize = iSize - iOldSize;

          server->CreateExtra (iAddSize,
                               vAddition.out ());


          vServers->length (iSize);
          for (iCount = 0 ; iCount < iAddSize ; iCount ++)
            {
              vServers [iOldSize + iCount] =
                Server::_duplicate (vAddition [iCount]);
            };

          vAddition = 0;

          if (iStep * 5 > iSize)
            {
              iSize += iStep;
            }
          else
            {
              iStep *= 10;
              iSize = iStep;
            };
        };

      //------------- End of donated untouched section

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Call delete on the server\n"));

      server->DeleteExtra (vServers.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Calling shutdown\n"));
      server->shutdown ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("");
    }

  return 0;
}
