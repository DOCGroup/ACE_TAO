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

// Start Global Macros
# define ACE_BEGIN_DUMP ACE_LIB_TEXT ("\n====\n(%P|%t|%x)\n")
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

#if defined (ACE_HAS_NO_THROW_SPEC)
#   define ACE_THROW_SPEC(X)
#else
# if defined (ACE_HAS_EXCEPTIONS)
#   define ACE_THROW_SPEC(X) throw X
#   if defined (ACE_WIN32) && defined(_MSC_VER) && !defined (ghs)
// @@ MSVC "supports" the keyword but doesn't implement it (Huh?).
//    Therefore, we simply supress the warning for now.
#     pragma warning( disable : 4290 )
#   endif /* ACE_WIN32 */
# else  /* ! ACE_HAS_EXCEPTIONS */
#   define ACE_THROW_SPEC(X)
# endif /* ! ACE_HAS_EXCEPTIONS */
#endif /*ACE_HAS_NO_THROW_SPEC*/

// ----------------------------------------------------------------

// Deal with MSVC++ 6 (or less) insanity for CORBA...
# if defined (ACE_HAS_BROKEN_NAMESPACES)
#   define ACE_CORBA_1(NAME) CORBA_##NAME
#   define ACE_CORBA_2(TYPE, NAME) CORBA_##TYPE##_##NAME
#   define ACE_CORBA_3(TYPE, NAME) CORBA_##TYPE::NAME
#   if !defined (ACE_NESTED_CLASS)
#     define ACE_NESTED_CLASS(TYPE, NAME) NAME
#   endif  /* !ACE_NESTED_CLASS */
# else  /* ! ACE_HAS_BROKEN_NAMESPACES */
#   define ACE_CORBA_1(NAME) CORBA::NAME
#   define ACE_CORBA_2(TYPE, NAME) CORBA::TYPE::NAME
#   define ACE_CORBA_3(TYPE, NAME) CORBA::TYPE::NAME
#   if !defined (ACE_NESTED_CLASS)
#     define ACE_NESTED_CLASS(TYPE, NAME) TYPE::NAME
#   endif  /* !ACE_NESTED_CLASS */
# endif /* ! ACE_HAS_BROKEN_NAMESPACES */

// ----------------------------------------------------------------

# define ACE_TRACE_IMPL(X) ACE_Trace ____ (ACE_LIB_TEXT (X), __LINE__, ACE_LIB_TEXT (__FILE__))

# if (ACE_NTRACE == 1)
#   define ACE_TRACE(X)
# else
#   define ACE_TRACE(X) ACE_TRACE_IMPL(X)
#   include "ace/Trace.h"
# endif /* ACE_NTRACE */

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


/*******************************************************************/

/// Service Objects, i.e., objects dynamically loaded via the service
/// configurator, must provide a destructor function with the
/// following prototype to perform object cleanup.
typedef void (*ACE_Service_Object_Exterminator)(void *);

/** @name Service Configurator macros
 *
 * The following macros are used to define helper objects used in
 * ACE's Service Configurator.  This is an implementation of the
 * Service Configurator pattern:
 *
 * http://www.cs.wustl.edu/~schmidt/PDF/SvcConf.pdf
 *
 * The intent of this pattern is to allow developers to dynamically
 * load and configure services into a system.  With a little help from
 * this macros statically linked services can also be dynamically
 * configured.
 *
 * More details about this component are available in the documentation
 * of the ACE_Service_Configurator class and also
 * ACE_Dynamic_Service.
 *
 * Notice that in all the macros the SERVICE_CLASS parameter must be
 * the name of a class derived from ACE_Service_Object.
 */
//@{
/// Declare a the data structure required to register a statically
/// linked service into the service configurator.
/**
 * The macro should be used in the header file where the service is
 * declared, its only argument is usually the name of the class that
 * implements the service.
 *
 * @param SERVICE_CLASS The name of the class implementing the
 *   service.
 */
# define ACE_STATIC_SVC_DECLARE(SERVICE_CLASS) \
extern ACE_Static_Svc_Descriptor ace_svc_desc_##SERVICE_CLASS ;

/// As ACE_STATIC_SVC_DECLARE, but using an export macro for NT
/// compilers.
/**
 * NT compilers require the use of explicit directives to export and
 * import symbols from a DLL.  If you need to define a service in a
 * dynamic library you should use this version instead.
 * Normally ACE uses a macro to inject the correct export/import
 * directives on NT.  Naturally it also the macro expands to a blank
 * on platforms that do not require such directives.
 * The first argument (EXPORT_NAME) is the prefix for this export
 * macro, the full name is formed by appending _Export.
 * ACE provides tools to generate header files that define the macro
 * correctly on all platforms, please see
 * $ACE_ROOT/bin/generate_export_file.pl
 *
 * @param EXPORT_NAME The export macro name prefix.
 * @param SERVICE_CLASS The name of the class implementing the service.
 */
