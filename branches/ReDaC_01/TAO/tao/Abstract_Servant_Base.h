// -*- C++ -*-

//=============================================================================
/**
 *  @file    Abstract_Servant_Base.h
 *
 *  $Id$
 *
 *  This files contains the TAO_Abstract_ServantBase, which is used
 *  as a way to decuple the PortableServer from the TAO Core,
 *  and make it possible to keep the reference to the servant in
 *  the CORBA::Object class.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ABSTRACT_SERVANT_BASE_H_
#define TAO_ABSTRACT_SERVANT_BASE_H_

#include /**/ "ace/pre.h"

#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/TAO_Export.h"
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

  class Environment;

  class Object;
  typedef Object *Object_ptr;
  typedef TAO_Pseudo_Var_T<Object> Object_var;
  typedef TAO_Pseudo_Out_T<Object> Object_out;
}

namespace TAO
{
  class Argument;
}

typedef void (*TAO_Skeleton)(
    TAO_ServerRequest &,
    void *,
    void *
#if !defined (TAO_HAS_EXCEPTIONS) || defined (ACE_ENV_BKWD_COMPAT)
    , CORBA::Environment &
#endif
  );

typedef void (*TAO_Collocated_Skeleton)(
    TAO_Abstract_ServantBase *,
    TAO::Argument **,
    int
#if !defined (TAO_HAS_EXCEPTIONS) || defined (ACE_ENV_BKWD_COMPAT)
    , CORBA::Environment &
#endif
  );

class TAO_Export TAO_Abstract_ServantBase
{
public:
  /// Destructor
  virtual ~TAO_Abstract_ServantBase (void);

  /// Local implementation of the CORBA::Object::_is_a method.
  virtual CORBA::Boolean _is_a (const char* logical_type_id
                                ACE_ENV_ARG_DECL) = 0;

  /// Default @c _non_existent: always returns false.
  virtual CORBA::Boolean _non_existent (
    ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Query the Interface Repository.
  virtual CORBA::InterfaceDef_ptr _get_interface (
      ACE_ENV_SINGLE_ARG_DECL
    ) = 0;

  /// Default @c_get_component: always returns nil.
  virtual CORBA::Object_ptr _get_component (
    ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Default @c_repository_id
  virtual char * _repository_id (
    ACE_ENV_SINGLE_ARG_DECL) = 0;

  //@{
  /**
   * @name Reference Counting Operations
   */
  /// Increase reference count by one.
  virtual void _add_ref (
    ACE_ENV_SINGLE_ARG_DECL) = 0;

  /**
   * Decreases reference count by one; if the resulting reference
   * count equals zero, _remove_ref invokes delete on its this pointer
   * in order to destroy the servant.
   */
  virtual void _remove_ref (
    ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Returns the current reference count value.
  virtual CORBA::ULong _refcount_value (
    ACE_ENV_SINGLE_ARG_DECL) const = 0;
  //@}

  /// This is an auxiliary method for _this() and _narrow().
  virtual TAO_Stub *_create_stub (ACE_ENV_SINGLE_ARG_DECL) = 0;

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
  virtual void _dispatch (TAO_ServerRequest &request,
                          void *servant_upcall
                          ACE_ENV_ARG_DECL) = 0;

  virtual void synchronous_upcall_dispatch (TAO_ServerRequest &req,
                                            void *servant_upcall,
                                            void *derived_this
                                            ACE_ENV_ARG_DECL) = 0;

  /// Get this interface's repository id (TAO specific).
  virtual const char *_interface_repository_id (void) const = 0;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ABSTRACT_SERVANT_BASE_H_ */
