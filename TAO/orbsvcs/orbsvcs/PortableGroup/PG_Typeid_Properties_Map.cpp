/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    PG_Typeid_Properties_Map.cpp
 *
 *  $Id$
 *
 *  This file implements classes to help manage PortableGroup::Properties
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "PG_Typeid_Properties_Map.h"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Hash_Map_Manager<
    ACE_CString,
    const ::TAO_PG::Properties_Decoder *,
    TAO_SYNCH_MUTEX>;

  template class ACE_Hash_Map_Iterator<
    ACE_CString, 
    const ::TAO_PG::Properties_Decoder *,
    TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate ACE_Hash_Map_Manager<
    ACE_CString,
    const ::TAO_PG::Properties_Decoder *,
    TAO_SYNCH_MUTEX> 

# pragma instantiate ACE_Hash_Map_Iterator<
    ACE_CString, 
    const ::TAO_PG::Properties_Decoder *,
    TAO_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
