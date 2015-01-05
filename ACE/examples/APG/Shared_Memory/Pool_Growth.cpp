// $Id$

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Malloc_T.h"
#include "ace/PI_Malloc.h"
#include "ace/Process_Mutex.h"
#include "ace/Process.h"
#include "ace/Unbounded_Queue.h"
#include "ace/MMAP_Memory_Pool.h"

#define BACKING_STORE "queue.dat"
#define QUEUE_NAME "queue.db"

typedef ACE_Allocator_Adapter<ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> > ALLOCATOR;

ACE_Process_Mutex coordMutex("Coord-Mutex");

// Listing 1 code/ch17
template <class T>
class Unbounded_Queue : public ACE_Unbounded_Queue<T>
{
public:
  typedef ACE_Unbounded_Queue<T> BASE;

  Unbounded_Queue(ACE_Allocator* allocator)
    : ACE_Unbounded_Queue<T> (allocator)
  { }

  int enqueue_tail (const T &new_item, ACE_Allocator* allocator)
  {
    this->allocator_ = allocator;
    return BASE::enqueue_tail (new_item);
  }

  int dequeue_head (T &item, ACE_Allocator* allocator)
  {
    this->allocator_ = allocator;
    return BASE::dequeue_head (item);
  }

  void delete_nodes (ACE_Allocator* allocator)
  {
    this->allocator_ = allocator;
    delete_nodes ();
  }
};
// Listing 1

#include "Record.h"

typedef Unbounded_Queue<Record> QUEUE;

QUEUE* squeue(ALLOCATOR* shmem_allocator)
{
  void *queue = 0;

  // This is the easy case since if we find hash table in the
  // memory-mapped file we know it's already initialized.
  if (shmem_allocator->find (QUEUE_NAME, queue) == 0)
    return (QUEUE *) queue;

  // Create a new map (because we've just created a new
  // memory-mapped file).
  size_t queue_size = sizeof (QUEUE);

  queue = shmem_allocator->malloc (queue_size);

  // If allocation failed ...
  if (queue == 0)
    return 0;

  new (queue) QUEUE (shmem_allocator);

  if (shmem_allocator->bind (QUEUE_NAME, queue) == -1)
    {
      // Attempt to clean up.
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("squeue bind\n")));
      shmem_allocator->remove();

      return 0;
    }

  return (QUEUE*)queue;
}

static ALLOCATOR * g_shmem_allocator = 0;

// Listing 4 code/ch17
int processRecord (ALLOCATOR *shmem_allocator)
{
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, coordMutex, -1);

  QUEUE* queue = squeue (shmem_allocator);
  if (queue == 0)
    {
      delete shmem_allocator;
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                         ACE_TEXT ("Could not obtain queue")),
                        -1);
    }

  if (queue->is_empty ())  // Check for anything to process.
    return 0;

  Record record;
  if (queue->dequeue_head (record, shmem_allocator) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                         ACE_TEXT ("dequeue_head\n")),
                        -1);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Processing record|name: %C")
              ACE_TEXT ("|Record id1:%d|Record id2:%d\n"),
              record.name (), record.id1 (), record.id2 ()));
  if (record.id1 () == -1)
    queue->enqueue_tail (record, shmem_allocator);
  return record.id1 ();
}
// Listing 4
// Listing 5 code/ch17
#if defined(WIN32)

int handle_remap (EXCEPTION_POINTERS *ep)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Handle a remap\n")));

  DWORD ecode = ep->ExceptionRecord->ExceptionCode;
  if (ecode != EXCEPTION_ACCESS_VIOLATION)
    return EXCEPTION_CONTINUE_SEARCH;

  void *addr =
    (void *) ep->ExceptionRecord->ExceptionInformation[1];
  if (g_shmem_allocator->alloc().memory_pool().remap (addr) == -1)
    return EXCEPTION_CONTINUE_SEARCH;
#if __X86__
  // This is 80x86-specific.
  ep->ContextRecord->Edi = (DWORD) addr;
#elif __MIPS__
  ep->ContextRecord->IntA0 =
    ep->ContextRecord->IntV0 = (DWORD) addr;
  ep->ContextRecord->IntT5 =
    ep->ContextRecord->IntA0 + 3;
#endif /* __X86__ */

  return EXCEPTION_CONTINUE_EXECUTION;
}

int processWin32Record (ALLOCATOR *shmem_allocator)
{
  ACE_SEH_TRY
  {
    return processRecord (shmem_allocator);
  }

  ACE_SEH_EXCEPT (handle_remap (GetExceptionInformation ()))
  { }

  return 0;
}
#endif /*WIN32*/
// Listing 5

int sendRecord (int recordId,  ALLOCATOR *shmem_allocator)
{
  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, coordMutex, -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Sending record %d\n"),
              recordId));

  QUEUE * queue = squeue (shmem_allocator);
  char buf[128];
  ACE_OS::sprintf (buf, "%s:%d", "Record", recordId);
  Record newRecord (recordId, recordId+1, buf);

  int result = queue->enqueue_tail (newRecord, shmem_allocator);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("enqueue failed\n")),
                      -1);
  return 0;
}

// Listing 2 code/ch17
int handle_parent (ACE_TCHAR *cmdLine)
{
  ALLOCATOR *shmem_allocator = 0;
  ACE_MMAP_Memory_Pool_Options options
    (ACE_DEFAULT_BASE_ADDR,
     ACE_MMAP_Memory_Pool_Options::ALWAYS_FIXED);

  // Create the allocator.
  ACE_NEW_RETURN (shmem_allocator,
                  ALLOCATOR (BACKING_STORE,
                             BACKING_STORE,
                             &options),
                  -1);

  ACE_Process processa, processb;
  ACE_Process_Options poptions;
  const ACE_TCHAR *args[3];
  args[0] = cmdLine;
  args[1] = ACE_TEXT ("a");
  args[2] = 0;
  poptions.command_line (args);
  processa.spawn (poptions);
  processb.spawn (poptions);

  // Make sure the child does map a partial pool in memory.
  ACE_OS::sleep (2);

  for (int i = 0; i < 100; i++)
    sendRecord (i, shmem_allocator);
  sendRecord (-1, shmem_allocator);

  processa.wait ();
  processb.wait ();
  shmem_allocator->remove ();
  return 0;
}
// Listing 2

// Listing 3 code/ch17
int handle_child (void)
{
  ALLOCATOR *shmem_allocator = 0;
  ACE_MMAP_Memory_Pool_Options options
    (ACE_DEFAULT_BASE_ADDR,
     ACE_MMAP_Memory_Pool_Options::ALWAYS_FIXED);
  ACE_NEW_RETURN (shmem_allocator,
                  ALLOCATOR (BACKING_STORE,
                             BACKING_STORE,
                             &options),
                  -1);
  g_shmem_allocator = shmem_allocator;

#if defined (WIN32)
  while (processWin32Record (shmem_allocator) != -1)
    ;
#else
  while (processRecord (shmem_allocator) != -1)
    ;
#endif
  return 0;
}
// Listing 3

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc == 1)
    handle_parent(argv[0]);
  else
    handle_child();

  return 0;
}

