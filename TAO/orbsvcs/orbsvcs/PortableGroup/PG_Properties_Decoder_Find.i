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
#ifndef TAO_PG_PROPERTIES_DECODER_FIND_I
#define TAO_PG_PROPERTIES_DECODER_FIND_I
#include /**/ "ace/pre.h"
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PG_Properties_Decodeer_Find.h"

/**
 * Find a value in a Properties::Properties_Decoder.
 * This is a work-around for the lack of
 * templated methods.
 */
template <typename TYPE>
int TAO_PG::find (const Properties_Decoder & decoder, const ACE_CString & key, TYPE & value)
{
  int result = 0;
  PortableGroup::Value * any;
  if ( decoder.find (key, any))
  {
    result = ((*any) >>= value);
  }
  return result;
}

#endif // TAO_PG_PROPERTIES_DECODER_FIND_I
