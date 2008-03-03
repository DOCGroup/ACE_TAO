// $Id$

/**
* @file RACE_utils.h
*
* This file contains utils RACE macro definitions.
*
* @author Nishanth Shankaran <nshankar@nospam.com>
*/

#ifndef RACE_UTILS_H
#define RACE_UTILS_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined ACE_LACKS_PRAGMA_ONCE
# pragma once
#endif /* ! ACE_LACKS_PRAGMA_ONCE */

//#include "RACE_utils_export.h"

namespace CIAO
{
  namespace RACE
  {
    /**
    * Return the debug level.  The debug level of RACE is control by
    * an environment variable "RACE_DEBUG_LEVEL".  It should be an int
    * value.  If it is not defined, the default debug level is 0.  The
    * value of debug value is evaluated on its first use and the value
    * is then cached.
    *
    * Some recommendation for using the debug_level
    *
    *   > 10 : Write log file as well as to STDOUT.
    */
    /*RACE_UTILS_Export */
    int debug_level (void);
  }
}
#include /**/ "ace/post.h"

#endif /* RACE_UTILS_H */
