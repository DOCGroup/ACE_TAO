// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "testC.h"

ACE_RCSID(MT_Client, client, "$Id$")

const char *ior = "file://test.ior";
int niterations = 5;
int shutdown_flag = 0;
int debug = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "dk:i:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        debug = 1;
        break;
      case 'k':
        ior = get_opts.optarg;
        break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'x':
        shutdown_flag = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-x "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class Handler : public POA_AMI_Simple_Server_Handler
{
public:
  Handler (void) {};
  // Constructor.

  void get_put_number (CORBA::Long result,
                       CORBA::Long out_l,
                       CORBA::Environment&)
    {
      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Callback method called: result <%d>, out_arg <%d>\n",
                      result, 
                      out_l));
        }
    };

  ~Handler (void) {};
  // Destructor.
};

int
main (int argc, char *argv[])
{

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      object = orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Simple_Server_var server =
        Simple_Server::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      // Instantiate the ReplyHandler and register that with the POA. 
      Handler handler;
      AMI_Simple_Server_Handler_var the_handler =
        handler._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      CORBA::Long l = 931247;

      for (ssize_t ni = 0; ni < niterations; ni++)
        {
          server->sendc_get_put_number (the_handler.in (),
                                        l,
                                        ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "<%d> Asynchronous methods issued\n",
                      niterations));
        }
      
      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Issuing a synchronous method to collect the AMI replies\n"));
        }
      
      CORBA::Long number = server->get_put_number (l,
                                                   l,
                                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (shutdown_flag)
        server->shutdown ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Catched exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
