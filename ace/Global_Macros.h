// -*- C++ -*-

//=============================================================================
/**
 *  @file   Global_Macros.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *  @This one is split from the famous OS.h
 */
//=============================================================================

#ifndef ACE_GLOBAL_MACROS_H
#define ACE_GLOBAL_MACROS_H

#include "ace/pre.h"

// Included just keep compilers that see #pragma dierctive first
// happy.
#include "ace/OS_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Moved from OS.h dah -- perhaps this should go in an msvc config file
// Deal with MSVC++ insanity for CORBA...
# if defined (ACE_HAS_BROKEN_NAMESPACES)
#   define ACE_CORBA_1(NAME) CORBA_##NAME
#   define ACE_CORBA_2(TYPE, NAME) CORBA_##TYPE##_##NAME
#   define ACE_CORBA_3(TYPE, NAME) CORBA_##TYPE::NAME
#   define ACE_NESTED_CLASS(TYPE, NAME) NAME
# else  /* ! ACE_HAS_BROKEN_NAMESPACES */
#   define ACE_CORBA_1(NAME) CORBA::NAME
#   define ACE_CORBA_2(TYPE, NAME) CORBA::TYPE::NAME
#   define ACE_CORBA_3(TYPE, NAME) CORBA::TYPE::NAME
#   define ACE_NESTED_CLASS(TYPE, NAME) TYPE::NAME
# endif /* ! ACE_HAS_BROKEN_NAMESPACES */



// Start Global Macros
# define ACE_BEGIN_DUMP ACE_LIB_TEXT ("\n====\n(%P|%t|%x)")
# define ACE_END_DUMP ACE_LIB_TEXT ("====\n")



# if defined (ACE_NDEBUG)
#   define ACE_DB(X)
# else
#   define ACE_DB(X) X
# endif /* ACE_NDEBUG */

// ACE_NO_HEAP_CHECK macro can be used to suppress false report of
// memory leaks. It turns off the built-in heap checking until the
// block is left. The old state will then be restored Only used for
// Win32 (in the moment).
# if defined (ACE_WIN32)

#   if defined (_DEBUG) && !defined (ACE_HAS_WINCE) && !defined (__BORLANDC__)
# include /**/ <crtdbg.h>

class ACE_OS_Export ACE_No_Heap_Check
{
public:
  ACE_No_Heap_Check (void)
    : old_state (_CrtSetDbgFlag (_CRTDBG_REPORT_FLAG))
  { _CrtSetDbgFlag (old_state & ~_CRTDBG_ALLOC_MEM_DF);}
  ~ACE_No_Heap_Check (void) { _CrtSetDbgFlag (old_state);}
private:
  int old_state;
};
#     define ACE_NO_HEAP_CHECK ACE_No_Heap_Check ____no_heap;
#   else /* !_DEBUG */
#     define ACE_NO_HEAP_CHECK
#   endif /* _DEBUG */
# else /* !ACE_WIN32 */
#   define ACE_NO_HEAP_CHECK
# endif /* ACE_WIN32 */

// Turn a number into a string.
# define ACE_ITOA(X) #X

// Create a string of a server address with a "host:port" format.
# define ACE_SERVER_ADDRESS(H,P) H":"P

// A couple useful inline functions for checking whether bits are
// enabled or disabled.

// Efficiently returns the least power of two >= X...
# define ACE_POW(X) (((X) == 0)?1:(X-=1,X|=X>>1,X|=X>>2,X|=X>>4,X|=X>>8,X|=X>>16,(++X)))
# define ACE_EVEN(NUM) (((NUM) & 1) == 0)
# define ACE_ODD(NUM) (((NUM) & 1) == 1)
# define ACE_BIT_ENABLED(WORD, BIT) (((WORD) & (BIT)) != 0)
# define ACE_BIT_DISABLED(WORD, BIT) (((WORD) & (BIT)) == 0)
# define ACE_BIT_CMP_MASK(WORD, BIT, MASK) (((WORD) & (BIT)) == MASK)
# define ACE_SET_BITS(WORD, BITS) (WORD |= (BITS))
# define ACE_CLR_BITS(WORD, BITS) (WORD &= ~(BITS))

# if !defined (ACE_ENDLESS_LOOP)
#  define ACE_ENDLESS_LOOP
# endif /* ! ACE_ENDLESS_LOOP */

# if defined (ACE_NEEDS_FUNC_DEFINITIONS)
    // It just evaporated ;-)  Not pleasant.
#   define ACE_UNIMPLEMENTED_FUNC(f)
# else
#   define ACE_UNIMPLEMENTED_FUNC(f) f;
# endif /* ACE_NEEDS_FUNC_DEFINITIONS */

