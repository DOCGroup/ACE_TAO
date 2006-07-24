// -*- C++ -*-

//====================================================================
/**
 *  @file    Service_Config.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//====================================================================

#ifndef ACE_SERVICE_CONFIG_H
#define ACE_SERVICE_CONFIG_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"
#include "ace/Default_Constants.h"
#include "ace/Service_Gestalt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "ace/OS_NS_signal.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decl.
class ACE_Service_Object;
class ACE_Service_Type;
class ACE_Service_Type_Impl;
class ACE_Service_Repository;
class ACE_Sig_Adapter;
class ACE_Allocator;
class ACE_Reactor;
class ACE_Thread_Manager;
class ACE_DLL;

#if (ACE_USES_CLASSIC_SVC_CONF == 1)
#define ACE_DYNAMIC_SERVICE_DIRECTIVE(ident, libpathname, objectclass, parameters) \
  ACE_LIB_TEXT ("dynamic ") \
  ACE_LIB_TEXT (ident) \
  ACE_LIB_TEXT (" Service_Object * ") \
  ACE_LIB_TEXT (libpathname) \
  ACE_LIB_TEXT (":") \
  ACE_LIB_TEXT (objectclass) \
  ACE_LIB_TEXT ("() \"") \
  ACE_LIB_TEXT (parameters) \
  ACE_LIB_TEXT ("\"")
#define ACE_REMOVE_SERVICE_DIRECTIVE(ident) \
  ACE_LIB_TEXT ("remove ") \
  ACE_LIB_TEXT (ident)
class ACE_Svc_Conf_Param;
#else
#define ACE_DYNAMIC_SERVICE_DIRECTIVE(ident, libpathname, objectclass, parameters) \
  ACE_LIB_TEXT ("<ACE_Svc_Conf><dynamic id=\"") \
  ACE_LIB_TEXT (ident) \
  ACE_LIB_TEXT ("\" type=\"Service_Object\">") \
  ACE_LIB_TEXT ("<initializer path=\"") \
  ACE_LIB_TEXT (libpathname) \
  ACE_LIB_TEXT ("\" init=\"") \
  ACE_LIB_TEXT (objectclass) \
  ACE_LIB_TEXT ("\"") \
  ACE_LIB_TEXT (" params=\"") \
  ACE_LIB_TEXT (parameters) \
  ACE_LIB_TEXT ("\"/></dynamic></ACE_Svc_Conf>")
#define ACE_REMOVE_SERVICE_DIRECTIVE(ident) \
  ACE_LIB_TEXT ("<ACE_Svc_Conf><remove id=\"") \
  ACE_LIB_TEXT (ident) \
  ACE_LIB_TEXT ("\"></remove></ACE_Svc_Conf>")
class ACE_XML_Svc_Conf;
#endif /* ACE_USES_CLASSIC_SVC_CONF == 1 */

ACE_END_VERSIONED_NAMESPACE_DECL

extern "C"
{
  typedef ACE_Service_Object *(*ACE_SERVICE_ALLOCATOR) (ACE_Service_Object_Exterminator *);
}

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Static_Svc_Descriptor
 *
 * @brief Holds the information necessary to describe a statically linked
 * Svc.
 */
class ACE_Static_Svc_Descriptor
{
public:
  /// Name of the service.
  const ACE_TCHAR *name_;

  /// Type of service.
  int type_;

  /// Factory function that allocates the service.
  ACE_SERVICE_ALLOCATOR alloc_;

  /// Bitmask flags indicating how the framework should delete memory.
  u_int flags_;

  /// Flag indicating whether the service starts out active.
  int active_;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

public:
  /// Compare two service descriptors for equality.
  bool operator== (ACE_Static_Svc_Descriptor &) const;

  /// Compare two service descriptors for inequality.
  bool operator!= (ACE_Static_Svc_Descriptor &) const;
};

#define ACE_Component_Config ACE_Service_Config

