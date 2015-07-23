// -*- C++ -*-

//==============================================================
/**
 *  @file DII_Reply_Handler.h
 *
 *  @brief Handler of DII twoway asynch replies type.
 */
// =============================================================

#ifndef TAO_DII_REPLY_HANDLER_H
#define TAO_DII_REPLY_HANDLER_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "dynamicinterface_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Forward declaration
class TAO_DII_Reply_Handler;
typedef TAO_DII_Reply_Handler* TAO_DII_Reply_Handler_ptr;

/**
 * @class TAO_DII_Reply_Handler_var
 *
 * @brief The T_var class for reply handler
 *
 * As any other pseudo object, this class has a T_var class too.
 */
class TAO_DynamicInterface_Export TAO_DII_Reply_Handler_var
{
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

  TAO_DII_Reply_Handler_ptr in (void) const;
  TAO_DII_Reply_Handler_ptr &inout (void);
  TAO_DII_Reply_Handler_ptr &out (void);
  TAO_DII_Reply_Handler_ptr _retn (void);
  TAO_DII_Reply_Handler_ptr ptr (void) const;

private:
  TAO_DII_Reply_Handler_ptr ptr_;
};

/**
 * @class TAO_DII_Reply_Handler
 *
 * @brief The class for DII reply handler
 *
 * Provides a way to create requests and populate it with parameters for
 * use in the Dynamic Invocation Interface.
 */
class TAO_DynamicInterface_Export TAO_DII_Reply_Handler
  : public virtual ::CORBA::LocalObject
{
public:
  TAO_DII_Reply_Handler ();
  virtual ~TAO_DII_Reply_Handler (void);

  /// Callback method for asynchronous requests.
  virtual void handle_response (TAO_InputCDR &incoming) = 0;

  virtual void handle_excep (TAO_InputCDR &incoming,
                             CORBA::ULong reply_status) = 0;

  /// Handle a location forward message. This one has a default method
  /// supplied that simply forwards to the handle_response, since that
  /// is what the legacy code did. This way we maintain backwards
  /// compatibility.
  virtual void handle_location_forward (TAO_InputCDR &incoming,
                                        CORBA::ULong reply_status);

  /// CORBA Object related methods
  static TAO_DII_Reply_Handler* _duplicate (TAO_DII_Reply_Handler_ptr);
  static TAO_DII_Reply_Handler* _nil (void);
  static TAO_DII_Reply_Handler* _narrow (CORBA::Object_ptr);
  static TAO_DII_Reply_Handler* _unchecked_narrow (CORBA::Object_ptr);

  static int _tao_class_id;

  typedef TAO_DII_Reply_Handler_ptr _ptr_type;
  typedef TAO_DII_Reply_Handler_var _var_type;

#if defined (ACE_WIN32_VC14)
  // Workaround for connect issue 1577211
  ACE_UNIMPLEMENTED_FUNC (TAO_DII_Reply_Handler (const TAO_DII_Reply_Handler &))
  ACE_UNIMPLEMENTED_FUNC (TAO_DII_Reply_Handler &operator = (const TAO_DII_Reply_Handler &))
#endif

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "DII_Reply_Handler.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_DII_REPLY_HANDLER_H */