// Easy way to designate that a class is used as a pseudo-namespace.
// Insures that g++ "friendship" anamolies are properly handled.
# define ACE_CLASS_IS_NAMESPACE(CLASSNAME) \
private: \
CLASSNAME (void); \
CLASSNAME (const CLASSNAME&); \
friend class ace_dewarn_gplusplus

// ----------------------------------------------------------------
// moved trace macros to Trace.h
// ----------------------------------------------------------------

// Convenient macro for testing for deadlock, as well as for detecting
// when mutexes fail.
#define ACE_GUARD_ACTION(MUTEX, OBJ, LOCK, ACTION, REACTION) \
   ACE_Guard< MUTEX > OBJ (LOCK); \
   if (OBJ.locked () != 0) { ACTION; } \
   else { REACTION; }
#define ACE_GUARD_REACTION(MUTEX, OBJ, LOCK, REACTION) \
  ACE_GUARD_ACTION(MUTEX, OBJ, LOCK, ;, REACTION)
#define ACE_GUARD(MUTEX, OBJ, LOCK) \
  ACE_GUARD_REACTION(MUTEX, OBJ, LOCK, return)
#define ACE_GUARD_RETURN(MUTEX, OBJ, LOCK, RETURN) \
  ACE_GUARD_REACTION(MUTEX, OBJ, LOCK, return RETURN)
# define ACE_WRITE_GUARD(MUTEX,OBJ,LOCK) \
  ACE_Write_Guard< MUTEX > OBJ (LOCK); \
    if (OBJ.locked () == 0) return;
# define ACE_WRITE_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
  ACE_Write_Guard< MUTEX > OBJ (LOCK); \
    if (OBJ.locked () == 0) return RETURN;
# define ACE_READ_GUARD(MUTEX,OBJ,LOCK) \
  ACE_Read_Guard< MUTEX > OBJ (LOCK); \
    if (OBJ.locked () == 0) return;
# define ACE_READ_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN) \
  ACE_Read_Guard< MUTEX > OBJ (LOCK); \
    if (OBJ.locked () == 0) return RETURN;

// ----------------------------------------------------------------

# define ACE_DES_NOFREE(POINTER,CLASS) \
   do { \
        if (POINTER) \
          { \
            (POINTER)->~CLASS (); \
          } \
      } \
   while (0)

# define ACE_DES_ARRAY_NOFREE(POINTER,SIZE,CLASS) \
   do { \
        if (POINTER) \
          { \
            for (size_t i = 0; \
                 i < SIZE; \
                 ++i) \
            { \
              (&(POINTER)[i])->~CLASS (); \
            } \
          } \
      } \
   while (0)

# define ACE_DES_FREE(POINTER,DEALLOCATOR,CLASS) \
   do { \
        if (POINTER) \
          { \
            (POINTER)->~CLASS (); \
            DEALLOCATOR (POINTER); \
          } \
      } \
   while (0)

# define ACE_DES_ARRAY_FREE(POINTER,SIZE,DEALLOCATOR,CLASS) \
   do { \
        if (POINTER) \
          { \
            for (size_t i = 0; \
                 i < SIZE; \
                 ++i) \
            { \
              (&(POINTER)[i])->~CLASS (); \
            } \
            DEALLOCATOR (POINTER); \
          } \
      } \
   while (0)

# if defined (ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR)
#   define ACE_DES_NOFREE_TEMPLATE(POINTER,T_CLASS,T_PARAMETER) \
     do { \
          if (POINTER) \
            { \
              (POINTER)->~T_CLASS (); \
            } \
        } \
     while (0)
#   define ACE_DES_ARRAY_NOFREE_TEMPLATE(POINTER,SIZE,T_CLASS,T_PARAMETER) \
     do { \
          if (POINTER) \
            { \
              for (size_t i = 0; \
                   i < SIZE; \
                   ++i) \
              { \
                (&(POINTER)[i])->~T_CLASS (); \
              } \
            } \
        } \
     while (0)
#if defined(__IBMCPP__) && (__IBMCPP__ >= 400)
#   define ACE_DES_FREE_TEMPLATE(POINTER,DEALLOCATOR,T_CLASS,T_PARAMETER) \
     do { \
          if (POINTER) \
            { \
              (POINTER)->~T_CLASS T_PARAMETER (); \
              DEALLOCATOR (POINTER); \
            } \
        } \
     while (0)
#else
#   define ACE_DES_FREE_TEMPLATE(POINTER,DEALLOCATOR,T_CLASS,T_PARAMETER) \
     do { \
          if (POINTER) \
            { \
              (POINTER)->~T_CLASS (); \
              DEALLOCATOR (POINTER); \
            } \
        } \
     while (0)
