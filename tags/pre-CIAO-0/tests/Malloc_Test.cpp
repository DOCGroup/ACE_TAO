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
#include "ace/Auto_Ptr.h"
#include "ace/Process_Mutex.h"
#include "ace/PI_Malloc.h"
#include "Malloc_Test.h"

ACE_RCSID(tests, Malloc_Test, "Malloc_Test.cpp,v 4.22 1999/12/13 22:24:42 nanbor Exp")

#if !defined (__Lynx__) && (!defined (ACE_LACKS_FORK) || defined (ACE_WIN32))

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
typedef ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block> MALLOC;
#else
typedef ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> MALLOC;
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */
#define MMAP_FILENAME ACE_TEXT ("test_file")
#define MUTEX_NAME ACE_TEXT ("test_lock")

#if !defined (linux) \
    && !(defined (ACE_WIN32) \
         && (defined (ghs) \
             || defined (__MINGW32__) \
             || (!defined(ACE_HAS_WINNT4) || (ACE_HAS_WINNT4 == 0))))
#define ACE_TEST_REMAP_ON_FAULT
// Linux seems to have problem when calling mmap from the signal handler.
// The Green Hills Native x86 compiler does not support structural exceptions.
// Mingw's gcc does not support structural exceptions.
// Win9x doesn't support remaps.
// On these plarforms, we make sure the remapping will never occur.
#endif /* linux && Win32 GHS*/

#if defined (ACE_WIN32)
// When looking for the file to execute a process on Win32, the directory from
// containing the parent process file is searched first. Since certain Win32
// configurations (e.g. Borland C++Builder) put the output files in a different
// directory we will use this feature rather than specifying '.\'.
#  define EXE_LOCATION ACE_TEXT ("")
#else
#  define EXE_LOCATION ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR
#endif /*ACE_WIN32*/

// Parents <ACE_Malloc> base address in shared memory.
static const void *PARENT_BASE_ADDR = ACE_DEFAULT_BASE_ADDR;

// If the platform supports position-independent malloc, choose
// another base address that's 1M higher so that <ACE_Malloc> will be
// mapped into a different address in the child's virtual memory.
// Note that on HP-UX on PA-RISC hardware, a single range of a file
// cannot be mapped into multiple virtual address ranges, even across
// processes.  So, though the whole PI pointer thing is tested here,
// it isn't actually using multiple address ranges.  Also, on Win9x,
// you need to map shared views to the same address.

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 && !defined (HPUX)) \
     && !(defined (ACE_WIN32) \
	  && (!defined (ACE_HAS_WINNT4) || (ACE_HAS_WINNT4 == 0)))
# define CHILD_ADDR_DELTA (1024*1024)
#else
# define CHILD_ADDR_DELTA 0
#endif /* CHILD_ADDR_DELTA */

static const void *CHILD_BASE_ADDR = CHILD_ADDR_DELTA + ACE_DEFAULT_BASE_ADDR;

// Shared memory allocator.  Hide the allocator inside this function
// so that it doesn't get constructed until after the
// <ACE_Object_Manager> gets constructed, even with
// <ACE_HAS_NONSTATIC_OBJECT_MANAGER>.

static MALLOC *
myallocator (const void *base_addr = 0)
{
  static auto_ptr<MALLOC> static_allocator;

  if (static_allocator.get () == 0)
    {

      ACE_MMAP_Memory_Pool_Options options (base_addr);

#if !defined (ACE_TEST_REMAP_ON_FAULT)
      options.minimum_bytes_ = 512 * 1024;
#endif /* ACE_TEST_REMAP_ON_FAULT */

      MALLOC *ptr = new MALLOC (MMAP_FILENAME,
                                MUTEX_NAME,
                                &options);
      ACE_AUTO_PTR_RESET(static_allocator, ptr, MALLOC);
    }
  return static_allocator.get ();
}

static void
init_test (const void *base_addr = 0)
{
  // Cleanup the MMAP file so we won't trip over the leftover mmap
  // file from the previous crash.
  ACE_MMAP_Memory_Pool_Options options (base_addr);
  ACE_MMAP_Memory_Pool mmap (MMAP_FILENAME, &options);

  size_t rbyte = 0;
  int ft = 0;
  mmap.init_acquire (1024, rbyte, ft);
  mmap.release ();
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
                  ACE_TEXT ("<<<< (%P) %s\ni1_ = %d, i2_ = %d, i3_ = %d, d1_ = %f\n"),
                  process_name,
                  t->i1_,
                  t->i2_,
                  t->i3_,
                  t->d1_));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("*t->bpl_ = %d, t->long_test_->array_[0] = ")
                  ACE_TEXT ("%d\n>>>>\n"),
                  *t->long_test_->bpl_,
                  t->long_test_->array_[0]));
    }
}

