/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    PG_Properties_Decoder.h
 *
 *  $Id$
 *
 *  This is a companion function for the properties docoder
 *  to work around compilers that don't support templated methods.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#ifndef TAO_PG_PROPERTIES_DECODER_FIND_H
#define TAO_PG_PROPERTIES_DECODER_FIND_H
#include /**/ "ace/pre.h"
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupS.h"
#include "orbsvcs/CosNamingC.h"
#include "portablegroup_export.h"

#include "ace/Hash_Map_Manager.h"

namespace TAO_PG
{
  /**
   * Find a value in a Properties::Properties_Decoder.
   * This is a work-around for the lack of
   * templated methods.
   */
  template <typename TYPE>
  int find (const Properties_Decoder & decoder, const ACE_CString & key, TYPE & value);

} //namespace TAO_PG

#if defined (__ACE_INLINE__)
# include "PG_Properties_Decoder_Find.i"
#endif /* __ACE_INLINE__ */

#endif // TAO_PG_PROPERTIES_DECODER_FIND_H
