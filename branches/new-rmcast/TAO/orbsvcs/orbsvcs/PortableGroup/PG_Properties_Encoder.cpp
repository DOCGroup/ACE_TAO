/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    PG_Properties_Encoder.cpp
 *
 *  $Id$
 *
 *  This file implements classes to help manage the Properties
 *  defined in the Portable Object Group.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#include "PG_Properties_Encoder.h"


#if ! defined (__ACE_INLINE__)
#include "PG_Properties_Encoder.inl"
#endif /* !  __ACE_INLINE__ */

//////////
// Properties_Encoder
TAO_PG::Properties_Encoder::Properties_Encoder ()
{
}

TAO_PG::Properties_Encoder::~Properties_Encoder ()
{
}


void TAO_PG::Properties_Encoder::add (
  const char * name,
  const PortableGroup::Value & value)
{
  NamedValue nv(name, value);
  values_.push_back(nv);
}

void TAO_PG::Properties_Encoder::encode (
  PortableGroup::Properties * property_set) const
{
  ACE_ASSERT (property_set != 0);
  size_t count = values_.size();
  property_set->length(count);
  for( size_t nItem = 0; nItem < count; ++nItem )
  {
    const NamedValue & nv = values_[nItem];
    PortableGroup::Property & property = (*property_set)[nItem];
    PortableGroup::Name & nsName = property.nam;
    PortableGroup::Value & anyValue = property.val;
    // assign the value
    anyValue = (nv.value_);

    // assign the name
    // @@: This restricts the name to a single level with no "kind"
    // @@: remove this restriction (?)
    nsName.length(1);
    CosNaming::NameComponent & nc = nsName[0];

    nc.id = CORBA::string_dup (nv.name_.c_str());
    // nc.kind defaults to empty.  Leave it that way (for now)
  }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_Vector< TAO_PG::Properties_Encoder::NamedValue, 10 >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Vector< TAO_PG::Properties_Encoder::NamedValue, 10 >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