#endif /* defined(__IBMCPP__) && (__IBMCPP__ >= 400) */
#   define ACE_DES_ARRAY_FREE_TEMPLATE(POINTER,SIZE,DEALLOCATOR,T_CLASS,T_PARAMETER) \
     do { \
          if (POINTER) \
            { \
              for (size_t i = 0; \
                   i < SIZE; \
                   ++i) \
              { \
                (&(POINTER)[i])->~T_CLASS (); \
              } \
              DEALLOCATOR (POINTER); \
            } \
        } \
     while (0)
#if defined(__IBMCPP__) && (__IBMCPP__ >= 400)
#   define ACE_DES_FREE_TEMPLATE2(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2) \
     do { \
          if (POINTER) \
            { \
              (POINTER)->~T_CLASS <T_PARAM1, T_PARAM2> (); \
              DEALLOCATOR (POINTER); \
            } \
        } \
     while (0)
#else
#   define ACE_DES_FREE_TEMPLATE2(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2) \
     do { \
          if (POINTER) \
            { \
              (POINTER)->~T_CLASS (); \
              DEALLOCATOR (POINTER); \
            } \
        } \
     while (0)
#endif /* defined(__IBMCPP__) && (__IBMCPP__ >= 400) */
#   define ACE_DES_FREE_TEMPLATE3(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2,T_PARAM3) \
     do { \
          if (POINTER) \
            { \
              (POINTER)->~T_CLASS (); \
              DEALLOCATOR (POINTER); \
            } \
        } \
     while (0)
#   define ACE_DES_FREE_TEMPLATE4(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2,T_PARAM3, T_PARAM4) \
     do { \
          if (POINTER) \
            { \
              (POINTER)->~T_CLASS (); \
              DEALLOCATOR (POINTER); \
            } \
        } \
     while (0)
#   define ACE_DES_ARRAY_FREE_TEMPLATE2(POINTER,SIZE,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2) \
     do { \
          if (POINTER) \
            { \
              for (size_t i = 0; \
                   i < SIZE; \
                   ++i) \
              { \
                (&(POINTER)[i])->~T_CLASS (); \
              } \
              DEALLOCATOR (POINTER); \
            } \
        } \
     while (0)
# else /* ! ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR */
#   define ACE_DES_NOFREE_TEMPLATE(POINTER,T_CLASS,T_PARAMETER) \
     do { \
          if (POINTER) \
            { \
              (POINTER)->T_CLASS T_PARAMETER::~T_CLASS (); \
            } \
        } \
     while (0)
#   define ACE_DES_ARRAY_NOFREE_TEMPLATE(POINTER,SIZE,T_CLASS,T_PARAMETER) \
     do { \
          if (POINTER) \
            { \
              for (size_t i = 0; \
                   i < SIZE; \
                   ++i) \
              { \
                (POINTER)[i].T_CLASS T_PARAMETER::~T_CLASS (); \
              } \
            } \
        } \
     while (0)
#   if defined (__Lynx__) && __LYNXOS_SDK_VERSION == 199701L
  // LynxOS 3.0.0's g++ has trouble with the real versions of these.
#     define ACE_DES_FREE_TEMPLATE(POINTER,DEALLOCATOR,T_CLASS,T_PARAMETER)
#     define ACE_DES_ARRAY_FREE_TEMPLATE(POINTER,DEALLOCATOR,T_CLASS,T_PARAMETER)
#     define ACE_DES_FREE_TEMPLATE2(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2)
#     define ACE_DES_FREE_TEMPLATE3(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2)
#     define ACE_DES_FREE_TEMPLATE4(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2)
#     define ACE_DES_ARRAY_FREE_TEMPLATE2(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2)
#   else
#     define ACE_DES_FREE_TEMPLATE(POINTER,DEALLOCATOR,T_CLASS,T_PARAMETER) \
       do { \
            if (POINTER) \
              { \
                POINTER->T_CLASS T_PARAMETER::~T_CLASS (); \
                DEALLOCATOR (POINTER); \
              } \
          } \
       while (0)
#     define ACE_DES_ARRAY_FREE_TEMPLATE(POINTER,SIZE,DEALLOCATOR,T_CLASS,T_PARAMETER) \
       do { \
            if (POINTER) \
              { \
                for (size_t i = 0; \
                     i < SIZE; \
                     ++i) \
                { \
                  POINTER[i].T_CLASS T_PARAMETER::~T_CLASS (); \
                } \
                DEALLOCATOR (POINTER); \
              } \
          } \
       while (0)