#define ACE_STATIC_SVC_DECLARE_EXPORT(EXPORT_NAME,SERVICE_CLASS) \
extern EXPORT_NAME##_Export ACE_Static_Svc_Descriptor ace_svc_desc_##SERVICE_CLASS;

/// Define the data structure used to register a statically linked
/// service into the Service Configurator.
/**
 * The service configurator requires several arguments to build and
 * control an statically linked service, including its name, the
 * factory function used to construct the service, and some flags.
 * All those parameters are configured in a single structure, an
 * instance of this structure is statically initialized using the
 * following macro.
 *
 * @param SERVICE_CLASS The name of the class that implements the
 *    service, must be derived (directly or indirectly) from
 *    ACE_Service_Object.
 * @param NAME The name for this service, this name is used by the
 *    service configurator to match configuration options provided in
 *    the svc.conf file.
 * @param TYPE The type of object.  Objects can be streams or service
 *    objects.  Please read the ACE_Service_Configurator and ASX
 *    documentation for more details.
 * @param FN The name of the factory function, usually the
 *    ACE_SVC_NAME macro can be used to generate the name.  The
 *    factory function is often defined using ACE_FACTORY_DECLARE and
 *    ACE_FACTORY_DEFINE.
 * @param FLAGS Flags to control the ownership and lifecycle of the
 *    object. Please read the ACE_Service_Configurator documentation
 *    for more details.
 * @param ACTIVE If not zero then a thread will be dedicate to the
 *    service. Please read the ACE_Service_Configurator documentation
 *    for more details.
 */
#define ACE_STATIC_SVC_DEFINE(SERVICE_CLASS, NAME, TYPE, FN, FLAGS, ACTIVE) \
ACE_Static_Svc_Descriptor ace_svc_desc_##SERVICE_CLASS = { NAME, TYPE, FN, FLAGS, ACTIVE };

/// Automatically register a service with the service configurator
/**
 * In some applications the services must be automatically registered
 * with the service configurator, before main() starts.
 * The ACE_STATIC_SVC_REQUIRE macro defines a class whose constructor
 * register the service, it also defines a static instance of that
 * class to ensure that the service is registered before main.
 *
 * On platforms that lack adequate support for static C++ objects the
 * macro ACE_STATIC_SVC_REGISTER can be used to explicitly register
 * the service.
 *
 * @todo One class per-Service_Object seems wasteful.  It should be
 *   possible to define a single class and re-use it for all the
 *   service objects, just by passing the Service_Descriptor as an
 *   argument to the constructor.
 */
#if defined(ACE_LACKS_STATIC_CONSTRUCTORS)
# define ACE_STATIC_SVC_REQUIRE(SERVICE_CLASS)\
class ACE_Static_Svc_##SERVICE_CLASS {\
public:\
  ACE_Static_Svc_##SERVICE_CLASS() { \
    ACE_Service_Config::static_svcs ()->insert (\
         &ace_svc_desc_##SERVICE_CLASS); \
  } \
};
#define ACE_STATIC_SVC_REGISTER(SERVICE_CLASS)\
ACE_Static_Svc_##SERVICE_CLASS ace_static_svc_##SERVICE_CLASS

#else /* !ACE_LACKS_STATIC_CONSTRUCTORS */

# define ACE_STATIC_SVC_REQUIRE(SERVICE_CLASS)\
class ACE_Static_Svc_##SERVICE_CLASS {\
public:\
  ACE_Static_Svc_##SERVICE_CLASS() { \
    ACE_Service_Config::static_svcs ()->insert (\
         &ace_svc_desc_##SERVICE_CLASS); \
    } \
};\
static ACE_Static_Svc_##SERVICE_CLASS ace_static_svc_##SERVICE_CLASS;
#define ACE_STATIC_SVC_REGISTER(SERVICE_CLASS) do {} while (0)

#endif /* !ACE_LACKS_STATIC_CONSTRUCTORS */

/// Declare the factory method used to create dynamically loadable
/// services.
/**
 * Once the service implementation is dynamically loaded the Service
 * Configurator uses a factory method to create the object.
 * This macro declares such a factory function with the proper
 * interface and export macros.
 * Normally used in the header file that declares the service
 * implementation.
 *
 * @param CLS must match the prefix of the export macro used for this
 *        service.
 * @param SERVICE_CLASS must match the name of the class that
 *        implements the service.
 *
 */
#define ACE_FACTORY_DECLARE(CLS,SERVICE_CLASS) \
extern "C" CLS##_Export ACE_Service_Object *\
_make_##SERVICE_CLASS (ACE_Service_Object_Exterminator *);

/// Define the factory method (and destructor) for a dynamically
/// loadable service.
/**
 * Use with arguments matching ACE_FACTORY_DECLARE.
 * Normally used in the .cpp file that defines the service
 * implementation.
 *
 * This macro defines both the factory method and the function used to
 * cleanup the service object.
 *
 * If this macro is used to define a factory function that need not be
 * exported (for example, in a static service situation), CLS can be
 * specified as ACE_Local_Service.
 */
