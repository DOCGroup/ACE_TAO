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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Unbounded_Queue.h"
#include "ace/Unbounded_Set.h"
#include "ace/SString.h"
#include "ace/XML_Svc_Conf.h"


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
class ACE_Svc_Conf_Param;
#endif /* ACE_USES_CLASSIC_SVC_CONF ==1 */

extern "C"
{
  typedef ACE_Service_Object *(*ACE_SERVICE_ALLOCATOR) (ACE_Service_Object_Exterminator *);
}

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
  int operator== (ACE_Static_Svc_Descriptor &) const;

  /// Compare two service descriptors for inequality.
  int operator!= (ACE_Static_Svc_Descriptor &) const;
};

// Maintain a set of the statically linked service descriptors.
typedef ACE_Unbounded_Set<ACE_Static_Svc_Descriptor *>
        ACE_STATIC_SVCS;
typedef ACE_Unbounded_Set_Iterator<ACE_Static_Svc_Descriptor *>
        ACE_STATIC_SVCS_ITERATOR;

// Maintain a queue of services to be configured from the
// command-line.
typedef ACE_Unbounded_Queue<ACE_TString>
        ACE_SVC_QUEUE;
typedef ACE_Unbounded_Queue_Iterator<ACE_TString>
        ACE_SVC_QUEUE_ITERATOR;

#define ACE_Component_Config ACE_Service_Config
/**
 * @class ACE_Service_Config
 *
 * @brief Supplies common server operations for dynamic and static
 * configuration of services.
 *
 * The <ACE_Service_Config> uses the Monostate pattern.  Therefore,
 * you can only have one of these instantiated per-process.
 * NOTE: the signal_handler_ static member is allocated by the
 * <ACE_Object_Manager>.  The <ACE_Service_Config> constructor
 * uses signal_handler_.  Therefore, if the program has any
 * static <ACE_Service_Config> objects, there might be
 * initialization order problems.  They can be minimized, but
 * not eliminated, by _not_ #defining
 * <ACE_HAS_NONSTATIC_OBJECT_MANAGER>.
 */
class ACE_Export ACE_Service_Config
{
public:
  enum
  {
    MAX_SERVICES = ACE_DEFAULT_SERVICE_REPOSITORY_SIZE
  };

  // = Initialization and termination methods.

  /// Initialize the Service Repository.
  ACE_Service_Config (int ignore_static_svcs = 1,
                      size_t size = ACE_Service_Config::MAX_SERVICES,
                      int signum = SIGHUP);

  /**
   * Performs an open without parsing command-line arguments.  The
   * @a logger_key indicates where to write the logging output, which
   * is typically either a STREAM pipe or a socket address.
   */
  ACE_Service_Config (const ACE_TCHAR program_name[],
                      const ACE_TCHAR *logger_key = ACE_DEFAULT_LOGGER_KEY);

  /**
   * Performs an open without parsing command-line arguments.  The
   * <logger_key> indicates where to write the logging output, which
   * is typically either a STREAM pipe or a socket address.  If
   * <ignore_default_svc_conf_file> is non-0 then the "svc.conf" file
   * will be ignored.  If <ignore_debug_flag> is non-0 then the
   * application is responsible for setting the
   * <ACE_Log_Msg::priority_mask> appropriately.  Returns number of
   * errors that occurred on failure and 0 otherwise.
   */
  static int open_i (const ACE_TCHAR program_name[],
                     const ACE_TCHAR *logger_key = ACE_DEFAULT_LOGGER_KEY,
                     int ignore_default_svc_conf_file = 0,
                     int ignore_debug_flag = 0);