#     define ACE_DES_FREE_TEMPLATE2(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2) \
       do { \
            if (POINTER) \
              { \
                POINTER->T_CLASS <T_PARAM1, T_PARAM2>::~T_CLASS (); \
                DEALLOCATOR (POINTER); \
              } \
          } \
       while (0)
#     define ACE_DES_FREE_TEMPLATE3(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2,T_PARAM3) \
       do { \
            if (POINTER) \
              { \
                POINTER->T_CLASS <T_PARAM1, T_PARAM2, T_PARAM3>::~T_CLASS (); \
                DEALLOCATOR (POINTER); \
              } \
          } \
       while (0)
#     define ACE_DES_FREE_TEMPLATE4(POINTER,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2,T_PARAM3,T_PARAM4) \
       do { \
            if (POINTER) \
              { \
                POINTER->T_CLASS <T_PARAM1, T_PARAM2, T_PARAM3, T_PARAM4>::~T_CLASS (); \
                DEALLOCATOR (POINTER); \
              } \
          } \
       while (0)
#     define ACE_DES_ARRAY_FREE_TEMPLATE2(POINTER,SIZE,DEALLOCATOR,T_CLASS,T_PARAM1,T_PARAM2) \
       do { \
            if (POINTER) \
              { \
                for (size_t i = 0; \
                     i < SIZE; \
                     ++i) \
                { \
                  POINTER[i].T_CLASS <T_PARAM1, T_PARAM2>::~T_CLASS (); \
                } \
                DEALLOCATOR (POINTER); \
              } \
          } \
       while (0)
#   endif /* defined (__Lynx__) && __LYNXOS_SDK_VERSION == 199701L */
# endif /* defined ! ACE_HAS_WORKING_EXPLICIT_TEMPLATE_DESTRUCTOR */


// Moved from OS.h dah
# if defined (ACE_HAS_USING_KEYWORD)
#   define ACE_USING using
# else
#   define ACE_USING
# endif /* ACE_HAS_USING_KEYWORD */

# if defined (ACE_HAS_TYPENAME_KEYWORD)
#   define ACE_TYPENAME typename
# else
#   define ACE_TYPENAME
# endif /* ACE_HAS_TYPENAME_KEYWORD */

# if defined (ACE_HAS_STD_TEMPLATE_SPECIALIZATION)
#   define ACE_TEMPLATE_SPECIALIZATION template<>
# else
#   define ACE_TEMPLATE_SPECIALIZATION
# endif /* ACE_HAS_STD_TEMPLATE_SPECIALIZATION */

# if defined (ACE_HAS_STD_TEMPLATE_METHOD_SPECIALIZATION)
#   define ACE_TEMPLATE_METHOD_SPECIALIZATION template<>
# else
#   define ACE_TEMPLATE_METHOD_SPECIALIZATION
# endif /* ACE_HAS_STD_TEMPLATE_SPECIALIZATION */

// The following is necessary since many C++ compilers don't support
// typedef'd types inside of classes used as formal template
// arguments... ;-(.  Luckily, using the C++ preprocessor I can hide
// most of this nastiness!

# if defined (ACE_HAS_TEMPLATE_TYPEDEFS)

// Handle ACE_Message_Queue.
#   define ACE_SYNCH_DECL class _ACE_SYNCH
#   define ACE_SYNCH_USE _ACE_SYNCH
#   define ACE_SYNCH_MUTEX_T ACE_TYPENAME _ACE_SYNCH::MUTEX
#   define ACE_SYNCH_CONDITION_T ACE_TYPENAME _ACE_SYNCH::CONDITION
#   define ACE_SYNCH_SEMAPHORE_T ACE_TYPENAME _ACE_SYNCH::SEMAPHORE

// Handle ACE_Malloc*
#   define ACE_MEM_POOL_1 class _ACE_MEM_POOL
#   define ACE_MEM_POOL_2 _ACE_MEM_POOL
#   define ACE_MEM_POOL _ACE_MEM_POOL
#   define ACE_MEM_POOL_OPTIONS ACE_TYPENAME _ACE_MEM_POOL::OPTIONS

// Handle ACE_Svc_Handler
#   define ACE_PEER_STREAM_1 class _ACE_PEER_STREAM
#   define ACE_PEER_STREAM_2 _ACE_PEER_STREAM
#   define ACE_PEER_STREAM _ACE_PEER_STREAM
#   define ACE_PEER_STREAM_ADDR ACE_TYPENAME _ACE_PEER_STREAM::PEER_ADDR

