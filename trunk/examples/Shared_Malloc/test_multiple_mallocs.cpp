#include "ace/Malloc.h"
#include "ace/Synch.h"

typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> MALLOC; 
// Default address for shared memory mapped files and SYSV shared memory
// (defaults to 64 M).
#define ACE_DEFAULT_BASE_ADDR_1 ((char *) (64 * 1024 * 1024)) 

// Default address for shared memory mapped files and SYSV shared memory
// (defaults to 64 M).
#define ACE_DEFAULT_BASE_ADDR_2 ((char *) (128 * 1024 * 1024)) 

int 
main ()
{
  ACE_MMAP_Memory_Pool_Options request_options(ACE_DEFAULT_BASE_ADDR_1);


  MALLOC *shmem_request = new MALLOC (request_options, "request_file", "RequestLock");
  ACE_MMAP_Memory_Pool_Options response_options(ACE_DEFAULT_BASE_ADDR_2);

  MALLOC *shmem_response = new MALLOC (response_options, "response_file","ResponseLock");

  void *data = 0; 
  if (shmem_request->find ("foo", data) == 0)
    cout << (char *) data << endl;
  else
    {
      data = shmem_request->malloc (strlen ("hello from request repository") + 1);
      strcpy ((char*) data, "hello from request repository");
      shmem_request->bind ("foo", data);
    }
  data = 0;
  if (shmem_response->find ("foo", data) == 0)
    cout << (char *) data << endl;
  else
    {
      data = shmem_response->malloc (strlen ("hello from response repository") + 1);
      strcpy ((char*) data, "hello from response repository");
      shmem_response->bind ("foo", data);
    }

  return 0;
}
