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
#include "ace/pre.h"
#include "PG_Properties_Encoder.h"

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
  PortableGroup::Properties_var & property_set) const
{
  size_t count = values_.size();
  property_set->length(count);
  for( size_t nItem = 0; nItem < count; ++nItem )
  {
    const NamedValue & nv = values_[nItem];
    PortableGroup::Property & property = property_set[nItem];
    PortableGroup::Name & nsName = property.nam;
    PortableGroup::Value & anyValue = property.val;
    // assign the value
    anyValue = (nv.second());

    // assign the name
    // TODO: This restricts the name to a single level with no "kind"
    // TODO: remove this restriction (?)
    nsName.length(1);
    CosNaming::NameComponent & nc = nsName[0];

    nc.id = CORBA::string_dup (nv.first().c_str());
    // nc.kind defaults to empty.  Leave it that way (for now)

  }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template ACE_Pair< ACE_CString, PortableGroup::Value>;
  template ACE_Vector< NamedValue, 10 >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Pair< ACE_CString, PortableGroup::Value>
# pragma instantiate ACE_Vector< NamedValue, 10 >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
