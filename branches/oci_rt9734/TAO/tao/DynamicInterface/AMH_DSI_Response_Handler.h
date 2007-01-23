// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    DynamicInterface
//
// = FILENAME
//    AMH_DSI_Response_Handler.h
//
// = DESCRIPTION
//    Header file for class TAO_AMH_DSI_Response_Handler
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_AMH_DSI_RESPONSE_HANDLER_H
#define TAO_AMH_DSI_RESPONSE_HANDLER_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Messaging/AMH_Response_Handler.h"
#include "tao/Valuetype/ValueBase.h"
#include "dynamicinterface_export.h"

class TAO_AMH_DSI_Response_Handler;
typedef TAO_AMH_DSI_Response_Handler *TAO_AMH_DSI_Response_Handler_ptr;

class TAO_DynamicInterface_Export TAO_AMH_DSI_Response_Handler_var
{
  // = TITLE
  //    The T_var class for Request.
  //
  // = DESCRIPTION
  //    As any other pseudo object Request must have a T_var class,
  //    the interface an semantics are specified in the CORBA spec.
  //
  // = NOTE
  //    We use TAO_AMH_DSI_Response_Handler_ptr as the _ptr type instead of
  //    CORBA::Request_ptr, this is an attempt to reduced the cyclic
  //    dependencies in TAO.
  //
public:
  TAO_AMH_DSI_Response_Handler_var (void);
  TAO_AMH_DSI_Response_Handler_var (TAO_AMH_DSI_Response_Handler_ptr);
  TAO_AMH_DSI_Response_Handler_var (const TAO_AMH_DSI_Response_Handler_var &);
  ~TAO_AMH_DSI_Response_Handler_var (void);

  TAO_AMH_DSI_Response_Handler_var &operator= (TAO_AMH_DSI_Response_Handler_ptr);
  TAO_AMH_DSI_Response_Handler_var &operator= (const TAO_AMH_DSI_Response_Handler_var &);
  TAO_AMH_DSI_Response_Handler_ptr operator-> (void) const;

  operator const TAO_AMH_DSI_Response_Handler_ptr &() const;
  operator TAO_AMH_DSI_Response_Handler_ptr &();

  // in, inout, out, _retn.
  TAO_AMH_DSI_Response_Handler_ptr in (void) const;
  TAO_AMH_DSI_Response_Handler_ptr &inout (void);
  TAO_AMH_DSI_Response_Handler_ptr &out (void);
  TAO_AMH_DSI_Response_Handler_ptr _retn (void);
  TAO_AMH_DSI_Response_Handler_ptr ptr (void) const;

private:
  TAO_AMH_DSI_Response_Handler_ptr ptr_;
};




class TAO_AMH_DSI_Exception_Holder;
typedef TAO_AMH_DSI_Exception_Holder *TAO_AMH_DSI_Exception_Holder_ptr;

  // TAO_IDL - Generated from
  // be/be_valuetype.cpp:183

class TAO_DynamicInterface_Export TAO_AMH_DSI_Exception_Holder_var
{
public:
  TAO_AMH_DSI_Exception_Holder_var (void);
  TAO_AMH_DSI_Exception_Holder_var (TAO_AMH_DSI_Exception_Holder*);
  TAO_AMH_DSI_Exception_Holder_var (const TAO_AMH_DSI_Exception_Holder*); // (TAO extension)
  TAO_AMH_DSI_Exception_Holder_var (const TAO_AMH_DSI_Exception_Holder_var &);
  ~TAO_AMH_DSI_Exception_Holder_var (void);

  TAO_AMH_DSI_Exception_Holder_var &operator= (TAO_AMH_DSI_Exception_Holder*);
  TAO_AMH_DSI_Exception_Holder_var &operator= (const TAO_AMH_DSI_Exception_Holder_var &);
  TAO_AMH_DSI_Exception_Holder* operator-> (void) const;

  operator const TAO_AMH_DSI_Exception_Holder* () const;
  operator TAO_AMH_DSI_Exception_Holder*& ();
  // in, inout, out, _retn
  TAO_AMH_DSI_Exception_Holder* in (void) const;
  TAO_AMH_DSI_Exception_Holder*& inout (void);
  TAO_AMH_DSI_Exception_Holder*& out (void);
  TAO_AMH_DSI_Exception_Holder* _retn (void);
  TAO_AMH_DSI_Exception_Holder* ptr (void) const;

  // Hooks used by template sequence and valuetype manager classes
  // for non-defined forward declared valuetypes.
  static void tao_add_ref (TAO_AMH_DSI_Exception_Holder *);
  static void tao_remove_ref (TAO_AMH_DSI_Exception_Holder *);

private:
  TAO_AMH_DSI_Exception_Holder* ptr_;
};

class TAO_DynamicInterface_Export TAO_AMH_DSI_Exception_Holder_out
{
public:
  TAO_AMH_DSI_Exception_Holder_out (TAO_AMH_DSI_Exception_Holder* &);
  TAO_AMH_DSI_Exception_Holder_out (TAO_AMH_DSI_Exception_Holder_var &);
  TAO_AMH_DSI_Exception_Holder_out (const TAO_AMH_DSI_Exception_Holder_out &);
  TAO_AMH_DSI_Exception_Holder_out &operator= (const TAO_AMH_DSI_Exception_Holder_out &);
  TAO_AMH_DSI_Exception_Holder_out &operator= (const TAO_AMH_DSI_Exception_Holder_var &);
  TAO_AMH_DSI_Exception_Holder_out &operator= (TAO_AMH_DSI_Exception_Holder*);
  operator TAO_AMH_DSI_Exception_Holder*& ();
  TAO_AMH_DSI_Exception_Holder*& ptr (void);
  TAO_AMH_DSI_Exception_Holder* operator-> (void);

private:
  TAO_AMH_DSI_Exception_Holder* &ptr_;
};

