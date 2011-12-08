// $Id$
// THREADING NOTE: the global values here  (debug_{level,filter,stream)
// are assumed to be modified "safely", e.g. in the main thread as
// part of process initialization.  They are treated as immutable
// values through all of this debuging package.
//
// XXX on Windows, make it always use OutputDebugString () instead of stdio.

#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Export unsigned int TAO_debug_level = 0;

TAO_Export unsigned int TAO_orbdebug = 0;

TAO_END_VERSIONED_NAMESPACE_DECL