// Handle ACE_Acceptor
#   define ACE_PEER_ACCEPTOR_1 class _ACE_PEER_ACCEPTOR
#   define ACE_PEER_ACCEPTOR_2 _ACE_PEER_ACCEPTOR
#   define ACE_PEER_ACCEPTOR _ACE_PEER_ACCEPTOR
#   define ACE_PEER_ACCEPTOR_ADDR ACE_TYPENAME _ACE_PEER_ACCEPTOR::PEER_ADDR

// Handle ACE_Connector
#   define ACE_PEER_CONNECTOR_1 class _ACE_PEER_CONNECTOR
#   define ACE_PEER_CONNECTOR_2 _ACE_PEER_CONNECTOR
#   define ACE_PEER_CONNECTOR _ACE_PEER_CONNECTOR
#   define ACE_PEER_CONNECTOR_ADDR ACE_TYPENAME _ACE_PEER_CONNECTOR::PEER_ADDR
#   if !defined(ACE_HAS_TYPENAME_KEYWORD)
#     define ACE_PEER_CONNECTOR_ADDR_ANY ACE_PEER_CONNECTOR_ADDR::sap_any
#   else
    //
    // If the compiler supports 'typename' we cannot use
    //
    // PEER_CONNECTOR::PEER_ADDR::sap_any
    //
    // because PEER_CONNECTOR::PEER_ADDR is not considered a type. But:
    //
    // typename PEER_CONNECTOR::PEER_ADDR::sap_any
    //
    // will not work either, because now we are declaring sap_any a
    // type, further:
    //
    // (typename PEER_CONNECTOR::PEER_ADDR)::sap_any
    //
    // is considered a casting expression. All I can think of is using a
    // typedef, I tried PEER_ADDR but that was a source of trouble on
    // some platforms. I will try:
    //
#     define ACE_PEER_CONNECTOR_ADDR_ANY ACE_PEER_ADDR_TYPEDEF::sap_any
#   endif /* ACE_HAS_TYPENAME_KEYWORD */

// Handle ACE_SOCK_*
#   define ACE_SOCK_ACCEPTOR ACE_SOCK_Acceptor
#   define ACE_SOCK_CONNECTOR ACE_SOCK_Connector
#   define ACE_SOCK_STREAM ACE_SOCK_Stream

// Handle ACE_MEM_*
#   define ACE_MEM_ACCEPTOR ACE_MEM_Acceptor
#   define ACE_MEM_CONNECTOR ACE_MEM_Connector
#   define ACE_MEM_STREAM ACE_MEM_Stream

// Handle ACE_LSOCK_*
#   define ACE_LSOCK_ACCEPTOR ACE_LSOCK_Acceptor
#   define ACE_LSOCK_CONNECTOR ACE_LSOCK_Connector
#   define ACE_LSOCK_STREAM ACE_LSOCK_Stream

// Handle ACE_TLI_*
#   define ACE_TLI_ACCEPTOR ACE_TLI_Acceptor
#   define ACE_TLI_CONNECTOR ACE_TLI_Connector
#   define ACE_TLI_STREAM ACE_TLI_Stream

// Handle ACE_SPIPE_*
#   define ACE_SPIPE_ACCEPTOR ACE_SPIPE_Acceptor
#   define ACE_SPIPE_CONNECTOR ACE_SPIPE_Connector
#   define ACE_SPIPE_STREAM ACE_SPIPE_Stream

// Handle ACE_UPIPE_*
#   define ACE_UPIPE_ACCEPTOR ACE_UPIPE_Acceptor
#   define ACE_UPIPE_CONNECTOR ACE_UPIPE_Connector
#   define ACE_UPIPE_STREAM ACE_UPIPE_Stream

// Handle ACE_FILE_*
#   define ACE_FILE_CONNECTOR ACE_FILE_Connector
#   define ACE_FILE_STREAM ACE_FILE_IO

// Handle ACE_*_Memory_Pool.
#   define ACE_MMAP_MEMORY_POOL ACE_MMAP_Memory_Pool
#   define ACE_LITE_MMAP_MEMORY_POOL ACE_Lite_MMAP_Memory_Pool
#   define ACE_SBRK_MEMORY_POOL ACE_Sbrk_Memory_Pool
#   define ACE_SHARED_MEMORY_POOL ACE_Shared_Memory_Pool
#   define ACE_LOCAL_MEMORY_POOL ACE_Local_Memory_Pool
#   define ACE_PAGEFILE_MEMORY_POOL ACE_Pagefile_Memory_Pool

# else /* TEMPLATES are broken in some form or another (i.e., most C++ compilers) */

