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
#include "ace/pre.h"

#include "Hash_Naming_Context.h"

template <class ITERATOR, class TABLE_ENTRY>
class TAO_Bindings_Iterator : public virtual PortableServer::RefCountServantBase,
                              public virtual POA_CosNaming::BindingIterator
{
  // = TITLE
  //     This class implements the <BindingIterator> interface that is
  //     part of the <CosNaming> idl module.   This class works with Hash_Map-based
  //     implementations of CosNaming::NamingContext.
  //
  // = DESCRIPTION
  //     This class is templatized by the types of the underlying
  //     hash table iterator and hash table entry, so that it can be
  //     used for both TAO_Transient_Naming_Context and
  //     TAO_Persistent_Naming_Context (and any other classes with
  //     underlying data structures supporting
  //     ACE_Hash_Map_Manager/Iterator - like interfaces).
  //
  //     Instances of <TAO_Bindings_Iterator>s affect reference counts
  //     of corresponding Naming Contexts. Reference count on a
  //     Naming Context is incremented by one for a lifetime of each
  //     instance of <TAO_Bindings_Iterator> created for that
  //     context, i.e., a Naming Context cannot
  //     be cleaned up (but, of course, it can be invalidated) before
  //     all of its iterators have been cleaned up.  When <destroy>
  //     method is invoked on a Naming Context, all of its iterators are
  //     destroyed in a "lazy evaluation" fashion, i.e., whenever a next
  //     operation is invoked on an iterator, and it can detect that
  //     the corresponding Naming Context has been invalidated, the
  //     iterator is destroyed.
  //
public:
  // = Intialization and termination methods.
  TAO_Bindings_Iterator (TAO_Hash_Naming_Context *context,
                         ITERATOR *hash_iter,
                         PortableServer::POA_ptr poa,
                         TAO_SYNCH_RECURSIVE_MUTEX &lock);
  // Constructor expects a pointer to a
  // dynamically allocated hash map iterator (destructor
  // deallocates hash map iterator).


  ~TAO_Bindings_Iterator (void);
  // Destructor.

  virtual PortableServer::POA_ptr _default_POA (TAO_ENV_SINGLE_ARG_DECL);
  // Returns the Default POA of this Servant object

  // = Idl methods.

  CORBA::Boolean next_one (CosNaming::Binding_out b
                           TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This operation passes back the next unseen binding.  True is
  // returned if a binding is passed back, and false is returned otherwise.

  CORBA::Boolean next_n (CORBA::ULong how_many,
                         CosNaming::BindingList_out bl
                         TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This operation passes back at most <how_many> unseen bindings.
  // True is returned if bindings were passed back, and false is
  // returned if no bindings were passed back.

  void destroy (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This operation destroys the iterator.

  // = Helper method.

  static int populate_binding (TABLE_ENTRY *hash_entry,
                               CosNaming::Binding &b);
  // Helper function used by TAO_*_Naming_Context and
  // TAO_BindingIterator: populate a binding <b> with info contained
  // in <hash_entry>.  Return 1 if everything went smoothly, 0 if an
  // allocation failed.

private:
  int destroyed_;
  // Flag indicating whether this iterator is still valid.  (The
  // iterator becomes invalid when <destroy> method has been invoked
  // on it, or when <destroy> method has been invoked on the
  // corresponding Naming Context.) This flag is necessary
  // because immediate destruction of this servant might not be possible
  // due to pending requests in the POA.

  TAO_Hash_Naming_Context *context_;
  // Pointer to the Naming Context we are iterating over.  We need
  // this pointer to make sure the context is still valid before
  // each iteration, and to decrement its reference count once we are <destroyed>.

  ITERATOR *hash_iter_;
  // A pointer to the hash map iterator.

  TAO_SYNCH_RECURSIVE_MUTEX &lock_;
  // Lock passed on from Naming Context to serialize access to the
  // internal data structure.

  PortableServer::POA_var poa_;
  // Implement a different _default_POA().
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Bindings_Iterator_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Bindings_Iterator_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_BINDINGS_ITERATOR_T_H */
