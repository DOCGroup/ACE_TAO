// -*- C++ -*-

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

#include /**/ "ace/pre.h"
#include <ace/config-all.h>
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupS.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/PortableGroup/portablegroup_export.h"

#include "ace/Vector_T.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    struct NamedValue
    {
    public:
      ACE_CString name_;
      PortableGroup::Value value_;
      NamedValue ();
      NamedValue (const ACE_CString & name, const PortableGroup::Value & value);
      NamedValue (const NamedValue & rhs);
      NamedValue & operator = (const NamedValue & rhs);
      /// Meaningless method to keep ACE_Vector happy
      bool operator == (const NamedValue &rhs) const;
      /// Meaningless method to keep ACE_Vector happy
      bool operator != (const NamedValue &rhs) const;
    };
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

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/PortableGroup/PG_Properties_Encoder.inl"
#endif /* __ACE_INLINE__ */
#include /**/ "ace/post.h"
#endif // TAO_PG_PROPERTIES_ENCODER_H
