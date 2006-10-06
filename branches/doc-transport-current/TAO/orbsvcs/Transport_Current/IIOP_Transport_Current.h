// -*- C++ -*-
// $Id$


#include "orbsvcs/Transport_Current/TCC.h"
#include "orbsvcs/Transport_Current/TC_IIOPC.h"
#include "orbsvcs/Transport_Current/IIOP_Current_Loader.h"

#if defined (TAO_AS_STATIC_LIBS)

// Only do this for static builds. It causes a circular dependency for
// dynamic builds.
static int tao_iiop_transport_current_initializer_called =
  TAO::Transport::IIOP::current_static_initializer ();

#endif /* TAO_AS_STATIC_LIBS */

