
// $Id$

#include "mmap.h"

int main (int, char *[])
{
    ACE_Shared_Memory_MM shm_client (SHM_KEY, SHMSZ);
    char *shm = (char *) shm_client.malloc ();

    ACE_DEBUG ((LM_INFO, "(%P|%t) Memory Mapped file is at 0x%x\n",
                shm ));
    
    if( ! shm )
    {
        ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) Could not get the mmapped file!\n"),100);
    }

    for (char *s = shm; *s != '\0'; s++)
    {
        putchar (*s);
        *s = toupper(*s);
    }
    
    putchar ('\n');
    *shm = '*';

    shm_client.close();

    return 0;
}
