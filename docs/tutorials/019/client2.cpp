
// $Id$

#include "shmem.h"

int main (int, char *[])
{
    ACE_Shared_Memory_SV shm_client (SHM_KEY, sizeof(SharedData));
    
    char *shm = (char *) shm_client.malloc ();

    ACE_DEBUG ((LM_INFO, "(%P|%t) Shared Memory is at 0x%x\n",
                shm ));

        /*
          More placement new.  The constructor parameter prevents
          clobbering what the server may have written with it's show() 
          method.
        */
    SharedData * sd = new(shm) SharedData(0);

        // Show it
    sd->show();
        // Change it
    sd->set();
        // Advertise it
    sd->available(1);

    shm_client.close();
        
    return 0;
}

