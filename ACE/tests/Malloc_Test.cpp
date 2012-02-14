
//=============================================================================
/**
 *  @file    Malloc_Test.cpp
 *
 *  $Id$
 *
 *   This is a test of the position-independent <ACE_Malloc> memory
 *   manager using the <ACE_MMAP_Memory_Pool> and <ACE_Process_Mutex>.
 *
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "Malloc_Test.h"

#include "ace/Malloc_T.h"
#include "ace/MMAP_Memory_Pool.h"
#include "ace/Process.h"
#include "ace/Auto_Ptr.h"
#include "ace/Process_Mutex.h"
#include "ace/PI_Malloc.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_unistd.h"



#if defined (ACE_HAS_PROCESS_SPAWN)

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
typedef ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block> MALLOC;
#else
typedef ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> MALLOC;
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */
#define MMAP_FILENAME ACE_TEXT ("test_file")
#define MUTEX_NAME ACE_TEXT ("test_lock")

#if !defined (ACE_LINUX) && !defined (ACE_OPENVMS) \
    && !(defined (ACE_WIN32) \
         && (defined (ghs) || defined (__MINGW32__) )) \
    && !(defined (__OpenBSD__) && defined (ACE_HAS_PTHREADS))
#define ACE_TEST_REMAP_ON_FAULT
// Linux seems to have problem when calling mmap from the signal handler.
// The Green Hills Native x86 compiler does not support structural exceptions.
// Mingw's gcc does not support structural exceptions.
// Win9x doesn't support remaps.
// OpenBSD causes this test to hang in the child when pthreads are enabled.
// On these plarforms, we make sure the remapping will never occur.
#endif /* ACE_LINUX && Win32 GHS*/

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
// it isn't actually using multiple address ranges.

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 && !defined (HPUX))
# define CHILD_ADDR_DELTA (1024*1024)
#else
# define CHILD_ADDR_DELTA 0
#endif /* CHILD_ADDR_DELTA */

static const void *CHILD_BASE_ADDR =
  (const void *)(CHILD_ADDR_DELTA + ACE_DEFAULT_BASE_ADDR);

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

#if defined (ACE_HAS_WINCE) || defined (ACE_OPENVMS)
      // WinCE cannot do fixed base, ever.
      ACE_UNUSED_ARG (base_addr);
      ACE_MMAP_Memory_Pool_Options options
        (0,
         ACE_MMAP_Memory_Pool_Options::NEVER_FIXED);
#else
      ACE_MMAP_Memory_Pool_Options options (base_addr);
#endif /* ACE_HAS_WINCE */

#if !defined (ACE_TEST_REMAP_ON_FAULT)
      options.minimum_bytes_ = 512 * 1024;
#endif /* ACE_TEST_REMAP_ON_FAULT */

      MALLOC *ptr = new MALLOC (MMAP_FILENAME,
                                MUTEX_NAME,
                                &options);
      ACE_auto_ptr_reset (static_allocator, ptr);
    }
  return static_allocator.get ();
}

static void
init_test (const void *base_addr = 0)
{
  // Cleanup the MMAP file so we won't trip over the leftover mmap
  // file from the previous crash.
#if defined (ACE_HAS_WINCE) || defined (ACE_OPENVMS)
  // WinCE cannot do fixed base, ever.
  ACE_UNUSED_ARG (base_addr);
  ACE_MMAP_Memory_Pool_Options options
    (0,
     ACE_MMAP_Memory_Pool_Options::NEVER_FIXED);
#else
  ACE_MMAP_Memory_Pool_Options options (base_addr);
#endif /* ACE_HAS_WINCE */
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_MMAP_Memory_Pool mmap (MMAP_FILENAME, &options);
  //FUZZ: enable check_for_lack_ACE_OS

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

  void *ptr = 0;
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

  ACE_TEST_ASSERT (long_cont_1 == 1000);
  ACE_TEST_ASSERT (long_cont_2 == 1003);

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

  ACE_TEST_ASSERT (long_cont_3 == 2000);
  ACE_TEST_ASSERT (long_cont_4 == 2004);

  return data2;
}

static void
print (const char *process_name,
       Test_Data *data)
{
  for (Test_Data *t = data; t != 0; t = t->next_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("<<<< (%P) %C\ni1_ = %d, i2_ = %d, i3_ = %d, d1_ = %f\n"),
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

  ACE_TEST_ASSERT (result != -1);
  return 0;
}

static int
child (void)
{
  void *bar = 0;
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
         reinterpret_cast<Test_Data *> (bar));
  return 0;
}

#if defined (ACE_WIN32)
// On Win9x/Me, a shared address needs to be on the shared arena,
// betweeen the second and third megabyte in the virtual address space
// of the process. Also, a mapped view of a file is shared on the same
// virtual address on every 32 bit process.  On WinNT/2k, memory above
// 2Gb is reserved for the system.  So, we need to check at runtime
// (we want an ACE_HAS_WINNT4 == 0 ace to run on either).
// To catch any odd case arising from Pharlap and/or WinCE, do the
// run time check and run the NT4-or-better code unless we're on
// CE or something other than NT4 (Pharlap reports itself as NT 3.51).
static void
get_base_addrs (void)
{
  OSVERSIONINFO vinfo;
  vinfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
  if (::GetVersionEx(&vinfo) == 0)
    return;

  if (vinfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
      vinfo.dwMajorVersion >= 4)
    PARENT_BASE_ADDR = (char*) (64 * 1024*1024);
  else
    PARENT_BASE_ADDR = (char*) ((2048UL + 512UL)*(1024UL*1024UL));

  CHILD_BASE_ADDR = CHILD_ADDR_DELTA + (char*) PARENT_BASE_ADDR;
}
#endif /* defined (ACE_WIN32) */

int
run_main (int argc, ACE_TCHAR *argv[])
{
#if defined (ACE_WIN32)
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

#if !defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
      static const ACE_TCHAR* format = ACE_TEXT ("%ls%ls%ls");
#else
      static const ACE_TCHAR* format = ACE_TEXT ("%s%s%s");
#endif /* !ACE_WIN32 && ACE_USES_WCHAR */
      options.command_line (format, EXE_LOCATION,
                            argc > 0 ? argv[0] : ACE_TEXT ("Malloc_Test"),
                            ACE_TEXT (" run_as_test"));

      MALLOC *myalloc = myallocator (PARENT_BASE_ADDR);

      Test_Data *data = initialize (myalloc);
      ACE_TEST_ASSERT (data != 0);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P) PARENT allocator at = %@, ")
                  ACE_TEXT ("data allocated at %@\n"),
                  myalloc,
                  data));
      myalloc->dump ();
      int result = myalloc->bind ("foo", data);
      ACE_TEST_ASSERT (result != -1);

      ACE_Process p;
      pid_t pid = p.spawn (options);
      if (pid == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("spawn")), 1);

      parent (data);

      // Synchronize on the exit of the child.
      result = p.wait ();
      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("wait")), 1);
      ACE_TEST_ASSERT (myalloc->ref_counter () == 1);
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
      ACE_TEST_ASSERT (result != -1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P) CHILD allocator at = %@, ")
                  ACE_TEXT ("data allocated at %@\n"),
                  myalloc,
                  data));
      myalloc->dump ();
      child ();
      myalloc->release ();
      ACE_END_LOG;
      return 0;
    }
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Malloc_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("process creation is not supported on this ")
              ACE_TEXT ("platform\n")));
  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_PROCESS_SPAWN */