  /**
   * Performs an open without parsing command-line arguments.  The
   * <logger_key> indicates where to write the logging output, which
   * is typically either a STREAM pipe or a socket address.  If
   * <ignore_static_svcs> is 1 then static services are not loaded,
   * otherwise, they are loaded.  If <ignore_default_svc_conf_file> is
   * non-0 then the <svc.conf> configuration file will be ignored.
   * Returns zero upon success, -1 if the file is not found or cannot
   * be opened (errno is set accordingly), otherwise returns the
   * number of errors encountered loading the services in the
   * specified svc.conf configuration file.  If <ignore_debug_flag> is
   * non-0 then the application is responsible for setting the
   * <ACE_Log_Msg::priority_mask> appropriately.
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
   * @retval -1   the configuration file is not found or cannot
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

  /// Perform user-specified close activities and remove dynamic
  /// memory.
  virtual ~ACE_Service_Config (void);

  /// Tidy up and perform last rites when ACE_Service_Config is shut
  /// down.  This method calls <close_svcs>.  Returns 0.
  static int close (void);

  /// Perform user-specified close hooks and possibly delete all of the
  /// configured services in the <Service_Repository>.
  static int fini_svcs (void);

  /**
   * Perform user-specified close hooks on all of the configured
   * services in the <Service_Repository>, then delete the
   * <Service_Repository> itself.  Returns 0.
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

  // = Accessors and mutators for process-wide Singletons.

  /// Returns a pointer to the list of statically linked services.
  static ACE_STATIC_SVCS *static_svcs (void);

  // = Utility methods.
  /// Dynamically link the shared object file and retrieve a pointer to
  /// the designated shared object in this file.
  static int initialize (const ACE_Service_Type *,
                         const ACE_TCHAR *parameters);

  /// Initialize and activate a statically <svc_name> service.
  static int initialize (const ACE_TCHAR *svc_name,
                         const ACE_TCHAR *parameters);

  /// Resume a <svc_name> that was previously suspended or has not yet
  /// been resumed (e.g., a static service).
  static int resume (const ACE_TCHAR svc_name[]);

  /**
   * Suspend <svc_name>.  Note that this will not unlink the service
   * from the daemon if it was dynamically linked, it will mark it as
   * being suspended in the Service Repository and call the <suspend>
   * member function on the appropriate <ACE_Service_Object>.  A
   * service can be resumed later on by calling the <RESUME> member
   * function...
   */
  static int suspend (const ACE_TCHAR svc_name[]);

  /// Totally remove <svc_name> from the daemon by removing it
  /// from the ACE_Reactor, and unlinking it if necessary.
  static int remove (const ACE_TCHAR svc_name[]);

#if defined (ACE_HAS_WINCE)
  // We must provide these function to bridge the Svc_Conf parser
  // with ACE.
  static int initialize (const ACE_Service_Type *, char parameters[]);
  static int initialize (const char svc_name[], char parameters[]);
  static int resume (const char svc_name[]);
  static int suspend (const char svc_name[]);
  static int remove (const char svc_name[]);
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

  /// Process one service configuration <directive>, which is passed as
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
   * <ACE_Service_Config>.  Note that <argv[0]> is assumed to be the
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

  static ACE_Service_Type_Impl *create_service_type_impl (const ACE_TCHAR *name,
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
#else
  /// Helper function to dynamically link in the XML Service Configurator parser.
  static ACE_XML_Svc_Conf *get_xml_svc_conf (ACE_DLL &d);
#endif /* ACE_USES_CLASSIC_SVC_CONF == 1 */

  /// Become a daemon.
  static int start_daemon (void);

  /// Add the default statically-linked services to the
  /// <ACE_Service_Repository>.
  static int load_static_svcs (void);

private:
  /// Indicates where to write the logging output.  This is typically
  /// either a STREAM pipe or a socket address.
  static const ACE_TCHAR *logger_key_;

  /// Singleton repository of statically linked services.
  static ACE_STATIC_SVCS *static_svcs_;

  /// Queue of services specified on the command-line.
  static ACE_SVC_QUEUE *svc_queue_;

  /// Queue of svc.conf files specified on the command-line.
  /// @@ This should probably be made to handle unicode filenames...
  static ACE_SVC_QUEUE *svc_conf_file_queue_;

  /// Initialize the <svc_conf_file_queue_> if necessary.
  static int init_svc_conf_file_queue (void);

  /// True if reconfiguration occurred.
  static sig_atomic_t reconfig_occurred_;

  // = Set by command-line options.
  /// Shall we become a daemon process?
  static int be_a_daemon_;

  /// Should we avoid loading the static services?
  static int no_static_svcs_;

  /// Number of the signal used to trigger reconfiguration.
  static int signum_;

  /// Handles the reconfiguration signals.
  static ACE_Sig_Adapter *signal_handler_;

  /**
   * Keep track of whether the <ACE_Service_Config> is already
   * initialized.  If so, we can't allow <yyparse> to be called since
   * it's not reentrant.  This variable is incremented by the
   * <ACE_Service_Config::open> method and decremented by the
   * <ACE_Service_Config::close> method.
   */
  static int is_initialized_;
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Config.i"
#endif /* __ACE_INLINE__ */

// These must go here to avoid circular includes...  (only left here
// for to not break applications which rely on this - no real need any
// longer)
#include "ace/Reactor.h"
#include "ace/Svc_Conf_Tokens.h"

#include /**/ "ace/post.h"

#endif /* ACE_SERVICE_CONFIG_H */
