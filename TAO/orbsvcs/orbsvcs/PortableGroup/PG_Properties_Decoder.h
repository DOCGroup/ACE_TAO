/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    PG_Properties_Decoder.h
 *
 *  $Id$
 *
 *  This file declares classes to help manage the Properties
 *  defined in the Portable Object Group.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#ifndef TAO_PG_PROPERTIES_DECODER_H
#define TAO_PG_PROPERTIES_DECODER_H
#include "orbsvcs/PortableGroupS.h"
#include "orbsvcs/CosNamingC.h"
#include "portablegroup_export.h"

#include "ace/Map_T.h"

namespace TAO_PG
{
  class TAO_PortableGroup_Export Properties_Decoder
  {
    typedef ACE_Hash_Map_Manager<ACE_CString, const PortableGroup::Value *, ACE_SYNCH_NULL_MUTEX> ValueMap;
  public:
    Properties_Decoder (const PortableGroup::Properties & property_set);
    ~Properties_Decoder ();

    // general purpose method
    int find (const ACE_CString & key, PortableGroup::Value *& pValue)const;

    // if templated methods were available:
    // template <typename TYPE >
    // int find (const ACE_CString & key, TYPE & value) const;
    // instead, see global function below

  private:
    Properties_Decoder();
    Properties_Decoder(const Properties_Decoder & rhs);
    Properties_Decoder & operator = (const Properties_Decoder & rhs);

  private:
    ValueMap values_;
    PortableGroup::Properties_var property_set_;
  };

  /**
   * Find a value in a Properties::Properties_Decoder.
   * This is a work-around for the lack of
   * templated methods.
   */
  template <typename TYPE>
  int find (Properties_Decoder & decoder, const ACE_CString & key, TYPE & value)
  {
    PortableGroup::Value * any;
    decoder.find (key, any);
    // if find fails, any will be empty and the attempt to extract
    // the value will fail, so there's no need to check status
    // on the find.
    return ((*any) >>= value);
  }

#ifdef PG_PS_UNIT_TEST

  /**
   * unit test: encode and decode properties.
   * Initialize CORBA before calling this function.
   * Success is silent, failure prints on cerr.
   * @returns 1 if test passed; 0 if test failed.
   */
  int test_encode_decode();
#endif // PG_PS_UNIT_TEST
} //namespace TAO_PG

#endif // TAO_PG_PROPERTIES_DECODER_H

