/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    PG_Typeid_Properties_Map.h
 *
 *  $Id$
 *
 *  This file declares classes to help manage the PortableGroup::Properties
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#ifndef TAO_PG_TYPEID_PROPERTIES_MAP_H
#define TAO_PG_TYPEID_PROPERTIES_MAP_H

#include "PG_Properties_Decoder.h"

namespace TAO
{
  typedef ACE_Hash_Map_Manager<
    ACE_CString,
    const ::TAO_PG::Properties_Decoder *,
    TAO_SYNCH_MUTEX> PG_Typeid_Properties_Map;
  typedef ACE_Hash_Map_Iterator<
    ACE_CString, 
    const ::TAO_PG::Properties_Decoder *,
    TAO_SYNCH_MUTEX> PG_Typeid_Properties_Map_Iterator;
} //namespace TAO_PG

#endif // TAO_PG_TYPEID_PROPERTIES_MAP_H
