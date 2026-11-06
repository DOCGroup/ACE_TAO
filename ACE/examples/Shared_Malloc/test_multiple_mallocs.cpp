// Test the capability of <ACE_Malloc> to handle multiple mallocs
// rooted at different base addresses.

#include "ace/OS_NS_string.h"
#include "ace/Malloc_T.h"
#include "ace/MMAP_Memory_Pool.h"
#include "ace/Process_Mutex.h"
#include <cstring>
#include <memory>

using TEST_MALLOC = ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex>;

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
// The Address for the shared memory mapped files defaults to wherever
// the OS wants to map it.
static void const *const REQUEST_BASE_ADDR {};
static void const *const RESPONSE_BASE_ADDR {};
#else
// Default address for shared memory mapped files and SYSV shared
// memory (defaults to 64 M).
static void const *const REQUEST_BASE_ADDR =
  reinterpret_cast<void const *> (static_cast<intptr_t> (64 * 1024 * 1024));

// Default address for shared memory mapped files and SYSV shared
// memory (defaults to 64 M).
static void const *const RESPONSE_BASE_ADDR =
  reinterpret_cast<void const *> (static_cast<intptr_t> (128 * 1024 * 1024));
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

static char const request_string[] = "hello from request repository";
static char const response_string[] = "hello from response repository";

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_MMAP_Memory_Pool_Options const request_options (REQUEST_BASE_ADDR);

  // Create an adapter version of an allocator.
  ACE_Allocator_Adapter<TEST_MALLOC> *raw_adapter {};
  ACE_NEW_RETURN (raw_adapter,
                  ACE_Allocator_Adapter<TEST_MALLOC> ("request_file",
                                                      "request_lock",
                                                      &request_options),
                  1);

  std::unique_ptr<ACE_Allocator_Adapter<TEST_MALLOC>> shmem_request (raw_adapter);
  ACE_MMAP_Memory_Pool_Options const response_options (RESPONSE_BASE_ADDR);

  TEST_MALLOC *raw_allocator {};
  // Create a non-adapter version of an allocator.
  ACE_NEW_RETURN (raw_allocator,
                  TEST_MALLOC (ACE_TEXT("response_file"),
                               ACE_TEXT("response_lock"),
                               &response_options),
                  1);
  std::unique_ptr<TEST_MALLOC> shmem_response (raw_allocator);
  // These raw pointers refer to blocks managed by ACE_Malloc and are stored
  // in its shared name database by bind ().
  void *data {};

  // If we find "foo" then we're running the "second" time, so we must
  // release the resources.
  if (shmem_request->find ("foo",
                           data) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  static_cast<char const *> (data)));
      shmem_request->remove ();
    }

  // This is the first time in, so we allocate the memory and bind it
  // to the name "foo".
  else
    {
      ACE_ALLOCATOR_RETURN (data,
                            shmem_request->malloc (std::strlen (request_string) + 1),
                            1);
      std::strcpy (static_cast<char *> (data), request_string);

      if (shmem_request->bind ("foo",
                               data) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "bind"),
                          1);
    }
  data = nullptr;

  // If we find "foo" then we're running the "second" time, so we must
  // release the resources.
  if (shmem_response->find ("foo",
                            data) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  static_cast<char const *> (data)));
      shmem_response->remove ();
      ACE_DEBUG ((LM_DEBUG,
                  "all shared memory resources have been released\n"));
    }

  // This is the first time in, so we allocate the memory and bind it
  // to the name "foo".
  else
    {
      ACE_ALLOCATOR_RETURN (data,
                            shmem_response->malloc (std::strlen (response_string) + 1),
                            1);
      std::strcpy (static_cast<char *> (data), response_string);

      if (shmem_response->bind ("foo",
                                data) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "bind"));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Run again to see results and release resources.\n"));
    }

  return 0;
}
