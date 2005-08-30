// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Based_Pointer_Test.cpp
//
// = DESCRIPTION
//    This test check the Based_Pointer and Based_Pointer_repository classes.
//
// = AUTHOR
//    Steve Williams  <steve@telxio>
//
// ============================================================================

#include "test_config.h"
#include "ace/DLL.h"
#include "ace/ACE.h"
#include "ace/OS.h"
#ifdef ACE_HAS_POSITION_INDEPENDENT_POINTERS
#include "ace/Based_Pointer_Repository.h"
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS */
#include "ace/Malloc_T.h"
#include "ace/PI_Malloc.h"
#include "ace/Null_Mutex.h"

ACE_RCSID(tests, Based_Pointer_Repository_Test, "$Id$")

#ifdef ACE_HAS_POSITION_INDEPENDENT_POINTERS

#if defined (ACE_WIN32) && defined (_MSC_VER) && defined (_DEBUG)
# define OBJ_SUFFIX ACE_TEXT ("d") ACE_DLL_SUFFIX
#elif defined (ACE_WIN32) && defined (__BORLANDC__)
# define OBJ_SUFFIX ACE_LD_DECORATOR_STR ACE_DLL_SUFFIX
#else
# define OBJ_SUFFIX ACE_DLL_SUFFIX
#endif /* ACE_WIN32 && && _MSC_VER && _DEBUG */

#if defined (ACE_WIN32)
#  define OBJ_PREFIX ACE_DLL_PREFIX
#else
#  define OBJ_PREFIX ACE_TEXT("./") ACE_DLL_PREFIX
#endif /* ACE_WIN32 */

// Declare the type of the DLL symbol:
typedef void *(*Get_Bp_Repository_Inst)(void);

// Declare an allocator based MMAP_Memory_Pool
typedef ACE_Malloc_T< ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex, ACE_PI_Control_Block
                   > MMAP_Allocator;


// Check that the ACE_Based_Pointer_Repository can be accessed
// from a Windows DLL
// (see http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=1991)
int singleton_test (void)
{
    void* baddr1 = ACE_BASED_POINTER_REPOSITORY::instance();
    void* baddr2 = ACE_BASED_POINTER_REPOSITORY::instance();

    if (baddr1 != baddr2)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ACE_Based_Pointer_Repository is not a singleton\n")),
                        -1);
    }

// Protection against this test being run on platforms not supporting Dlls.
#if defined(ACE_HAS_DYNAMIC_LINKING)

    ACE_DLL dll;

    // If DLL causes multiple instances of singleton
    // then the ACE_Cleanup object registered
    // with the ACE_Object_manager will no longer be valid,
    // at exit time if the library is unloaded. Override
    // the default close on destruct.
    int retval = dll.open (OBJ_PREFIX
                           ACE_TEXT ("Based_Pointer_Test")
                           OBJ_SUFFIX,
                           ACE_DEFAULT_SHLIB_MODE,
                           0);

    if (retval != 0)
       {
       ACE_TCHAR *dll_error = dll.error ();
       ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Error in DLL Open: %s\n"),
                           dll_error ? dll_error : ACE_TEXT ("unknown error")),
                           -1);
       }

    void* foo = dll.symbol (ACE_TEXT ("get_based_pointer_repository_instance"));

    // Cast the void* to function* with a long as intermediate.
    ptrdiff_t tmp = reinterpret_cast<ptrdiff_t> (foo);
    Get_Bp_Repository_Inst get_bp_repository_inst =
       reinterpret_cast<Get_Bp_Repository_Inst> (tmp);
    if (get_bp_repository_inst == 0)
       ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       dll.error ()),
                       -1);

    void* baddr_dll = get_bp_repository_inst ();

    dll.close ();

    if (baddr_dll != baddr1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ACE_Based_Pointer_Repository is not a singleton in DLL\n")),
                        -1);
    }

#endif /* ACE_HAS_DYNAMIC_LINKING */

    return 0;
}

// Check that MMAP memory blocks are correctly mapped
// into the Based_Pointer_Repository
int
mmap_map_test(void)
{
    MMAP_Allocator* alloc = 0;

    ACE_OS::unlink("foo");
      {
        ACE_NEW_RETURN (alloc, MMAP_Allocator (ACE_TEXT ("foo"), ACE_TEXT ("foo")), -1);

        void* addr = alloc->base_addr();
        if(addr == 0)
         {
           ACE_ERROR((LM_ERROR,
               ACE_TEXT ("Unable to get base to MMAP Memory Pool\n")));
           alloc->remove();
           delete alloc;
            return -1;
         }

       // Check a base address mapping was added to the Repository
       // when the pool was created
       void* ba = 0;
       if(ACE_BASED_POINTER_REPOSITORY::instance()->find(addr, ba) == -1)
        {
           ACE_ERROR((LM_ERROR,
               ACE_TEXT ("Unable to access repository\n")));
           alloc->remove();
           delete alloc;
           return -1;
         }

       alloc->remove();
       delete alloc;

       if(ba != addr)
         {
           ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT (
                           "MMAP pool mapping not present\n")),
                        -1);
         }

       // Check Mapping is removed when object is deleted
       if(ACE_BASED_POINTER_REPOSITORY::instance()->find(addr, ba) == -1)
         {
           ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unable to access repository\n")),
                        -1);
         }
       if(ba != 0)
         {
           ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("MMAP pool mapping not removed\n")),
                        -1);
         }
    }
    return 0;
}

