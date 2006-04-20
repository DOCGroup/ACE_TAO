// $Id$

#include "Consumer_Handler.h"
#include "tao/ORB_Core.h"

ACE_RCSID(Consumer, Consumer_Handler, "$Id$")

Consumer_Handler::Consumer_Handler (void)
  : receiver_ (0),
    notifier_ (0),
    shutdowncallback (0)
{
  // No-Op.
}

Consumer_Handler::~Consumer_Handler (void)
{
  // No-Op.
}

int
Consumer_Handler::init (int argc,
                        char *argv[],
                        ShutdownCallback *_shutdowncallback)
{
  const char *filtering_criteria = "";

   // First see if we have any environment variables.
  filtering_criteria =
    ACE_OS::getenv ("FILTERING_CRITERIA");

   // Then override these variables with command-line arguments if
   // necessary.
  filtering_criteria = argc > 1 ? argv[1] : "";

  ACE_TRY_NEW_ENV
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    0
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object  =
        this->orb_->resolve_initial_references("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Save the Shutdown callback.
      this->shutdowncallback = _shutdowncallback;
      // Set the ShutdownCallback callback object
      // in the Consumer object implementation.
      this->receiver_i_.set (_shutdowncallback);

      // Start the servant.
      this->receiver_ =
        this->receiver_i_._this (ACE_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;

      if (this->get_notifier () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the notifier "
                           "the TAO_Naming_Client. \n"),
                          -1);

      // Subscribe ourselves with the notifier's broker.
      this->notifier_->subscribe (this->receiver_.in (),
                                  filtering_criteria
                                  ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
   {
     ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                          "Consumer_Handler::init\n");
     return -1;
   }
  ACE_ENDTRY;

  return 0;
}

int
Consumer_Handler::get_notifier (void)
{
  ACE_TRY_NEW_ENV
    {
      // Initialization of the naming service.
      if (naming_services_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      CosNaming::Name notifier_ref_name (1);
      notifier_ref_name.length (1);
      notifier_ref_name[0].id = CORBA::string_dup (NOTIFIER_BIND_NAME);

      CORBA::Object_var notifier_obj =
        this->naming_services_client_->resolve (notifier_ref_name
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The CORBA::Object_var object is downcast to Notifier_var
      // using the <_narrow> method.
      this->notifier_ =
        Event_Comm::Notifier::_narrow (notifier_obj.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Consumer_Handler::get_notifier\n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

void
Consumer_Handler::close (void)
{
  this->orb_->shutdown ();
}

void
Consumer_Handler::shutdown (void)
{
  ACE_ASSERT (this->shutdowncallback != 0);

  this->shutdowncallback->close ();
}

int
Consumer_Handler::run (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Running the Consumer...\n"));

  // Run the ORB.
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  return 0;
}

ACE_Reactor*
Consumer_Handler::reactor(void)
{
  // @@ Please see if there's a way to get to the Reactor without
  // using the TAO_ORB_Core_instance().
  return TAO_ORB_Core_instance ()->reactor ();
}

Event_Comm::Consumer_ptr
Consumer_Handler::receiver (void)
{
  return this->receiver_.in ();
}

Event_Comm::Notifier_ptr
Consumer_Handler::notifier (void)
{
  return this->notifier_.in ();
}
