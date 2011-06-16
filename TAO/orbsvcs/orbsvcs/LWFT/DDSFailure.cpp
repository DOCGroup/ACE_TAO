// -*- C++ -*-

//=============================================================================
/**
 *  @file    DDSFailure.cpp
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include "DDSFailure.h"

#if defined (FLARE_USES_DDS)

DDSFailure::DDSFailure (const std::string & description)
  : description_ (description)
{
}

std::string
DDSFailure::description (void) const
{
  return description_;
}

#endif // FLARE_USES_DDS
