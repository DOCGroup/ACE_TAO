// $Id$

/*
  The client and server both need to know the shared memory key and
  size.  To prevent headaches, I've put those into a header they both
  can share.
 */
#include "shmem.h"

#if defined (ACE_LACKS_SYSV_SHMEM)
int
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "System V Shared Memory not available on this platform\n"),
                    100);
}
#else // ACE_LACKS_SYSV_SHMEM
int 
main (int, char *argv[])
{
  /*
    You can use the ACE_Malloc template to create memory pools
    from various shared memory strategies.  It's really cool.
    We're not going to use it.

    Instead, I want to get to the roots of it all and directly
    use ACE_Shared_Memory_SV.  Like many ACE objects, this is a
    wrapper around OS services.

    With this constructor we create a shared memory area to
    use.  The ACE_CREATE flag will cause it to be created if it
    doesn't already exist.  The SHM_KEY value (from shmem.h)
    uniquely identifies the segment and allows other apps to
    attach to the same segment. Execute 'ipcs -m' before and
    after starting this app to see that the segment is created.
    (I can't for the life of me correlate the SHM_KEY value back 
    to the key/id reported by ipcs though.)
    */
  ACE_Shared_Memory_SV shm_server (SHM_KEY, SHMSZ,
                                   ACE_Shared_Memory_SV::ACE_CREATE);

  /*
    The constructor created the segment for us but we still need 
    to map the segment into our address space.  (Note that you
    can pass a value to malloc() but it will be silently
    igored.)  The void* (cast to char*) that is returned will
    point to the beginning of the shared segment.
    */
  char *shm = (char *) shm_server.malloc ();

  /*
    Since we're asking to create the segment, we will fail if it 
    already exists.  We could fall back and simply attach to it
    like the client but I'd rather not assume it was a previous
    instance of this app that left the segment around.
    */
  if (shm == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n\t(%P|%t) Cannot create shared memory segment.\n"
                       "\tUse 'ipcs' to see if it already exists\n", 
                       argv[0]),
                      100);

  /*
    This second pointer will be used to walk through the block
    of memory...
    */
  char *s = shm;

  /*
    Out of curiosity, I added this output message.  The tests
    I've done so far show me the same address for client and
    server.  What does your OS tell you?
    */
  ACE_DEBUG ((LM_INFO,
              "(%P|%t) Shared Memory is at 0x%x\n",
              shm ));

  /*
    At this point, our application can use the pointer just like 
    any other given to us by new or malloc.  For our purposes,
    we'll copy in the alpabet as a null-terminated string.
    */
  for (char c = 'a'; c <= 'z'; c++)
    *s++ = c;

  *s = '\0';

  /*
    Using a simple not-too-busy loop, we'll wait for the client
    (or anyone else) to change the first byte in the shared area 
    to a '*' character.  This is where you would rather use
    semaphores or some similar "resource light" approach.
    */
  while (*shm != '*')
    ACE_OS::sleep (1);

  /*
    Let's see what the client did to the segment...
    */
  for (s = shm; *s != '\0'; s++)
    putchar (*s);

  putchar ('\n');

  /*
    If you're done with the segment and ready for it to be
    removed from the system, use the remove() method.  Once the
    program exits, do 'ipcs -m' again and you'll see that the
    segment is gone.  If you just want to terminate your use of
    the segment but leave it around for other apps, use the
    close() method instead.

    The free() method may be tempting but it doesn't actually do 
    anything.  If your app is *really* done with the shared
    memory then use either close() or remove().
    */
  if (shm_server.remove () < 0)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "remove"));
  return 0;
}

#endif /* ACE_LACKS_SYSV_SHMEM */
