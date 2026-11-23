
/**
 *  @file    Any_Unknown_IDL_Type.h
 *
 *  @author Carlos O'Ryan
 *  @author Jeff Parsons
 */

#ifndef TAO_ANY_UNKNOWN_IDL_TYPE_H
#define TAO_ANY_UNKNOWN_IDL_TYPE_H

#include /**/ "ace/pre.h"

#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Lock_Adapter_T.h"

#include "tao/AnyTypeCode/Any_Impl.h"
#include "tao/CDR.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
    Unknown_IDL_Type () = delete;
    Unknown_IDL_Type (Unknown_IDL_Type const &) = delete;
    Unknown_IDL_Type (Unknown_IDL_Type &&) = delete;
    Unknown_IDL_Type &operator= (Unknown_IDL_Type const &) = delete;
    Unknown_IDL_Type &operator= (Unknown_IDL_Type &&) = delete;

    Unknown_IDL_Type (CORBA::TypeCode_ptr,
                      TAO_InputCDR &cdr);

    explicit Unknown_IDL_Type (CORBA::TypeCode_ptr);

    ~Unknown_IDL_Type () override;

    CORBA::Boolean marshal_value (TAO_OutputCDR &) override;
    void const *value () const;
    void free_value () override;

    TAO_InputCDR &_tao_get_cdr ();
    int _tao_byte_order () const override;

    void _tao_decode (TAO_InputCDR &) override;

    CORBA::Boolean to_object (CORBA::Object_ptr &) const override;
    CORBA::Boolean to_value (CORBA::ValueBase *&) const override;
    CORBA::Boolean to_abstract_base (CORBA::AbstractBase_ptr &) const override;

  private:
    /// A shared lock protects copies of the CDR state held by Any values.
    /// The function-local static avoids initialization-order problems, and
    /// the reference-counted pointer keeps the lock alive while instances
    /// use it.
    using LOCK = ACE_Refcounted_Auto_Ptr<ACE_Lock,
                                         ACE_Lock_Adapter<TAO_SYNCH_MUTEX> >;
    static LOCK const lock_i ();
    LOCK const lock_;
    mutable TAO_InputCDR cdr_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ANY_UNKNOWN_IDL_TYPE_H */
