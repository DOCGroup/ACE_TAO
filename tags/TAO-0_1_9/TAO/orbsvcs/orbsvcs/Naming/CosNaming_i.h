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
  
class TAO_ORBSVCS_Export NS_NamingContext : public POA_CosNaming::NamingContext
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
    // @@ Marina, why is this starting with such an odd value?
    // The size of hash map for a NS_NamingContext object.
  };

  typedef ACE_Hash_Map_Manager<NS_ExtId, NS_IntId, ACE_Null_Mutex> 
          HASH_MAP;
         
  NS_NamingContext (void);
  // default constructor.

  ~NS_NamingContext (void);
  // destructor.

  // @@ Marina, can you please add comments to these methods?
  virtual void bind (const CosNaming::Name &n, 
		     CORBA::Object_ptr obj, 
		     CORBA::Environment &IT_env);
                
  virtual void rebind (const CosNaming::Name &n, 
		       CORBA::Object_ptr obj, 
		       CORBA::Environment &IT_env);
                
  virtual void bind_context (const CosNaming::Name &n, 
			     CosNaming::NamingContext_ptr nc, 
			     CORBA::Environment &IT_env);
                
  virtual void rebind_context (const CosNaming::Name &n, 
			       CosNaming::NamingContext_ptr nc, 
			       CORBA::Environment &IT_env);
                
  virtual CORBA::Object_ptr resolve (const CosNaming::Name &n, 
				     CORBA::Environment &IT_env);
                
  virtual void unbind (const CosNaming::Name &n, 
		       CORBA::Environment &IT_env);
                
  virtual CosNaming::NamingContext_ptr new_context (CORBA::Environment &IT_env);
                
  virtual CosNaming::NamingContext_ptr bind_new_context (const CosNaming::Name &n, 
							 CORBA::Environment &IT_env);
                
  virtual void destroy (CORBA::Environment &IT_env);
                
  virtual void list (CORBA::ULong how_many, 
		     CosNaming::BindingList_out bl, 
		     CosNaming::BindingIterator_out bi, 
		     CORBA::Environment &IT_env);

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
  // Lock to serialize access to the underlying data structure.  
  // This is a lock adapter that hides the type of lock, which may be
  // a null lock, if the ORB decides it is not needed.

  // CosNaming::NamingContext_ptr tie_ref_;
  // @@ I think this is not needed anymore, since we can obtain the 
  //    object reference from _this().
  // Stores CORBA object reference to the TIE object this object
  // implements.  This is needed to implement the <destroy> method.
};

class TAO_ORBSVCS_Export NS_BindingIterator : public POA_CosNaming::BindingIterator
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
  NS_BindingIterator (NS_NamingContext::HASH_MAP::ITERATOR *hash_iter, ACE_Lock * lock);
  // constructor.

  ~NS_BindingIterator (void);
  // destructor.

  // @@ Marina, please add comments.
  CORBA::Boolean next_one (CosNaming::Binding_out b, 
			   CORBA::Environment &IT_env);
 
  CORBA::Boolean next_n (CORBA::ULong how_many, 
			 CosNaming::BindingList_out bl, 
			 CORBA::Environment &IT_env);

  void destroy (CORBA::Environment &IT_env);
    
 private:
  NS_NamingContext::HASH_MAP::ITERATOR *hash_iter_;
  // A pointer to the hash map iterator.

  ACE_Lock *lock_;
  // lock passed on from NS_NamingContext to serialize access 
  //   to the internal data structure.
}; 

#endif /* COSNAMING_I_H */
