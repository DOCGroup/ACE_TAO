/* -*- C++ -*- */
// $Id$

/*-*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ACE
// 
// = FILENAME
//    ACE_Local_Name_Space_T.h
//
// = AUTHOR
//    Prashant Jain (pjain@cs.wustl.edu), Irfan Pyarali
//    (irfan@wuerl.wustl.edu), and Douglas C. Schmidt
//    (schmidt@cs.wustl.edu). 
// 
// ============================================================================

#if !defined (ACE_LOCAL_NAME_SPACE_T_H)
#define ACE_LOCAL_NAME_SPACE_T_H

#include "ace/Name_Space.h"
#include "ace/Naming_Context.h"
#include "ace/SString.h"

// A short-hand name for our set of name/value/type tuples passed back
// to callers.
typedef ACE_Unbounded_Set<ACE_WString> ACE_WSTRING_SET;

// Simplify later usage by defining typedefs.
#if (1)
#include "ace/Hash_Map_Manager.h"
typedef ACE_Hash_Map_Manager<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex> MAP_MANAGER;
typedef ACE_Hash_Map_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex> MAP_ITERATOR;
typedef ACE_Hash_Map_Entry <ACE_NS_String, ACE_NS_Internal> MAP_ENTRY;
#else
#include "ace/Map_Manager.h"
typedef ACE_Map_Manager<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex> MAP_MANAGER;
typedef ACE_Map_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex> MAP_ITERATOR;
typedef ACE_Map_Entry <ACE_NS_String, ACE_NS_Internal> MAP_ENTRY;
#endif /* 0 */

template <class ALLOCATOR>
class ACE_Name_Space_Map : public MAP_MANAGER
{
  // = TITLE
  //     This class serves as a Proxy that ensures our process always
  //     has the appropriate allocator in place for every operation
  //     that accesses or updates the Map Manager.
  //
  // = DESCRIPTION
  //     We need this class because otherwise the SHARED_MALLOC
  //     pointer will be stored in the Map_Manager that resides within
  //     shared memory.  Naturally, this will cause horrible problems
  //     since only the first process to set that pointer will be
  //     guaranteed the address of the SHARED_MALLOC allocator is
  //     meaningful!
public:
  ACE_Name_Space_Map (ALLOCATOR *allocator);
  // Constructor.

  // = The following methods are Proxies to the underlying methods
  // provided by <ACE_Hash_Map_Manager>.  When they are called, they
  // acquire the lock, set the allocator to the one specific to this
  // process, and then call down to perform the intended operation.
  int bind (const ACE_NS_String &, 
	    const ACE_NS_Internal &,
	    ALLOCATOR *allocator);

  int unbind (const ACE_NS_String &, 
	      ACE_NS_Internal &,
	      ALLOCATOR *allocator);

  int rebind (const ACE_NS_String &, 
	      const ACE_NS_Internal &,
	      ACE_NS_String &, 
	      ACE_NS_Internal &,
	      ALLOCATOR *allocator);

  int find (const ACE_NS_String &, 
	    ACE_NS_Internal &,
	    ALLOCATOR *allocator);

  int close (ALLOCATOR* allocator);
  
private:
};

template <ACE_MEM_POOL_1, class LOCK>
class ACE_Local_Name_Space : public ACE_Name_Space
  // = TITLE
  //     Maintaining accesses Local Name Server Database.  Allows to
  //     add NameBindings, change them, remove them and resolve
  //     NameBindings.
  //
  // = DESCRIPTION
  //     Manages a Naming Service for a local name space which includes
  //	 bindings for node_local and host_local naming contexts.
  //	 All strings are stored in wide character format.
  //     A Name Binding consists of a name (that's the key), a value 
  //	 string and an optional type string (no wide chars).
{
public:
  // = Initialization and termination methods.
  ACE_Local_Name_Space (void);
  // "Do-nothing" constructor.

  ACE_Local_Name_Space (ACE_Naming_Context::Context_Scope_Type scope_in,
			ACE_Name_Options *name_options);
  // Specifies the scope of this namespace, opens and memory-maps the
  // associated file (if accessible) or contacts the dedicated name
  // server process for NET_LOCAL namespace.

  int open (ACE_Naming_Context::Context_Scope_Type scope_in);
  // Specifies the scope of this namespace, opens and memory-maps the
  // associated file (if accessible) or contacts the dedicated name
  // server process for NET_LOCAL namespace.

  ~ACE_Local_Name_Space (void);
  // destructor, do some cleanup :TBD: last dtor should "compress"
  // file

  virtual int bind (const ACE_WString &name,
		    const ACE_WString &value,
		    const char *type = "");
  // Bind a new name to a naming context (Wide character strings).
  
  virtual int rebind (const ACE_WString &name, 
		      const ACE_WString &value,
		      const char *type = "");
  // Overwrite the value or type of an existing name in a
  // ACE_Local_Name_Space or bind a new name to the context, if it
  // didn't exist yet. (Wide charcter strings interface).
  
  virtual int unbind (const ACE_WString &name);
  virtual int unbind_i (const ACE_WString &name);
  // Delete a name from a ACE_Local_Name_Space (Wide charcter strings
  // Interface).
  
  virtual int resolve (const ACE_WString &name, 
		       ACE_WString &value, 
		       char *&type);
  virtual int resolve_i (const ACE_WString &name, 
			 ACE_WString &value, 
			 char *&type);
  // Get value and type of a given name binding (Wide chars).  The
  // caller is responsible for deleting <type>!
  
  virtual int list_names (ACE_WSTRING_SET &set, 
			  const ACE_WString &pattern);
  // Get a set of names matching a specified pattern (wchars). Matching
  // means the names must begin with the pattern string.

  virtual int list_values (ACE_WSTRING_SET &set, 
			   const ACE_WString &pattern);
  // Get a set of values matching a specified pattern (wchars). Matching
  // means the values must begin with the pattern string.

  virtual int list_types (ACE_WSTRING_SET &set, 
			  const ACE_WString &pattern);
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
  // Dump the state of the object

private:
#if defined (ACE_WIN32)
  int remap (EXCEPTION_POINTERS *ep);
  // Remap the backing store
#endif /* ACE_WIN32 */

  int shared_bind (const ACE_WString &name, const ACE_WString &value,
		   const char *type, int rebind);
  int shared_bind_i (const ACE_WString &name, const ACE_WString &value,
		     const char *type, int rebind);
  // Factor out code from bind() and rebind().

  int create_manager (void);
  int create_manager_i (void);
  // Allocate the appropriate type of map manager that stores the
  // key/value binding.

  // = I just know this is going to cause problems on some platform...
  typedef ACE_Allocator_Adapter <ACE_Malloc <ACE_MEM_POOL_2, LOCK> > ALLOCATOR;

  ALLOCATOR *allocator_;
  // Pointer to the allocator

  ACE_Name_Space_Map <ALLOCATOR> *name_space_map_;
  // Pointer to the allocated map manager.

  ACE_Naming_Context::Context_Scope_Type ns_scope_;
  // Scope of this naming context (e.g., PROC_LOCAL, NODE_LOCAL, or NET_LOCAL).

  ACE_Name_Options *name_options_;
  // Keep track of the options such as database name etc

  TCHAR context_file_[MAXNAMELEN];
  // Name of the file used as the backing store.

  LOCK *lock_;
  // Synchronization variable.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Local_Name_Space_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Local_Name_Space_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

// Put this here to avoid circular include problems.
#include "Local_Name_Space.h"

#endif /* ACE_LOCAL_NAME_SPACE_T_H */
