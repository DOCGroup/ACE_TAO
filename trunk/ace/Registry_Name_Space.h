/*-*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ACE
// 
// = FILENAME
//    ACE_Registry_Name_Space
//
// = AUTHOR
//    Irfan Pyarali (irfan@cs.wustl.edu)
//
// ============================================================================

#if !defined (ACE_REGISTRY_NAME_SPACE_H)
#define ACE_REGISTRY_NAME_SPACE_H

#include "ace/Registry.h"

#if (defined (ACE_WIN32) && defined (UNICODE))
// This only works on Win32 platforms when UNICODE is turned on

#include "ace/Naming_Context.h"
#include "ace/Name_Space.h"

class ACE_Export ACE_Registry_Name_Space : public ACE_Name_Space
  // = TITLE
  //     Interface to a Name Server Database which is maintained
  //     by the Win32 Registry.  Allows to add, change, remove and
  //     resolve NameBindings. 
  //
  // = DESCRIPTION
  //     Manages a Naming Service for a registry name space which includes
  //	 bindings for all contexts. All strings are stored in wide character format.
  //     A Name Binding consists of a name (that's the key), a value
  //     string. There is no type string support in this Name Space. 
{
public:

  ACE_Registry_Name_Space (void);
  // Constructor

  ACE_Registry_Name_Space (ACE_Name_Options *name_options);
  // Contacts and opens the registry on the specified server 

  ~ACE_Registry_Name_Space (void);
  // Destructor

  int open (ACE_Name_Options *name_options);
  // Contacts and opens the registry on the specified server 

  int bind (const ACE_WString &name_in, 
	    const ACE_WString &value_in, 
	    const char *type_in = "");
  // Bind a new name to a naming context (Wide character strings).

  int rebind (const ACE_WString &name_in, 
	      const ACE_WString &value_in, 
	      const char *type_in = "");
  // Overwrite the value or type of an existing name in a
  // ACE_Name_Space or bind a new name to the context, if it didn't
  // exist yet. (Wide charcter strings interface).

  int unbind (const ACE_WString &name_in);
  // Delete a name from a ACE_Name_Space (Wide charcter strings
  // Interface).

  int resolve (const ACE_WString &name_in, 
	       ACE_WString &value_out, 
	       char *&type_out);
  // Get value and type of a given name binding (Wide chars).  The
  // caller is responsible for deleting both <value_out> and <type_out>!
  
  int list_names (ACE_WSTRING_SET &set_out, 
		  const ACE_WString &pattern_in);
  // Get a set of names matching a specified pattern (wchars). Matching
  // means the names must begin with the pattern string.

  int list_values (ACE_WSTRING_SET &set_out, 
		   const ACE_WString &pattern_in);
  // Get a set of values matching a specified pattern (wchars). Matching
  // means the values must begin with the pattern string.

  int list_types (ACE_WSTRING_SET &set_out, 
		  const ACE_WString &pattern_in);
  // Get a set of types matching a specified pattern (wchars). Matching
  // means the types must begin with the pattern string.

  int list_name_entries (ACE_BINDING_SET &set, 
			 const ACE_WString &pattern);
  // Get a set of names matching a specified pattern (wchars). Matching
  // means the names must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  int list_value_entries (ACE_BINDING_SET &set, 
			  const ACE_WString &pattern);
  // Get a set of values matching a specified pattern (wchars). Matching
  // means the values must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  int list_type_entries (ACE_BINDING_SET &set, 
			 const ACE_WString &pattern);
  // Get a set of types matching a specified pattern (wchars). Matching
  // means the types must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  void dump (void) const;
  // Dump the state of the object

private:

  ACE_Registry::Naming_Context context_;
  // current context
};

#endif /* ACE_WIN32 && UNICODE */
#endif /* ACE_REGISTRY_NAME_SPACE_H */
