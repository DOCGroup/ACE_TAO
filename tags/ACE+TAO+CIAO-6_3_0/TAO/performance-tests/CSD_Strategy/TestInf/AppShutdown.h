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
    TAO_SYNCH_MUTEX lock_;
    CORBA::ORB_var orb_;
    unsigned num_clients_;
    unsigned num_clients_shutdown_;

    OrbShutdownTask orb_shutdown_task_;
};


#define TheAppShutdown AppShutdown::instance()

#endif
