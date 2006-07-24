// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_CEC_Pull_Event::TAO_CEC_Pull_Event (TAO_CEC_ConsumerAdmin *consumer_admin,
                                        TAO_CEC_SupplierControl *control)
  :  consumer_admin_ (consumer_admin),
     supplier_control_ (control)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
