
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
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_ABSTRACT_SERVANT_BASE_H_
#define TAO_ABSTRACT_SERVANT_BASE_H_

// -- ACE Include --
#include "ace/pre.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ServerRequest;

class TAO_Export TAO_Abstract_ServantBase
{
public:
  /// Destructor
  virtual ~TAO_Abstract_ServantBase (void);

  /// Local implementation of the CORBA::Object::_is_a method.
  virtual CORBA::Boolean _is_a (const char* logical_type_id,
                                CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()) = 0;

  /// Default <_non_existent>: always returns false.
  virtual CORBA::Boolean _non_existent (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()) = 0;

  //@{
  /// @name Reference counting hooks: no-ops by default.
  virtual void _add_ref (CORBA_Environment &ACE_TRY_ENV =
                         TAO_default_environment ());
  virtual void _remove_ref (CORBA_Environment &ACE_TRY_ENV =
                            TAO_default_environment ());
  //@}

  /// Get the correct vtable.
  virtual void *_downcast (const char *repository_id) = 0;

  /// This is an auxiliar method for _this() and _narrow().
  virtual TAO_Stub *_create_stub (CORBA_Environment &ACE_TRY_ENV =
                                  TAO_default_environment ()) = 0;

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
   * any) or the exceptions thrown into <request>.
   */
  virtual void _dispatch (TAO_ServerRequest &request,
                          void *context,
                          CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()) = 0;

  virtual void synchronous_upcall_dispatch (TAO_ServerRequest &req,
                                            void *context,
                                            void *derived_this,
                                            CORBA::Environment &ACE_TRY_ENV) = 0;

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

#include "ace/post.h"
#endif /* TAO_ABSTRACT_SERVANT_BASE_H_ */
