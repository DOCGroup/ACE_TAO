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
#ifdef ACE_HAS_POSITION_INDEPENDENT_POINTERS
#include "ace/Based_Pointer_Repository.h"
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS */
#include "ace/Malloc_T.h"
#include "ace/MMAP_Memory_Pool.h"
#include "ace/PI_Malloc.h"
#include "ace/Null_Mutex.h"
#include "ace/Based_Pointer_T.h"
#include "ace/SString.h"
#include "ace/OS_NS_unistd.h"

class Foo
{
private:
  Foo(const Foo &)
  {
  }
public:
  friend class ace_dewarn_gplusplus;
  ~Foo ()
  {
  }
};

class Void_Pointer : public ACE_Based_Pointer<void>
{
};

#ifdef ACE_HAS_POSITION_INDEPENDENT_POINTERS

#if defined (ACE_LD_DECORATOR_STR)
# define OBJ_SUFFIX ACE_LD_DECORATOR_STR ACE_DLL_SUFFIX
#else
# define OBJ_SUFFIX ACE_DLL_SUFFIX
#endif /* ACE_LD_DECORATOR_STR */

#if defined (ACE_WIN32) || defined (ACE_OPENVMS)
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

    ACE_TString dll_file;
    const char *subdir_env = ACE_OS::getenv ("ACE_EXE_SUB_DIR");
    if (subdir_env)
      {
        dll_file = ACE_TEXT_CHAR_TO_TCHAR (subdir_env);
        dll_file += ACE_DIRECTORY_SEPARATOR_STR;
      }

    dll_file += OBJ_PREFIX ACE_TEXT ("Based_Pointer_Test_Lib") OBJ_SUFFIX;

    // If DLL causes multiple instances of singleton
    // then the ACE_Cleanup object registered
    // with the ACE_Object_manager will no longer be valid,
    // at exit time if the library is unloaded. Override
    // the default close on destruct.
    ACE_DLL dll;
    int retval = dll.open (dll_file.c_str (),
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

#if defined (ACE_OPENVMS)
    // with OPENVMS symbol names > 31 cause us trouble with dlsym()
    void* foo = dll.symbol (ACE_TEXT ("get_based_pointer_repo_inst"));
#else
    void* foo = dll.symbol (ACE_TEXT ("get_based_pointer_repository_instance"));
#endif

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
                           ACE_TEXT ("ACE_Based_Pointer_Repository is not a ")
                           ACE_TEXT ("singleton in DLL <%@> <%@>\n"),
                           baddr_dll,
                           baddr1),
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
        // The 'options' are only here to quiet MSVC 6. It can be removed
        // when MSVC 6 support is removed.
        MMAP_Allocator::MEMORY_POOL_OPTIONS *options = 0;
        ACE_NEW_RETURN
          (alloc,
           MMAP_Allocator (ACE_TEXT ("foo"), ACE_TEXT ("foo"), options),
           -1);

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
           ACE_ERROR((LM_ERROR, ACE_TEXT ("Unable to access repository\n")));
           alloc->remove();
           delete alloc;
           return -1;
         }

       alloc->remove();
       delete alloc;

       if(ba != addr)
         {
           ACE_ERROR_RETURN ((LM_ERROR,
                              ACE_TEXT ("MMAP pool mapping not present\n")),
                             -1);
         }

       // Check Mapping is removed when object is deleted
       if (ACE_BASED_POINTER_REPOSITORY::instance()->find(addr, ba) == -1)
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

    // The 'options' are only here to quiet MSVC 6. It can be removed
    // when MSVC 6 support is removed.
    MMAP_Allocator::MEMORY_POOL_OPTIONS *options = 0;
    ACE_OS::unlink("foo");
      {
        ACE_NEW_RETURN
          (alloc,
           MMAP_Allocator (ACE_TEXT ("foo"), ACE_TEXT ("foo"), options),
           -1);
        alloc->sync();

        // Delete Malloc and the memory pool, but do not remove
        // the backing store
        alloc->memory_pool().release(0);
        delete alloc;
      }
    //
    // Recreate segment with existing backing store
    //
    ACE_NEW_RETURN
      (alloc,
       MMAP_Allocator (ACE_TEXT ("foo"), ACE_TEXT("foo"), options),
       -1);

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

    MMAP_Allocator *alloc[ 3 ]= {0, 0, 0};
    void *pool_base[ 3 ]= {0, 0, 0};

    // Make sure the Pool options are set to allow
    // the segment to move
    ACE_MMAP_Memory_Pool_Options data_opts(
               0,
               ACE_MMAP_Memory_Pool_Options::NEVER_FIXED );
    int i;

    for (i= 0; i<3; ++i)
      {
        ACE_TCHAR store[ MAXPATHLEN + 1 ];
        ACE_OS::sprintf( store, ACE_TEXT("foo%d"), i );
        ACE_OS::unlink( store );

        ACE_NEW_RETURN (alloc[ i ],
                        MMAP_Allocator (store, store, &data_opts),
                        -1);
        pool_base[ i ]= alloc[ i ]->base_addr();
      }

    // sort pools into base address order
    for (i= 0; i<2; ++i)
      {
        if (pool_base[ i ] < pool_base[ i+1 ])
          {
            void *tmp1= pool_base[ i ];
            MMAP_Allocator *tmp2= alloc[ i ];
            pool_base[ i ]= pool_base[ i+1 ];
            alloc[ i ]= alloc[ i+1 ];
            pool_base[ i+1 ]= tmp1;
            alloc[ i+1 ]= tmp2;
            i= -1;
          }
      }

    // alloc[1] is now bounded, whether memory grows up or
    // down, it will hit either alloc[0] or alloc[2] and have
    // to be remapped.
    //
    // Calculate maximum space between base addresses

    size_t size= (char *) pool_base[ 0 ] - (char *) pool_base[ 1 ];
    size_t tmpsize= (char *) pool_base[ 1 ] - (char *) pool_base[ 2 ];
    size= (size < tmpsize) ? tmpsize : size;

    // force pool to move
    ++size;

    (void)alloc[ 1 ]->malloc(size);
    void *nba= alloc[ 1 ]->base_addr();

    if (pool_base[ 1 ] == nba)
      {
        ACE_ERROR ((LM_ERROR,
                   ACE_TEXT ("MMAP Pool did not move base address as expected\n")));
        for (i= 0; i<3; ++i)
          {
            alloc[ i ]->remove();
            delete alloc[ i ];
          }
        return -1;
      }

    void *ba= 0;
    if (ACE_BASED_POINTER_REPOSITORY::instance()->find(nba, ba) == -1)
      {
        ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("Unable to find base address after remap of segment\n")));
        for (i= 0; i<3; ++i)
          {
            alloc[ i ]->remove();
            delete alloc[ i ];
          }
        return -1;
      }

    if (ba != nba)
      {
        ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("New base address not mapped after MMAP remap\n")));
        for (i= 0; i<3; ++i)
          {
            alloc[ i ]->remove();
            delete alloc[ i ];
          }
        return -1;
      }

    // Check old base address has been removed
    // from the repository
    if (ACE_BASED_POINTER_REPOSITORY::instance()->find( pool_base[ 1 ], ba ) == -1)
      {
         ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("Unable to find base address after remap of segment\n")));
         for (i= 0; i<3; ++i)
           {
             alloc[ i ]->remove();
             delete alloc[ i ];
           }
         return -1;
      }

    if (ba == pool_base[ 1 ])
      {
        ACE_ERROR ((LM_ERROR,
            ACE_TEXT ("Old base address not removed after MMAP remap\n")));
        for (i= 0; i<3; ++i)
          {
            alloc[ i ]->remove();
            delete alloc[ i ];
          }
        return -1;
      }

    for (i= 0; i<3; ++i)
      {
        alloc[ i ]->remove();
        delete alloc[ i ];
      }
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

    ACE_Based_Pointer_Basic<Foo> Foo_Ptr;
    ACE_UNUSED_ARG (Foo_Ptr);

    ACE_END_TEST;
    return retval == 0 ? 0 : 1;
}

#else /* ! ACE_HAS_POSITION_INDEPENDENT_POINTERS */
// Nothing to test !
int
run_main (int, ACE_TCHAR *[])
{
    ACE_START_TEST (ACE_TEXT ("Based_Pointer_Test"));
    ACE_Based_Pointer_Basic<Foo> Foo_Ptr;
    ACE_UNUSED_ARG (Foo_Ptr);
    ACE_END_TEST;
    return 0;
}
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS */
