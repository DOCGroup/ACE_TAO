// $Id$

#include "tao/corba.h"
#include "ORB_Task.h"

int main( int argc, char* argv[] )
{
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    ORB_Task worker (orb.in ());
    worker.activate (THR_NEW_LWP | THR_JOINABLE,
                      10);

    ACE_Time_Value tv (20, 0);
    orb->run(tv);

    orb->destroy ();

    worker.thr_mgr ()->wait ();

    return 0;
}
