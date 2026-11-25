/**
 *  @file    Naming_Context.h
 *
 *  @author Gerhard Lenzer
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Prashant Jain <pjain@uci.edu>
 */

#ifndef ACE_NAMING_CONTEXT_H
#define ACE_NAMING_CONTEXT_H
#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/Name_Space.h"
#include "ace/os_include/os_netdb.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decl
class ACE_Name_Options;
class ACE_Static_Svc_Descriptor;

/**
 * @class ACE_Naming_Context
 *
 * @brief Maintaining accesses Name Server Databases.  Allows to add
 * NameBindings, change them, remove them and resolve
 * NameBindings
 *
 * Manages a Naming Service that represents a persistent string-to-string
 * mapping for different scopes. The scope of an ACE_Naming_Context may be
 * local to the calling process (where process identity is based on argv[0],
 * not pid), global to all processes running on one host, or global to all
 * processes on the network (that know the address of the net name server
 * socket). Strings may be plain character strings or Wide
 * character strings. A Name Binding consists of a name string
 * (that's the key), a value string and an optional type string
 * (no wide chars).
 * @note On UNIX, different processes with the same argv[0] may access the
 * same NameBindings.
 */
class ACE_Export ACE_Naming_Context : public ACE_Service_Object
{
public:
  enum Context_Scope_Type
    {
      /// Name lookup is local to the process.
      PROC_LOCAL,
      /// Name lookup is local to the node (host).
      NODE_LOCAL,
      /// Name lookup is local to the (sub)network.
      NET_LOCAL
    };

  /// "Do-nothing" constructor.
  ACE_Naming_Context ();

  /**
   * Specifies the scope of this namespace, opens and memory-maps the
   * associated file (if accessible) or contacts the dedicated name
   * server process for NET_LOCAL namespace. Note that @a light
   * specifies whether or not we want to use
   * ACE_Lite_MMap_Memory_Pool. By default we use ACE_MMap_Memory_Pool.
   */
  ACE_Naming_Context (Context_Scope_Type scope_in, int light = 0);

  /**
   * Specifies the scope of this namespace, opens and memory-maps the
   * associated file (if accessible) or contacts the dedicated name
   * server process for NET_LOCAL namespace. Note that @a light
   * specifies whether or not we want to use
   * ACE_Lite_MMap_Memory_Pool. By default we use ACE_MMap_Memory_Pool.
   */
  int open (Context_Scope_Type scope_in = ACE_Naming_Context::PROC_LOCAL,
            int light = 0);

  /// Deletes the instance of Name Space. Must be called before
  /// switching name spaces.
  int close ();

  /// Release all resources. Gets called by destructor and fini.
  int close_down ();

  /// destructor, do some cleanup :TBD: last dtor should "compress"
  /// file
  ~ACE_Naming_Context () override;

  /// @name Dynamic initialization hooks
  /// @{
  /// Initialize name options and naming context when dynamically
  /// linked.
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Close down the test when dynamically unlinked.
  int fini () override;
  /// @}

  /// Returns information about this context.
  int info (ACE_TCHAR **strp, size_t length) const override;

  /// Returns the ACE_Name_Options associated with the Naming_Context
  ACE_Name_Options *name_options ();

  /// Bind a new name to a naming context (Wide character strings).
  int bind (ACE_NS_WString const &name_in,
            ACE_NS_WString const &value_in,
            char const *type_in = "");

  /// Bind a new name to a naming context ( character strings).
  int bind (char const *name_in,
            char const *value_in,
            char const *type_in = "");

  /**
   * Overwrite the value or type of an existing name in a
   * ACE_Naming_Context or bind a new name to the context, if it
   * didn't exist yet. (Wide character strings interface).
   */
  int rebind (ACE_NS_WString const &name_in,
              ACE_NS_WString const &value_in,
              char const *type_in = "");

  /**
   * Overwrite the value or type of an existing name in a
   * ACE_Naming_Context or bind a new name to the context, if it
   * didn't exist yet. ( character strings interface)
   */
  int rebind (char const *name_in,
              char const *value_in,
              char const *type_in = "");

  /// Delete a name from a ACE_Naming_Context (Wide character strings
  /// Interface).
  int unbind (ACE_NS_WString const &name_in);

  /// Delete a name from a ACE_Naming_Context (character strings
  /// interface).
  int unbind (char const *name_in);

  /// Get value and type of a given name binding (Wide chars).  The
  /// caller is responsible for deleting both @a value_out> and @a type_out!
  int resolve (ACE_NS_WString const &name_in,
               ACE_NS_WString &value_out,
               char *&type_out);

  /**
   * Get value and type of a given name binding (Wide chars output).
   * The caller is responsible for deleting both @a value_out and
   * @a type_out!
   */
  int resolve (char const *name_in,
               ACE_NS_WString &value_out,
               char *&type_out);

  /// Get value and type of a given name binding ( chars ).  The caller
  /// is responsible for deleting both @a value_out and @a type_out!
  int resolve (char const *name_in,
               char *&value_out,
               char *&type_out);

  /// Get a set of names matching a specified pattern (wchars). Matching
  /// means the names must begin with the pattern string.
  int list_names (ACE_PWSTRING_SET &set_out,
                  ACE_NS_WString const &pattern_in);

  /// Get a set of names matching a specified pattern (chars). Matching
  /// means the names must begin with the pattern string.
  int list_names (ACE_PWSTRING_SET &set_out,
                  char const *pattern_in);

  /// Get a set of values matching a specified pattern (wchars). Matching
  /// means the values must begin with the pattern string.
  int list_values (ACE_PWSTRING_SET &set_out,
                   ACE_NS_WString const &pattern_in);