class TAO_DynamicInterface_Export TAO_AMH_DSI_Exception_Holder
  : public virtual CORBA::DefaultValueRefCountBase,
    public virtual CORBA::ValueBase
{
public:
  typedef TAO_AMH_DSI_Exception_Holder_var _var_type;
  TAO_AMH_DSI_Exception_Holder (CORBA::Exception *ex)
  { this->exception = ex; }

  virtual ~TAO_AMH_DSI_Exception_Holder (void);

  static TAO_AMH_DSI_Exception_Holder* _downcast ( CORBA::ValueBase* );
  // The address of static _downcast is implicit used as type id

  // (TAO extensions or internals)
  static CORBA::Boolean _tao_unmarshal ( TAO_InputCDR &,
                                         TAO_AMH_DSI_Exception_Holder *& );
  virtual const char* _tao_obv_repository_id (void) const;
  static const char* _tao_obv_static_repository_id (void);

  static void _tao_any_destructor (void *);

  virtual void raise_invoke ( ACE_ENV_SINGLE_ARG_DECL )
    ACE_THROW_SPEC (( CORBA::SystemException ));

protected:
  TAO_AMH_DSI_Exception_Holder (void);
  // *** Terrible Hack ? ***
  virtual CORBA::Boolean _tao_marshal_v (TAO_OutputCDR &) const { return 1; }
  virtual CORBA::Boolean _tao_unmarshal_v (TAO_InputCDR &) { return 1; }

private:
  TAO_AMH_DSI_Exception_Holder (const TAO_AMH_DSI_Exception_Holder &);
  void operator= (const TAO_AMH_DSI_Exception_Holder &);
  CORBA::Exception *exception;

protected:
  virtual CORBA::Boolean _tao_marshal__Test_TAO_AMH_DSI_Exception_Holder (TAO_OutputCDR &) {return 1;}
  virtual CORBA::Boolean _tao_unmarshal__Test_TAO_AMH_DSI_Exception_Holder (TAO_InputCDR &) {return 1;}
};

class TAO_DynamicInterface_Export TAO_AMH_DSI_Response_Handler
  : public virtual TAO_AMH_Response_Handler
{
  // = TITLE
  //    "interface" class for DSI using AMH
  //
  // = DESCRIPTION
  //    With ordinary DSI, the POA calls _dispatch, which in turn calls invoke
  //    on the DSI handler. Invoke completely handles the request, and packages
  //    the result in the request object so that it can be marshalled and sent
  //    back to the caller.
  //
  //    With the AMH variation, the DSI handler's invoke method starts the
  //    processing, but delegates the result packaging to a response handler.
  //    This way, if the invocation takes a long time, or involves blocking
  //    calls, then the result handling can be managed by another thread.
  //    This is particularly useful for gateway implementations, where the
  //    middle tier should not have to block waiting for replies.
  //
  //    As this is built on the CORBA object model, there are separate classes
  //    representing the interface and the implementation.
public:
  typedef TAO_AMH_DSI_Response_Handler_ptr _ptr_type;
  typedef TAO_AMH_DSI_Response_Handler_var _var_type;

  TAO_AMH_DSI_Response_Handler (TAO_ServerRequest &sr);
  ~TAO_AMH_DSI_Response_Handler (void);

  virtual void invoke_reply (CORBA::NVList_ptr args,
                             CORBA::NamedValue_ptr result
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // The invoke() method receives requests issued to any CORBA object
  // incarnated by the DSI servant and performs the processing
  // necessary to execute the request.

  virtual void invoke_excep ( TAO_AMH_DSI_Exception_Holder * h
                              ACE_ENV_ARG_DECL )
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual void gateway_exception_reply (CORBA::ULong reply_status,
                                        TAO_OutputCDR &encap);

#if 0
  virtual void _primary_interface_reply (CORBA::RepositoryId *result
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // The _primary_interface() method receives an ObjectId value and a
  // POA_ptr as input parameters and returns a valid RepositoryId
  // representing the most-derived interface for that oid.
#endif

  CORBA::Object_ptr _this (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Returns a CORBA::Object_ptr for the target object.
#if 0
  virtual CORBA_InterfaceDef_ptr _get_interface (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    );
  // Query the Interface Repository for the interface definition.
#endif
  // CORBA Object related methods
  static TAO_AMH_DSI_Response_Handler* _duplicate (TAO_AMH_DSI_Response_Handler_ptr);
  static TAO_AMH_DSI_Response_Handler* _nil (void);
  static TAO_AMH_DSI_Response_Handler* _narrow (CORBA::Object_ptr
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  static TAO_AMH_DSI_Response_Handler* _unchecked_narrow (CORBA::Object_ptr
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS);

protected:

  virtual const char *_interface_repository_id (void) const;
  // Return 0.  Should never be used.

  virtual void *_downcast (const char *repository_id);
  // Simply returns "this"

private:
  TAO_ORB_Core *orb_core_;
  // TODO: why not
  //  TAO_GIOP_ReplyStatusType exception_type_;
  /// Exception type (will be NO_EXCEPTION in the majority of the
  /// cases).
  // @@ Mayur: I do not think we need this one, we can deduce the type
  //    of reply depending on the _tao_rh_*() method called.
  CORBA::ULong exception_type_;
};

#if defined (__ACE_INLINE__)
# include "AMH_DSI_Response_Handler.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_DII_RESPONSE_HANDLER_H */
