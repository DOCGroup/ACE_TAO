// $Id$

#ifndef SERVICE_H
#define SERVICE_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/CosNamingS.h"
#include "orbsvcs/Naming/Naming_Utils.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Service
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //   This class is a consumer of events.
  //   It simply registers for one event type.
  //
public:
  Service (void);
  // Constructor

  int run (int argc, char* argv[]);
  // Run the test

private:
  CORBA::ORB_ptr orb_;
};

#endif /* CONSUMER_H */
