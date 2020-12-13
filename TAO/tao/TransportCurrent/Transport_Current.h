// -*- C++ -*-
#ifndef TRANSPORT_CURRENT_H
#define TRANSPORT_CURRENT_H

#include "TCC.h"
#include "Current_Loader.h"

#if defined (TAO_AS_STATIC_LIBS)

// Only do this for static builds. It causes a circular dependency for
// dynamic builds.
static int tao_transport_current_initializer_called =
  TAO::Transport::current_static_initializer ();

#endif /* TAO_AS_STATIC_LIBS */

#endif /* TRANSPORT_CURRENT_H */
