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
  class TAO_PortableGroup_Export Properties_Encoder
  {
    typedef ACE_Pair< ACE_CString, PortableGroup::Value> NamedValue;
    typedef ACE_Vector< NamedValue, 10 > NamedValueVec;

  public:
    Properties_Encoder ();
    ~Properties_Encoder ();

    void add (const char * name, const PortableGroup::Value & value);

    void encode (PortableGroup::Properties_var & property_set) const;

  private:
    Properties_Encoder (const Properties_Encoder & rhs);
    Properties_Encoder & operator = (const Properties_Encoder & rhs);
  private:
    NamedValueVec values_;
  };

} //namespace TAO_PG

#endif // TAO_PG_PROPERTIES_ENCODER_H

