// $Id$

#include "Consumer_Handler.h"

ACE_RCSID(Consumer, Consumer_Handler, "$Id$")

Consumer_Handler::Consumer_Handler (void)
: receiver_ (0),
  notifier_ (0)
{
  // No-Op.
}

// Destroy a Receiver target object.

Consumer_Handler::~Consumer_Handler (void)
{
  // No-Op.
}

int
Consumer_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  if (this->receiver_ != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "closing down Consumer_Handler\n"));

      CORBA::release (this->receiver_.in ());
      this->receiver_ = 0;
      CORBA::release (this->notifier_.in ());
      this->notifier_ = 0;
    }
  return 0;
}

int
Consumer_Handler::init (int argc, char *argv[])
{
  char *filtering_criteria = "";

   // First see if we have any environment variables.
  filtering_criteria = ACE_OS::getenv ("FILTERING_CRITERIA");

   // Then override these variables with command-line arguments.
  filtering_criteria = argc > 1 ? argv[1] : "";

  TAO_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (argc,
				    argv,
				    0,
				    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // start the servant.
      this->receiver_ =
	this->receiver_i_._this (TAO_TRY_ENV);

      TAO_CHECK_ENV;

      if (this->get_notifier() == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the notifier "
                           "the TAO_Naming_Client. \n"),
                          -1);

      // Subscribe ourselves with the notifier's broker.
      this->notifier_->subscribe (this->receiver_,
				  filtering_criteria,
				  TAO_TRY_ENV);
      return 0;
    }
  TAO_CATCHANY
   {
     TAO_TRY_ENV.print_exception ("Consumer_Handler::init\n");
     return -1;
   }
  TAO_ENDTRY;
  return 0;
}

int
Consumer_Handler::get_notifier(void)
{
 TAO_TRY
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
        this->naming_services_client_->resolve (notifier_ref_name,
                                                TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The CORBA::Object_var object is downcast to Notifier_var
      // using the <_narrow> method.
      this->notifier_ =
         Event_Comm::Notifier::_narrow (notifier_obj.in (),
					TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
 TAO_CATCHANY
   {
     TAO_TRY_ENV.print_exception ("Consumer_Handler::get_notifier\n");
     return -1;
   }
 TAO_ENDTRY;

 return 0;
}

void
Consumer_Handler:: close (void)
{
  this->orb_->shutdown();
}

int
Consumer_Handler::run (void)
{
  // Run the ORB.
  this->orb_->run ();
  return 0;
}

ACE_Reactor*
Consumer_Handler::reactor(void)
{
  return TAO_ORB_Core_instance ()->reactor ();
}

Event_Comm::Consumer *
Consumer_Handler::receiver (void)
{
  return this->receiver_;
}

Event_Comm::Notifier *
Consumer_Handler::notifier (void)
{
  return this->notifier_;
}
