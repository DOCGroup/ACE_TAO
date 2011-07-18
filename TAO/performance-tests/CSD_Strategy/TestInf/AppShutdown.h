// $Id$
#ifndef APP_SHUTDOWN_H
#define APP_SHUTDOWN_H

#include "CSD_PT_TestInf_Export.h"
#include "OrbShutdownTask.h"
#include "tao/ORB.h"

class CSD_PT_TestInf_Export AppShutdown
{
  public:

    AppShutdown();
    virtual ~AppShutdown();

    void init(CORBA::ORB_ptr orb,
              unsigned num_clients);

    void wait ();

    void client_done();

    static AppShutdown* instance ();

  private:

    typedef TAO_SYNCH_MUTEX LockType;
    typedef ACE_Guard<LockType> GuardType;

    LockType lock_;
    CORBA::ORB_var orb_;
    unsigned num_clients_;
    unsigned num_clients_shutdown_;

    OrbShutdownTask orb_shutdown_task_;
};


#define TheAppShutdown AppShutdown::instance()

#endif
