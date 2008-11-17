// -*- C++ -*-

//=============================================================================
/**
 *  @file    DDSFailure.h
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _DDS_FAILURE_H_
#define _DDS_FAILURE_H_

#if defined (FLARE_USES_DDS)

#include <string>
#include "ssa_export.h"

class SSA_Export DDSFailure 
{
 public:
  /// ctor
  DDSFailure (const std::string & description);

  /// getter method
  std::string description () const;
 private:
  /// description what DDS problem occured
  std::string description_;
};

#endif // FLARE_USES_DDS

#endif /* _DDS_FAILURE_H_ */