/**
 * @class ACE_Service_Config
 *
 * @brief Supplies common server operations for dynamic and static
 * configuration of service.
 *
 * The ACE_Service_Config uses the Monostate pattern.  Therefore,
 * you can only have one of these instantiated per-process. It
 * represents the process-wide collection of services, which is
 * typicaly shared among all other configurable entities. The only
 * ACE_Service_Config instance is registered with and owned by the
 * ACE_Object_Manager.
 *
 * By contrast, the ACE_Service_Gestalt represents the collection
 * of services, pertaining to a configurable entity. Typicaly, a
 * "configurable entity" is an instance, which owns an instance of
 * ACE_Service_Gestalt in order to ensure full controll over the
 * services it needs.
 *
 * Another facet of ACE_Service_Config is that for a given thread,
 * it provides access to its current, process-global
 * ACE_Service_Gestalt instance through its curent() method.
 *
 * @note The signal_handler_ static member is allocated by the
 * ACE_Object_Manager.  The ACE_Service_Config constructor
 * uses signal_handler_.  Therefore, if the program has any
 * static ACE_Service_Config objects, there might be
 * initialization order problems.  They can be minimized, but
 * not eliminated, by _not_ #defining
 * ACE_HAS_NONSTATIC_OBJECT_MANAGER.
 */
class ACE_Export ACE_Service_Config: public ACE_Service_Gestalt
{

public:

  // = Initialization and termination methods.

  /**
   * Initialize the Service Repository. Note that initialising @a
   * signum to a negative number will prevent a signal handler being
   * registered when the repository is opened.
   */
  ACE_Service_Config (int ignore_static_svcs = 1,
                      size_t size = ACE_Service_Gestalt::MAX_SERVICES,
                      int signum = SIGHUP);

  /**
   * Performs an open without parsing command-line arguments.  The
   * @a logger_key indicates where to write the logging output, which
   * is typically either a STREAM pipe or a socket address.
   */
  ACE_Service_Config (const ACE_TCHAR program_name[],
                      const ACE_TCHAR *logger_key = ACE_DEFAULT_LOGGER_KEY);

  /// Perform user-specified close activities and remove dynamic
  /// memory.
  virtual ~ACE_Service_Config (void);


protected:

  /**
   * Performs an open without parsing command-line arguments.
   * Implements whats different in the opening sequence
   * for this class, as opposed to the base class.
   *
   * The @a logger_key indicates where to write the logging output, which
   * is typically either a STREAM pipe or a socket address.  If
   * @a ignore_default_svc_conf_file is non-0 then the "svc.conf" file
   * will be ignored.  If @a ignore_debug_flag is non-0 then the
   * application is responsible for setting the
   * @c ACE_Log_Msg::priority_mask() appropriately.  Returns number of
   * errors that occurred on failure and 0 otherwise.
   */
  virtual int open_i (const ACE_TCHAR program_name[],
                      const ACE_TCHAR *logger_key,
                      bool ignore_static_svcs,
                      bool ignore_default_svc_conf_file,
                      bool ignore_debug_flag);

  /**
   * Implements whats different in the command line parameter processing
   * for this class, as opposed to the base class.
   */
  virtual int parse_args_i (int argc, ACE_TCHAR *argv[]);



  /// = Static interfaces

private:

  /// A private, non-locking mutator to set the "current" (TSS) gestalt instance.
  /// Make sure to call with the proper locks held!
  static ACE_Service_Gestalt* current_i (ACE_Service_Gestalt *newcurrent);

  /// A Wrapper for the TSS-stored pointer.
  struct TSS_Resources {
    TSS_Resources (void) : ptr_ (0) {};
    ACE_Service_Gestalt *ptr_;
  };

  /// A type for the TSS-stored resources. The typedef helps to
  /// abstract from the particularities of single-threaded vs
  /// multi-threaded environments.
  typedef ACE_TSS_TYPE (ACE_Service_Config::TSS_Resources) TSS_Service_Gestalt_Ptr;

  /// Provides access to the static ptr, containing the TSS
  /// accessor. Ensures the desired order of initialization, even when
  /// other static initializers need the value.
  static TSS_Service_Gestalt_Ptr * & impl_ (void);

protected:

  /// Mutator to set the (TSS) global instance. Intended for use by helper
  /// classes, like ACE_Service_Config_Guard which when instantiated on the
  /// stack, can temporarily change which gestalt instance is viewed as
  /// global from the point of view of the static initializers in DLLs.
  static ACE_Service_Gestalt* current (ACE_Service_Gestalt*);


public:

  /// If not yet initialized, creates a process-wide instance
  /// global instance, which is registered with the ACE_Object_Manager,
  /// via ACE_Singleton. Note that this is allways the same instance,
  /// in contrast with current (), which may be different instance at
  /// different times, dependent on the context.
  static  ACE_Service_Gestalt* global (void);


  /// Accessor for the "current" service repository through a pointer
  /// held in TSS.
  static ACE_Service_Gestalt* current (void);

