// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_CEC_Ping_Push_Supplier::
    TAO_CEC_Ping_Push_Supplier (TAO_CEC_SupplierControl *control)
  :  control_ (control)
{
}

// ****************************************************************

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
ACE_INLINE
TAO_CEC_Ping_Typed_Push_Supplier::
    TAO_CEC_Ping_Typed_Push_Supplier (TAO_CEC_SupplierControl *control)
  :  control_ (control)
{
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

// ****************************************************************

ACE_INLINE
TAO_CEC_Ping_Pull_Supplier::
    TAO_CEC_Ping_Pull_Supplier (TAO_CEC_SupplierControl *control)
  :  control_ (control)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
