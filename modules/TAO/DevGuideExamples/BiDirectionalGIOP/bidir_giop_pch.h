// $Id$

#ifndef BIDIR_GIOP_PCH_H
#define BIDIR_GIOP_PCH_H

// See the Devguide chapter on Getting Started With Visual C++ for
// more information on how to correctly use precompiled headers.
// Currently only Windows VC++ defines USING_PCH so this file
// is effectively empty for other platforms.

#ifdef USING_PCH
#include "tao/corba.h"
#include "tao/ORB_Core.h"
#include "tao/Stub.h"
#endif

#endif
