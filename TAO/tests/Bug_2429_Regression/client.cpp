#include "ace/Get_Opt.h"
#include "tao/Messaging/Messaging.h"
#include "ChildS.h"

class Reply_Handler
  : public POA_AMI_ChildHandler
{
public:
  Reply_Handler () = default;
  Reply_Handler (Reply_Handler const &) = delete;
  Reply_Handler &operator= (Reply_Handler const &) = delete;
  Reply_Handler (Reply_Handler &&) = delete;
  Reply_Handler &operator= (Reply_Handler &&) = delete;

  void childMethod () override
    {
    }

  void childMethod_excep (Messaging::ExceptionHolder * excep_holder) override
    {
      try
        {
          excep_holder->raise_exception ();
        }
      catch (CORBA::SystemException const &ex)
        {
          ex._tao_print_exception ("Reply_Handler::childMethod_excep: ");
        }
    }

  void parentMethod () override
    {
      ACE_DEBUG ((LM_DEBUG,
                  "client: parentMethod reply %d @ %T\n",
                  ++this->parent_method_count_));
    }

  void parentMethod_excep (Messaging::ExceptionHolder *excep_holder) override
    {
      try
        {
          ++this->parent_method_excep_count_;
          excep_holder->raise_exception ();
        }
      catch (CORBA::INTERNAL const &)
        {
          ACE_DEBUG ((LM_DEBUG, "Successfully received an Internal Exception as expected.\n"));
        }
      catch (CORBA::SystemException const &ex)
        {
          ex._tao_print_exception ("Reply_Handler::parentMethod_excep: ");
        }
    }

  CORBA::ULong reply_count () const
  {
    return this->parent_method_count_ + this->parent_method_excep_count_;
  }

private:
  CORBA::ULong parent_method_count_ {};
  CORBA::ULong parent_method_excep_count_ {};
};

static int
client_parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&server_ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("f:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        server_ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s -f <server IOR file>\n",
                           argv[0]),
                          1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *server_ior = ACE_TEXT ("file://server.ior");
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (client_parse_args (argc, argv, server_ior) != 0)
        {
          return 1;
        }

      CORBA::Object_var object = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (object.in ());

      // Get an object reference from the nominated file
      object = orb->string_to_object (server_ior);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      poa_manager->activate ();

      Child_var const child = Child::_narrow (object.in ());

      Reply_Handler reply_handler_servant;

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&reply_handler_servant);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      AMI_ChildHandler_var const reply_handler_object =
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
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
