// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "ace/Get_Opt.h"

ACE_RCSID(CosEvent_Service, CosEvent_Service_Native, "$Id$")

  // @@ Just remove this class, we don't use it...
  //    The "Right Way"[tm] to fix this is to modify
  //    TAO_CEC_EventChannel to get a new option.. If the option is
  //    set then we destroy the ORB on EventChannel::destroy(),
  //    otherwise we don't.
  //    The option will not be set by default, but the user can pass
  //    it in the command line, and in this main program with set it
  //    to 1 by default or something like that...
  //    Don't worry about this change yet... Let's get all the changes
  //    in and then we can fix the EC shutdown problem...
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
      int result;

      // Intialize the ORB
      CORBA::ORB_var orb = 
        CORBA::ORB_init (argc, argv, 0, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Call TAO_Event_Loader::init (argc, argv) from here.
      TAO_Event_Loader event_service;

      // To intialise the service
      result = event_service.init (argc, argv);
      
      if (result == -1)
        return 1;
      
      orb->run();
      
      // Destroy the ORB
      orb->destroy();
      
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
