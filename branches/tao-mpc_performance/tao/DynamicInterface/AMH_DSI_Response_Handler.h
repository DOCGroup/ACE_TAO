// -*- C++ -*-

//=====================================================================
/**
 *  @file AMH_DSI_Response_Handler.h
 *
 *  $Id$
 *
 *  @brief Handle the Response appropriately.
 */
//=====================================================================

#ifndef TAO_AMH_DSI_RESPONSE_HANDLER_H
#define TAO_AMH_DSI_RESPONSE_HANDLER_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Messaging/AMH_Response_Handler.h"
#include "tao/Valuetype/ValueBase.h"
#include "dynamicinterface_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_AMH_DSI_Response_Handler;
typedef TAO_AMH_DSI_Response_Handler *TAO_AMH_DSI_Response_Handler_ptr;

/**
 * @class TAO_AMH_DSI_Response_Handler_var
 *
 * @brief The T_var class for response handler
 *
 * As any other pseudo object, this class has a T_var class too.
 *
 */
class TAO_DynamicInterface_Export TAO_AMH_DSI_Response_Handler_var
{
public:
  TAO_AMH_DSI_Response_Handler_var (void);
  TAO_AMH_DSI_Response_Handler_var (TAO_AMH_DSI_Response_Handler_ptr);
  TAO_AMH_DSI_Response_Handler_var (
      const TAO_AMH_DSI_Response_Handler_var &);
  ~TAO_AMH_DSI_Response_Handler_var (void);

  TAO_AMH_DSI_Response_Handler_var &operator= (
      TAO_AMH_DSI_Response_Handler_ptr);
  TAO_AMH_DSI_Response_Handler_var &operator= (
      const TAO_AMH_DSI_Response_Handler_var &);
  TAO_AMH_DSI_Response_Handler_ptr operator-> (void) const;

  operator const TAO_AMH_DSI_Response_Handler_ptr &() const;
  operator TAO_AMH_DSI_Response_Handler_ptr &();

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

/**
 * @class TAO_AMH_DSI_Exception_Holder_var
 *
 * @brief The T_var class for exception holder
 *
 * As any other pseudo object, this class has a T_var class too.
 *
 */
class TAO_DynamicInterface_Export TAO_AMH_DSI_Exception_Holder_var
{
public:
  TAO_AMH_DSI_Exception_Holder_var (void);
  TAO_AMH_DSI_Exception_Holder_var (TAO_AMH_DSI_Exception_Holder*);
  TAO_AMH_DSI_Exception_Holder_var (const TAO_AMH_DSI_Exception_Holder*);
  TAO_AMH_DSI_Exception_Holder_var (const TAO_AMH_DSI_Exception_Holder_var &);
  ~TAO_AMH_DSI_Exception_Holder_var (void);

  TAO_AMH_DSI_Exception_Holder_var &operator= (TAO_AMH_DSI_Exception_Holder*);
  TAO_AMH_DSI_Exception_Holder_var &operator= (
      const TAO_AMH_DSI_Exception_Holder_var &);
  TAO_AMH_DSI_Exception_Holder* operator-> (void) const;

