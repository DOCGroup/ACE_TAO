/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Hash_Binding_Iterator_T.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_BINDINGS_ITERATOR_T_H
#define TAO_BINDINGS_ITERATOR_T_H

#include "orbsvcs/CosNamingS.h"

template <class ITERATOR, class TABLE_ENTRY>
class TAO_Bindings_Iterator : public virtual PortableServer::RefCountServantBase,
                              public virtual POA_CosNaming::BindingIterator
{
  // = TITLE
  //     This class implements the <BindingIterator> interface that is
  //     part of the <CosNaming> idl module based on the Hash_Map
  //     implementation for CosNaming::NamingContext.
  //
  // = DESCRIPTION
  //     <TAO_Hash_Binding_Iterator> constructor expects a pointer to a
  //     dynamically allocated hash map iterator. Destructor
  //     deallocates hash map iterator.
public:
  // = Intialization and termination methods.
  TAO_Bindings_Iterator (ITERATOR *hash_iter,
                         PortableServer::POA_ptr poa,
                         ACE_SYNCH_RECURSIVE_MUTEX &lock);
  // Constructor.

  ~TAO_Bindings_Iterator (void);
  // Destructor.

  // Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  CORBA::Boolean next_one (CosNaming::Binding_out b,
                           CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This operation returns the next binding.  If there are no more
  // bindings false is returned.

  CORBA::Boolean next_n (CORBA::ULong how_many,
                         CosNaming::BindingList_out bl,
                         CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This operation returns at most the requested number of bindings.

  void destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // This operation destroys the iterator.

  // = Helper method.

  static int populate_binding (TABLE_ENTRY *hash_entry,
                               CosNaming::Binding &b);
  // Helper function used by TAO_Hash_Naming_Context and
  // TAO_BindingIterator: populate a binding <b> with info contained
  // in <hash_entry>.  Return 1 if everything went smoothly, 0 if an
  // allocation failed.

private:
  int destroyed_;

  ITERATOR *hash_iter_;
  // A pointer to the hash map iterator.

  ACE_SYNCH_RECURSIVE_MUTEX &lock_;
  // Lock passed on from <TAO_NamingContext> to serialize access to the
  // internal data structure.

  PortableServer::POA_var poa_;
  // Implement a different _default_POA()
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Bindings_Iterator_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Bindings_Iterator_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_BINDINGS_ITERATOR_T_H */
