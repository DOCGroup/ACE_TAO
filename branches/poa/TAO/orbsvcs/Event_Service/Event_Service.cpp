//
// $Id$
//

#include "ace/Get_Opt.h"
#include "tao/corba.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Event_Utilities.h"
#include "Event_Channel.h"



int main (int argc, char *argv[])
{
  TAO_TRY
    {
      // Initialize ORB.
      CORBA::ORB_ptr orb = 
	CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
      TAO_CHECK_ENV;

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
      TAO_CHECK_ENV;

      CosNaming::NamingContext_var naming_context = 
        CosNaming::NamingContext::_narrow (objref, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "got reference to NameService\n"));

      ACE_Scheduler_Factory::use_config (naming_context.ptr ());

      // Register Event_Service with Naming Service.
      ACE_EventChannel* ec;
      ACE_NEW_RETURN (ec, ACE_EventChannel, -1);
      TAO_CHECK_ENV;

      RtecEventChannelAdmin::EventChannel_ptr impl = 
	ec->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String str =
	orb->object_to_string (impl, TAO_TRY_ENV);
      ACE_OS::puts ((char *) str);

      CosNaming::Name channel_name (1);
      channel_name[0].id = CORBA::string_dup ("EventService");
      channel_name.length (1);
      naming_context->bind (channel_name, impl, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      orb->run ();

      naming_context->unbind (channel_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::release (impl);
      TAO_CHECK_ENV;

      delete ec;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("EC");
    }
  TAO_ENDTRY;


  return 0;
}