static int
parent (Test_Data *data)
{
  MALLOC *myalloc = myallocator ();

  {
    ACE_GUARD_RETURN (ACE_Process_Mutex, guard, myalloc->mutex (), -1);
    print ("parent", data);
  }

  // Sleep for a 200 msecs so that the child will have a chance to spin!
  ACE_OS::sleep (ACE_Time_Value (0, 200 * 1000));

#if defined (ACE_TEST_REMAP_ON_FAULT)
  char *small_buf[1024];
  int cntr;

  for (cntr = 0 ; cntr < 1024; ++cntr)
    small_buf[cntr] = (char *) myalloc->malloc (1);
  char *big_buf = (char *) myalloc->malloc (1024 * 4069);
#endif /* ACE_TEST_REMAP_ON_FAULT */

  int result = myalloc->bind ("bar", data);

#if defined (ACE_TEST_REMAP_ON_FAULT)
  myalloc->free (big_buf);
  for (cntr = 0 ; cntr < 1024; ++cntr)
    myalloc->free (small_buf[cntr]);
#endif /* ACE_TEST_REMAP_ON_FAULT */

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
                  ACE_TEXT ("(%P) sleeping for 10 milliseconds!\n")));
      ACE_OS::sleep (timeout);
    }

  print ("child",
         ACE_reinterpret_cast (Test_Data *,
                               bar));
  return 0;
}

#if defined (ACE_WIN32) \
    && (!defined (ACE_HAS_WINNT4) || (ACE_HAS_WINNT4 == 0))
// On Win9x/Me, a shared address needs to be on the shared arena,
// betweeen the second and third megabyte in the virtual address space
// of the process. Also, a mapped view of a file is shared on the same
// virtual address on every 32 bit process.  On WinNT/2k, memory above
// 2Gb is reserved for the system.  So, we need to check at runtime
// (we want an ACE_HAS_WINNT4 == 0 ace to run on either).
static void
get_base_addrs (void)
{
  OSVERSIONINFO vinfo;
  vinfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
  if (::GetVersionEx(&vinfo) == 0)
    return;

  if (vinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
    PARENT_BASE_ADDR = (char*) (64 * 1024*1024);
  else
    PARENT_BASE_ADDR = (char*) ((2048UL + 512UL)*(1024UL*1024UL));

  CHILD_BASE_ADDR = CHILD_ADDR_DELTA + (char*) PARENT_BASE_ADDR;
}
#endif /* defined (ACE_WIN32) && (!defined (ACE_HAS_WINNT4) || (ACE_HAS_WINNT4 == 0)) */

int
ACE_TMAIN (int argc, ACE_TCHAR *[])
{
#if defined (ACE_WIN32) \
    && (!defined (ACE_HAS_WINNT4) || (ACE_HAS_WINNT4 == 0))
  get_base_addrs();
#endif

  if (argc == 1)
    {
      ACE_START_TEST (ACE_TEXT ("Malloc_Test"));
      ACE_INIT_LOG (ACE_TEXT ("Malloc_Test-child"));

      init_test (PARENT_BASE_ADDR);

      ACE_Control_Block::print_alignment_info ();
# if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
      ACE_PI_Control_Block::print_alignment_info ();
# endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

      // No arguments means we're the parent process.
      ACE_Process_Options options (1);

      options.command_line (EXE_LOCATION
                            ACE_TEXT ("Malloc_Test")
                            ACE_PLATFORM_EXE_SUFFIX
                            ACE_TEXT (" run_as_test"));

      MALLOC *myalloc = myallocator (PARENT_BASE_ADDR);

      Test_Data *data = initialize (myalloc);
      ACE_ASSERT (data != 0);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P) PARENT allocator at = %x, ")
                  ACE_TEXT ("data allocated at %x\n"),
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
      ACE_ASSERT (myalloc->ref_counter () == 1);
      myalloc->remove ();
      ACE_END_TEST;
      return 0;
    }
  else
    {
      // In this case we're the child process.
      ACE_APPEND_LOG (ACE_TEXT ("Malloc_Test-child"));

      void *data = 0;
      MALLOC *myalloc = myallocator (CHILD_BASE_ADDR);
      int result = myalloc->find ("foo", data);
      ACE_ASSERT (result != -1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P) CHILD allocator at = %x, ")
                  ACE_TEXT ("data allocated at %x\n"),
                  myalloc,
                  data));
      myalloc->dump ();
      child ();
      myalloc->release ();
      ACE_END_LOG;
      return 0;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
template class auto_ptr< ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block> >;
template class ACE_Auto_Basic_Ptr< ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block> >;
#else
template class auto_ptr< ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> >;
template class ACE_Auto_Basic_Ptr< ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> >;
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */
template class ACE_Based_Pointer<Test_Data>;
template class ACE_Based_Pointer_Basic<Test_Data>;
template class ACE_Based_Pointer_Basic<long>;
template class ACE_Based_Pointer_Basic<Long_Test>;
template class ACE_Based_Pointer<Long_Test>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
#pragma instantiate auto_ptr< ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block> >
#pragma instantiate ACE_Auto_Basic_Ptr< ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block> >
#else
#pragma instantiate auto_ptr< ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> >
#pragma instantiate ACE_Auto_Basic_Ptr< ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> >
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */
#pragma instantiate ACE_Based_Pointer<Test_Data>
#pragma instantiate ACE_Based_Pointer_Basic<Test_Data>
#pragma instantiate ACE_Based_Pointer_Basic<long>
#pragma instantiate ACE_Based_Pointer_Basic<Long_Test>
#pragma instantiate ACE_Based_Pointer<Long_Test>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Malloc_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("process creation is not supported on this ")
              ACE_TEXT ("platform\n")));
  ACE_END_TEST;
  return 0;
}
#endif /* ! __Lynx__  &&  (! ACE_LACKS_FORK || ACE_WIN32) */
