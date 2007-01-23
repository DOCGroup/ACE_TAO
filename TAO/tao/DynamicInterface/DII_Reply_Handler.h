// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//    TAO_DynamicInvocation
//
// = FILENAME
//    DII_Reply_Handler.h
//
// = DESCRIPTION
//    Header file for handler of DII twoway asynch replies
//    type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
//     Adapted from Request.h by Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_DII_REPLY_HANDLER_H
#define TAO_DII_REPLY_HANDLER_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "dynamicinterface_export.h"

class TAO_DII_Reply_Handler;
typedef TAO_DII_Reply_Handler* TAO_DII_Reply_Handler_ptr;

class TAO_DynamicInterface_Export TAO_DII_Reply_Handler_var
{
  // = TITLE
  //    The T_var class for Request.
  //
  // = DESCRIPTION
  //    As any other pseudo object Request must have a T_var class,
  //    the interface an semantics are specified in the CORBA spec.
  //
  // = NOTE
  //    We use TAO_DII_Reply_Handler_ptr as the _ptr type instead of
  //    CORBA::Request_ptr, this is an attempt to reduced the cyclic
  //    dependencies in TAO.
  //
public:
  TAO_DII_Reply_Handler_var (void);
  TAO_DII_Reply_Handler_var (TAO_DII_Reply_Handler_ptr);
  TAO_DII_Reply_Handler_var (const TAO_DII_Reply_Handler_var &);
  ~TAO_DII_Reply_Handler_var (void);

  TAO_DII_Reply_Handler_var &operator= (TAO_DII_Reply_Handler_ptr);
  TAO_DII_Reply_Handler_var &operator= (const TAO_DII_Reply_Handler_var &);
  TAO_DII_Reply_Handler_ptr operator-> (void) const;

  operator const TAO_DII_Reply_Handler_ptr &() const;
  operator TAO_DII_Reply_Handler_ptr &();

  // in, inout, out, _retn.
  TAO_DII_Reply_Handler_ptr in (void) const;
  TAO_DII_Reply_Handler_ptr &inout (void);
  TAO_DII_Reply_Handler_ptr &out (void);
  TAO_DII_Reply_Handler_ptr _retn (void);
  TAO_DII_Reply_Handler_ptr ptr (void) const;

private:
  TAO_DII_Reply_Handler_ptr ptr_;
};

class TAO_DynamicInterface_Export TAO_DII_Reply_Handler
  : public virtual TAO_Local_RefCounted_Object
{
  // = TITLE
  //    TAO_DII_Reply_Handler
  //
  // = DESCRIPTION
  //    Provides a way to create requests and populate it with parameters for
  //    use in the Dynamic Invocation Interface.
  //
public:
  TAO_DII_Reply_Handler ();
  virtual ~TAO_DII_Reply_Handler (void);

  // Callback method for deferred synchronous requests.
  virtual void handle_response (TAO_InputCDR &incoming
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

  virtual void handle_excep (TAO_InputCDR &incoming,
                             CORBA::ULong reply_status
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

  // CORBA Object related methods
  static TAO_DII_Reply_Handler* _duplicate (TAO_DII_Reply_Handler_ptr);
  static TAO_DII_Reply_Handler* _nil (void);
  static TAO_DII_Reply_Handler* _narrow (CORBA::Object_ptr
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  static TAO_DII_Reply_Handler* _unchecked_narrow (CORBA::Object_ptr
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  static int _tao_class_id;

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef TAO_DII_Reply_Handler_ptr _ptr_type;
  typedef TAO_DII_Reply_Handler_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

};

#if defined (__ACE_INLINE__)
# include "DII_Reply_Handler.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_DII_REPLY_HANDLER_H */