# define ACE_Local_Service_Export

# define ACE_FACTORY_DEFINE(CLS,SERVICE_CLASS) \
void _gobble_##SERVICE_CLASS (void *p) { \
  ACE_Service_Object *_p = ACE_static_cast (ACE_Service_Object *, p); \
  ACE_ASSERT (_p != 0); \
  delete _p; } \
extern "C" CLS##_Export ACE_Service_Object *\
_make_##SERVICE_CLASS (ACE_Service_Object_Exterminator *gobbler) \
{ \
  ACE_TRACE (#SERVICE_CLASS); \
  if (gobbler != 0) \
    *gobbler = (ACE_Service_Object_Exterminator) _gobble_##SERVICE_CLASS; \
  return new SERVICE_CLASS; \
}

/// The canonical name for a service factory method
#define ACE_SVC_NAME(SERVICE_CLASS) _make_##SERVICE_CLASS

/// The canonical way to invoke (i.e. construct) a service factory
/// method.
#define ACE_SVC_INVOKE(SERVICE_CLASS) _make_##SERVICE_CLASS (0)

//@}

/** @name Helper macros for services defined in the netsvcs library.
 *
 * The ACE services defined in netsvcs use this helper macros for
 * simplicity.
 *
 */
//@{
# define ACE_SVC_FACTORY_DECLARE(X) ACE_FACTORY_DECLARE (ACE_Svc, X)
# define ACE_SVC_FACTORY_DEFINE(X) ACE_FACTORY_DEFINE (ACE_Svc, X)
//@}

#if !defined (ACE_WIN32)
// Add some typedefs and macros to enhance Win32 conformance...
#   if !defined (LPSECURITY_ATTRIBUTES)
#     define LPSECURITY_ATTRIBUTES int
#   endif /* !defined LPSECURITY_ATTRIBUTES */
#   if !defined (GENERIC_READ)
#     define GENERIC_READ 0
#   endif /* !defined GENERIC_READ */
#   if !defined (FILE_SHARE_READ)
#     define FILE_SHARE_READ 0
#   endif /* !defined FILE_SHARE_READ */
#   if !defined (OPEN_EXISTING)
#     define OPEN_EXISTING 0
#   endif /* !defined OPEN_EXISTING */
#   if !defined (FILE_ATTRIBUTE_NORMAL)
#     define FILE_ATTRIBUTE_NORMAL 0
#   endif /* !defined FILE_ATTRIBUTE_NORMAL */
#   if !defined (MAXIMUM_WAIT_OBJECTS)
#     define MAXIMUM_WAIT_OBJECTS 0
#   endif /* !defined MAXIMUM_WAIT_OBJECTS */
#   if !defined (FILE_FLAG_OVERLAPPED)
#     define FILE_FLAG_OVERLAPPED 0
#   endif /* !defined FILE_FLAG_OVERLAPPED */
#   if !defined (FILE_FLAG_SEQUENTIAL_SCAN)
#     define FILE_FLAG_SEQUENTIAL_SCAN 0
#   endif   /* FILE_FLAG_SEQUENTIAL_SCAN */
#   if !defined(FILE_FLAG_WRITE_THROUGH)
#     define FILE_FLAG_WRITE_THROUGH 0
#   endif /* !defined FILE_FLAG_WRITE_THROUGH */
#   if !defined(PIPE_WAIT)
#     define PIPE_WAIT 0
#   endif /* !defined PIPE_WAIT */
#   if !defined(PIPE_NOWAIT)
#     define PIPE_NOWAIT 0
#   endif /* !defined PIPE_WAIT */
#   if !defined(PIPE_READMODE_BYTE)
#     define PIPE_READMODE_BYTE 0
#   endif /* !defined PIPE_READMODE_BYTE */
#   if !defined(PIPE_READMODE_MESSAGE)
#     define PIPE_READMODE_MESSAGE 0
#   endif /* !defined PIPE_READMODE_MESSAGE */
#   if !defined(PIPE_TYPE_BYTE)
#     define PIPE_TYPE_BYTE 0
#   endif /* !defined PIPE_TYPE_BYTE */
#   if !defined(PIPE_TYPE_MESSAGE)
#     define PIPE_TYPE_MESSAGE 0
#   endif /* !defined PIPE_TYPE_MESSAGE */
#endif /* !ACE_WIN32 */

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
# define ACE_ALLOCATOR_NORETURN(POINTER,ALLOCATOR) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM; } \
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
# define ACE_NEW_MALLOC_NORETURN(POINTER,ALLOCATOR,CONSTRUCTOR) \
   do { POINTER = ALLOCATOR; \
     if (POINTER == 0) { errno = ENOMEM;} \
     else { new (POINTER) CONSTRUCTOR; } \
   } while (0)

# define ACE_NOOP(x)

#include "ace/post.h"

#endif /*ACE_GLOBAL_MACROS_H*/
