/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
// 
// = FILENAME
//   CosNaming_i.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu> &
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
// 
// ============================================================================

#if !defined (COSNAMING_I_H)
#define	COSNAMING_I_H

#include "orbsvcs/CosNamingS.h"
#include "Entries.h"
#include "ace/Synch.h"
  
class TAO_ORBSVCS_Export NS_NamingContext : public POA_CosNaming::NamingContext
{
  // = TITLE
  //     This class implements the NamingContext interface that is
  //     part of the CosNaming idl module.
  //
  // = DESCRIPTION
  //     Extensive comments can be found in the Naming IDL file.
public:
  typedef ACE_Hash_Map_Manager<NS_ExtId, NS_IntId, ACE_Null_Mutex> 
          HASH_MAP;
         
  // = Initialization and termination methods.
  NS_NamingContext (size_t default_hash_table_size = ACE_DEFAULT_MAP_SIZE);
  // Default constructor, which initializes the <size> of the table.

  int init (void);
  // Initialize the lock.  The ORB must be initialized when this
  // constructor is called.

  ~NS_NamingContext (void);
  // destructor.

  virtual void bind (const CosNaming::Name &n, 
		     CORBA::Object_ptr obj, 
		     CORBA::Environment &IT_env);
  // Create a binding for name <n> and object <obj> in the naming
  // context.  Compound names are treated as follows: ctx->bind (<c1;
  // c2; c3; cn>, obj) = (ctx->resolve (<c1; c2; cn-1>))->bind (<cn>,
  // obj) if the there already exists a binding for the specified
  // name, <AlreadyBound> exception is thrown.  Naming contexts should
  // be bound using <bind_context> and <rebind_context> in order to
  // participate in name resolution later.
                
  virtual void rebind (const CosNaming::Name &n, 
		       CORBA::Object_ptr obj, 
		       CORBA::Environment &IT_env);
  // This is similar to <bind> operation above, except for when the
  // binding for the specified name already exists in the specified
  // context.  In that case, the existing binding is replaced with the
  // new one.
                
  virtual void bind_context (const CosNaming::Name &n, 
			     CosNaming::NamingContext_ptr nc, 
			     CORBA::Environment &IT_env);
  // This is the version of <bind> specifically for binding naming
  // contexts, so that they will participate in name resolution when
  // compound names are passed to be resolved.
              
  virtual void rebind_context (const CosNaming::Name &n, 
			       CosNaming::NamingContext_ptr nc, 
			       CORBA::Environment &IT_env);
  // This is a version of <rebind> specifically for naming contexts,
  // so that they can participate in name resolution when compound
  // names are passed.
              
  virtual CORBA::Object_ptr resolve (const CosNaming::Name &n, 
				     CORBA::Environment &IT_env);
  // Return object reference that is bound to the name.  Compound name
  // resolve is defined as follows: ctx->resolve (<c1; c2; cn>) =
  // ctx->resolve (<c1; c2 cn-1>)->resolve (<cn>) The naming service
  // does not return the type of the object.  Clients are responsible
  // for "narrowing" the object to the appropriate type.
              
  virtual void unbind (const CosNaming::Name &n, 
		       CORBA::Environment &IT_env);
  // Remove the name binding from the context.  When compound names
  // are used, unbind is defined as follows: ctx->unbind (<c1; c2;
  // cn>) = (ctx->resolve (<c1; c2; cn-1>))->unbind (<cn>)
              
  virtual CosNaming::NamingContext_ptr new_context (CORBA::Environment &IT_env);
  // This operation returns a new naming context implemented by the
  // same naming server in which the operation was invoked.  The
  // context is not bound.
              
  virtual CosNaming::NamingContext_ptr bind_new_context (const CosNaming::Name &n, 
							 CORBA::Environment &IT_env);
  // This operation creates a new context and binds it to the name
  // supplied as an argument.  The newly-created context is
  // implemented by the same server as the context in which it was
  // bound (the name argument excluding the last component).
  
  virtual void destroy (CORBA::Environment &IT_env);
  // Delete the naming context.  NOTE: the user should <unbind> any
  // bindings in which the given context is bound to some names before
  // invoking <destroy> operation on it.  Ignoring this rule may cause
  // unexpected behaviour.  <destroy> deletes the context object if it
  // is not bound to any names in the given address space.  <destroys>
  // decrements the reference count of the context if bindings to it
  // exist.
  
  virtual void list (CORBA::ULong how_many, 
		     CosNaming::BindingList_out bl, 
		     CosNaming::BindingIterator_out bi, 
		     CORBA::Environment &IT_env);
  // Returns at most the requested number of bindings <how_many> in
  // <bl>.  If the naming context contains additional bindings, they
  // are returned with a BindingIterator.  In the naming context does
  // not contain any additional bindings <bi> returned as null.

protected:
  // = These are the helper methods used by other methods.
  
  CosNaming::NamingContext_ptr get_context (const CosNaming::Name &name);
  // This is used by methods that need to resolve a compound name to
  // get the reference to the target context before performing the
  // actual operation (e.g. bind, unbind, etc.)  Takes a full name
  // (including the last component that doesn't need to be resolved)
  // Returns a pointer to the target context.
  
private:    
  HASH_MAP context_;
  // This implementation of NamingContext uses ACE thread-safe Hash
  // Map for storage and manipulation of name-object bindings.
  
  ACE_Lock *lock_;
  // Lock to serialize access to the underlying data structure.  This
  // is a lock adapter that hides the type of lock, which may be a
  // null lock, if the ORB decides it is not needed.
};

class TAO_ORBSVCS_Export NS_BindingIterator : public POA_CosNaming::BindingIterator
{
  // = TITLE
  //     This class implements the BindingIterator interface 
  //     that is part of the CosNaming idl module.
  //
  // = DESCRIPTION
  //     NS_BindingIterator constructor expects a pointer to a
  //     DYNAMICALLY allocated hash map iterator. Destructor
  //     deallocates hash map iterator.
public:
  // = Intialization and termination methods.
  NS_BindingIterator (NS_NamingContext::HASH_MAP::ITERATOR *hash_iter,
                      ACE_Lock *lock);
  // constructor.

  ~NS_BindingIterator (void);
  // destructor.

  CORBA::Boolean next_one (CosNaming::Binding_out b, 
			   CORBA::Environment &IT_env);
  // This operation returns the next binding.  If there are no more
  // bindings false is returned.

  CORBA::Boolean next_n (CORBA::ULong how_many, 
			 CosNaming::BindingList_out bl, 
			 CORBA::Environment &IT_env);
  // This operation returns at most the requested number of bindings.

  void destroy (CORBA::Environment &IT_env);
  // This operation destroys the iterator.
   
private:
  NS_NamingContext::HASH_MAP::ITERATOR *hash_iter_;
  // A pointer to the hash map iterator.

  ACE_Lock *lock_;
  // Lock passed on from NS_NamingContext to serialize access to the
  // internal data structure.
}; 

#endif /* COSNAMING_I_H */
