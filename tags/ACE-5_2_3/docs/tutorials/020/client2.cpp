
// $Id$

#include "mmap.h"

#include "ace/Log_Msg.h"

int main (int, char *[])
{
    ACE_Shared_Memory_MM shm_client (SHM_KEY, sizeof(SharedData));

    char *shm = (char *) shm_client.malloc ();

    ACE_DEBUG ((LM_INFO, "(%P|%t) Memory Mapped file is at 0x%x\n",
                shm ));

    SharedData * sd = new(shm) SharedData(0);

    sd->show();
    sd->set();
    sd->available(1);

    shm_client.close();

    return 0;
}
