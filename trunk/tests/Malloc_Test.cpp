// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Malloc_Test.cpp
//
// = DESCRIPTION
//     This is a test of the position-independent <ACE_Malloc> memory
//     manager using the <ACE_MMAP_Memory_Pool> and <ACE_Process_Mutex>.
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Malloc.h"
#include "ace/Process.h"
#include "Malloc_Test.h"

ACE_RCSID(tests, SV_Shared_Memory_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if !defined (__Lynx__) && (!defined (ACE_LACKS_FORK) || defined (ACE_WIN32))

typedef ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> MALLOC;

// Parents <ACE_Malloc> base address in shared memory.
static const void *PARENT_BASE_ADDR = ACE_DEFAULT_BASE_ADDR;

// If the platform supports position-independent malloc, choose
// another base address that's 64k higher so that <ACE_Malloc> will be
// mapped into a different address in the child's virtual memory.
static const void *CHILD_BASE_ADDR =
#if defined (ACE_HAS_POSITION_INDEPENDENT_MALLOC)
       64 * 1024 +
#endif /* ACE_HAS_POSITION_INDEPENDENT_MALLOC */
       ACE_DEFAULT_BASE_ADDR;

// Shared memory allocator.  Hide the allocator inside this function
// so that it doesn't get constructed until after the
// <ACE_Object_Manager> gets constructed, even with
// <ACE_HAS_NONSTATIC_OBJECT_MANAGER>.

static MALLOC *
myallocator (const void *base_addr = 0)
{
  static ACE_MMAP_Memory_Pool_Options options (base_addr);
  static MALLOC static_allocator (ACE_TEXT ("test_file"),
                                  ACE_TEXT ("test_lock"),
                                  &options);
  return &static_allocator;
}

static Test_Data *
initialize (MALLOC *allocator)
{
  double *temp = 0;
  ACE_ALLOCATOR_RETURN (temp,
                        (double *) allocator->malloc (sizeof (double)),
                        0);
  // Make sure that doubles work!
  *temp = 5.0;
  allocator->free (temp);

  void *ptr;
  ACE_ALLOCATOR_RETURN (ptr,
                        allocator->malloc (sizeof (Test_Data)),
                        0);
  Test_Data *data1 = new (ptr) Test_Data;

  data1->i1_ = 111;
  data1->i2_ = 222;
  data1->i3_ = 333;
  data1->d1_ = 87.5;

  void *gap = 0;
  ACE_ALLOCATOR_RETURN (gap,
                        allocator->malloc (sizeof (256)),
                        0);
  allocator->free (gap);


  ACE_ALLOCATOR_RETURN (ptr,
                        allocator->malloc (sizeof (Test_Data)),
                        0);
  Test_Data *data2 = new (ptr) Test_Data;

  data1->next_ = 0;
  data2->next_ = data1;
  data2->i1_ = -111;
  data2->i2_ = -222;
  data2->i3_ = -333;
  data2->d1_ = 77.34;

  // Test in shared memory using long (array/pointer)
  ACE_ALLOCATOR_RETURN (ptr,
                        allocator->malloc (sizeof (Long_Test)),
                        0);
  Long_Test *lt = new (ptr) Long_Test;

  lt->array_[0] = 1000;
  lt->array_[1] = 1001;
  lt->array_[2] = 1002;
  lt->array_[3] = 1003;
  lt->array_[4] = 1004;
  lt->bpl_ = lt->array_;

  data1->long_test_= lt;

  long long_cont_1 = *lt->bpl_;
  long long_cont_2 = lt->bpl_[3];

  ACE_ASSERT (long_cont_1 == 1000);
  ACE_ASSERT (long_cont_2 == 1003);

  ACE_ALLOCATOR_RETURN (ptr,
                        allocator->malloc (sizeof (Long_Test)),
                        0);
  lt = new (ptr) Long_Test;

  lt->array_[0] = 2000;
  lt->array_[1] = 2001;
  lt->array_[2] = 2002;
  lt->array_[3] = 2003;
  lt->array_[4] = 2004;
  lt->bpl_ = lt->array_;

  data2->long_test_= lt;

  long long_cont_3 = *lt->bpl_;
  long long_cont_4 = lt->bpl_[4];

  ACE_ASSERT (long_cont_3 == 2000);
  ACE_ASSERT (long_cont_4 == 2004);

  return data2;
}

static void
print (const char *process_name,
       Test_Data *data)
{
  for (Test_Data *t = data; t != 0; t = t->next_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("<<<< (%P) %s\ni1_ = %d, i2_ = %d, i3_ = %d, d1_ = %f\n"),
                  process_name,
                  t->i1_,
                  t->i2_,
                  t->i3_,
                  t->d1_));
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("*t->bpl_ = %d, t->long_test_->array_[0] = ")
                  ASYS_TEXT ("%d\n>>>>\n"),
                  *t->long_test_->bpl_,
                  t->long_test_->array_[0]));
    }
}