  /// This is what the static service initializators are hard-wired
  /// to use, so in order to keep interface changes to a minimum this
  /// method merely forwards to current(). Thus it is possible to
  /// temporarily replace what those initializers think is the global
  /// service repository, for instance when dynamically loading a
  /// service from a DLL, which in turn, contains its own static services.
  static  ACE_Service_Gestalt* instance (void);

  /**
   * Performs an open without parsing command-line arguments.  The
   * @a logger_key indicates where to write the logging output, which
   * is typically either a STREAM pipe or a socket address.  If
   * @a ignore_static_svcs is 1 then static services are not loaded,
   * otherwise, they are loaded.  If @a ignore_default_svc_conf_file is
   * non-0 then the <svc.conf> configuration file will be ignored.
   * Returns zero upon success, -1 if the file is not found or cannot
   * be opened (errno is set accordingly), otherwise returns the
   * number of errors encountered loading the services in the
   * specified svc.conf configuration file.  If @a ignore_debug_flag is
   * non-0 then the application is responsible for setting the
   * @c ACE_Log_Msg::priority_mask appropriately.
   */
  static int open (const ACE_TCHAR program_name[],
                   const ACE_TCHAR *logger_key = ACE_DEFAULT_LOGGER_KEY,
                   int ignore_static_svcs = 1,
                   int ignore_default_svc_conf_file = 0,
                   int ignore_debug_flag = 0);

  /**
   * This is the primary entry point into the ACE_Service_Config (the
   * constructor just handles simple initializations).  It parses
   * arguments passed in from @a argc and @a argv parameters.  The
   * arguments that are valid in a call to this method include:
   *
   * - '-b' Option to indicate that we should be a daemon. Note that when
   *        this option is used, the process will be daemonized before the
   *        service configuration file(s) are read. During daemonization,
   *        (on POSIX systems) the current directory will be changed to "/"
   *        so the caller should either fully specify the file names, or
   *        execute a @c chroot() to the appropriate directory.
   *        @sa ACE::daemonize().
   * - '-d' Turn on debugging mode
   * - '-f' Specifies a configuration file name other than the default
   *        svc.conf. Can be specified multiple times to use multiple files.
   * - '-k' Specifies the rendezvous point to use for the ACE distributed
   *        logger.
   * - '-y' Explicitly enables the use of static services. This flag
   *        overrides the @a ignore_static_svcs parameter value.
   * - '-n' Explicitly disables the use of static services. This flag
   *        overrides the @a ignore_static_svcs parameter value.
   * - '-p' Specifies a pathname which is used to store the process id.
   * - '-s' Specifies a signal number other than SIGHUP to trigger reprocessing
   *        of the configuration file(s). Ignored for platforms that do not
   *        have POSIX signals, such as Windows.
   * - '-S' Specifies a service directive string. Enclose the string in quotes
   *        and escape any embedded quotes with a backslash. This option
   *        specifies service directives without the need for a configuration
   *        file.
   *
   * @param argc The number of commandline arguments.
   * @param argv The array with commandline arguments
   * @param logger_key   Indicates where to write the logging output,
   *                     which is typically either a STREAM pipe or a
   *                     socket address.
   * @param ignore_static_svcs   If 1 then static services are not loaded,
   *                             otherwise, they are loaded.
   * @param ignore_default_svc_conf_file  If non-0 then the @c svc.conf
   *                                      configuration file will be ignored.
   * @param ignore_debug_flag If non-0 then the application is responsible
   *                          for setting the @c ACE_Log_Msg::priority_mask
   *                          appropriately.
   *
   * @retval -1   The configuration file is not found or cannot
   *              be opened (errno is set accordingly).
   * @retval  0   Success.
   * @retval  >0  The number of errors encountered while processing
   *              the service configuration file(s).
   */
  static int open (int argc,
                   ACE_TCHAR *argv[],
                   const ACE_TCHAR *logger_key = ACE_DEFAULT_LOGGER_KEY,
                   int ignore_static_svcs = 1,
                   int ignore_default_svc_conf_file = 0,
                   int ignore_debug_flag = 0);

  /// Tidy up and perform last rites when ACE_Service_Config is shut
  /// down.  This method calls <close_svcs>.  Returns 0.
  static int close (void);

  /// Perform user-specified close hooks and possibly delete all of the
  /// configured services in the <Service_Repository>.
  static int fini_svcs (void);

  /**
   * Perform user-specified close hooks on all of the configured
   * services in the Service_Repository, then delete the
   * Service_Repository itself.  Returns 0.
   */
  static int close_svcs (void);

  /// True if reconfiguration occurred.
  static int reconfig_occurred (void);

