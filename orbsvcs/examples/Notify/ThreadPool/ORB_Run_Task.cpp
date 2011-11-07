// $Id$

#include "ORB_Run_Task.h"



TAO_Notify_ORB_Run_Task::TAO_Notify_ORB_Run_Task (TAO_Notify_ORB_Objects& orb_objects)
  : orb_objects_ (orb_objects)
{
}

TAO_Notify_ORB_Run_Task::~TAO_Notify_ORB_Run_Task ()
{
}

int
TAO_Notify_ORB_Run_Task::svc (void)
{
  try
    {
      this->orb_objects_.current_->the_priority (0);

      this->orb_objects_.orb_->run ();
    }
  catch (const CORBA::Exception&)
    {
    }

  return 0;
}
