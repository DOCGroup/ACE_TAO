// -*- C++ -*-

//=============================================================================
/**
 *  @file     MIOP.h
 *
 *  $Id$
 *
 *  @author Bala Natarajan
 */
//=============================================================================
#ifndef TAO_MIOP_H
#define TAO_MIOP_H
#include /**/ "ace/pre.h"

#include "portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{

  class TAO_PortableGroup_Export MIOP_Loader
  {
    public:
    /// Used to force the insertion of UIPMC_Factory in the service
    /// configurator.
    static int Initializer (void);
  };

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*Module_Initializer) (void);

static Module_Initializer
Requires_MIOP_Initializer = &MIOP_Loader::Initializer;

#else

static int
Requires_MIOP_Initializer = MIOP_Loader::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */
}

#include /**/ "ace/post.h"
#endif /*TAO_MIOP_H*/