// Handle ACE_Message_Queue.
#   if defined (ACE_HAS_OPTIMIZED_MESSAGE_QUEUE)
#     define ACE_SYNCH_DECL class _ACE_SYNCH_MUTEX_T, class _ACE_SYNCH_CONDITION_T, class _ACE_SYNCH_SEMAPHORE_T
#     define ACE_SYNCH_USE _ACE_SYNCH_MUTEX_T, _ACE_SYNCH_CONDITION_T, _ACE_SYNCH_SEMAPHORE_T
#   else
#     define ACE_SYNCH_DECL class _ACE_SYNCH_MUTEX_T, class _ACE_SYNCH_CONDITION_T
#     define ACE_SYNCH_USE _ACE_SYNCH_MUTEX_T, _ACE_SYNCH_CONDITION_T
#   endif /* ACE_HAS_OPTIMIZED_MESSAGE_QUEUE */
#   define ACE_SYNCH_MUTEX_T _ACE_SYNCH_MUTEX_T
#   define ACE_SYNCH_CONDITION_T _ACE_SYNCH_CONDITION_T
#   define ACE_SYNCH_SEMAPHORE_T _ACE_SYNCH_SEMAPHORE_T

// Handle ACE_Malloc*
#   define ACE_MEM_POOL_1 class _ACE_MEM_POOL, class _ACE_MEM_POOL_OPTIONS
#   define ACE_MEM_POOL_2 _ACE_MEM_POOL, _ACE_MEM_POOL_OPTIONS
#   define ACE_MEM_POOL _ACE_MEM_POOL
#   define ACE_MEM_POOL_OPTIONS _ACE_MEM_POOL_OPTIONS

// Handle ACE_Svc_Handler
#   define ACE_PEER_STREAM_1 class _ACE_PEER_STREAM, class _ACE_PEER_ADDR
#   define ACE_PEER_STREAM_2 _ACE_PEER_STREAM, _ACE_PEER_ADDR
#   define ACE_PEER_STREAM _ACE_PEER_STREAM
#   define ACE_PEER_STREAM_ADDR _ACE_PEER_ADDR

// Handle ACE_Acceptor
#   define ACE_PEER_ACCEPTOR_1 class _ACE_PEER_ACCEPTOR, class _ACE_PEER_ADDR
#   define ACE_PEER_ACCEPTOR_2 _ACE_PEER_ACCEPTOR, _ACE_PEER_ADDR
#   define ACE_PEER_ACCEPTOR _ACE_PEER_ACCEPTOR
#   define ACE_PEER_ACCEPTOR_ADDR _ACE_PEER_ADDR

// Handle ACE_Connector
#   define ACE_PEER_CONNECTOR_1 class _ACE_PEER_CONNECTOR, class _ACE_PEER_ADDR
#   define ACE_PEER_CONNECTOR_2 _ACE_PEER_CONNECTOR, _ACE_PEER_ADDR
#   define ACE_PEER_CONNECTOR _ACE_PEER_CONNECTOR
#   define ACE_PEER_CONNECTOR_ADDR _ACE_PEER_ADDR
#   define ACE_PEER_CONNECTOR_ADDR_ANY ACE_PEER_CONNECTOR_ADDR::sap_any

// Handle ACE_SOCK_*
#   define ACE_SOCK_ACCEPTOR ACE_SOCK_Acceptor, ACE_INET_Addr
#   define ACE_SOCK_CONNECTOR ACE_SOCK_Connector, ACE_INET_Addr
#   define ACE_SOCK_STREAM ACE_SOCK_Stream, ACE_INET_Addr

// Handle ACE_MEM_*
#   define ACE_MEM_ACCEPTOR ACE_MEM_Acceptor, ACE_MEM_Addr
#   define ACE_MEM_CONNECTOR ACE_MEM_Connector, ACE_INET_Addr
#   define ACE_MEM_STREAM ACE_MEM_Stream, ACE_INET_Addr

// Handle ACE_LSOCK_*
#   define ACE_LSOCK_ACCEPTOR ACE_LSOCK_Acceptor, ACE_UNIX_Addr
#   define ACE_LSOCK_CONNECTOR ACE_LSOCK_Connector, ACE_UNIX_Addr
#   define ACE_LSOCK_STREAM ACE_LSOCK_Stream, ACE_UNIX_Addr

// Handle ACE_TLI_*
#   define ACE_TLI_ACCEPTOR ACE_TLI_Acceptor, ACE_INET_Addr
#   define ACE_TLI_CONNECTOR ACE_TLI_Connector, ACE_INET_Addr
#   define ACE_TLI_STREAM ACE_TLI_Stream, ACE_INET_Addr

