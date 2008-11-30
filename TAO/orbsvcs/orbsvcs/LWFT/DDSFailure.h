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
  DDSFailure (const std::string & description);

  /// Getter method.
  std::string description (void) const;
  
 private:
  /// Description of what DDS problem occurred.
  std::string description_;
};

#endif // FLARE_USES_DDS

#endif /* _DDS_FAILURE_H_ */
