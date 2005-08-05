// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Base.h
 *
 *  $Id$
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_SERVANT_BASE_H
#define TAO_SERVANT_BASE_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PS_ForwardC.h"
#include "ace/Atomic_Op.h"
#include "tao/Abstract_Servant_Base.h"

class TAO_Operation_Table;

/**
 * @class TAO_ServantBase
 *
 * @brief Base class for skeletons and servants.
 *
 * The POA spec requires that all servants inherit from this class'
 * base class.
 *
 * An instance of a servant class derived from
 * ServantBase initially has a reference count of
 * one. Invoking _add_ref on the servant instance increases its
 * reference count by one. Invoking _remove_ref on the servant
 * instance decreases its reference count by one; if the
 * resulting reference count equals zero, _remove_ref invokes
 * delete on its this pointer in order to destroy the
 * servant. For ORBs that operate in multi-threaded
 * environments, the implementations of _add_ref and _remove_ref
 * that the ServantBase class provides shall be
 * thread-safe.
 *
 * Like ServantBase supports copy
 * construction and the default assignment operation. Copy
 * construction always sets the reference count of the new
 * servant instance to one. The default assignment
 * implementation merely returns *this and does not affect the
 * reference count.
 *
 */
class TAO_PortableServer_Export TAO_ServantBase
  : public virtual TAO_Abstract_ServantBase
{
public:

  /// Destructor.
  virtual ~TAO_ServantBase (void);

  /// Returns the default POA for this servant.
  virtual PortableServer::POA_ptr _default_POA (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );

  /// Local implementation of the CORBA::Object::_is_a method.
  virtual CORBA::Boolean _is_a (const char *logical_type_id
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Default _non_existent: always returns false.
  virtual CORBA::Boolean _non_existent (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );

  /// Query the Interface Repository for the interface definition.
  virtual CORBA::InterfaceDef_ptr _get_interface (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );

  /// Default _get_component: always returns CORBA::Object::_nil().
  virtual CORBA::Object_ptr _get_component (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );

  /// Get the repository id.
  virtual char * _repository_id (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );

  /// This is an auxiliary method for _this() and _narrow().
  virtual TAO_Stub *_create_stub (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * Dispatches a request to the object: find the operation, cast the
   * type to the most derived type, demarshall all the parameters from
   * the request and finally invokes the operation, storing the
   * results and out parameters (if any) or the exceptions thrown into
   * @a request.
   */
  virtual void _dispatch (TAO_ServerRequest &request,
                          void *servant_upcall
                          ACE_ENV_ARG_DECL) = 0;

  /// Please see documentation in tao/Abstract_Servant_Base.h for
  /// details.
  virtual int _find (const char *opname,
                     TAO_Skeleton &skelfunc,
                     const unsigned int length = 0);

  virtual int _find (const char *opname,
                     TAO_Collocated_Skeleton &skelfunc,
                     TAO::Collocation_Strategy st,
                     const unsigned int length = 0);

  /// Get this interface's repository id (TAO specific).
  virtual const char *_interface_repository_id (void) const = 0;

  //@{
  /**
   * @name Reference Counting Operations
   */
  /// Increase reference count by one.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * Decreases reference count by one; if the resulting reference
   * count equals zero, _remove_ref invokes delete on its this pointer
   * in order to destroy the servant.
   */
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * Returns the current reference count value.
   */
  virtual CORBA::ULong _refcount_value (ACE_ENV_SINGLE_ARG_DECL) const;
  //@}

protected:

  /// Default constructor, only derived classes can be created.
  TAO_ServantBase (void);

  /// Copy constructor, protected so no instances can be created.
  TAO_ServantBase (const TAO_ServantBase &);

  /// Assignment operator.
  TAO_ServantBase &operator= (const TAO_ServantBase &);

  virtual void synchronous_upcall_dispatch (TAO_ServerRequest & req,
                                            void * servant_upcall,
                                            void * derived_this
                                            ACE_ENV_ARG_DECL);

  virtual void asynchronous_upcall_dispatch (TAO_ServerRequest & req,
                                             void * servant_upcall,
                                             void * derived_this
                                             ACE_ENV_ARG_DECL);

protected:
  /// Reference counter.
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, long> ref_count_;

  /// The operation table for this servant.  It is initialized by the
  /// most derived class.
  TAO_Operation_Table * optable_;
};

class TAO_PortableServer_Export TAO_Servant_Hash
{
public:
  /// Returns hash value.
  u_long operator () (PortableServer::Servant servant) const;
};

/**
 * @class TAO_ServantBase_var
 *
 * @brief Auto pointer for reference counting servants.
 *
 * For the convenience of automatically managing servant
 * reference counts, the PortableServer namespace also provides
 * the ServantBase_var class. This class behaves similarly to
 * _var classes for object references (see Section 20.3.1).
 */
class TAO_PortableServer_Export TAO_ServantBase_var
{
public:
  TAO_ServantBase_var (void);

  TAO_ServantBase_var (TAO_ServantBase *p);

  TAO_ServantBase_var (const TAO_ServantBase_var &b);

  ~TAO_ServantBase_var (void);

  TAO_ServantBase_var &operator= (TAO_ServantBase *p);

  TAO_ServantBase_var &operator= (const TAO_ServantBase_var &b);

  TAO_ServantBase *operator-> () const;

  TAO_ServantBase *in (void) const;

  TAO_ServantBase *&inout (void);

  TAO_ServantBase *&out (void);

  TAO_ServantBase *_retn (void);

private:

  TAO_ServantBase *ptr_;
};

#if defined (__ACE_INLINE__)
# include "Servant_Base.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_SERVANT_BASE_H */
