// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Base.h
 *
 *  $Id$
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#ifndef TAO_SERVANT_BASE_H
#define TAO_SERVANT_BASE_H
#include "ace/pre.h"

#include "PortableServerC.h"
#include "tao/Abstract_Servant_Base.h"
#include "ace/Atomic_Op.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Operation_Table;

/**
 * @class TAO_ServantBase
 *
 * @brief Base class for skeletons and servants.
 *
 * The POA spec requires that all servants inherit from this class'
 * base class.
 */
class TAO_PortableServer_Export TAO_ServantBase
  : public virtual TAO_Abstract_ServantBase
{
public:
  friend class TAO_POA;
  friend class TAO_Object_Adapter;
  friend class TAO_Local_ServantBase;

  /// Destructor.
  virtual ~TAO_ServantBase (void);

  /// Returns the default POA for this servant.
  virtual PortableServer::POA_ptr _default_POA (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );

  /// Local implementation of the CORBA::Object::_is_a method.
  virtual CORBA::Boolean _is_a (const char *logical_type_id
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Default <_non_existent>: always returns false.
  virtual CORBA::Boolean _non_existent (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );

  /// Query the Interface Repository for the interface definition.
  virtual CORBA::InterfaceDef_ptr _get_interface (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );

  /// Default <_get_component>: always returns CORBA::Object::_nil().
  virtual CORBA::Object_ptr _get_component (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );

  /// Get the correct vtable.
  virtual void *_downcast (const char *repository_id) = 0;

  /// This is an auxiliar method for _this() and _narrow().
  virtual TAO_Stub *_create_stub (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * Dispatches a request to the object: find the operation, cast the
   * type to the most derived type, demarshall all the parameters from
   * the request and finally invokes the operation, storing the
   * results and out parameters (if any) or the exceptions thrown into
   * <request>.
   */
  virtual void _dispatch (TAO_ServerRequest &request,
                          void *servant_upcall
                          ACE_ENV_ARG_DECL) = 0;

protected:

  /// Default constructor, only derived classes can be created.
  TAO_ServantBase (void);

  /// Copy constructor, protected so no instances can be created.
  TAO_ServantBase (const TAO_ServantBase &);

  /// Assignment operator.
  TAO_ServantBase &operator= (const TAO_ServantBase &);


  virtual void synchronous_upcall_dispatch (TAO_ServerRequest &req,
                                            void *servant_upcall,
                                            void *derived_this
                                            ACE_ENV_ARG_DECL);

  virtual void asynchronous_upcall_dispatch (TAO_ServerRequest &req,
                                             void *servant_upcall,
                                             void *derived_this
                                             ACE_ENV_ARG_DECL);

  /// Find an operation in the operation table.
  virtual int _find (const char *opname,
                     TAO_Skeleton &skelfunc,
                     const unsigned int length = 0);

  /// Register a CORBA IDL operation name.
  virtual int _bind (const char *opname,
                     const TAO_Skeleton skel_ptr);

  /// Get this interface's repository id (TAO specific).
  virtual const char *_interface_repository_id (void) const = 0;

protected:
  /// The operation table for this servant, it is initialized by the
  /// most derived class.
  TAO_Operation_Table *optable_;
};

/**
 * @class TAO_RefCountServantBase
 *
 * @brief Reference counting mix-in class.
 *
 * The RefCountServantBase mix-in class overrides the inherited
 * _add_ref and _remove_ref functions it inherits from
 * ServantBase, implementing them to provide true reference
 * counting. An instance of a servant class derived from
 * RefCountServantBase initially has a reference count of
 * one. Invoking _add_ref on the servant instance increases its
 * reference count by one. Invoking _remove_ref on the servant
 * instance decreases its reference count by one; if the
 * resulting reference count equals zero, _remove_ref invokes
 * delete on its this pointer in order to destroy the
 * servant. For ORBs that operate in multi-threaded
 * environments, the implementations of _add_ref and _remove_ref
 * that the RefCountServantBase class provides shall be
 * thread-safe.
 *
 * Like ServantBase, RefCountServantBase supports copy
 * construction and the default assignment operation. Copy
 * construction always sets the reference count of the new
 * servant instance to one. The default assignment
 * implementation merely returns *this and does not affect the
 * reference count.
 */
class TAO_PortableServer_Export TAO_RefCountServantBase
  : public virtual TAO_ServantBase
{
public:

  /// Destructor.
  ~TAO_RefCountServantBase (void);

  /// Increase reference count by one.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /**
   * Decreases reference count by one; if the resulting reference
   * count equals zero, _remove_ref invokes delete on its this pointer
   * in order to destroy the servant.
   */
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

protected:

  /// Constructor: initial reference count is one.
  TAO_RefCountServantBase (void);

  /// Copy Constructor: Always sets the reference count of the new
  /// servant instance to one.
  TAO_RefCountServantBase (const TAO_RefCountServantBase &);

  /// The default assignment implementation merely returns *this and
  /// does not affect the reference count.
  TAO_RefCountServantBase &operator= (const TAO_RefCountServantBase &);

private:

  /// Reference counter.
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, long> ref_count_;
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

class TAO_PortableServer_Export TAO_Servant_Hash
{
public:
  /// Returns hash value.
  u_long operator () (PortableServer::Servant servant) const;
};

/**
 * @class TAO_Local_ServantBase
 *
 * @brief Base class for local servants.
 *
 * This servant does not register with the POA and does not
 * produce a valid stub, i.e., object references of this servant
 * cannot be exported.  The (collocated) stubs of these servants
 * will always be direct, i.e., call directly to the servant and
 * don't call through the POA since this servant is not
 * registered with the POA.
 */
class TAO_PortableServer_Export TAO_Local_ServantBase
  : public virtual TAO_ServantBase
{
protected:
  /// This is an auxiliar method for _this().  Make sure *not* to
  /// register with the default POA.
  TAO_Stub *_create_stub (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Throws CORBA::BAD_OPERATION exception.
  void _dispatch (TAO_ServerRequest &request,
                  void *servant_upcall
                  ACE_ENV_ARG_DECL);
};

#if defined (__ACE_INLINE__)
# include "Servant_Base.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_SERVANT_BASE_H */
