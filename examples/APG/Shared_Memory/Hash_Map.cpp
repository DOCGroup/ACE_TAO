// $Id$

#include "ace/OS_NS_stdio.h"

// Listing 1 code/ch17
#include "ace/Hash_Map_With_Allocator_T.h"
#include "ace/Malloc_T.h"
#include "ace/PI_Malloc.h"
#include "ace/Process_Mutex.h"
#include "ace/Process.h"

#define BACKING_STORE "map.store"
#define MAP_NAME "records.db"

#include "Record.h"

typedef ACE_Allocator_Adapter<ACE_Malloc_T <ACE_MMAP_MEMORY_POOL,
                                            ACE_Process_Mutex,
                                            ACE_Control_Block>
                             > ALLOCATOR;
typedef ACE_Hash_Map_With_Allocator<int, Record> MAP;

ACE_Process_Mutex coordMutex("Coord-Mutex");
// Listing 1

// Listing 2 code/ch17
MAP* smap (ALLOCATOR *shmem_allocator)
{
  void *db = 0;
  if (shmem_allocator->find (MAP_NAME, db) == 0)
    return (MAP *) db;
  size_t hash_table_size = sizeof (MAP);
  void *hash_map = shmem_allocator->malloc (hash_table_size);
  if (hash_map == 0)
    return 0;
  new (hash_map) MAP (hash_table_size, shmem_allocator);
  if (shmem_allocator->bind (MAP_NAME, hash_map) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                  ACE_TEXT ("allocate_map")));
      shmem_allocator->remove ();
      return 0;
    }
  return (MAP*)hash_map;
}
// Listing 2
// Listing 6 code/ch17
int processRecords (MAP *map, ALLOCATOR *shmem_allocator)
{
  ACE_TRACE ("processRecords");

  size_t mapLength = map->current_size ();
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Found %d records\n\n"),
              mapLength));

  int *todelete = new int[mapLength];
  int i = 0;

  for (MAP::iterator iter = map->begin ();
      iter != map->end ();
      iter++)
    {
      int key = (*iter).ext_id_;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) [%d] Preprocessing %d:%@\n"),
                  i+1, key, &(*iter).ext_id_));

      todelete[i++] = key;    // Mark message for deletion.

      // Illustrate the find feature of the map.
      Record record;
      int result = map->find (key, record, shmem_allocator);
      if (result == -1)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("Could not find record for %d\n"),
                    key));
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Record name: %C|id1:%d|id2:%d\n"),
                    record.name (), record.id1(), record.id2()));
    }

  // Delete everything we processed.
  for (int j = 0; j < i ; j++)
    {
      int result = map->unbind (todelete[j],
                                shmem_allocator);
      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed on key %d: %p\n"),
                           ACE_TEXT ("unbind"),
                           todelete[j]),
                          -1);
      else
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("Fully processed and removed %d\n"),
                    j));
    }

  delete [] todelete;

  return 0;
}
// Listing 6
// Listing 4 code/ch17
int addRecords(MAP *map, ALLOCATOR *shmem_allocator)
{
  ACE_TRACE ("addRecords");

  char buf[32];
  int mapLength = ACE_static_cast (int, map->current_size ());
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Map has %d entries; adding 20 more\n"),
              mapLength));

  for (int i = mapLength ; i < mapLength + 20; i++)
    {
      ACE_OS::sprintf (buf, "%s:%d", "Record", i);

      // Allocate new record on stack;
      Record newRecord (i, i+1, buf);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Adding a record for %d\n"), i));

      int result = map->bind (i, newRecord, shmem_allocator);
      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("bind failed")), -1);
    }

  return 0;
}
// Listing 4
// Listing 5 code/ch17
int handle_child (void)
{
  ACE_TRACE ("handle_child");

  ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon, coordMutex, -1);

  ALLOCATOR * shmem_allocator = 0;
  ACE_MMAP_Memory_Pool_Options options
    (ACE_DEFAULT_BASE_ADDR,
     ACE_MMAP_Memory_Pool_Options::ALWAYS_FIXED);

  ACE_NEW_RETURN (shmem_allocator,
                  ALLOCATOR (BACKING_STORE,
                             BACKING_STORE,
                             &options),
                  -1);

  MAP *map = smap (shmem_allocator);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Map has %d entries\n"),
              map->current_size ()));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("In child, map is located at %@\n"),
              map));

  processRecords (map, shmem_allocator);
  shmem_allocator->sync ();
  delete shmem_allocator;

  return 0;
}
// Listing 5
// Listing 3 code/ch17
int handle_parent (ACE_TCHAR *cmdLine)
{
  ACE_TRACE ("handle_parent");

  ALLOCATOR * shmem_allocator = 0;
  ACE_MMAP_Memory_Pool_Options options
    (ACE_DEFAULT_BASE_ADDR,
     ACE_MMAP_Memory_Pool_Options::ALWAYS_FIXED);

  ACE_NEW_RETURN
    (shmem_allocator,
     ALLOCATOR (BACKING_STORE, BACKING_STORE, &options),
     -1);

  MAP *map = smap (shmem_allocator);

  ACE_Process processa, processb;
  ACE_Process_Options poptions;
  const ACE_TCHAR *args[3];
  args[0] = cmdLine;
  args[1] = ACE_TEXT ("a");
  args[2] = 0;
  poptions.command_line (args);
  {
    ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon,
                      coordMutex, -1);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) Map has %d entries\n"),
                map->current_size ()));
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("In parent, map is located at %@\n"),
                map));

    // Then have the child show and eat them up.
    processa.spawn (poptions);

    // First append a few records.
    addRecords (map, shmem_allocator);
  }


  {
    ACE_GUARD_RETURN (ACE_Process_Mutex, ace_mon,
                      coordMutex, -1);

    // Add a few more records..
    addRecords (map, shmem_allocator);

    // Let's see what's left.
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) Parent finished adding, ")
                ACE_TEXT ("map has %d entries\n"),
                map->current_size ()));

    // Have another child try to eat them up.
    processb.spawn (poptions);
  }

  processa.wait ();
  processb.wait ();

  // No processes are left and we don't want to keep the data
  // around anymore; it's now safe to remove it.
  // !!This will remove the backing store.!!
  shmem_allocator->remove ();
  delete shmem_allocator;
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc == 1) // parent
    ACE_ASSERT (handle_parent (argv[0]) == 0);
  else
    ACE_ASSERT (handle_child () == 0);

  ACE_UNUSED_ARG (argv);
  return 0;
}
// Listing 3
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Allocator_Adapter<ACE_Malloc_T<ACE_MMAP_MEMORY_POOL,ACE_Process_Mutex,ACE_Control_Block> >;
template class ACE_Hash_Map_Entry<int,Record>;
template class ACE_Hash_Map_Manager_Ex<int,Record,ACE_Hash<int>,ACE_Equal_To<int>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<int,Record,ACE_Hash<int>,ACE_Equal_To<int>,ACE_Null_Mutex>;
template class ACE_Hash_Map_With_Allocator<int,Record>;
// The following instantiation is in ace/System_Time.cpp:
// template class ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Malloc_T<ACE_MMAP_MEMORY_POOL,ACE_Process_Mutex,ACE_Control_Block>;
template class ACE_Malloc_FIFO_Iterator <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Malloc_LIFO_Iterator <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Malloc_FIFO_Iterator_T <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex, ACE_Control_Block>;
template class ACE_Malloc_LIFO_Iterator_T <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex, ACE_Control_Block>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Allocator_Adapter<ACE_Malloc_T<ACE_MMAP_MEMORY_POOL,ACE_Process_Mutex,ACE_Control_Block> >
#pragma instantiate ACE_Hash_Map_Entry<int,Record>
#pragma instantiate ACE_Hash_Map_Manager_Ex<int,Record,ACE_Hash<int>,ACE_Equal_To<int>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<int,Record,ACE_Hash<int>,ACE_Equal_To<int>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_With_Allocator<int,Record>
#pragma instantiate ACE_Malloc_T<ACE_MMAP_MEMORY_POOL,ACE_Process_Mutex,ACE_Control_Block>
// The following instantiation is in ace/System_Time.cpp:
// #pragma instantiate ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>
#pragma instantiate ACE_Malloc_FIFO_Iterator <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>
#pragma instantiate ACE_Malloc_LIFO_Iterator <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>
#pragma instantiate ACE_Malloc_FIFO_Iterator_T <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex, ACE_Control_Block>
#pragma instantiate ACE_Malloc_LIFO_Iterator_T <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex, ACE_Control_Block>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