// Check that persistent MMAP memory blocks are correctly remapped
// into the Based_Pointer_Repository
// (i.e. maps based on backing stores that are already
//  present in the filesystem)
// (see http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=2216)
int
mmap_persistent_map_test(void)
{
    MMAP_Allocator* alloc = 0;

    ACE_OS::unlink("foo");
      {
        ACE_NEW_RETURN (alloc, MMAP_Allocator ("foo", "foo"), -1);
        alloc->sync();

        // Delete Malloc and the memory pool, but do not remove
        // the backing store
        alloc->memory_pool().release(0);
        delete alloc;
      }
    //
    // Recreate segment with existing backing store
    //
    ACE_NEW_RETURN (alloc, MMAP_Allocator ("foo", "foo"), -1);

    void* addr = alloc->base_addr();
    if(addr == 0)
      {
        ACE_ERROR ((LM_ERROR,
           ACE_TEXT ("Unable to get base to persistent MMAP Memory Pool\n")));
        alloc->remove();
        delete alloc;
        return -1;
      }
    void* ba = 0;
    if(ACE_BASED_POINTER_REPOSITORY::instance()->find(addr, ba) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Unable to find base address after map of persistent segment\n")));
        alloc->remove();
        delete alloc;
        return -1;
      }
    if(ba == 0)
      {
        ACE_ERROR ((LM_ERROR,
           ACE_TEXT ("Persistent MMAP Memory Pool not mapped\n")));
        alloc->remove();
        delete alloc;
        return -1;
      }

    alloc->remove();
    delete alloc;
    return 0;
}

// Check that MMAP memory blocks are correctly remapped
// into the Based_Pointer_Repository
// (i.e. when a segment is resized it may move its base address
// because the OS cannot fit the new segment size at the same
// base address, in this case the Repository must be updated)
// (see http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=2218)
int
mmap_remap_test(void)
{
    // Use a Position Independent memory segment
    // because this one is going to move
    MMAP_Allocator* alloc = 0;
    // Make sure the Pool options are set to allow
    // the segment to move
    ACE_MMAP_Memory_Pool_Options data_opts(
               0,
               ACE_MMAP_Memory_Pool_Options::NEVER_FIXED);

    ACE_OS::unlink("foo");
    ACE_NEW_RETURN (alloc, MMAP_Allocator ("foo", "foo", &data_opts), -1);

    // cause memory segment to grow until it is forced
    // to be remapped at different base address

    size_t size = 4096;
    void* bar = 0;
    void* oba = alloc->base_addr();
    while(oba == alloc->base_addr())
    {
        bar = alloc->malloc(size);
        size += size;
    }
    void* nba = alloc->base_addr();

    void* ba = 0;
    if(ACE_BASED_POINTER_REPOSITORY::instance()->find(nba, ba) == -1)
      {
        ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("Unable to find base address after remap of segment\n")));
        alloc->remove();
        delete alloc;
        return -1;
      }

    if(ba != nba)
      {
        ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("New base address not mapped after MMAP remap\n")));
        alloc->remove();
        delete alloc;
       return -1;
      }

    // Check old base address has been removed
    // from the repository
    if(ACE_BASED_POINTER_REPOSITORY::instance()->find(oba, ba) == -1)
      {
         ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("Unable to find base address after remap of segment\n")));
         alloc->remove();
         delete alloc;
         return -1;
      }
    if(ba)
      {
        ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("Old base address not removed after MMAP remap\n")));
        alloc->remove();
        delete alloc;
        return -1;
      }
    alloc->remove();
    delete alloc;
    return 0;
}


int
run_main (int, ACE_TCHAR *[])
{
    ACE_START_TEST (ACE_TEXT ("Based_Pointer_Test"));

    int retval = 0;

    retval += singleton_test ();
    retval += mmap_map_test();
    retval += mmap_persistent_map_test();
    retval += mmap_remap_test();

    ACE_END_TEST;
    return retval == 0 ? 0 : 1;
}
#else /* ! ACE_HAS_POSITION_INDEPENDENT_POINTERS */
// Nothing to test !
int
run_main (int, ACE_TCHAR *[])
{
    ACE_START_TEST (ACE_TEXT ("Based_Pointer_Test"));
    ACE_END_TEST;
    return 0;
}
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS */
