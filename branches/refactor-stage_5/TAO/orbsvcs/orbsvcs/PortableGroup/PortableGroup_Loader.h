// -*- C++ -*-

//=============================================================================
/**
 *  @file     PortableGroup_Loader.h
 *
 *  $Id$
 *
 *  Loads and initializes the PortableGroup extensions to TAO.
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_PORTABLEGROUP_LOADER_H
#define TAO_PORTABLEGROUP_LOADER_H
#include /**/ "ace/pre.h"

#include "portablegroup_export.h"
#include "tao/corbafwd.h"
#include "ace/Service_Config.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_PortableGroup_Adapter
 *
 * @brief TAO_PortableGroup_Adapter.
 *
 * This class acts as a facade for the PortableGroup library to the
 * ORB.
 */
class TAO_PortableGroup_Export TAO_PortableGroup_Loader 
  : public ACE_Service_Object
{
  friend class TAO_POA_Hooks;

public:
  /// Constructor.
  TAO_PortableGroup_Loader (void);

  /// Initialize the PortableGroup loader hooks.
  virtual int init (int argc,
                    char *[]);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

ACE_STATIC_SVC_DECLARE (TAO_PortableGroup_Loader)
ACE_FACTORY_DECLARE (TAO_PortableGroup, TAO_PortableGroup_Loader)

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_PortableGroup_Initializer = 
  &TAO_PortableGroup_Loader::Initializer;

#else

static int
TAO_Requires_PortableGroup_Initializer = TAO_PortableGroup_Loader::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLEGROUP_LOADER_H */
