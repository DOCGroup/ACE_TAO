// -*- C++ -*-
//=============================================================================
/**
 *  @file    Any.h
 *
 *  $Id$
 *
 *  @authors  Carlos O'Ryan and Jeff Parsons
 */
//=============================================================================

#ifndef TAO_ANY_UNKNOWN_IDL_TYPE_H
#define TAO_ANY_UNKNOWN_IDL_TYPE_H

#include /**/ "ace/pre.h"

#include "tao/Any_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Char_Codeset_Translator;
class ACE_WChar_Codeset_Translator;

namespace TAO
{
  /**
   * @class Unknown_IDL_Type
   *
   * @brief CDR-based Any impl class.
   *
   * Used when demarshaling an Any, and with DII/DSI, and Dynamic Anys.
   */
  class TAO_Export Unknown_IDL_Type : public Any_Impl
  {
  public:
    Unknown_IDL_Type (CORBA::TypeCode_ptr,
                      const ACE_Message_Block *mb = 0,
                      int byte_order = TAO_ENCAP_BYTE_ORDER,
                      ACE_Char_Codeset_Translator *ctrans = 0,
                      ACE_WChar_Codeset_Translator *wtrans = 0);
    virtual ~Unknown_IDL_Type (void);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    virtual const void *value (void) const;
    virtual void free_value (void);

    virtual ACE_Message_Block *_tao_get_cdr (void) const;
    virtual int _tao_byte_order (void) const;

    virtual void _tao_decode (TAO_InputCDR &
                              ACE_ENV_ARG_DECL);
    virtual void assign_translator (CORBA::TypeCode_ptr,
                                    TAO_InputCDR *
                                    ACE_ENV_ARG_DECL);

    virtual CORBA::Boolean to_object (CORBA::Object_ptr &) const;
    virtual CORBA::Boolean to_value (CORBA::ValueBase *&) const;
    virtual CORBA::Boolean to_abstract_base (CORBA::AbstractBase_ptr &) const;

  private:
    ACE_Message_Block *cdr_;
    int byte_order_;
    ACE_Char_Codeset_Translator *char_translator_;
    ACE_WChar_Codeset_Translator *wchar_translator_;
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_ANY_UNKNOWN_IDL_TYPE_H */
