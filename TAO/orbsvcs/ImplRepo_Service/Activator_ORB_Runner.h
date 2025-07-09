// -*- C++ -*-
#ifndef TAO_IMR_ACTIVATOR_ORB_RUNNER_H
#define TAO_IMR_ACTIVATOR_ORB_RUNNER_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ImR_Activator_Loader;

class ImR_Activator_ORB_Runner : public ACE_Task_Base
{
public:
  ImR_Activator_ORB_Runner (ImR_Activator_Loader& service);
  ~ImR_Activator_ORB_Runner () = default;

  int svc () override;
private:
  ImR_Activator_Loader& service_;
};

#endif