  /// Get a set of values matching a specified pattern (chars). Matching
  /// means the values must begin with the pattern string.
  int list_values (ACE_PWSTRING_SET &set_out,
                   char const *pattern_in);

  /// Get a set of types matching a specified pattern (wchars). Matching
  /// means the types must begin with the pattern string.
  int list_types (ACE_PWSTRING_SET &set_out,
                  ACE_NS_WString const &pattern_in);

  /// Get a set of types matching a specified pattern (chars). Matching
  /// means the types must begin with the pattern string.
  int list_types (ACE_PWSTRING_SET &set_out,
                  char const *pattern_in);

  /**
   * Get a set of names matching a specified pattern (wchars). Matching
   * means the names must begin with the pattern string. Returns the
   * complete binding associated each pattern match.
   */
  virtual int list_name_entries (ACE_BINDING_SET &set_out,
                                 ACE_NS_WString const &pattern_in);

  /**
   * Get a set of names matching a specified pattern (wchars). Matching
   * means the names must begin with the pattern string. Returns the
   * complete binding associated each pattern match.
   */
  virtual int list_name_entries (ACE_BINDING_SET &set_out,
                                 char const *pattern_in);

  /**
   * Get a set of values matching a specified pattern (wchars). Matching
   * means the values must begin with the pattern string. Returns the
   * complete binding associated each pattern match.
   */
  virtual int list_value_entries (ACE_BINDING_SET &set_out,
                                  ACE_NS_WString const &pattern_in);

  /**
   * Get a set of values matching a specified pattern (wchars). Matching
   * means the values must begin with the pattern string. Returns the
   * complete binding associated each pattern match.
   */
  virtual int list_value_entries (ACE_BINDING_SET &set_out,
                                  char const *pattern_in);

  /**
   * Get a set of types matching a specified pattern (wchars). Matching
   * means the types must begin with the pattern string. Returns the
   * complete binding associated each pattern match.
   */
  virtual int list_type_entries (ACE_BINDING_SET &set_out,
                                 ACE_NS_WString const &pattern_in);

  /**
   * Get a set of types matching a specified pattern (wchars). Matching
   * means the types must begin with the pattern string. Returns the
   * complete binding associated each pattern match.
   */
  virtual int list_type_entries (ACE_BINDING_SET &set_out,
                                 char const *pattern_in);

  /// Dump the state of the object.
  void dump ();

private:
  /// Keep track of the options such as database name etc per Naming Context
  ACE_Name_Options *name_options_ {};

  /// Name space (can be either local or remote) dynamically bound.
  ACE_Name_Space *name_space_ {};

  /// Holds the local hostname.
  ACE_TCHAR hostname_[MAXHOSTNAMELEN + 1];

  /// Holds name of net name server.
  ACE_TCHAR const *netnameserver_host_ {};

  /// Holds port number of the net name server.
  int netnameserver_port_ {};

  /// 1 if we're on the same local machine as the name server, else 0.
  int local ();
};

/**
 * @class ACE_Name_Options
 *
 * @brief Manages the options for the ACE Name_Server.
 */
class ACE_Export ACE_Name_Options
{
public:
  ACE_Name_Options ();
  ~ACE_Name_Options ();

  /// Parse arguments.
  void parse_args (int argc,
                   ACE_TCHAR *argv[]);

  /// Set the port number
  void nameserver_port (int port);

  /// Get the port number
  int nameserver_port ();

  /// Get the context
  ACE_Naming_Context::Context_Scope_Type context ();

  /// Set the context
  void context (ACE_Naming_Context::Context_Scope_Type);

  /// Set the host name
  void nameserver_host (ACE_TCHAR const *host);

  /// Get the host name
  ACE_TCHAR const *nameserver_host ();

  /// Set name space directory
  void namespace_dir (ACE_TCHAR const *dir);

  /// Get name space directory
  ACE_TCHAR const *namespace_dir ();

  /// Set process name
  void process_name (ACE_TCHAR const *dir);

  /// Get process name
  ACE_TCHAR const *process_name ();

  /// Set database name
  void database (ACE_TCHAR const *);

  /// Get database name
  ACE_TCHAR const *database ();

  /// Set base address of the underlying allocator
  void base_address (char *address);

  /// Get base address of the underlying allocator
  char *base_address ();

  /// Get use of registry in naming
  bool use_registry () const;

  /// Set use of registry in naming
  void use_registry (bool x);

  /// Return debug status
  bool debug ();

  /// Return verbose status
  bool verbose ();

  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Extra debugging info
  bool debugging_ {false};

  /// Extra verbose messages
  bool verbosity_ {false};

  /// Use Win32 Registry
  bool use_registry_ {false};

  /// Port to connect to nameserver process.
  int nameserver_port_;

  /// Hostname of nameserver.
  ACE_TCHAR const *nameserver_host_;

  /// Directory to hold name_bindings.
  ACE_TCHAR *namespace_dir_;

  /// Name of this process.
  ACE_TCHAR const *process_name_ {};

  /// Name of the database that stores the name/value/type bindings.
  ACE_TCHAR const *database_;

  /// Base address of the underlying allocator
  char *base_address_;

  /// The context in which the naming database will be created.
  ACE_Naming_Context::Context_Scope_Type context_
    {ACE_Naming_Context::PROC_LOCAL};
};

ACE_FACTORY_DECLARE (ACE, ACE_Naming_Context)
ACE_STATIC_SVC_DECLARE_EXPORT (ACE, ACE_Naming_Context)

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Naming_Context.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_NAMING_CONTEXT_H */
