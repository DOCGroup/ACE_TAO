/* -*- C++ -*- */
// $Id$

/*-*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ACE
// 
// = FILENAME
//    ACE_Remote_Name_Space
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================

#if !defined (ACE_REMOTE_NAME_SPACE_H)
#define ACE_REMOTE_NAME_SPACE_H

#include "ace/ACE.h"
#include "ace/SString.h"
#include "ace/Set.h"
#include "ace/Name_Proxy.h"
#include "ace/Name_Space.h"

typedef ACE_Unbounded_Set<ACE_WString> ACE_WSTRING_SET;

class ACE_Export ACE_Remote_Name_Space : public ACE_Name_Space
  // = TITLE
  //     Maintaining accesses Remote Name Server Database.  Allows to 
  //     add NameBindings, change them, remove them and resolve
  //     NameBindings.
  //
  // = DESCRIPTION
  //     Manages a Naming Service for a remote name space which includes
  //	 bindings for net_local naming context.
  //	 All strings are stored in wide character format.
  //     A Name Binding consists of a name (that's the key), a value 
  //	 string and an optional type string (no wide chars).
{
public:
  // = Initialization and termination methods.
  ACE_Remote_Name_Space (void);
  // "Do-nothing" constructor.

  ACE_Remote_Name_Space (const char *hostname, int port);
  // Specifies the scope of this namespace, opens and memory-maps the
  // associated file (if accessible) or contacts the dedicated name
  // server process for NET_LOCAL namespace.

  int open (const char *servername, int port);
  // Specifies the scope of this namespace, opens and memory-maps the
  // associated file (if accessible) or contacts the dedicated name
  // server process for NET_LOCAL namespace.

  ~ACE_Remote_Name_Space (void);
  // destructor, do some cleanup :TBD: last dtor should "compress"
  // file

  virtual int bind (const ACE_WString &name_in, 
		    const ACE_WString &value_in, 
		    const char *type_in = "");
  // Bind a new name to a naming context (Wide character strings).
  
  virtual int rebind (const ACE_WString &name_in, 
		      const ACE_WString &value_in, 
		      const char *type_in = "");
  // Overwrite the value or type of an existing name in a
  // ACE_Remote_Name_Space or bind a new name to the context, if it
  // didn't exist yet. (Wide charcter strings interface).
  
  virtual int unbind (const ACE_WString &name_in);
  // Delete a name from a ACE_Remote_Name_Space (Wide charcter strings
  // Interface).
  
  virtual int resolve (const ACE_WString &name_in, 
		       ACE_WString &value_out, 
		       char *&type_out);
  // Get value and type of a given name binding (Wide chars).  The
  // caller is responsible for deleting both <value_out> and <type_out>!
  
  virtual int list_names (ACE_WSTRING_SET &set_out, 
			  const ACE_WString &pattern_in);
  // Get a set of names matching a specified pattern (wchars). Matching
  // means the names must begin with the pattern string.

  virtual int list_values (ACE_WSTRING_SET &set_out, 
			   const ACE_WString &pattern_in);
  // Get a set of values matching a specified pattern (wchars). Matching
  // means the values must begin with the pattern string.

  virtual int list_types (ACE_WSTRING_SET &set_out, 
			  const ACE_WString &pattern_in);
  // Get a set of types matching a specified pattern (wchars). Matching
  // means the types must begin with the pattern string.

  virtual int list_name_entries (ACE_BINDING_SET &set, 
				 const ACE_WString &pattern);
  // Get a set of names matching a specified pattern (wchars). Matching
  // means the names must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  virtual int list_value_entries (ACE_BINDING_SET &set, 
				  const ACE_WString &pattern);
  // Get a set of values matching a specified pattern (wchars). Matching
  // means the values must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  virtual int list_type_entries (ACE_BINDING_SET &set, 
				 const ACE_WString &pattern);
  // Get a set of types matching a specified pattern (wchars). Matching
  // means the types must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  virtual void dump (void) const;
  // Dump the state of the object.
  
private:

  ACE_Name_Proxy ns_proxy_;
  // Interface to Name server process for NET_LOCAL namespace.
};

#endif /* ACE_REMOTE_NAME_SPACE_H */
