// -*- C++ -*-
#ifndef IIOP_TRANSPORT_CURRENT_H
#define IIOP_TRANSPORT_CURRENT_H

#include "TCC.h"
#include "TC_IIOPC.h"
#include "IIOP_Current_Loader.h"

#if defined (TAO_AS_STATIC_LIBS)

// Only do this for static builds. It causes a circular dependency for
// dynamic builds.
static int tao_iiop_transport_current_initializer_called =
  TAO::Transport::IIOP::current_static_initializer ();

#endif /* TAO_AS_STATIC_LIBS */

#endif /* IIOP_TRANSPORT_CURRENT_H */
