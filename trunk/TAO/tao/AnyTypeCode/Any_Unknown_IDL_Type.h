// -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_Unknown_IDL_Type.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_ANY_UNKNOWN_IDL_TYPE_H
#define TAO_ANY_UNKNOWN_IDL_TYPE_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/Any_Impl.h"
#include "tao/CDR.h"

#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Char_Codeset_Translator;
class ACE_WChar_Codeset_Translator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class Unknown_IDL_Type
   *
   * @brief CDR-based Any impl class.
   *
   * Used when demarshaling an Any, and with DII/DSI, and Dynamic Anys.
   */
  class TAO_AnyTypeCode_Export Unknown_IDL_Type : public Any_Impl
  {
  public:
    Unknown_IDL_Type (CORBA::TypeCode_ptr,
                      TAO_InputCDR &cdr);

    Unknown_IDL_Type (CORBA::TypeCode_ptr);

    virtual ~Unknown_IDL_Type (void);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    virtual const void *value (void) const;
    virtual void free_value (void);

    virtual TAO_InputCDR &_tao_get_cdr (void);
    virtual int _tao_byte_order (void) const;

    virtual void _tao_decode (TAO_InputCDR &
                              ACE_ENV_ARG_DECL);

    virtual CORBA::Boolean to_object (CORBA::Object_ptr &) const;
    virtual CORBA::Boolean to_value (CORBA::ValueBase *&) const;
    virtual CORBA::Boolean to_abstract_base (CORBA::AbstractBase_ptr &) const;

  private:
    // We make the lock global, so that it won't  be deleted when shared.
    // For instance, see Any_Basic_Impl::extract() which copies the insides
    // from an Unknown_IDL_Type to an Any_Basic_Impl.
    static ACE_Auto_Ptr<ACE_Lock> lock_;
    mutable TAO_InputCDR cdr_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ANY_UNKNOWN_IDL_TYPE_H */
