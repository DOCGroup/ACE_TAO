// $Id$

// Test the capabilities of the ACE shared memory manager in terms of
// its ability to handle multiple mallocs rooted at different base
// addresses.

#include "ace/Malloc.h"
#include "ace/Synch.h"

typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> MALLOC; 

// Default address for shared memory mapped files and SYSV shared
// memory (defaults to 64 M).
void *REQUEST_BASE_ADDR = ((void *) (64 * 1024 * 1024));
const char *REQUEST_STRING = "hello from request repository";

// Default address for shared memory mapped files and SYSV shared
// memory (defaults to 64 M).
void *RESPONSE_BASE_ADDR = ((void *) (128 * 1024 * 1024));
const char *RESPONSE_STRING = "hello from response repository";

int 
main (void)
{
  ACE_MMAP_Memory_Pool_Options request_options (REQUEST_BASE_ADDR);

  // Create an adapter version of an allocator.
  ACE_Allocator_Adapter<MALLOC> *shmem_request =
    new ACE_Allocator_Adapter<MALLOC> ("request_file", "RequestLock", &request_options);

  ACE_MMAP_Memory_Pool_Options response_options (RESPONSE_BASE_ADDR);

  // Create a non-adapter version of an allocator.
  MALLOC *shmem_response = 
    new MALLOC ("response_file","ResponseLock", &response_options);

  void *data = 0; 

  if (shmem_request->find ("foo", data) == 0)
    ACE_OS::printf ("%s\n", data);
  else
    {
      data = shmem_request->malloc (ACE_OS::strlen (REQUEST_STRING) + 1);
      ACE_OS::strcpy ((char *) data, REQUEST_STRING);
      shmem_request->bind ("foo", data);
    }
  data = 0;

  if (shmem_response->find ("foo", data) == 0)
    ACE_OS::printf ("%s\n", data);
  else
    {
      data = shmem_response->malloc (ACE_OS::strlen (RESPONSE_STRING) + 1);
      ACE_OS::strcpy ((char *) data, RESPONSE_STRING);
      shmem_response->bind ("foo", data);
    }

  return 0;
}
