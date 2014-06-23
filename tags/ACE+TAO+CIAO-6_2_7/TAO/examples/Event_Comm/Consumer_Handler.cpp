// $Id$

#include "Consumer_Handler.h"
#include "tao/ORB_Core.h"

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
                        ACE_TCHAR *argv[],
                        ShutdownCallback *_shutdowncallback)
{
  const char *filtering_criteria = "";

   // First see if we have any environment variables.
  filtering_criteria =
    ACE_OS::getenv ("FILTERING_CRITERIA");

   // Then override these variables with command-line arguments if
   // necessary.
  filtering_criteria = argc > 1 ? ACE_TEXT_ALWAYS_CHAR(argv[1]) : "";

  try
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object  =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();

      poa_manager->activate ();

      // Save the Shutdown callback.
      this->shutdowncallback = _shutdowncallback;
      // Set the ShutdownCallback callback object
      // in the Consumer object implementation.
      this->receiver_i_.set (_shutdowncallback);

      // Start the servant.
      this->receiver_ =
        this->receiver_i_._this ();


      if (this->get_notifier () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the notifier "
                           "the TAO_Naming_Client.\n"),
                          -1);

      // Subscribe ourselves with the notifier's broker.
      this->notifier_->subscribe (this->receiver_.in (),
                                  filtering_criteria);
    }
  catch (const CORBA::Exception& ex)
   {
     ex._tao_print_exception ("Consumer_Handler::init\n");
     return -1;
   }

  return 0;
}

int
Consumer_Handler::get_notifier (void)
{
  try
    {
      // Initialization of the naming service.
      if (naming_services_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client.\n"),
                          -1);

      CosNaming::Name notifier_ref_name (1);
      notifier_ref_name.length (1);
      notifier_ref_name[0].id = CORBA::string_dup (NOTIFIER_BIND_NAME);

      CORBA::Object_var notifier_obj =
        this->naming_services_client_->resolve (notifier_ref_name);

      // The CORBA::Object_var object is downcast to Notifier_var
      // using the <_narrow> method.
      this->notifier_ =
        Event_Comm::Notifier::_narrow (notifier_obj.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Consumer_Handler::get_notifier\n");
      return -1;
    }

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
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::Exception&)
    {
    }
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
