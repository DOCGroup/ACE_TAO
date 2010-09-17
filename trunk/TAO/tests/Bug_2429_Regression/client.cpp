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
   childMethod (void)
    {
    }

   virtual void
   childMethod_excep (Messaging::ExceptionHolder *
                         excep_holder)
    {
      try
        {
          excep_holder->raise_exception ();
        }
      catch (const CORBA::SystemException& ex)
        {
          ex._tao_print_exception ("Reply_Handler::childMethod_excep: ");
        }
    }

   virtual void
   parentMethod (void)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "client: parentMethod reply %d @ %T\n",
                  ++parentMethod_count));

    }

   virtual void
   parentMethod_excep (Messaging::ExceptionHolder * excep_holder)
   {
      try
        {
          ++parentMethod_excep_count;
          excep_holder->raise_exception ();
        }
      catch (const CORBA::INTERNAL&)
        {
          ACE_DEBUG ((LM_DEBUG, "Successfully received an Internal Excep*ion as expected.\n"));
        }
      catch (const CORBA::SystemException& ex)
        {
          ex._tao_print_exception ("Reply_Handler::childMethod_excep: ");
        }
   }

   CORBA::ULong reply_count (void) { return parentMethod_count + parentMethod_excep_count; };

 private:
   // Count of replies and excep
   CORBA::ULong parentMethod_count;
   CORBA::ULong parentMethod_excep_count;
};

// Server ior.
static const ACE_TCHAR *server_ior = ACE_TEXT("file://server.ior");

static int
client_parse_args(int argc, ACE_TCHAR *argv[])
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

   // Indicates successful parsing of the command line
   return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  CORBA::ORB_var orb;

  try
    {
      // Initialize the ORB.
      orb = CORBA::ORB_init(argc, argv);

      // Initialize options based on command-line arguments.
      int parse_args_result = client_parse_args(argc, argv);
      if (parse_args_result != 0)
        {
          return 1;
        }

      CORBA::Object_var object =
         orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
         PortableServer::POA::_narrow (object.in ());

      // Get an object reference from the nominated file
      object = orb->string_to_object (server_ior);


      PortableServer::POAManager_var poa_manager =
         root_poa->the_POAManager();

      poa_manager->activate();

      Child_var child = Child::_narrow (object.in());

      Reply_Handler reply_handler_servant;

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&reply_handler_servant);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      AMI_ChildHandler_var reply_handler_object =
        AMI_ChildHandler::_narrow (object_act.in ());

      // Invoke the AMI parentMethod
      child->sendc_parentMethod (reply_handler_object.in ());

      // Loop until all replies have been received.
      while (reply_handler_servant.reply_count () == 0)
        {
          orb->perform_work ();
        }

      // Shutdown server.
      child->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

   return 0;
}
