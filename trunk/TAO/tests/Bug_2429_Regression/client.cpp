// $Id$

#include "ace/Get_Opt.h"
#include "tao/Messaging/Messaging.h"
#include "ChildS.h"

class Reply_Handler
 : public POA_AMI_ChildHandler
{
 public:
   Reply_Handler()
   : parentMethod_count (0),
     parentMethod_excep_count (0)
    {
    }

   virtual void
   childMethod (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

   virtual void
   childMethod_excep (Messaging::ExceptionHolder *
                         excep_holder ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ACE_TRY
        {
          excep_holder->raise_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::SystemException, ex)
        {
          ACE_PRINT_EXCEPTION (ex, "Reply_Handler::childMethod_excep: ");
        }
      ACE_ENDTRY;
    }

   virtual void
   parentMethod (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "client: parentMethod reply %d @ %T\n",
                  ++parentMethod_count));

    }

   virtual void
   parentMethod_excep (Messaging::ExceptionHolder * excep_holder
              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
   ACE_THROW_SPEC ((CORBA::SystemException))
   {
      ACE_TRY
        {
          ++parentMethod_excep_count;
          excep_holder->raise_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::INTERNAL, iex)
        {
          ACE_DEBUG ((LM_DEBUG, "Successfully received an Internal Excep*ion as expected.\n"));
        }
      ACE_CATCH (CORBA::SystemException, ex)
        {
          ACE_PRINT_EXCEPTION (ex, "Reply_Handler::childMethod_excep: ");
        }
      ACE_ENDTRY;
   }

   CORBA::ULong reply_count (void) { return parentMethod_count + parentMethod_excep_count; };

 private:
   // Count of replies and excep
   CORBA::ULong parentMethod_count;
   CORBA::ULong parentMethod_excep_count;
};

// Server ior.
static const char * server_ior = "file://server.ior";

static int
client_parse_args(int argc, char *argv[])
{
   ACE_Get_Opt
   get_opts(argc, argv, "f:");
   int c;

   while ((c = get_opts()) != -1)
   {
      switch (c)
      {
      case 'f':
         server_ior = get_opts.opt_arg();
         break;
      case '?':
      default:
         ACE_DEBUG((LM_ERROR,
                           "usage:  %s "
                           "-f server IOR file\n"));
         return 1;
      }
   }

   // Indicates sucessful parsing of the command line
   return 0;
}

int
main(int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  CORBA::ORB_var orb;

  ACE_TRY
    {
      // Initialize the ORB.
      orb = CORBA::ORB_init(argc, argv, 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize options based on command-line arguments.
      int parse_args_result = client_parse_args(argc, argv);
      if (parse_args_result != 0)
        {
          return 1;
        }

      CORBA::Object_var object =
         orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
         PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get an object reference from the nominated file
      object = orb->string_to_object (server_ior);

      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
         root_poa->the_POAManager(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Child_var child = Child::_narrow (object.in() ACE_ENV_ARG_PARAMETER);

      Reply_Handler reply_handler_servant;

      AMI_ChildHandler_var reply_handler_object =
         reply_handler_servant._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Invoke the AMI parentMethod
      child->sendc_parentMethod (reply_handler_object.in ()
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Loop until all replies have been received.
      while (reply_handler_servant.reply_count () == 0)
        {
          orb->perform_work ();
        }

      // Shutdown server.
      child->shutdown ();
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

   return 0;
}
