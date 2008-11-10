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

DDSFailure::DDSFailure (const std::string & description)
  : description_ (description)
{
}

std::string
DDSFailure::description () const
{
  return description_;
}
