#include "ORB_Run_Task.h"

TAO_Notify_ORB_Run_Task::TAO_Notify_ORB_Run_Task (TAO_Notify_ORB_Objects& orb_objects)
  : orb_objects_ (orb_objects)
{
}

int
TAO_Notify_ORB_Run_Task::svc ()
{
  try
    {
      this->orb_objects_.current_->the_priority ({});

      this->orb_objects_.orb_->run ();
    }
  catch (CORBA::Exception const&)
    {
    }

  return 0;
}
