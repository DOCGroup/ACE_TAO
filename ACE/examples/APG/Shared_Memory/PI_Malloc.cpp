// $Id$

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

// Listing 1 code/ch17
#include "ace/MMAP_Memory_Pool.h"
#include "ace/Malloc_T.h"
#include "ace/Null_Mutex.h"
#include "ace/PI_Malloc.h"

typedef ACE_Malloc_T <ACE_MMAP_MEMORY_POOL,
                      ACE_Null_Mutex,
                      ACE_PI_Control_Block>
  ALLOCATOR;
typedef ACE_Malloc_LIFO_Iterator_T<ACE_MMAP_MEMORY_POOL,
                                   ACE_Null_Mutex,
                                   ACE_PI_Control_Block>
  MALLOC_LIFO_ITERATOR;

ALLOCATOR  *g_allocator;
// Listing 1
// Listing 2 code/ch17
class Record
{
public:
  Record (int id1, int id2, char *name)
    : id1_(id1), id2_(id2)
  {
    size_t len = ACE_OS::strlen (name) + 1;
    char *buf =
      reinterpret_cast<char *> (g_allocator->malloc (len));
    ACE_OS::strcpy (buf, name);
    name_ = buf;
  }

  ~Record() { g_allocator->free (name_.addr ()); }

  char *name (void) { return name_; }
  int id1 (void) { return id1_; }
  int id2 (void) { return id2_; }

private:
  int id1_;
  int id2_;
  ACE_Based_Pointer_Basic<char> name_;
};
// Listing 2

void showRecords (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("The following records were found:\n")));

  {
    MALLOC_LIFO_ITERATOR iter (*g_allocator);

    for (void *temp = 0; iter.next (temp) != 0; iter.advance ())
      {
        Record *record =
          reinterpret_cast<Record *> (temp);
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Record name: %C|id1:%d|id2:%d\n"),
                    record->name(), record->id1(), record->id2()));
      }
  }
}

int addRecords (void)
{
  char buf[32];

  for (int i = 0; i < 10; i++)
    {
      ACE_OS::sprintf (buf, "%s:%d", "Record", i);

      void *memory = g_allocator->malloc (sizeof (Record));
      if (memory == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("Unable to malloc")),
                          -1);

      // Allocate and place record
      Record* newRecord = new (memory) Record (i, i+1, buf);
      if (g_allocator->bind (buf, newRecord) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("bind failed")),
                          -1);
    }

  return 0;
}

// Listing 3 code/ch17
// Backing file where the data is kept.
#define BACKING_STORE ACE_TEXT("backing2.store")

int ACE_TMAIN (int argc, ACE_TCHAR *[])
{
  if (argc > 1)
    {
      ACE_MMAP_Memory_Pool_Options options
        (ACE_DEFAULT_BASE_ADDR,
         ACE_MMAP_Memory_Pool_Options::ALWAYS_FIXED);
      ACE_NEW_RETURN (g_allocator,
                      ALLOCATOR (BACKING_STORE,
                                 BACKING_STORE,
                                 &options),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Mapped to base address %@\n"),
                  g_allocator->base_addr ()));

      showRecords ();
    }
  else
    {
      ACE_MMAP_Memory_Pool_Options options
        (0, ACE_MMAP_Memory_Pool_Options::NEVER_FIXED);
      ACE_NEW_RETURN (g_allocator,
                      ALLOCATOR (BACKING_STORE,
                                 BACKING_STORE,
                                 &options),
                      -1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Mapped to base address %@\n"),
                  g_allocator->base_addr ()));

      addRecords();
    }

  g_allocator->sync ();
  delete g_allocator;
  return 0;
}
// Listing 3