// Handle ACE_SPIPE_*
#   define ACE_SPIPE_ACCEPTOR ACE_SPIPE_Acceptor, ACE_SPIPE_Addr
#   define ACE_SPIPE_CONNECTOR ACE_SPIPE_Connector, ACE_SPIPE_Addr
#   define ACE_SPIPE_STREAM ACE_SPIPE_Stream, ACE_SPIPE_Addr

// Handle ACE_UPIPE_*
#   define ACE_UPIPE_ACCEPTOR ACE_UPIPE_Acceptor, ACE_SPIPE_Addr
#   define ACE_UPIPE_CONNECTOR ACE_UPIPE_Connector, ACE_SPIPE_Addr
#   define ACE_UPIPE_STREAM ACE_UPIPE_Stream, ACE_SPIPE_Addr

// Handle ACE_FILE_*
#   define ACE_FILE_CONNECTOR ACE_FILE_Connector, ACE_FILE_Addr
#   define ACE_FILE_STREAM ACE_FILE_IO, ACE_FILE_Addr

// Handle ACE_*_Memory_Pool.
#   define ACE_MMAP_MEMORY_POOL ACE_MMAP_Memory_Pool, ACE_MMAP_Memory_Pool_Options
#   define ACE_LITE_MMAP_MEMORY_POOL ACE_Lite_MMAP_Memory_Pool, ACE_MMAP_Memory_Pool_Options
#   define ACE_SBRK_MEMORY_POOL ACE_Sbrk_Memory_Pool, ACE_Sbrk_Memory_Pool_Options
#   define ACE_SHARED_MEMORY_POOL ACE_Shared_Memory_Pool, ACE_Shared_Memory_Pool_Options
#   define ACE_LOCAL_MEMORY_POOL ACE_Local_Memory_Pool, ACE_Local_Memory_Pool_Options
#   define ACE_PAGEFILE_MEMORY_POOL ACE_Pagefile_Memory_Pool, ACE_Pagefile_Memory_Pool_Options
# endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

// These two are only for backward compatibility. You should avoid
// using them if not necessary.
# define ACE_SYNCH_1 ACE_SYNCH_DECL
# define ACE_SYNCH_2 ACE_SYNCH_USE

# ifndef howmany
#   define howmany(x, y)   (((x)+((y)-1))/(y))
# endif /* howmany */

# if defined (ACE_HAS_BROKEN_BITSHIFT)
  // This might not be necessary any more:  it was added prior to the
  // (fd_mask) cast being added to the version below.  Maybe that cast
  // will fix the problem on tandems.    Fri Dec 12 1997 David L. Levine
#   define ACE_MSB_MASK (~(ACE_UINT32 (1) << ACE_UINT32 (NFDBITS - 1)))
# else
  // This needs to go here to avoid overflow problems on some compilers.
#   if defined (ACE_WIN32)
    //  Does ACE_WIN32 have an fd_mask?
#     define ACE_MSB_MASK (~(1 << (NFDBITS - 1)))
#   else  /* ! ACE_WIN32 */
#     define ACE_MSB_MASK (~((fd_mask) 1 << (NFDBITS - 1)))
#   endif /* ! ACE_WIN32 */
# endif /* ACE_HAS_BROKEN_BITSHIFT */


// Byte swapping macros to deal with differences between little endian
// and big endian machines.  Note that "long" here refers to 32 bit
// quantities.
# define ACE_SWAP_LONG(L) ((ACE_SWAP_WORD ((L) & 0xFFFF) << 16) \
            | ACE_SWAP_WORD(((L) >> 16) & 0xFFFF))
# define ACE_SWAP_WORD(L) ((((L) & 0x00FF) << 8) | (((L) & 0xFF00) >> 8))

# if defined (ACE_LITTLE_ENDIAN)
#   define ACE_HTONL(X) ACE_SWAP_LONG (X)
#   define ACE_NTOHL(X) ACE_SWAP_LONG (X)
#   define ACE_IDL_NCTOHL(X) (X)
#   define ACE_IDL_NSTOHL(X) (X)
# else
#   define ACE_HTONL(X) X
#   define ACE_NTOHL(X) X
#   define ACE_IDL_NCTOHL(X) (X << 24)
#   define ACE_IDL_NSTOHL(X) ((X) << 16)
# endif /* ACE_LITTLE_ENDIAN */

# if defined (ACE_LITTLE_ENDIAN)
#   define ACE_HTONS(x) ACE_SWAP_WORD(x)
#   define ACE_NTOHS(x) ACE_SWAP_WORD(x)
# else
#   define ACE_HTONS(x) x
#   define ACE_NTOHS(x) x
# endif /* ACE_LITTLE_ENDIAN */

// Some useful abstrations for expressions involving
// ACE_Allocator.malloc ().  The difference between ACE_NEW_MALLOC*
// with ACE_ALLOCATOR* is that they call constructors also.

