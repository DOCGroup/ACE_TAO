// $Id$

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
