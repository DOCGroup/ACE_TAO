/* -*- C++ -*- */
// $Id$

/*-*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ACE
// 
// = FILENAME
//    ACE_Name_Space
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================

#if !defined (ACE_NAME_SPACE_H)
#define ACE_NAME_SPACE_H

#include "ace/ACE.h"
#include "ace/SString.h"
#include "ace/Containers.h"
#include "ace/Name_Proxy.h"

typedef ACE_Unbounded_Set<ACE_WString> ACE_WSTRING_SET;

class ACE_Export ACE_Name_Binding
{
public:
  // = Initialization and termination.
  ACE_Name_Binding (const ACE_WString &n,
		    const ACE_WString &v,
		    const char *t);
  // Main constructor that initializes all the fields.

  ACE_Name_Binding (void); 
  // Default constructor.

  ACE_Name_Binding (const ACE_Name_Binding &);
  // Copy constructor.

  void operator= (const ACE_Name_Binding &);
  // Assignment operator.

  ~ACE_Name_Binding (void);
  // Destructor.

  int operator == (const ACE_Name_Binding &s) const;
  // Test for equality.

  ACE_WString name_;
  // Name of the binding.
  
  ACE_WString value_;
  // Value of the binding.

  char *type_;
  // Type of the binding.
};

typedef ACE_Unbounded_Set<ACE_Name_Binding> ACE_BINDING_SET;
typedef ACE_Unbounded_Set_Iterator<ACE_Name_Binding> ACE_BINDING_ITERATOR;

typedef ACE_Unbounded_Set<ACE_WString> ACE_PWSTRING_SET;
typedef ACE_Unbounded_Set_Iterator<ACE_WString> ACE_PWSTRING_ITERATOR;

class ACE_Export ACE_Name_Space
  // = TITLE
  //     Abstract base class that provides an abstract interface to
  //	 the database without exposing any implemenation details.
  //
  // = DESCRIPTION
  //     Manages a Naming Service Name Space. Provides the basic
  //	 methods -- bind, unbind, rebind, find, and listnames.
{
public:

  virtual ~ACE_Name_Space (void);
  // virtual destructor to ensure destructors of subclasses get
  // called.  
  
  virtual int bind (const ACE_WString &name_in, 
		    const ACE_WString &value_in, 
		    const char *type_in = "") = 0;
  // Bind a new name to a naming context (Wide character strings).


  virtual int rebind (const ACE_WString &name_in, 
		      const ACE_WString &value_in, 
		      const char *type_in = "") = 0;
  // Overwrite the value or type of an existing name in a
  // ACE_Name_Space or bind a new name to the context, if it didn't
  // exist yet. (Wide charcter strings interface).

  virtual int unbind (const ACE_WString &name_in) = 0;
  // Delete a name from a ACE_Name_Space (Wide charcter strings
  // Interface).

  virtual int resolve (const ACE_WString &name_in, 
		       ACE_WString &value_out, 
		       char *&type_out) = 0;
  // Get value and type of a given name binding (Wide chars).  The
  // caller is responsible for deleting both <value_out> and <type_out>!
  
  virtual int list_names (ACE_WSTRING_SET &set_out, 
			  const ACE_WString &pattern_in) = 0;
  // Get a set of names matching a specified pattern (wchars). Matching
  // means the names must begin with the pattern string.

  virtual int list_values (ACE_WSTRING_SET &set_out, 
			   const ACE_WString &pattern_in) = 0;
  // Get a set of values matching a specified pattern (wchars). Matching
  // means the values must begin with the pattern string.

  virtual int list_types (ACE_WSTRING_SET &set_out, 
			  const ACE_WString &pattern_in) = 0;
  // Get a set of types matching a specified pattern (wchars). Matching
  // means the types must begin with the pattern string.

  virtual int list_name_entries (ACE_BINDING_SET &set, 
				 const ACE_WString &pattern) = 0;
  // Get a set of names matching a specified pattern (wchars). Matching
  // means the names must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  virtual int list_value_entries (ACE_BINDING_SET &set, 
				  const ACE_WString &pattern) = 0;
  // Get a set of values matching a specified pattern (wchars). Matching
  // means the values must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  virtual int list_type_entries (ACE_BINDING_SET &set, 
				 const ACE_WString &pattern) = 0;
  // Get a set of types matching a specified pattern (wchars). Matching
  // means the types must begin with the pattern string. Returns the
  // complete binding associated each pattern match.

  virtual void dump (void) const = 0;
  // Dump the state of the object
};

#endif /* ACE_NAME_SPACE_H */

