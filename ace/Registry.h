/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Registry.h
//
// = AUTHOR
//    Irfan Pyarali (irfan@cs.wustl.edu)
// 
// ============================================================================

#if !defined (ACE_REGISTRY_H)
#define ACE_REGISTRY_H

#include "ace/OS.h"

#if defined (ACE_WIN32)
// This only works on Win32 platforms

#include "vector.h"
#include "bstring.h"
// You must configure the STL components in order to use this wrapper.

class ACE_Export ACE_Registry
  // = TITLE
  //     A Name Server implementation
  // 
  // = DESCRIPTION
  //     The registry interface is inspired by the interface 
  //     specified in the CORBA Naming Service Specification.
  //     The implementation is done through Win32 Reg*() functions. 
  //     Other than providing an OO wrapper for the Win32 Reg*()
  //     functions, ACE_Registry provides an abstraction for iteration
  //     over the elements of the Registry.
{
public:
  
  typedef std::basic_string<TCHAR> Istring;
  // International string

  struct ACE_Export Name_Component 
  {
    Istring id_;
    Istring kind_;
  };
  // The <id_> field is used,
  // but the <kind_> field is currently ignored

  typedef std::vector<Name_Component> Name;
  // A Name is an ordered collections of components (ids)

  static LPCTSTR STRING_SEPARATOR;
  // Separator for components in a name 

  static Istring make_string (const Name &name);
  // Convert a <name> to a <string>
    
  static Name make_name (const Istring &string);
  // Convert a <string> to a <name>

  enum Binding_Type {INVALID, OBJECT, CONTEXT};
  // There are two types of bindings

  struct ACE_Export Binding 
  {
    Binding ();
    // Empty (default) constructor

    Binding (const Name &binding_name,
             Binding_Type binding_type);
    // Constructor 
    // (Name version)

    Binding (const Istring &binding_name,
             Binding_Type binding_type);
    // Constructor 
    // (String version)
    
    void name (Name &name);
    // Name accessor
    // (Name version)

    void name (Istring &name);
    Istring name (void);
    // Name accessors
    // (String version)

    Binding_Type type (void);      
    // Type accessor

  private:
    Istring name_;
    Binding_Type type_;  
    // A binding has a name and a type
  };
  
  typedef std::vector<Binding> Binding_List;
  // A list of bindings

  class Binding_Iterator;
  // Forward declaration of iterator

  class ACE_Export Object
    // = TITLE
    //     An object representation
    // = DESCRIPTION
    //     In CORBA, all objects inherit from (CORBA::Object).
    //     For the registry, this is used as a wrapper for an 
    //     instance of a built-in data type.  
    //     Think about an object as being similar to a file 
    //     in a file system.
    {
    public:   
      Object (void *data = 0,
	      u_long size = 0, 
	      u_long type = REG_NONE);
      // Default constructor 

      void data (void *data);
      void *data (void) const;
      // Set/Get data
    
      void size (u_long size);
      u_long size (void) const;
      // Set/Get size

      void type (u_long type);      
      u_long type (void) const;
      // Set/Get type

    private:
      void *data_;
      // Pointer to data

      u_long size_;
      // Size of the data

      u_long type_;
      // Type of data
    };

  class ACE_Export Naming_Context 
    // = TITLE
    //     An context representation
    // = DESCRIPTION
    //     Think about a context as being similar to a directory
    //     in a file system.
    {
    friend class ACE_Predefined_Naming_Contexts;
    // Friend factory

  public:

    enum { MAX_OBJECT_NAME_SIZE = BUFSIZ,
	   MAX_CONTEXT_NAME_SIZE = MAXPATHLEN + 1 };
    // Max sizes of names
    // (Not too sure about this value)
    
    Naming_Context (void);
    // Empty constructor: keys will be NULL

    Naming_Context (const HKEY &key);
    // Constructor: key_ will be set to <key>
    
    ~Naming_Context (void);
    // Destructor will call this->close()


    // The following interfaces are for objects

    int bind_new (const Name &name, 
		  const Object &object);
    // Insert <object> with <name> into <this> context
    // This will fail if <name> already exists
    // (Name version)

    int bind_new (const Istring &name, 
		  const Object &object);
    // Insert <object> with <name> into <this> context
    // This will fail if <name> already exists
    // (String version)

    int bind (const Name &name, 
	      const Object &object);
    // Insert or update <object> with <name> into <this> context
    // This will not fail if <name> already exists
    // (Name version)

    int bind (const Istring &name, 
	      const Object &object);
    // Insert or update <object> with <name> into <this> context
    // This will not fail if <name> already exists
    // (String version)

    int rebind (const Name &name, 
		const Object &object);
    // Update <object> with <name> in <this> context
    // (Name version)
    
    int rebind (const Istring &name, 
		const Object &object);
    // Update <object> with <name> in <this> context
    
    int resolve (const Name &name, 
		 Object &object);
    // Find <object> with <name> in <this> context
    // (Name version)
    
    int resolve (const Istring &name, 
		 Object &object);
    // Find <object> with <name> in <this> context
    
    int unbind (const Name &name);
    // Delete object with <name> in <this> context
    // (Name version)
    
    int unbind (const Istring &name);
    // Delete object with <name> in <this> context
    

    // The following interfaces are for Naming Context
    
    int new_context (Naming_Context &naming_context);
    // Create new <naming_context> 

    int bind_new_context (const Name &name,
			  Naming_Context &naming_context,
			  u_long persistence = REG_OPTION_NON_VOLATILE,
			  u_long security_access = KEY_ALL_ACCESS,
			  LPSECURITY_ATTRIBUTES security_attributes = 0);
    // Insert <naming_context> with <name> relative to <this> context
    // This will fail if <name> already exists
    // (Name version)
    
    int bind_new_context (const Istring &name,
			  Naming_Context &naming_context,
			  u_long persistence = REG_OPTION_NON_VOLATILE,
			  u_long security_access = KEY_ALL_ACCESS,
			  LPSECURITY_ATTRIBUTES security_attributes = 0);
    // Insert <naming_context> with <name> relative to <this> context
    // This will fail if <name> already exists
    
    int bind_context (const Name &name, 
		      /* const */ Naming_Context &naming_context,
		      u_long persistence = REG_OPTION_NON_VOLATILE,
		      u_long security_access = KEY_ALL_ACCESS,
		      LPSECURITY_ATTRIBUTES security_attributes = 0);
    // Insert or update <naming_context> with <name> relative to <this> context
    // This will not fail if <name> already exists
    // (Name version)
    
    int bind_context (const Istring &name, 
		      /* const */ Naming_Context &naming_context,
		      u_long persistence = REG_OPTION_NON_VOLATILE,
		      u_long security_access = KEY_ALL_ACCESS,
		      LPSECURITY_ATTRIBUTES security_attributes = 0);
    // Insert or update <naming_context> with <name> relative to <this> context
    // This will not fail if <name> already exists
    
    int rebind_context (const Name &name, 
			/* const */ Naming_Context &naming_context);
    // Rename <naming_context> to <name> 
    // (Name version)
    
    int rebind_context (const Istring &name, 
			/* const */ Naming_Context &naming_context);
    // Rename <naming_context> to <name> 
    
    int resolve_context (const Name &name, 
			 Naming_Context &naming_context,
			 u_long security_access = KEY_ALL_ACCESS);
    // Find <naming_context> with <name> in <this> context
    // (Name version)
    
    int resolve_context (const Istring &name, 
			 Naming_Context &naming_context,
			 u_long security_access = KEY_ALL_ACCESS);
    // Find <naming_context> with <name> in <this> context
    
    int unbind_context (const Name &name);
    // Remove naming_context with <name> from <this> context
    // (Name version)
    
    int unbind_context (const Istring &name);
    // Remove naming_context with <name> from <this> context
    
    int destroy (void);
    // Same as unbind_context() with <this> as naming_context
    
    int list (u_long how_many, 
	      Binding_List &list, 
	      Binding_Iterator &iterator);
    // listing function: iterator creator
    // This is useful when there are many objects and contexts 
    // in <this> context and you only want to look at a few entries 
    // at a time

    int list (Binding_List &list);
    // listing function: iterator creator
    // This gives back a listing of all entries in <this> context.


    // Some other necessary functions which are
    // not part of the CORBA interface

    int flush (void);
    // Sync content of context to disk
    
    int close (void);
    // Close the handle of the context
    // Note: close() does not call flush()
    
    
    // Accessors

    HKEY key (void);
    // Get key

    // void parent (HKEY parent);
    HKEY parent (void);
    // Get parent

    void name (Name &name);
    // Get name
    // (Name version)

    void name (Istring &name);
    Istring name (void);
    // Get name
    // (String version)
    
  protected:  
    void key (HKEY key);
    // Set key
    
    void parent (HKEY parent);
    // Set parent

    void name (const Name &name);
    // Set name
    // (Name version)

    void name (const Istring &name);
    // Set name
    // (String version)
      
  private:
    Naming_Context (const Naming_Context &rhs);
    // Disallow copy constructors

    const Naming_Context &operator= (const Naming_Context &rhs);
    // Disallow assignment
    
    HKEY key_;
    // Key for self
    
    HKEY parent_key_;
    // Key for parent
    
    Istring name_;
    // Name of self
  };

  class ACE_Export Binding_Iterator 
    // = TITLE
    //     An iterator
    // = DESCRIPTION
    //     Useful when iteratorating over a few entries at a time
  {
    friend class Naming_Context;
    // Friend factory

  public:
    Binding_Iterator ();
    // Default constructor

    int next_one (Binding &binding);
    // Next entry

    int next_n (u_long how_many, 
		Binding_List &list);
    // Next <how_many> entries

    int destroy (void);
    // Cleanup

    void reset (void);
    // Reset the internal state of the iterator

    Naming_Context &naming_context (void);
    // Get naming_context that the iterator is iterating over

  private:

    void naming_context (Naming_Context& naming_context);
    // Set naming_context that the iterator is iterating over

    Naming_Context *naming_context_;
    // Reference to context

  public:
    // This should really be private
    // But the compiler is broken

    class ACE_Export Iteration_State
      // Base class for state
      {
      public:
	Iteration_State (Binding_Iterator &iterator);

	virtual int next_n (u_long how_many, 
			    Binding_List &list) = 0;
	// Next <how_many> entries

	void reset ();
	// Reset state

      protected:
	Binding_Iterator *parent_;
	// Pointer to parent iterator

	u_long index_;
      };

  private:
    class ACE_Export Object_Iteration : public Iteration_State
      {
      public:
	Object_Iteration (Binding_Iterator &iterator);

	int next_n (u_long how_many, 
		    Binding_List &list);
	// Next <how_many> entries
      };

    class ACE_Export Context_Iteration : public Iteration_State
      {
      public:
	Context_Iteration (Binding_Iterator &iterator);

	int next_n (u_long how_many, 
		    Binding_List &list);
	// Next <how_many> entries
      };

    class ACE_Export Iteration_Complete : public Iteration_State
      {
      public:
	Iteration_Complete (Binding_Iterator &iterator);
	
	int next_n (u_long how_many, 
		    Binding_List &list);
	// Next <how_many> entries
      };

    friend class Iteration_State;
    friend class Object_Iteration;
    friend class Context_Iteration;
    friend class Iteration_Complete;
    // Friend states
    
    Object_Iteration object_iteration_;
    Context_Iteration context_iteration_;
    Iteration_Complete iteration_complete_;
    // Instances of all states
    
    Iteration_State *current_enumeration_;
    // Pointer to current state

    void current_enumeration (Iteration_State& current_enumeration);
    Iteration_State &current_enumeration (void);
    // Set/Get current_enumeration
  };
};



class ACE_Export ACE_Predefined_Naming_Contexts
  // = TITLE
  //     A factory for predefined registries, which exist by default
  //     on Win32 platforms
  // = DESCRIPTION
  //     This factory can connect to both local and remote 
  //     predefined registries.
{
public:
  static int connect (ACE_Registry::Naming_Context &naming_context,
		      HKEY predefined = HKEY_LOCAL_MACHINE,
		      LPCTSTR machine_name = 0);
  // Factory method for connecting to predefined registries.  This
  // method works for both remote and local machines.  However, for
  // remote machines, HKEY_CLASSES_ROOT and HKEY_CURRENT_USER types
  // are not allowed

private:
  static int is_local_host (LPCTSTR machine_name);
  // Check if <machine_name> is the local host

};

#endif /* ACE_WIN32 */
#endif /* ACE_REGISTRY_H */
