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

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"

class TAO_ServerRequest;
class TAO_Stub;

typedef void (*TAO_Skeleton)(
    TAO_ServerRequest &,
    void *,
    void *
#if !defined (TAO_HAS_EXCEPTIONS) || defined (ACE_ENV_BKWD_COMPAT)
    , CORBA::Environment &
#endif
  );

namespace CORBA
{
  class InterfaceDef;
  typedef InterfaceDef *InterfaceDef_ptr;
};

class TAO_Export TAO_Abstract_ServantBase
{
public:
  /// Destructor
  virtual ~TAO_Abstract_ServantBase (void);

  /// Local implementation of the CORBA::Object::_is_a method.
  virtual CORBA::Boolean _is_a (const char* logical_type_id
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

  /// Default <_non_existent>: always returns false.
  virtual CORBA::Boolean _non_existent (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) = 0;

  /// Query the Interface Repository.
  virtual CORBA::InterfaceDef_ptr _get_interface (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    ) = 0;

  /// Default <_get_component>: always returns nil.
  virtual CORBA::Object_ptr _get_component (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) = 0;

  //@{
  /**
   * @name Reference Counting Hooks
   *
   * Reference counting hooks are no-ops by default.
   */
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  //@}

  /// Get the correct vtable.
  virtual void *_downcast (const char *repository_id) = 0;

  /// This is an auxiliary method for _this() and _narrow().
  virtual TAO_Stub *_create_stub (ACE_ENV_SINGLE_ARG_DECL) = 0;

protected:

  /// Default constructor, only derived classes can be created.
  TAO_Abstract_ServantBase (void);

  /// Copy constructor, protected so no instances can be created.
  TAO_Abstract_ServantBase (const TAO_Abstract_ServantBase &);

  /// assignment operator.
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

  /// Find an operation in the operation table.
  virtual int _find (const char *opname,
                     TAO_Skeleton &skelfunc,
                     const unsigned int length = 0) = 0;

  /// Register a CORBA IDL operation name.
  virtual int _bind (const char *opname,
                     const TAO_Skeleton skel_ptr) = 0;

  /// Get this interface's repository id (TAO specific).
  virtual const char *_interface_repository_id (void) const = 0;

};


#include /**/ "ace/post.h"

#endif /* TAO_ABSTRACT_SERVANT_BASE_H_ */
