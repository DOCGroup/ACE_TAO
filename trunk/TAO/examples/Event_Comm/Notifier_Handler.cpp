// $Id$

#include "Notifier_Handler.h"

ACE_RCSID(Supplier, Notifier_Handler, "$Id$")

Notifier_Handler::Notifier_Handler (void)
{
  // No-Op.
}

// Destroy a Notifier target object.

Notifier_Handler::~Notifier_Handler (void)
{
  // No-Op.
}

int
Notifier_Handler::close (void)
{
  if (this->notifier_ != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "closing down Notifier_Handler\n"));
      CORBA::release (this->notifier_);
      this->notifier_ = 0;
    }

  // shutdown the ORB.
  this->orb_->shutdown ();
  return 0;
}

void
Notifier_Handler::shutdown (void)
{
  ACE_ASSERT (this->shutdowncallback != 0);

  this->shutdowncallback->close ();
}

int
Notifier_Handler::run (void)
{
  // Run the ORB.
  this->orb_->run ();
  return 0;
}

ACE_Reactor*
Notifier_Handler::reactor(void)
{
  // @@ Please see if there's a way to get to the Reactor without
  // using the TAO_ORB_Core_instance().
  return TAO_ORB_Core_instance ()->reactor ();
}

Event_Comm::Notifier *
Notifier_Handler::notifier (void)
{
  return this->notifier_;
}

void
Notifier_Handler::notifier (Event_Comm::Notifier *notifier)
{
  if (this->notifier_ != notifier)
    {
      CORBA::release (this->notifier_);
      this->notifier_ = notifier;
    }
}

// Init function.

int
Notifier_Handler::init (int argc, 
			char *argv[], 
			ShutdownCallback* _shutdowncallback)
{
  // set the callback
 shutdowncallback = _shutdowncallback;

 ACE_TRY_NEW_ENV
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    0,
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Initialization of the naming service.
      if (this->naming_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      CosNaming::Name notifier_ref_name (1);
      notifier_ref_name.length (1);
      notifier_ref_name[0].id =
      CORBA::string_dup (NOTIFIER_BIND_NAME);

      CORBA::Object_var notifier_obj =
       this->naming_client_->resolve (notifier_ref_name,
				      ACE_TRY_ENV);
      ACE_TRY_CHECK;


      // The CORBA::Object_var object is downcast to Notifier_var
      // using the <_narrow> method.
      this->notifier_ =
         Event_Comm::Notifier::_narrow (notifier_obj.in (),
					ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;

  }
 ACE_CATCHANY
   {
     ACE_TRY_ENV.print_exception ("Notifier_Handler::init\n");
     return -1;
   }
 ACE_ENDTRY;

 return 0;
}