  /// Indicate that reconfiguration occurred.
  static void reconfig_occurred (int);

  /// Perform the reconfiguration process.
  static void reconfigure (void);

  // = The following methods are static in order to enforce Singleton
  // semantics for the Reactor, Service_Repository, Thread_Manager,
  // and Acceptor/Connector Strategy factory.  Other portions of the
  // system may need to access them at some point or another...

  // = This is not strictly needed, anymore since the service configurator
  // has been refactored to allow multiple service configuration
  // instances (called gestalts). The interfaces, however were retained in for
  // the sake of maintaining source-code compatibility.


  // = Accessors and mutators for process-wide Singletons.

  /// Returns a pointer to the list of statically linked services.
  ///
  /// @deprecated - Same as instance(), but still useful in legacy code,
  /// (notably, one that can not be easily modified) which uses the following
  /// idiom for registering static services:
  ///
  ///    ACE_Service_Config::static_svcs ()->insert (...);
  static ACE_Service_Gestalt *static_svcs (void);

  /// Insert a static service descriptor for processing on open_i(). The
  /// corresponding ACE_STATIC_SVC_* macros were chaged to use this method
  /// instead of obtaining a ptr to a container. See the note on static_svcs().
  /// Added to prevent exposing the internal storage representation of the
  /// services repository and provide a better way of debugging service
  /// loading and registration problems.
  static int insert (ACE_Static_Svc_Descriptor *svc);

  // = Utility methods.
  /// Dynamically link the shared object file and retrieve a pointer to
  /// the designated shared object in this file.
  static int initialize (const ACE_Service_Type *,
                         const ACE_TCHAR *parameters);

  /// Initialize and activate a statically @a svc_name service.
  static int initialize (const ACE_TCHAR *svc_name,
                         const ACE_TCHAR *parameters);

  /// Resume a @a svc_name that was previously suspended or has not yet
  /// been resumed (e.g., a static service).
  static int resume (const ACE_TCHAR svc_name[]);

  /**
   * Suspend @a svc_name.  Note that this will not unlink the service
   * from the daemon if it was dynamically linked, it will mark it as
   * being suspended in the Service Repository and call the <suspend>
   * member function on the appropriate ACE_Service_Object.  A
   * service can be resumed later on by calling the <RESUME> member
   * function...
   */
  static int suspend (const ACE_TCHAR svc_name[]);

  /// Totally remove @a svc_name from the daemon by removing it
  /// from the ACE_Reactor, and unlinking it if necessary.
  static int remove (const ACE_TCHAR svc_name[]);

#if defined (ACE_HAS_WINCE) && defined (ACE_USES_WCHAR)
  // We must provide these function to bridge the Svc_Conf parser
  // with ACE.
  static int initialize (const ACE_Service_Type *, ACE_ANTI_TCHAR []);
  static int initialize (const char svc_name[], ACE_ANTI_TCHAR parameters[]);
  static int resume (const ACE_ANTI_TCHAR svc_name[]);
  static int suspend (const ACE_ANTI_TCHAR svc_name[]);
  static int remove (const ACE_ANTI_TCHAR svc_name[]);
#endif /* ACE_HAS_WINCE */

  /// Dump the state of an object.
  void dump (void) const;

  /// Set the signal_handler;for internal use by ACE_Object_Manager only.
  static ACE_INLINE void signal_handler (ACE_Sig_Adapter *);

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  /// Process a file containing a list of service configuration
  /// directives.
  static int process_file (const ACE_TCHAR file[]);

  /// Process one service configuration @a directive, which is passed as
  /// a string.  Returns the number of errors that occurred.
  static int process_directive (const ACE_TCHAR directive[]);

  /// Process one static service definition.
  /**
   * Load a new static service into the ACE_Service_Repository.
   *
   * @param ssd Service descriptor, see the document of
   *        ACE_Static_Svc_Descriptor for more details.
   *
   * @param force_replace If set the new service descriptor replaces
   *        any previous instance in the ACE_Service_Repository.
   *
   * @return Returns -1 if the service cannot be 'loaded'.
   */
  static int process_directive (const ACE_Static_Svc_Descriptor &ssd,
                                int force_replace = 0);

  /**
   * Process (or re-process) service configuration requests that are
   * provided in the svc.conf file(s).  Returns the number of errors
   * that occurred.
   */
  static int process_directives (void);

  /// Handles signals to trigger reconfigurations.
  static void handle_signal (int sig, siginfo_t *, ucontext_t *);

