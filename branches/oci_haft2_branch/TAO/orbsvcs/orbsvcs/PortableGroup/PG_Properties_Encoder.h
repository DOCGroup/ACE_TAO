/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    PG_Properties_Encoder.h
 *
 *  $Id$
 *
 *  This file declares a class to help manage PortableGroup::Properties
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#ifndef TAO_PG_PROPERTIES_ENCODER_H
#define TAO_PG_PROPERTIES_ENCODER_H
#include "orbsvcs/PortableGroupS.h"
#include "orbsvcs/CosNamingC.h"
#include "portablegroup_export.h"

#include "ace/Vector_T.h"
#include "ace/Pair_T.h"

namespace TAO_PG
{
  /**
   * A helper to assemble a set of properties into a PortableGroup::Properties structure.
   *
   * To use a Properties_Encoder:
   *  Create it.
   *  Add properties to it using the add method.
   *  Allocate a new PortableGroup::Properties.
   *  Use the encode method to transfer the properties into the
   *  PortableGroup::Properties.
   */
  class TAO_PortableGroup_Export Properties_Encoder
  {
    typedef ACE_Pair< ACE_CString, PortableGroup::Value> NamedValue;
    typedef ACE_Vector< NamedValue, 10 > NamedValueVec;

  public:
    /**
     * Construct an empty set of properties.
     */
    Properties_Encoder ();

    /// standard destructor
    ~Properties_Encoder ();

    /**
     * add a name/value property to the Properties_Encoder.
     */
    void add (const char * name, const PortableGroup::Value & value);

    /**
     * Encode all properties in this Properties_Encoder into a PortableGroup::Properties.
     *
     */
    void encode (PortableGroup::Properties * property_set) const;

  private:
    Properties_Encoder (const Properties_Encoder & rhs);
    Properties_Encoder & operator = (const Properties_Encoder & rhs);
  private:
    NamedValueVec values_;
  };

} //namespace TAO_PG

#endif // TAO_PG_PROPERTIES_ENCODER_H
