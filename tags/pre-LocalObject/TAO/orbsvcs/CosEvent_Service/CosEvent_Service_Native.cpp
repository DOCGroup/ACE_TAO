// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "ace/Get_Opt.h"

ACE_RCSID(CosEvent_Service, CosEvent_Service_Native, "$Id$")

class EventChannel : public TAO_CEC_EventChannel
{
  // = TITLE
  //   EventChannel
  //
  // = DESCRIPTION
  //   Since this process only implements the EventService interface
  //   it makes sense to terminate it once the EventService is
  //   destroyed.
  //   To do so we override the default implementation of the
  //   EventChannel::destroy() operation to set a flag that terminates
  //   an application controled event loop.
  //   We don't use ORB::shutdown() because that leaves the ORB is a
  //   state where it is hard to cleanup the system.
  //   
public:
  EventChannel (const TAO_CEC_EventChannel_Attributes &attr,
                int *terminate_flag);

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Override the default implementation to shutdown the ORB too.

private:
  int *terminate_flag_;
  // This flag is set to 1 when the destroy() method is invoked.
};

int
main (int argc, char* argv[])
{
  TAO_CEC_Default_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************
      
      // Parse the options, check if we should bind with the naming
      // service and under what name...
      ACE_Get_Opt get_opt (argc, argv, "n:xr");
      int opt;
      const char *service_name = "CosEventService";
      int bind_to_naming_service = 1;
      int use_rebind = 0;

      while ((opt = get_opt ()) != EOF)
        {
          switch (opt)
            {
            case 'n':
              service_name = get_opt.optarg;
              break;

            case 'x':
              bind_to_naming_service = 0;
              break;

            case 'r':
              use_rebind = 1;
              break;

            case '?':
            default:
              ACE_DEBUG ((LM_DEBUG,
                          "Usage: %s "
                          "-n service_name "
                          "-x [disable naming service bind] "
                          "-r [rebind, no AlreadyBound failures] "
                          "\n",
                          argv[0]));
              return -1;
            }
        }

      // ****************************************************************

      // Control the event loop
      int terminate_flag = 0;

      // Create and activate the event service
      TAO_CEC_EventChannel_Attributes attributes (poa.in (),
                                                  poa.in ());

      EventChannel ec_impl (attributes, &terminate_flag);
      ec_impl.activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CosEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      CosNaming::Name channel_name (1);
      CosNaming::NamingContext_var naming_context;

      if (bind_to_naming_service)
        {
          CORBA::Object_var obj =
            orb->resolve_initial_references ("NameService",
                                             ACE_TRY_ENV);
          ACE_TRY_CHECK;

          naming_context =
            CosNaming::NamingContext::_narrow (obj.in (),
                                               ACE_TRY_ENV);
          ACE_TRY_CHECK;

          channel_name.length (1);
          channel_name[0].id = CORBA::string_dup (service_name);

          if (use_rebind)
            {
              naming_context->rebind (channel_name,
                                      event_channel.in (),
                                      ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else
            {
              naming_context->bind (channel_name,
                                    event_channel.in (),
                                    ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
        }

      while (!terminate_flag)
        {
          ACE_Time_Value tv (0, 20000);
          orb->perform_work (tv, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      if (bind_to_naming_service)
        {
          naming_context->unbind (channel_name,
                                  ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      
      poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, argv[0]);
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

// ****************************************************************

EventChannel::EventChannel (const TAO_CEC_EventChannel_Attributes &attr,
                            int *terminate_flag)
  :  TAO_CEC_EventChannel (attr),
     terminate_flag_ (terminate_flag)
{
}

void
EventChannel::destroy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  *(this->terminate_flag_) = 1;

  this->TAO_CEC_EventChannel::destroy (ACE_TRY_ENV);
}