static int
parent (Test_Data *data)
{
  {
    ACE_GUARD_RETURN (ACE_Process_Mutex, guard, myallocator ()->mutex (), -1);
    print ("parent", data);
  }

  // Sleep for a 200 msecs so that the child will have a chance to spin!
  ACE_OS::sleep (ACE_Time_Value (0, 200 * 1000));
  int result = myallocator ()->bind ("bar", data);
  ACE_ASSERT (result != -1);
  return 0;
}

static int
child (void)
{
  void *bar;
  // Perform "busy waiting" here until the parent stores data under a
  // new name called "bar" in <ACE_Malloc>.  This isn't a good design
  // -- it's just to test that synchronization is working across
  // processes via <ACE_Malloc>.
  for (ACE_Time_Value timeout (0, 1000 * 10);
       myallocator ()->find ("bar",
                             bar) == -1;
       )
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P) sleeping for 10 milliseconds!\n")));
      ACE_OS::sleep (timeout);
    }

  print ("child",
         ACE_reinterpret_cast (Test_Data *,
                               bar));
  return 0;
}

int
main (int argc, ASYS_TCHAR *[])
{
  if (argc == 1)
    {
      ACE_START_TEST (ASYS_TEXT ("Malloc_Test"));
      ACE_INIT_LOG (ASYS_TEXT ("Malloc_Test-child"));

#if 0
      cout << "Sizeof header padding: " << ACE_MALLOC_PADDING << endl
           << "Sizeof header pointer: " << sizeof (ACE_MALLOC_HEADER_PTR) << endl
           << "Sizeof size_t: " << sizeof (size_t) << endl
           << "Sizeof long: " << sizeof (long) << endl
           << "Sizeof (Malloc Header): " << sizeof (ACE_Malloc_Header) << endl
        //           << "Sizeof padding size: " << ACE_MALLOC_PADDING_SIZE << endl
        ;
#endif
      // No arguments means we're the parent process.
      ACE_Process_Options options (1);
      options.command_line (ACE_TEXT (".")
                            ACE_DIRECTORY_SEPARATOR_STR
                            ACE_TEXT ("Malloc_Test")
                            ACE_PLATFORM_EXE_SUFFIX
                            ACE_TEXT (" run_as_test"));

      MALLOC *myalloc = myallocator (PARENT_BASE_ADDR);

      Test_Data *data = initialize (myalloc);
      ACE_ASSERT (data != 0);

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P) PARENT allocator at = %x, ")
                  ASYS_TEXT ("data allocated at %x\n"),
                  myalloc,
                  data));
      myalloc->dump ();
      int result = myalloc->bind ("foo", data);
      ACE_ASSERT (result != -1);

      ACE_Process p;
      pid_t pid = p.spawn (options);
      ACE_ASSERT (pid != -1);

      parent (data);

      // Synchronize on the exit of the child.
      result = p.wait ();
      ACE_ASSERT (result != -1);
      myalloc->remove ();
      ACE_END_TEST;
      return 0;
    }
  else
    {
      // In this case we're the child process.
      ACE_APPEND_LOG (ASYS_TEXT ("Malloc_Test-child"));

      void *data = 0;
      MALLOC *myalloc = myallocator (CHILD_BASE_ADDR);
      int result = myalloc->find ("foo", data);
      ACE_ASSERT (result != -1);

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P) CHILD allocator at = %x, ")
                  ASYS_TEXT ("data allocated at %x\n"),
                  myalloc,
                  data));
      myalloc->dump ();
      child ();
      ACE_END_LOG;
      return 0;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex>;
template class ACE_Write_Guard<ACE_Process_Mutex>;
template class ACE_Read_Guard<ACE_Process_Mutex>;
template class ACE_Based_Pointer<Test_Data>;
template class ACE_Based_Pointer_Basic<Test_Data>;
template class ACE_Based_Pointer_Basic<long>;
template class ACE_Based_Pointer_Basic<Long_Test>;
template class ACE_Based_Pointer<Long_Test>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_Process_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_Process_Mutex>
#pragma instantiate ACE_Based_Pointer<Test_Data>
#pragma instantiate ACE_Based_Pointer_Basic<Test_Data>
#pragma instantiate ACE_Based_Pointer_Basic<long>
#pragma instantiate ACE_Based_Pointer_Basic<Long_Test>
#pragma instantiate ACE_Based_Pointer_Basic<Long>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Malloc_Test"));
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("process creation is not supported on this ")
              ASYS_TEXT ("platform\n")));
  ACE_END_TEST;
  return 0;
}
#endif /* ! __Lynx__  &&  (! ACE_LACKS_FORK || ACE_WIN32) */
