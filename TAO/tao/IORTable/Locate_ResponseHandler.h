// -*- C++ -*-


//=============================================================================
/**
 *  @file    Locate_ResponseHandler.h
 *
 *  $Id$
 *
 *  @author Phil Mesnier <mesnier_p@ociweb>
 */
//=============================================================================


#ifndef TAO_LOCATE_RESPONSEHANDLER_H
#define TAO_LOCATE_RESPONSEHANDLER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "async_iortable_export.h"
#include "tao/Versioned_Namespace.h"
#include "tao/Messaging/AMH_Response_Handler.h"
#include "tao/Valuetype/ValueBase.h"
#include "tao/ORB.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Exception;
}

class TAO_AMH_Locate_ResponseHandler;

typedef TAO_AMH_Locate_ResponseHandler *TAO_AMH_Locate_ResponseHandler_ptr;

/**
 * @class TAO_AMH_Locate_ResponseHandler_var
 *
 * @brief The T_var class for response handler
 *
 * As any other pseudo object, this class has a T_var class too.
 *
 */
class TAO_Async_IORTable_Export TAO_AMH_Locate_ResponseHandler_var
{
public:
  TAO_AMH_Locate_ResponseHandler_var (void);
  TAO_AMH_Locate_ResponseHandler_var (TAO_AMH_Locate_ResponseHandler_ptr);
  TAO_AMH_Locate_ResponseHandler_var (
      const TAO_AMH_Locate_ResponseHandler_var &);
  ~TAO_AMH_Locate_ResponseHandler_var (void);

  TAO_AMH_Locate_ResponseHandler_var &operator= (
      TAO_AMH_Locate_ResponseHandler_ptr);
  TAO_AMH_Locate_ResponseHandler_var &operator= (
      const TAO_AMH_Locate_ResponseHandler_var &);
  TAO_AMH_Locate_ResponseHandler_ptr operator-> (void) const;

  operator const TAO_AMH_Locate_ResponseHandler_ptr &() const;
  operator TAO_AMH_Locate_ResponseHandler_ptr &();

  TAO_AMH_Locate_ResponseHandler_ptr in (void) const;
  TAO_AMH_Locate_ResponseHandler_ptr &inout (void);
  TAO_AMH_Locate_ResponseHandler_ptr &out (void);
  TAO_AMH_Locate_ResponseHandler_ptr _retn (void);
  TAO_AMH_Locate_ResponseHandler_ptr ptr (void) const;

private:
  TAO_AMH_Locate_ResponseHandler_ptr ptr_;
};


/**
 * @class TAO_AMH_Locate_ResponseHandler
 *
 * @brief This is the main class for DSI using AMH.
 *
 * With ordinary DSI, the POA calls _dispatch, which in turn calls invoke
 * on the DSI handler. Invoke completely handles the request, and packages
 * the result in the request object so that it can be marshalled and sent
 * back to the caller.
 *
 * With the AMH variation, the DSI handler's invoke method starts the
 * processing, but delegates the result packaging to a response handler.
 * This way, if the invocation takes a long time, or involves blocking
 * calls, then the result handling can be managed by another thread.
 * This is particularly useful for gateway implementations, where the
 * middle tier should not have to block waiting for replies.
 *
 * As this is built on the CORBA object model, there are separate classes
 * representing the interface and the implementation.
 *
 */
class TAO_Async_IORTable_Export TAO_AMH_Locate_ResponseHandler
  : public virtual TAO_AMH_Response_Handler
{
public:
  typedef TAO_AMH_Locate_ResponseHandler_ptr _ptr_type;
  typedef TAO_AMH_Locate_ResponseHandler_var _var_type;

  TAO_AMH_Locate_ResponseHandler (TAO_ServerRequest &sr);
  ~TAO_AMH_Locate_ResponseHandler (void);

  virtual void raise_excep (const CORBA::Exception &ex);

  virtual void forward_ior (const char *ior, CORBA::Boolean is_perm);

  /// Returns a CORBA::Object_ptr for the target object.
  CORBA::Object_ptr _this ();

  /// CORBA Object related methods
  static TAO_AMH_Locate_ResponseHandler* _duplicate (
             TAO_AMH_Locate_ResponseHandler_ptr);
  static TAO_AMH_Locate_ResponseHandler* _nil (void);
  static TAO_AMH_Locate_ResponseHandler* _narrow (CORBA::Object_ptr);
  static TAO_AMH_Locate_ResponseHandler* _unchecked_narrow (CORBA::Object_ptr);

protected:

  /// Return 0.  Should never be used.
  virtual const char *_interface_repository_id (void) const;

  /// Simply returns "this"
  virtual void *_downcast (const char *repository_id);

private:
  CORBA::ORB_var orb_;
};


namespace IORTable
{
  // satisfy the native type requirement for the asnyc locate interface
  typedef const TAO_AMH_Locate_ResponseHandler_var &Locate_ResponseHandler;

}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "Locate_ResponseHandler.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_LOCATE_RESPONSEHANDLER_H */
