// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Base.h
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_SERVANT_BASE_H
#define TAO_SERVANT_BASE_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PS_ForwardC.h"
#include "tao/PortableServer/Servant_var.h"
#include "tao/Abstract_Servant_Base.h"
#include <atomic>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
 */
class TAO_PortableServer_Export TAO_ServantBase
  : public virtual TAO_Abstract_ServantBase
{
public:
  /// Useful for template programming.
  typedef CORBA::Object     _stub_type;
  typedef CORBA::Object_ptr _stub_ptr_type;
  typedef CORBA::Object_var _stub_var_type;

  /// Destructor.
  virtual ~TAO_ServantBase () = default;

  static void _is_a_skel (TAO_ServerRequest & req,
                          TAO::Portable_Server::Servant_Upcall* servant_upcall,
                          TAO_ServantBase *servant);

  static void _is_a_thru_poa_skel (TAO_ServerRequest & server_request,
                                   TAO::Portable_Server::Servant_Upcall* servant_upcall,
                                   TAO_ServantBase *servant);

#if (TAO_HAS_MINIMUM_CORBA == 0)
  static void _non_existent_skel (TAO_ServerRequest & req,
                                  TAO::Portable_Server::Servant_Upcall* servant_upcall,
                                  TAO_ServantBase *servant);
  static void _non_existent_thru_poa_skel (TAO_ServerRequest & server_request,
                                           TAO::Portable_Server::Servant_Upcall* servant_upcall,
                                           TAO_ServantBase *servant);

# if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  static void _interface_skel (TAO_ServerRequest & req,
                               TAO::Portable_Server::Servant_Upcall* servant_upcall,
                               TAO_ServantBase *servant);

  static void _component_skel (TAO_ServerRequest & req,
                               TAO::Portable_Server::Servant_Upcall* servant_upcall,
                               TAO_ServantBase *servant);
  static void _component_thru_poa_skel (TAO_ServerRequest & server_request,
                                        TAO::Portable_Server::Servant_Upcall* servant_upcall,
                                        TAO_ServantBase *servant);
# endif /* !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */
  static void _repository_id_skel (TAO_ServerRequest & req,
                                  TAO::Portable_Server::Servant_Upcall* servant_upcall,
                                  TAO_ServantBase *servant);
  static void _repository_id_thru_poa_skel (TAO_ServerRequest & server_request,
                                            TAO::Portable_Server::Servant_Upcall* servant_upcall,
                                            TAO_ServantBase *servant);
#endif /* TAO_HAS_MINIMUM_CORBA */

  /// Returns the default POA for this servant.
  virtual PortableServer::POA_ptr _default_POA ();

  /// Local implementation of the CORBA::Object::_is_a method.
  virtual CORBA::Boolean _is_a (const char *logical_type_id);

#if (TAO_HAS_MINIMUM_CORBA == 0)
  /// Default _non_existent: always returns false.
  virtual CORBA::Boolean _non_existent ();

  /// Query the Interface Repository for the interface definition.
  virtual CORBA::InterfaceDef_ptr _get_interface ();

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  /// Default _get_component: always returns CORBA::Object::_nil().
  virtual CORBA::Object_ptr _get_component ();
#endif

  /// Get the repository id.
  virtual char * _repository_id ();
#endif /* TAO_HAS_MINIMUM_CORBA */

  /// This is an auxiliary method for _this() and _narrow().
  virtual TAO_Stub *_create_stub ();

  /**
   * Dispatches a request to the object: find the operation, cast the
   * type to the most derived type, demarshall all the parameters from
   * the request and finally invokes the operation, storing the
   * results and out parameters (if any) or the exceptions thrown into
   * @a request.
   */
  virtual void _dispatch (
    TAO_ServerRequest &request,
    TAO::Portable_Server::Servant_Upcall* servant_upcall) = 0;

  /// Please see documentation in tao/Abstract_Servant_Base.h for
  /// details.
  virtual int _find (const char *opname,
                     TAO_Skeleton &skelfunc,
                     const size_t length = 0);

  virtual int _find (const char *opname,
                     TAO_Collocated_Skeleton &skelfunc,
                     TAO::Collocation_Strategy st,
                     const size_t length = 0);

  /// Get this interface's repository id (TAO specific).
  virtual const char *_interface_repository_id () const = 0;

  //@{
  /**
   * @name Reference Counting Operations
   */
  /// Increase reference count by one.
  virtual void _add_ref ();

  /**
   * Decreases reference count by one; if the resulting reference
   * count equals zero, _remove_ref invokes delete on its this pointer
   * in order to destroy the servant.
   */
  virtual void _remove_ref ();

  /**
   * Returns the current reference count value.
   */
  virtual CORBA::ULong _refcount_value () const;
  //@}

  virtual void _collocated_dispatch (::CORBA::Object_ptr obj,
                                     ::CORBA::Object_out forward_obj,
                                     bool &is_forwarded,
                                     TAO::Argument ** args,
                                     int num_args,
                                     const char * op,
                                     size_t op_len,
                                     TAO::Collocation_Strategy strategy);

protected:
  /// Default constructor, only derived classes can be created.
  explicit TAO_ServantBase (TAO_Operation_Table *optable = nullptr);

  TAO_ServantBase (const TAO_ServantBase &);

  TAO_ServantBase &operator= (const TAO_ServantBase &);

  void synchronous_upcall_dispatch (
    TAO_ServerRequest & req,
    TAO::Portable_Server::Servant_Upcall* servant_upcall,
    TAO_ServantBase *derived_this);

  void asynchronous_upcall_dispatch (
    TAO_ServerRequest & req,
    TAO::Portable_Server::Servant_Upcall* servant_upcall,
    TAO_ServantBase *derived_this);

protected:
  /// Reference counter.
  std::atomic<uint32_t> ref_count_;

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

namespace PortableServer
{
  typedef Servant_var<TAO_ServantBase> ServantBase_var;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/Servant_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_SERVANT_BASE_H */
