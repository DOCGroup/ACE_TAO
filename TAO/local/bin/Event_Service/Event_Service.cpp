//
// $Id$
//

#include "ace/Get_Opt.h"
#include "tao/corba.h"

#include "CosNamingC.h"
#include "Scheduler_Factory.h"
#include "Event_Channel.h"
#include "Event_Utilities.h"



int main (int argc, char *argv[])
{
  ACE_TRY
    {
      // Initialize ORB.
      CORBA::ORB_ptr orb = 
	CORBA::ORB_init (argc, argv, "internet", ACE_TRY_ENV);
      ACE_CHECK_ENV;

      CORBA::POA_ptr poa =
	orb->POA_init(argc, argv, "POA");
      if (poa == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     " (%P|%t) Unable to initialize the POA.\n"),
			    1);
	}

      CORBA::Object_ptr objref =
	orb->resolve_initial_references ("NameService");
      ACE_CHECK_ENV;

      CosNaming::NamingContext_var naming_context = 
        CosNaming::NamingContext::_narrow (objref, ACE_TRY_ENV);
      ACE_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "got reference to NameService\n"));

      ACE_Scheduler_Factory::use_config (naming_context.ptr ());

      // Register Event_Service with Naming Service.
      ACE_EventChannel* ec;
      ACE_NEW_RETURN (ec, ACE_EventChannel, -1);
      CORBA::Object::_duplicate(ec);
      ACE_CHECK_ENV;

      CORBA::String str =
	orb->object_to_string (ec, ACE_TRY_ENV);
      ACE_OS::puts ((char *) str);

      CosNaming::Name channel_name (1);
      channel_name[0].id = CORBA::string_dup ("EventService");
      channel_name.length (1);
      naming_context->bind (channel_name, ec, ACE_TRY_ENV);
      ACE_CHECK_ENV;

      orb->run ();

      CORBA::release (ec);
      ACE_CHECK_ENV;
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("EC");
    }
  ACE_ENDTRY;


  return 0;
}
