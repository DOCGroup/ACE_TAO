// -*- C++ -*-

//=============================================================================
/**
 *  @file    Abstract_Servant_Base.h
 *
 *  $Id$
 *
 *  This files contains the TAO_Abstract_ServantBase, which is used
 *  as a way to decouple the PortableServer from the TAO Core,
 *  and make it possible to keep the reference to the servant in
 *  the CORBA::Object class.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ABSTRACT_SERVANT_BASE_H_
#define TAO_ABSTRACT_SERVANT_BASE_H_

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/Collocation_Strategy.h"
#include "tao/CORBA_methods.h"
#include "tao/Pseudo_VarOut_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ServerRequest;
class TAO_Stub;
class TAO_Abstract_ServantBase;

namespace CORBA
{
  class InterfaceDef;
  typedef InterfaceDef *InterfaceDef_ptr;

  class Object;
  typedef Object *Object_ptr;
  typedef TAO_Pseudo_Var_T<Object> Object_var;
  typedef TAO_Pseudo_Out_T<Object> Object_out;
}

namespace TAO
{
  class Argument;

  namespace Portable_Server
  {
    class Servant_Upcall;
  };
}

typedef void (*TAO_Skeleton)(
    TAO_ServerRequest &,
    TAO::Portable_Server::Servant_Upcall *,
    void *);

typedef void (*TAO_Collocated_Skeleton)(
    TAO_Abstract_ServantBase *,
    TAO::Argument **);

class TAO_Export TAO_Abstract_ServantBase
{
public:
  /// Destructor
  virtual ~TAO_Abstract_ServantBase (void);

  /// Local implementation of the CORBA::Object::_is_a method.
  virtual CORBA::Boolean _is_a (const char* logical_type_id) = 0;

#if (TAO_HAS_MINIMUM_CORBA == 0)
  /// Default @c _non_existent: always returns false.
  virtual CORBA::Boolean _non_existent (void) = 0;

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  /// Query the Interface Repository.
  virtual CORBA::InterfaceDef_ptr _get_interface (void) = 0;

  /// Default @c _get_component: always returns nil.
  virtual CORBA::Object_ptr _get_component (void) = 0;
#endif

  /// Default @c _repository_id
  virtual char * _repository_id (void) = 0;
#endif /* TAO_HAS_MINIMUM_CORBA */

  //@{
  /**
   * @name Reference Counting Operations
   */
  /// Increase reference count by one.
  virtual void _add_ref (void) = 0;

  /**
   * Decreases reference count by one; if the resulting reference
   * count equals zero, _remove_ref invokes delete on its this pointer
   * in order to destroy the servant.
   */
  virtual void _remove_ref (void) = 0;

  /// Returns the current reference count value.
  virtual CORBA::ULong _refcount_value (void) const = 0;
  //@}

  /// This is an auxiliary method for _this() and _narrow().
  virtual TAO_Stub *_create_stub (void) = 0;

  /// Find an operation in the operation table and return a
  /// TAO_Skeleton which can be used to make upcalls
  virtual int _find (const char *opname,
                     TAO_Skeleton &skelfunc,
                     const size_t length = 0) = 0;

  /// Find an operation in the operation table and return a
  /// TAO_Collocated_Skeleton which can be used to make upcalls onto
  /// collocated servants.
  virtual int _find (const char *opname,
                     TAO_Collocated_Skeleton &skelfunc,
                     TAO::Collocation_Strategy s,
                     const size_t length = 0) = 0;

  virtual void _collocated_dispatch (::CORBA::Object_ptr obj,
                                     ::CORBA::Object_out forward_obj,
                                     bool &is_forwarded,
                                     TAO::Argument ** args,
                                     int num_args,
                                     const char * op,
                                     size_t op_len,
                                     TAO::Collocation_Strategy strategy) = 0;

protected:

  /// Default constructor, only derived classes can be created.
  TAO_Abstract_ServantBase (void);

  /// Copy constructor, protected so no instances can be created.
  TAO_Abstract_ServantBase (const TAO_Abstract_ServantBase &);

  /// Assignment operator.
  TAO_Abstract_ServantBase &operator= (const TAO_Abstract_ServantBase &);

  /// Dispatches a request to the object
  /**
   * Find the operation, cast the type to the most derived type,
   * demarshall all the parameters from the request and finally
   * invokes the operation, storing the results and out parameters (if
   * any) or the exceptions thrown into @a request.
   */
  virtual void _dispatch (
    TAO_ServerRequest &request,
    TAO::Portable_Server::Servant_Upcall *servant_upcall) = 0;

  virtual void synchronous_upcall_dispatch (
    TAO_ServerRequest &req,
    TAO::Portable_Server::Servant_Upcall *servant_upcall,
    void *derived_this) = 0;

  /// Get this interface's repository id (TAO specific).
  virtual const char *_interface_repository_id (void) const = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "tao/Abstract_Servant_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_ABSTRACT_SERVANT_BASE_H_ */
