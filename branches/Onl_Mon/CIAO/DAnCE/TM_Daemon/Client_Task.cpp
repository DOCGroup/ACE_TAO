#include "Client_Task.h"

Client_Task::Client_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))

{
}

int
Client_Task::svc (void)
{
  try
    {
//       while (!manager->got_all_response ())
//         {
//           if (orb->work_pending ())
//             {
//               orb->perform_work ();
//             }
//         }

      this->orb_->run ();
    }
  catch (const CORBA::Exception&){}
  return 0;
}