  operator const TAO_AMH_DSI_Exception_Holder* () const;
  operator TAO_AMH_DSI_Exception_Holder*& ();

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

/**
 * @class TAO_AMH_DSI_Exception_Holder_out
 *
 * @brief The T_out class for exception holder
 *
 */
class TAO_DynamicInterface_Export TAO_AMH_DSI_Exception_Holder_out
{
public:
  TAO_AMH_DSI_Exception_Holder_out (TAO_AMH_DSI_Exception_Holder* &);
  TAO_AMH_DSI_Exception_Holder_out (TAO_AMH_DSI_Exception_Holder_var &);
  TAO_AMH_DSI_Exception_Holder_out (const TAO_AMH_DSI_Exception_Holder_out &);
  TAO_AMH_DSI_Exception_Holder_out &operator= (
      const TAO_AMH_DSI_Exception_Holder_out &);
  TAO_AMH_DSI_Exception_Holder_out &operator= (
      const TAO_AMH_DSI_Exception_Holder_var &);
  TAO_AMH_DSI_Exception_Holder_out &operator= (
      TAO_AMH_DSI_Exception_Holder*);
  operator TAO_AMH_DSI_Exception_Holder*& ();
  TAO_AMH_DSI_Exception_Holder*& ptr (void);
  TAO_AMH_DSI_Exception_Holder* operator-> (void);

private:
  TAO_AMH_DSI_Exception_Holder* &ptr_;
};

/**
 * @class TAO_AMH_DSI_Exception_Holder
 *
 * @brief This is the class for holding exception.
 *
 */
class TAO_DynamicInterface_Export TAO_AMH_DSI_Exception_Holder
  : public virtual CORBA::DefaultValueRefCountBase,
    public virtual CORBA::ValueBase
{
public:
  typedef TAO_AMH_DSI_Exception_Holder_var _var_type;
  TAO_AMH_DSI_Exception_Holder (CORBA::Exception *ex);

  virtual ~TAO_AMH_DSI_Exception_Holder (void);

  static TAO_AMH_DSI_Exception_Holder* _downcast ( CORBA::ValueBase* );

  static CORBA::Boolean _tao_unmarshal ( TAO_InputCDR &,
                                         TAO_AMH_DSI_Exception_Holder *& );
  virtual const char* _tao_obv_repository_id (void) const;
  static const char* _tao_obv_static_repository_id (void);

  static void _tao_any_destructor (void *);

  virtual void raise_invoke ( );

  CORBA::TypeCode_ptr _tao_type (void) const;
  void _tao_obv_truncatable_repo_ids (Repository_Id_List &) const;
  CORBA::Boolean _tao_match_formal_type (ptrdiff_t ) const;

protected:
  TAO_AMH_DSI_Exception_Holder (void);
  ///  *** Terrible Hack ? ***
  virtual CORBA::Boolean _tao_marshal_v (TAO_OutputCDR &) const;
  virtual CORBA::Boolean _tao_unmarshal_v (TAO_InputCDR &);

private:
  TAO_AMH_DSI_Exception_Holder (const TAO_AMH_DSI_Exception_Holder &);
  void operator= (const TAO_AMH_DSI_Exception_Holder &);
  CORBA::Exception *exception_;

protected:
  virtual CORBA::Boolean _tao_marshal__Test_TAO_AMH_DSI_Exception_Holder (
              TAO_OutputCDR &);
  virtual CORBA::Boolean _tao_unmarshal__Test_TAO_AMH_DSI_Exception_Holder (
              TAO_InputCDR &);
};

/**
 * @class TAO_AMH_DSI_Response_Handler
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
class TAO_DynamicInterface_Export TAO_AMH_DSI_Response_Handler
  : public virtual TAO_AMH_Response_Handler
{
public:
  typedef TAO_AMH_DSI_Response_Handler_ptr _ptr_type;
  typedef TAO_AMH_DSI_Response_Handler_var _var_type;

  TAO_AMH_DSI_Response_Handler (TAO_ServerRequest &sr);
  ~TAO_AMH_DSI_Response_Handler (void);

  virtual void invoke_reply (CORBA::NVList_ptr args,
                             CORBA::NamedValue_ptr result);

  virtual void invoke_excep ( TAO_AMH_DSI_Exception_Holder * h);

  virtual void invoke_location_forward (CORBA::Object_ptr fwd,
                                        CORBA::Boolean is_perm);

  virtual void gateway_exception_reply (CORBA::ULong reply_status,
                                        TAO_OutputCDR &encap);

  virtual void gateway_exception_reply (CORBA::ULong reply_status,
                                        TAO_InputCDR &encap);

  /// Returns a CORBA::Object_ptr for the target object.
  CORBA::Object_ptr _this ();

  /// CORBA Object related methods
  static TAO_AMH_DSI_Response_Handler* _duplicate (
             TAO_AMH_DSI_Response_Handler_ptr);
  static TAO_AMH_DSI_Response_Handler* _nil (void);
  static TAO_AMH_DSI_Response_Handler* _narrow (CORBA::Object_ptr);
  static TAO_AMH_DSI_Response_Handler* _unchecked_narrow (CORBA::Object_ptr);

protected:

  /// Return 0.  Should never be used.
  virtual const char *_interface_repository_id (void) const;

  /// Simply returns "this"
  virtual void *_downcast (const char *repository_id);

private:
  TAO_ORB_Core *orb_core_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "AMH_DSI_Response_Handler.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_AMH_DSI_RESPONSE_HANDLER_H */
