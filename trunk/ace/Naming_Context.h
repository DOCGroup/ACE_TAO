/* -*- C++ -*- */
// $Id$

/*-*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ACE
// 
// = FILENAME
//    ACE_Naming_Context.h
//
// = AUTHOR
//    Gerhard Lenzer, Douglas C. Schmidt, and Prashant Jain
// 
// ============================================================================

#if !defined (ACE_NAMING_CONTEXT_H)
#define ACE_NAMING_CONTEXT_H

#include "ace/ACE.h"
#include "ace/SString.h"
#include "ace/Set.h"
#include "ace/Service_Object.h"
#include "ace/Name_Proxy.h"
#include "ace/Name_Space.h"

// Forward decl
class ACE_Name_Options;

class ACE_Export ACE_Naming_Context : public ACE_Service_Object
  // = TITLE
  //     Maintaining accesses Name Server Databases.  Allows to add
  //     NameBindings, change them, remove them and resolve
  //     NameBindings
  //
  // = DESCRIPTION
  //     Manages a Naming Service .  That represents a persistent
  //     string to string mapping for different scopes. The scope of a
  //     ACE_Naming_Context may be either local for the calling
  //     process (Note : A process is hereby not identified by it's
  //     pid, but by it's argv[0]. So different processes (in UNIX
  //     syntax) may access the same NameBindings), global for all
  //     processes running on one host or global for all processes on
  //     the net (that know the address of the net name server
  //     socket). Strings may be plain character strings or Wide
  //     character strings. A Name Binding consists of a name string
  //     (that's the key), a value string and an optional type string
  //     (no wide chars).
{
public:
  enum Context_Scope_Type
    {
      PROC_LOCAL, // Name lookup is local to the process.
      NODE_LOCAL, // Name lookup is local to the node (host).
      NET_LOCAL   // Name lookup is local to the (sub)network.
    };

  // = Initialization and termination methods.
  ACE_Naming_Context (void);
  // "Do-nothing" constructor.

  ACE_Naming_Context (Context_Scope_Type scope_in, int light = 0);
  // Specifies the scope of this namespace, opens and memory-maps the
  // associated file (if accessible) or contacts the dedicated name
  // server process for NET_LOCAL namespace. Note that <light>
  // specifies whether or not we want to use
  // ACE_Lite_MMap_Memory_Pool. By default we use ACE_MMap_Memory_Pool.

  int open (Context_Scope_Type scope_in = ACE_Naming_Context::PROC_LOCAL,
	    int light = 0);
  // Specifies the scope of this namespace, opens and memory-maps the
  // associated file (if accessible) or contacts the dedicated name
  // server process for NET_LOCAL namespace. Note that <light>
  // specifies whether or not we want to use
  // ACE_Lite_MMap_Memory_Pool. By default we use ACE_MMap_Memory_Pool.

  int close (void);
  // Deletes the instance of Name Space. Must be called before
  // switching name spaces.

  ~ACE_Naming_Context (void);
  // destructor, do some cleanup :TBD: last dtor should "compress"
  // file

  // = Dynamic initialization hooks.
  virtual int init (int argc, char *argv[]);
  // Initialize name options and naming context when dynamically
  // linked.

  virtual int fini (void);
  // Close down the test when dynamically unlinked.

  virtual int info (char **strp, size_t length) const;
  // Returns information about this context.

  ACE_Name_Options *name_options (void);
  // Returns the ACE_Name_Options associated with the Naming_Context

  int bind (const ACE_WString &name_in, 
	    const ACE_WString &value_in, 
	    const char *type_in = "");
  // Bind a new name to a naming context (Wide character strings).

  int bind (const char *name_in, 
	    const char *value_in, 
	    const char *type_in = "");
  // Bind a new name to a naming context ( character strings).

  int rebind (const ACE_WString &name_in, 
	      const ACE_WString &value_in, 
	      const char *type_in = "");
  // Overwrite the value or type of an existing name in a
  // ACE_Naming_Context or bind a new name to the context, if it
  // didn't exist yet. (Wide charcter strings interface).

  int rebind (const char *name_in, 
	      const char *value_in, 
	      const char *type_in = "");
  // Overwrite the value or type of an existing name in a
  // ACE_Naming_Context or bind a new name to the context, if it
  // didn't exist yet. ( charcter strings interface)

  int unbind (const ACE_WString &name_in);
  // Delete a name from a ACE_Naming_Context (Wide charcter strings
  // Interface).

  int unbind (const char *name_in);
  // Delete a name from a ACE_Naming_Context (character strings
  // interface).

  int resolve (const ACE_WString &name_in, 
	       ACE_WString &value_out, 
	       char *&type_out);
  // Get value and type of a given name binding (Wide chars).  The
  // caller is responsible for deleting both <value_out> and <type_out>!

  int resolve (const char *name_in, 
	       ACE_WString &value_out, 
	       char *&type_out);
  // Get value and type of a given name binding (Wide chars output).
  // The caller is responsible for deleting both <value_out> and
  // <type_out>!

  int resolve (const char *name_in, 
	       char *&value_out, 
	       char *&type_out);
  // Get value and type of a given name binding ( chars ).  The caller
  // is responsible for deleting both <value_out> and <type_out>!

  int list_names (ACE_PWSTRING_SET &set_out, 
		  const ACE_WString &pattern_in);
  // Get a set of names matching a specified pattern (wchars). Matching
  // means the names must begin with the pattern string.

  int list_names (ACE_PWSTRING_SET &set_out, 
		  const char *pattern_in);
  // Get a set of names matching a specified pattern (chars). Matching
  // means the names must begin with the pattern string.

  int list_values (ACE_PWSTRING_SET &set_out, 
		   const ACE_WString &pattern_in);
  // Get a set of values matching a specified pattern (wchars). Matching
  // means the values must begin with the pattern string.

  int list_values (ACE_PWSTRING_SET &set_out, 
		   const char *pattern_in);
  // Get a set of values matching a specified pattern (chars). Matching
  // means the values must begin with the pattern string.

  int list_types (ACE_PWSTRING_SET &set_out, 
		  const ACE_WString &pattern_in);
  // Get a set of types matching a specified pattern (wchars). Matching
  // means the types must begin with the pattern string.

  int list_types (ACE_PWSTRING_SET &set_out, 
		  const char *pattern_in);
  // Get a set of types matching a specified pattern (chars). Matching
  // means the types must begin with the pattern string.

  virtual int list_name_entries (ACE_BINDING_SET &set_out, 
				 const ACE_WString &pattern_in);
  // Get a set of names matching a specified pattern (wchars). Matching
  // means the names must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  virtual int list_name_entries (ACE_BINDING_SET &set_out, 
				 const char *pattern_in);
  // Get a set of names matching a specified pattern (wchars). Matching
  // means the names must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  virtual int list_value_entries (ACE_BINDING_SET &set_out, 
				  const ACE_WString &pattern_in);
  // Get a set of values matching a specified pattern (wchars). Matching
  // means the values must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  virtual int list_value_entries (ACE_BINDING_SET &set_out, 
				  const char *pattern_in);
  // Get a set of values matching a specified pattern (wchars). Matching
  // means the values must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  virtual int list_type_entries (ACE_BINDING_SET &set_out, 
				 const ACE_WString &pattern_in);
  // Get a set of types matching a specified pattern (wchars). Matching
  // means the types must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  virtual int list_type_entries (ACE_BINDING_SET &set_out, 
				 const char *pattern_in);
  // Get a set of types matching a specified pattern (wchars). Matching
  // means the types must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  void dump ();
  // Dump the state of the object.

private:
  ACE_Name_Options *name_options_;
  // Keep track of the options such as database name etc per Naming Context

  ACE_Name_Space *name_space_;
  // Name space (can be either local or remote) dynamically bound.

  char hostname_[MAXHOSTNAMELEN + 1];   
  // Holds the local hostname.

  const char *netnameserver_host_;
  // Holds name of net name server.

  int netnameserver_port_;
  // Holds port number of the net name server.

  int local (void);
  // 1 if we're on the same local machine as the name server, else 0.

};

class ACE_Export ACE_Name_Options
  // = TITLE
  //     Manages the options for the ACE Name_Server.
{
public:
  // = Initialization and termination methods.
  ACE_Name_Options (void);
  ~ACE_Name_Options (void);

  void parse_args (int argc, char *argv[]);
  // Parse arguments

  // = Set/Get port number
  void nameserver_port (int port);
  int nameserver_port (void);

  // = Set/Get the context
  ACE_Naming_Context::Context_Scope_Type context (void);
  void context (ACE_Naming_Context::Context_Scope_Type);

  // = Set/Get host name
  void nameserver_host (const char *host);
  const char *nameserver_host (void);

  // = Set/Get name space directory
  void namespace_dir (const char *dir);
  const char *namespace_dir (void);

  // = Set/Get process name
  void process_name (const char *dir);
  const char *process_name (void);

  // = Set/Get database name
  void database (const char *db);
  const char *database (void);

  int debug (void);
  // Return debug status

  int verbose (void);
  // Return verbose status

private:
  int debugging_;		
  // Extra debugging info

  int verbosity_;		
  // Extra verbose messages

  int nameserver_port_;	
  // Port to connect to nameserver process.

  const char *nameserver_host_;	
  // Hostname of nameserver.

  const char *namespace_dir_;	
  // Directory to hold name_bindings.

  const char *process_name_;         
  // Name of this process.

  const char *database_;
  // Name of the database that stores the name/value/type bindings.

  ACE_Naming_Context::Context_Scope_Type context_;
  // The context in which the naming database will be created.
};

extern "C" ACE_Export ACE_Service_Object *_make_ACE_Naming_Context (void);

#endif /* ACE_NAMING_CONTEXT_H */