  /**
   * Handle the command-line options intended for the
   * ACE_Service_Config.  Note that @c argv[0] is assumed to be the
   * program name.
   * The arguments that are valid in a call to this method are
   * - '-b' Option to indicate that we should be a daemon
   * - '-d' Turn on debugging mode
   * - '-f' Option to read in the list of svc.conf file names
   * - '-k' Option to read a wide string where in the logger output can
   *        be written
   * - '-y' Turn on the flag for a  repository of statically
   *        linked services
   * - '-n' Need not have a repository of statically linked services
   * - '-S' Option to read in the list of services on the command-line
   *        Please observe the difference between options '-f' that looks
   *        for a list of files and here a list of services.
   */
  static int parse_args (int, ACE_TCHAR *argv[]);
#if (ACE_USES_CLASSIC_SVC_CONF == 0)
  static ACE_Service_Type *create_service_type  (const ACE_TCHAR *n,
                                                 ACE_Service_Type_Impl *o,
                                                 ACE_DLL &dll,
                                                 int active);
#endif /* ACE_USES_CLASSIC_SVC_CONF == 0 */

  static ACE_Service_Type_Impl *
    create_service_type_impl (const ACE_TCHAR *name,
                              int type,
                              void *symbol,
                              u_int flags,
                              ACE_Service_Object_Exterminator gobbler);

protected:

  /// Process service configuration requests that were provided on the
  /// command-line.  Returns the number of errors that occurred.
  static int process_commandline_directives (void);

#if (ACE_USES_CLASSIC_SVC_CONF == 1)
  /// This is the implementation function that process_directives()
  /// and process_directive() both call.  Returns the number of errors
  /// that occurred.
  static int process_directives_i (ACE_Svc_Conf_Param *param);
#endif /* ACE_USES_CLASSIC_SVC_CONF == 1 */

  /// Become a daemon.
  static int start_daemon (void);

  /// Add the default statically-linked services to the
  /// ACE_Service_Repository.
  static int load_static_svcs (void);


private:

  /// True if reconfiguration occurred.
  static sig_atomic_t reconfig_occurred_;

  // = Set by command-line options.
  /// Shall we become a daemon process?
  static int be_a_daemon_;

  /// Pathname of file to write process id.
  static ACE_TCHAR *pid_file_name_;

  /// Number of the signal used to trigger reconfiguration.
  static int signum_;

  /// Handles the reconfiguration signals.
  static ACE_Sig_Adapter *signal_handler_;

  /// Pointer to the Singleton (ACE_Cleanup) Gestalt instance.
  /// There is thread-specific global instance pointer, which is used to
  /// temporarily change which Gestalt instance is used for static service
  /// registrations.
  ///
  /// A specific use case is a thread, which loads a _dynamic_ service from
  /// a DLL. If the said DLL also contains additional _static_ services,
  /// those *must* be registered with the same configuration repository as
  /// the dynamic service. Otherwise, the DLL's static services would be
  /// registered with the global Gestalt and may outlive the DLL that
  /// contains their code and perhaps the memory in which they are in.
  /// This is a problem because if the DLL gets unloaded (as it will, if
  /// it was loaded in an instance of Gestalt), the DLL's memory will be
  /// deallocated, but the global service repository will still "think"
  /// it must finalize the (DLL's) static services - with disastrous
  /// consequences, occurring in the post-main code (at_exit()).

  /// This class needs the intimate access to be able to swap the
  /// current TSS pointer for the global Gestalt.
  friend class ACE_Service_Config_Guard;
};

/**
 * @class ACE_Service_Config_Guard
 *
 * @brief A guard class, designed to be instantiated on the stack.
 *
 * Instantiating it with a specific configuration ensures any references to
 * ACE_Service_Config::instance(), even when occuring in static constructors,
 * will allways access the designated configuration instance.
 * This comes very handy when a dynamic service also registers any static
 * services of its own and their static factories.
 */
class ACE_Export ACE_Service_Config_Guard
{
public:
  ACE_Service_Config_Guard (ACE_Service_Gestalt * psg);
  ~ACE_Service_Config_Guard (void);

private:
  // Private AND not implemented to disable copying
  ACE_Service_Config_Guard(const ACE_Service_Config_Guard&);
  ACE_Service_Config_Guard& operator= (const ACE_Service_Config_Guard&);

private:
  ACE_Service_Gestalt* saved_;
};


ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Service_Config.inl"
#endif /* __ACE_INLINE__ */


#include /**/ "ace/post.h"

#endif /* ACE_SERVICE_CONFIG_H */
