#include "Activator_ORB_Runner.h"
#include "Activator_Loader.h"

ImR_Activator_ORB_Runner::ImR_Activator_ORB_Runner (ImR_Activator_Loader& service)
  : service_ (service)
{
}

int 
ImR_Activator_ORB_Runner::svc () 
{
  // Block until service_.fini() calls orb->destroy()
  this->service_.run ();
  return 0;
}
