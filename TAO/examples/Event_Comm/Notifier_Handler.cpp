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
  this->handle_close ();
}

int
Notifier_Handler::handle_close (void)
{
  if (this->notifier_ != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "closing down Notifier_Handler\n"));
      CORBA::release (this->notifier_);
      this->notifier_ = 0;
    }

  return 0;
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
Notifier_Handler::init (int argc, char *argv[])
{
 TAO_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    0,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

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
				      TAO_TRY_ENV);
      TAO_CHECK_ENV;


      // The CORBA::Object_var object is downcast to Echo_var using
      // the <_narrow> method.
      this->notifier_ =
         Event_Comm::Notifier::_narrow (notifier_obj.in (),
					TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG,
		  "Notifier initialized.. \n"));

  }
 TAO_CATCHANY
   {
     TAO_TRY_ENV.print_exception ("Notifier_Handler::init\n");
     return -1;
   }
 TAO_ENDTRY;
}
