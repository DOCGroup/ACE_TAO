// $Id$

// Test the capability of <ACE_Malloc> to handle multiple mallocs
// rooted at different base addresses.

#include "ace/Malloc.h"
#include "ace/Synch.h"

ACE_RCSID(Shared_Malloc, test_multiple_mallocs, "$Id$")

typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> MALLOC; 

// Default address for shared memory mapped files and SYSV shared
// memory (defaults to 64 M).
static void *request_base_addr = ((void *) (64 * 1024 * 1024));
static const char *request_string = "hello from request repository";

// Default address for shared memory mapped files and SYSV shared
// memory (defaults to 64 M).
static void *response_base_addr = ((void *) (128 * 1024 * 1024));
static const char *response_string = "hello from response repository";

int 
main (int, char *[])
{
  ACE_MMAP_Memory_Pool_Options request_options (request_base_addr);

  // Create an adapter version of an allocator.
  ACE_Allocator_Adapter<MALLOC> *shmem_request;

  ACE_NEW_RETURN (shmem_request,
                  ACE_Allocator_Adapter<MALLOC> ("request_file",
                                                 "RequestLock",
                                                 &request_options),
                  1);

  ACE_MMAP_Memory_Pool_Options response_options (response_base_addr);

  // Create a non-adapter version of an allocator.
  MALLOC *shmem_response;
  ACE_NEW_RETURN (shmem_response,
                  MALLOC ("response_file",
                          "ResponseLock",
                          &response_options),
                  1);
  void *data = 0; 

  // If we find "foo" then we're running the "second" time, so we must
  // release the resources.
  if (shmem_request->find ("foo",
                           data) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, 
                  "%s\n",
                  data));
      shmem_request->remove ();
    }

  // This is the first time in, so we allocate the memory and bind it
  // to the name "foo".
  else
    {
      ACE_ALLOCATOR_RETURN (data,
                            shmem_request->malloc (ACE_OS::strlen (request_string) + 1),
                            1);
      ACE_OS::strcpy ((char *) data,
                      request_string);

      if (shmem_request->bind ("foo",
                               data) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "bind"),
                          1);
    }
  data = 0;

  // If we find "foo" then we're running the "second" time, so we must
  // release the resources.
  if (shmem_response->find ("foo",
                            data) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  data));
      shmem_response->remove ();
      ACE_DEBUG ((LM_DEBUG,
                  "all resources have been released\n"));
    }

  // This is the first time in, so we allocate the memory and bind it
  // to the name "foo".
  else
    {
      ACE_ALLOCATOR_RETURN (data,
                            shmem_response->malloc (ACE_OS::strlen (response_string) + 1),
                            1);
      ACE_OS::strcpy ((char *) data,
                      response_string);

      if (shmem_response->bind ("foo",
                                data) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "bind"),
                          1);

      ACE_DEBUG ((LM_DEBUG,
                  "Run again to see results and release resources.\n"));
    }

  return 0;
}
