/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    cos
// 
// = FILENAME
//   NS_CosNaming.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
// 
// ============================================================================

#if !defined (NS_COSNAMING_H)
#define NS_COSNAMING_H

#include "CosNaming.hh"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "ace/SString.h"

class NS_IntId 
  // = TITLE
  //     Stores information a context keeps for each bound name
  //     (object reference and the type of binding).
  //
  // = DESCRIPTION
  //
  //
  //
  //
{
public:
  // = Initialization and termination methods.
  NS_IntId (void);
  // default constructor.

  NS_IntId (CORBA::Object_ptr obj,
	    CosNaming::BindingType type = CosNaming::nobject);
  // constructor.
  
  NS_IntId (const NS_IntId & rhs);
  // copy constructor

  ~NS_IntId (void);
  // destructor 

  void operator= (const NS_IntId & rhs);
  // Assignment operator (does copy memory).

  CORBA::Object_ptr ref_;   
  // CORBA object reference of the bound object.

  CosNaming::BindingType type_;  
  // Indicator of whether the object is a NamingContext that should
  // participate in name resolution when compound names are used.
};

class NS_ExtId 
  // = TITLE
  //    Stores the name to which an object is bound.
  //
  // = DESCRIPTION
  //
  //
  //
  //
{
public:
  // = Initialization and termination methods.
  NS_ExtId (void);
  // default constructor.

  NS_ExtId (const char *id, 
	    const char *kind);
  // constructor.
  
  NS_ExtId (const NS_ExtId & rhs);
  // copy constructor

  ~NS_ExtId (void);
  // destructor 

  void operator= (const NS_ExtId & rhs);
  // Assignment operator (does copy memory).

  int operator== (const NS_ExtId &rhs) const;
  // Equality comparison operator (must match both id_ and kind_).

  int operator!= (const NS_ExtId &rhs) const;
  // Inequality comparison operator.

  u_long hash (void) const;
  // This class has to have a hash for use with ACE_Hash_Map_Manager.

  ACE_CString kind_;  
  // any information user wants to store (not used by Naming Service).

  ACE_CString id_;  
  // any information user wants to store (not used by Naming Service).
};

