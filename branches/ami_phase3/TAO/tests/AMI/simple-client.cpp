// $Id$

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "testC.h"

ACE_RCSID(MT_Client, client, "$Id$")

const char *ior = "file://test.ior";
int niterations = 5;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
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
 
  ~Handler (void) {};
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

      Handler handler;
      AMI_Simple_Server_Handler_var the_handler =
        handler._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Long number = 0;
      
      for (ssize_t ni = 0; ni < niterations; ni++)
        {
        
          server->sendc_get_number (the_handler.in (),
                                    ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      number = server->get_number (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      ACE_DEBUG ((LM_DEBUG,
                  "get_number = %d\n",
                  number));
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
