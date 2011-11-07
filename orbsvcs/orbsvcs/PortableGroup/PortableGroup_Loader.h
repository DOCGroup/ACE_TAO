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

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

#include "ace/Service_Config.h"
#include "ace/Service_Object.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
public:
  /// Constructor.
  TAO_PortableGroup_Loader (void);

  /// Initialize the PortableGroup loader hooks.
  virtual int init (int argc, ACE_TCHAR *[]);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_PortableGroup_Initializer =
  TAO_PortableGroup_Loader::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_PortableGroup_Loader)
ACE_FACTORY_DECLARE (TAO_PortableGroup, TAO_PortableGroup_Loader)

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLEGROUP_LOADER_H */