class NS_NamingContext 
  // = TITLE
  //     This class implements the NamingContext interface that is part of the 
  //     CosNaming idl module.  
  //
  // = DESCRIPTION
  //     Extensive comments can be found in the idl file. 
{
public:
  
  enum
  {
    NS_MAP_SIZE = 23
    // The size of hash map for a NS_NamingContext object.
  };

  typedef ACE_Hash_Map_Manager<NS_ExtId, NS_IntId, ACE_Null_Mutex> HASH_MAP;
         
  NS_NamingContext (void);
  // default constructor.

  ~NS_NamingContext (void);
  // destructor.

  void bind (const CosNaming::Name &n, 
	     CORBA::Object_ptr obj, 
	     CORBA::Environment &IT_env = CORBA::default_environment) 
     throw (CORBA::SystemException, 
	    CosNaming::NamingContext::NotFound, 
	    CosNaming::NamingContext::CannotProceed, 
	    CosNaming::NamingContext::InvalidName, 
	    CosNaming::NamingContext::AlreadyBound);
                
  void rebind (const CosNaming::Name &n, 
	       CORBA::Object_ptr obj, 
	       CORBA::Environment &IT_env = CORBA::default_environment) 
    throw (CORBA::SystemException, 
	   CosNaming::NamingContext::NotFound, 
	   CosNaming::NamingContext::CannotProceed, 
	   CosNaming::NamingContext::InvalidName);
                
  void bind_context (const CosNaming::Name &n, 
		     CosNaming::NamingContext_ptr nc, 
		     CORBA::Environment &IT_env = CORBA::default_environment) 
    throw (CORBA::SystemException, 
	   CosNaming::NamingContext::NotFound, 
	   CosNaming::NamingContext::CannotProceed, 
	   CosNaming::NamingContext::InvalidName, 
	   CosNaming::NamingContext::AlreadyBound);
                
  void rebind_context (const CosNaming::Name &n, 
		       CosNaming::NamingContext_ptr nc, 
		       CORBA::Environment &IT_env = CORBA::default_environment)
    throw (CORBA::SystemException, 
	   CosNaming::NamingContext::NotFound, 
	   CosNaming::NamingContext::CannotProceed, 
	   CosNaming::NamingContext::InvalidName);
                
  CORBA::Object_ptr resolve (const CosNaming::Name &n, 
			     CORBA::Environment &IT_env = CORBA::default_environment) 
    throw (CORBA::SystemException, 
	   CosNaming::NamingContext::NotFound, 
	   CosNaming::NamingContext::CannotProceed, 
	   CosNaming::NamingContext::InvalidName);
                
  void unbind (const CosNaming::Name &n, 
	       CORBA::Environment &IT_env = CORBA::default_environment) 
    throw (CORBA::SystemException, 
	   CosNaming::NamingContext::NotFound, 
	   CosNaming::NamingContext::CannotProceed, 
	   CosNaming::NamingContext::InvalidName);
                
  CosNaming::NamingContext_ptr new_context (CORBA::Environment &IT_env = CORBA::default_environment) 
    throw (CORBA::SystemException);
                
  CosNaming::NamingContext_ptr bind_new_context (const CosNaming::Name &n, 
						 CORBA::Environment &IT_env = CORBA::default_environment) 
    throw (CORBA::SystemException, 
	   CosNaming::NamingContext::NotFound, 
	   CosNaming::NamingContext::AlreadyBound, 
	   CosNaming::NamingContext::CannotProceed, 
	   CosNaming::NamingContext::InvalidName);
                
  void destroy (CORBA::Environment &IT_env = CORBA::default_environment) 
    throw (CORBA::SystemException, 
	   CosNaming::NamingContext::NotEmpty);
                
  void list (CORBA::ULong how_many, 
	     CosNaming::BindingList *&bl, 
	     CosNaming::BindingIterator_ptr &bi, 
	     CORBA::Environment &IT_env = CORBA::default_environment) 
    throw (CORBA::SystemException);


  void initialize (CosNaming::NamingContext_ptr tie_ref);
  // This method is called after context constructor.  It provides the
  // implementation object with the reference to its TIE object. (This
  // is needed for implementation of destroy method)

protected:
  // = These are the helper methods used by other methods.

  CosNaming::NamingContext_ptr get_context (const CosNaming::Name &name)
       throw (CORBA::SystemException, 
	      CosNaming::NamingContext::NotFound, 
	      CosNaming::NamingContext::CannotProceed, 
	      CosNaming::NamingContext::AlreadyBound);
  // This is used by methods that need to resolve a compound name to
  // get the reference to the target context before performing the
  // actual operation (e.g. bind, unbind, etc.)  Takes a full name
  // (including the last component that doesn't need to be resolved)
  // Returns a pointer to the target context.

private:    
  
  HASH_MAP context_;
  // This implementation of NamingContext uses ACE thread-safe Hash
  // Map for storage and manipulation of name-object bindings.

  CosNaming::NamingContext_ptr tie_ref_;
  // Stores CORBA object reference to the TIE object this object
  // implements.  This is needed to implement the <destroy> method.
};

class NS_BindingIterator
  // = TITLE
  //     This class implements the BindingIterator interface 
  //     that is part of the CosNaming idl module.
  //
  // = DESCRIPTION
  //     NS_BindingIterator constructor expects a pointer to a
  //     DYNAMICALLY allocated hash map iterator. Destructor
  //     deallocates hash map iterator.
{
public:
  // = Intialization and termination methods.
  NS_BindingIterator (NS_NamingContext::HASH_MAP::ITERATOR *hash_iter);
  // constructor.

  ~NS_BindingIterator (void);
  // destructor.

  void initialize (CosNaming::BindingIterator_ptr tie_ref);
  // This method is called after the constructor.  It provides
  // implementation object with the reference to the TIE object it
  // implements.

  // Marina, please add comments.
  CORBA::Boolean next_one (CosNaming::Binding *&b, 
			   CORBA::Environment &IT_env = CORBA::default_environment) 
     throw (CORBA::SystemException);
 
  CORBA::Boolean next_n (CORBA::ULong how_many, 
			 CosNaming::BindingList *&bl, 
			 CORBA::Environment &IT_env = CORBA::default_environment) 
    throw (CORBA::SystemException);

    
  void destroy (CORBA::Environment &IT_env = CORBA::default_environment) 
    throw (CORBA::SystemException);

private:
  NS_NamingContext::HASH_MAP::ITERATOR *hash_iter_;
  // A pointer to the hash map iterator.

  CosNaming::BindingIterator_ptr tie_ref_;
  // A reference to the TIE object this object implements.  this is
  // used for implementation of <destroy>.
};

DEF_TIE_CosNaming_NamingContext (NS_NamingContext)
DEF_TIE_CosNaming_BindingIterator (NS_BindingIterator)

#endif /* NS_COSNAMING_H */
