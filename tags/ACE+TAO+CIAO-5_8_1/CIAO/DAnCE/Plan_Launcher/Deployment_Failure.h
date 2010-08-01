/**
 * @file Deployment_Failure.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Defines exceptions used internally to the Plan Launcher
 */

#ifndef DANCE_DEPLOYMENT_FAILURE
#define DANCE_DEPLOYMENT_FAILURE

#include "ace/SString.h"

namespace DAnCE
{
  struct Deployment_Failure
  {
    Deployment_Failure (const ACE_CString &ex) : ex_ (ex) {}
    
    ACE_CString ex_;
  };
}

#endif