# define ACE_ALLOCATOR_RETURN(POINTER,ALLOCATOR,RET_VAL) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL; } \
   } while (0)
# define ACE_ALLOCATOR(POINTER,ALLOCATOR) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return; } \
   } while (0)

# define ACE_NEW_MALLOC_RETURN(POINTER,ALLOCATOR,CONSTRUCTOR,RET_VAL) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return RET_VAL;} \
     else { new (POINTER) CONSTRUCTOR; } \
   } while (0)
# define ACE_NEW_MALLOC(POINTER,ALLOCATOR,CONSTRUCTOR) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; return;} \
     else { new (POINTER) CONSTRUCTOR; } \
   } while (0)

# define ACE_NOOP(x)




// these are more or less constants, but I'll move them later...

// Define the Wide character and normal versions of some of the string macros
# if defined (ACE_HAS_WCHAR)
#   define ACE_DIRECTORY_SEPARATOR_STR_W ACE_TEXT_WIDE(ACE_DIRECTORY_SEPARATOR_STR_A)
#   define ACE_DIRECTORY_SEPARATOR_CHAR_W ACE_TEXT_WIDE(ACE_DIRECTORY_SEPARATOR_CHAR_A)
#   define ACE_PLATFORM_W ACE_TEXT_WIDE(ACE_PLATFORM_A)
#   define ACE_PLATFORM_EXE_SUFFIX_W ACE_TEXT_WIDE(ACE_PLATFORM_EXE_SUFFIX_A)
# endif /* ACE_HAS_WCHAR */

# define ACE_DIRECTORY_SEPARATOR_STR ACE_LIB_TEXT (ACE_DIRECTORY_SEPARATOR_STR_A)
# define ACE_DIRECTORY_SEPARATOR_CHAR ACE_LIB_TEXT (ACE_DIRECTORY_SEPARATOR_CHAR_A)
# define ACE_PLATFORM ACE_LIB_TEXT (ACE_PLATFORM_A)
# define ACE_PLATFORM_EXE_SUFFIX ACE_LIB_TEXT (ACE_PLATFORM_EXE_SUFFIX_A)

// Theses defines are used by the ACE Name Server.
# if !defined (ACE_DEFAULT_LOCALNAME_A)
#   define ACE_DEFAULT_LOCALNAME_A "localnames"
# endif /* ACE_DEFAULT_LOCALNAME_A */
# if !defined (ACE_DEFAULT_GLOBALNAME_A)
#   define ACE_DEFAULT_GLOBALNAME_A "globalnames"
# endif /* ACE_DEFAULT_GLOBALNAME_A */

// ACE_DEFAULT_NAMESPACE_DIR is for legacy mode apps.  A better
// way of doing this is something like ACE_Lib_Find::get_temp_dir, since
// this directory may not exist
# if defined (ACE_LEGACY_MODE)
#   if defined (ACE_WIN32)
#     define ACE_DEFAULT_NAMESPACE_DIR_A "C:\\temp"
#   else /* ACE_WIN32 */
#     define ACE_DEFAULT_NAMESPACE_DIR_A "/tmp"
#   endif /* ACE_WIN32 */
#   if defined (ACE_HAS_WCHAR)
#     define ACE_DEFAULT_NAMESPACE_DIR_W ACE_TEXT_WIDE(ACE_DEFAULT_NAMESPACE_DIR_A)
#   endif /* ACE_HAS_WCHAR */
#   define ACE_DEFAULT_NAMESPACE_DIR ACE_LIB_TEXT(ACE_DEFAULT_NAMESPACE_DIR_A)
# endif /* ACE_LEGACY_MODE */

# if defined (ACE_HAS_WCHAR)
#   define ACE_DEFAULT_LOCALNAME_W ACE_TEXT_WIDE(ACE_DEFAULT_LOCALNAME_A)
#   define ACE_DEFAULT_GLOBALNAME_W ACE_TEXT_WIDE(ACE_DEFAULT_GLOBALNAME_A)
# endif /* ACE_HAS_WCHAR */

# define ACE_DEFAULT_LOCALNAME ACE_LIB_TEXT (ACE_DEFAULT_LOCALNAME_A)
# define ACE_DEFAULT_GLOBALNAME ACE_LIB_TEXT (ACE_DEFAULT_GLOBALNAME_A)


// The "null" device on UNIX.
#if !defined (ACE_DEV_NULL)
#   define ACE_DEV_NULL "/dev/null"
#endif /* !ACE_DEV_NULL */



#include "ace/post.h"

#endif /*ACE_GLOBAL_MACROS_H*/
