/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Service_Object.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_SERVICE_OBJECT_H
#define ACE_SERVICE_OBJECT_H
#include "ace/pre.h"

#include "ace/Shared_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"
#include "ace/os_include/dlfcn.h"

#define ACE_Component ACE_Service_Object
/**
 * @class ACE_Service_Object
 *
 * @brief Provide the abstract base class common to all service
 * implementations.
 *
 * Classes that inherit from <ACE_Service_Objects> are capable
 * of being registered with the <ACE_Reactor> (due to the
 * <ACE_Event_Handler>, as well as being dynamically linked by
 * the <ACE_Service_Config> (due to the <ACE_Shared_Object>).
 */
class ACE_Export ACE_Service_Object : public ACE_Event_Handler, public ACE_Shared_Object
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  ACE_Service_Object (ACE_Reactor * = 0);

  /// Destructor.
  virtual ~ACE_Service_Object (void);

    /// Temporarily disable a service without removing it completely.
  virtual int suspend (void);

    /// Re-enable a previously suspended service.
  virtual int resume (void);
};

// Forward decl.
class ACE_Service_Type_Impl;

/**
 * @class ACE_Service_Type
 *
 * @brief Keeps track of information related to the various
 * <ACE_Service_Type_Impl> subclasses.
 *
 * This class acts as the interface of the "Bridge" pattern.
 */
class ACE_Export ACE_Service_Type
{
public:
  enum
  {
    /// Delete the payload object.
    DELETE_OBJ = 1,

    /// Delete the enclosing object.
    DELETE_THIS = 2
  };

  // = Initialization and termination methods.
  ACE_Service_Type (const ACE_TCHAR *n,
                    ACE_Service_Type_Impl *o,
                    const ACE_SHLIB_HANDLE handle,
                    int active);
  ~ACE_Service_Type (void);

  const ACE_TCHAR *name (void) const;
  void name (const ACE_TCHAR *);

  const ACE_Service_Type_Impl *type (void) const;
  void type (const ACE_Service_Type_Impl *,
             int active = 1);

  ACE_SHLIB_HANDLE handle (void) const;
  void handle (const ACE_SHLIB_HANDLE);

  void suspend (void) const;
  void resume (void) const;
  int  active (void) const;
  void active (int);

  /// Calls <fini> on <type_>
  void fini (void);

  /// Check if the service has been fini'ed.
  int fini_called (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Humanly readible name of svc.
  const ACE_TCHAR *name_;

  /// Pointer to C++ object that implements the svc.
  const ACE_Service_Type_Impl *type_;

  /// Handle to shared object file (non-zero if dynamically linked).
  ACE_SHLIB_HANDLE handle_;

  /// 1 if svc is currently active, otherwise 0.
  int active_;

  /// 1 if <fini> on <type_> has already been called, otherwise 0.
  int fini_already_called_;
};

/**
 * @class ACE_Service_Object_Ptr
 *
 * @brief This is a smart pointer that holds onto the associated
 * <ACE_Service_Object> * until the current scope is left, at
 * which point the object's <fini> hook is called and the
 * service_object_ gets deleted.
 *
 * This class is similar to the Standard C++ Library class
 * <auto_ptr>.  It is used in conjunction with statically linked
 * <ACE_Service_Objects>, as shown in the
 * ./netsvcs/server/main.cpp example.
 */
class ACE_Export ACE_Service_Object_Ptr
{
public:
  // = Initialization and termination methods.
  /// Acquire ownership of the <so>.
  ACE_Service_Object_Ptr (ACE_Service_Object *so);

  /// Release the held <ACE_Service_Object> by calling its <fini> hook.
  ~ACE_Service_Object_Ptr (void);

  /// Smart pointer to access the underlying <ACE_Service_Object>.
  ACE_Service_Object *operator-> ();

private:
  /// Holds the service object until we're done.
  ACE_Service_Object *service_object_;
};


// Moved from OS.h dah
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
# define ACE_STATIC_SVC_DECLARE_EXPORT(EXPORT_NAME,SERVICE_CLASS) \
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
# define ACE_STATIC_SVC_DEFINE(SERVICE_CLASS, NAME, TYPE, FN, FLAGS, ACTIVE) \
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
# if defined(ACE_LACKS_STATIC_CONSTRUCTORS)
#   define ACE_STATIC_SVC_REQUIRE(SERVICE_CLASS)\
class ACE_Static_Svc_##SERVICE_CLASS {\
public:\
  ACE_Static_Svc_##SERVICE_CLASS() { \
    ACE_Service_Config::static_svcs ()->insert (\
         &ace_svc_desc_##SERVICE_CLASS); \
  } \
};
#   define ACE_STATIC_SVC_REGISTER(SERVICE_CLASS)\
ACE_Static_Svc_##SERVICE_CLASS ace_static_svc_##SERVICE_CLASS

# else /* !ACE_LACKS_STATIC_CONSTRUCTORS */

#   define ACE_STATIC_SVC_REQUIRE(SERVICE_CLASS)\
class ACE_Static_Svc_##SERVICE_CLASS {\
public:\
  ACE_Static_Svc_##SERVICE_CLASS() { \
    ACE_Service_Config::static_svcs ()->insert (\
         &ace_svc_desc_##SERVICE_CLASS); \
    } \
};\
static ACE_Static_Svc_##SERVICE_CLASS ace_static_svc_##SERVICE_CLASS;
#   define ACE_STATIC_SVC_REGISTER(SERVICE_CLASS) do {} while (0)

# endif /* !ACE_LACKS_STATIC_CONSTRUCTORS */

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
# define ACE_FACTORY_DECLARE(CLS,SERVICE_CLASS) \
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
 */
# define ACE_FACTORY_DEFINE(CLS,SERVICE_CLASS) \
void _gobble_##SERVICE_CLASS (void *p) { \
  ACE_Service_Object *_p = ACE_reinterpret_cast (ACE_Service_Object *, p); \
  ACE_ASSERT (_p != 0); \
  delete _p; } \
extern "C" ACE_Service_Object *\
_make_##SERVICE_CLASS (ACE_Service_Object_Exterminator *gobbler) \
{ \
  ACE_TRACE (#SERVICE_CLASS); \
  if (gobbler != 0) \
    *gobbler = (ACE_Service_Object_Exterminator) _gobble_##SERVICE_CLASS; \
  return new SERVICE_CLASS; \
}

/// The canonical name for a service factory method
# define ACE_SVC_NAME(SERVICE_CLASS) _make_##SERVICE_CLASS

/// The canonical way to invoke (i.e. construct) a service factory
/// method.
# define ACE_SVC_INVOKE(SERVICE_CLASS) _make_##SERVICE_CLASS (0)

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

#if defined (__ACE_INLINE__)
#include "ace/Service_Object.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_SERVICE_OBJECT_H */
