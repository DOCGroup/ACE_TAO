// -*- C++ -*-

//=============================================================================
/**
 *  @file    Topology_Loader.h
 *
 *  $Id$
 *
 *  @author Jonathan Pollack <pollack_j@ociweb.com>
 */
//=============================================================================

#ifndef TOPOLOGY_LOADER_H
#define TOPOLOGY_LOADER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Environment;
}

namespace TAO_Notify
{

/// A forward declaration.
class Topology_Object;

/**
 * \brief An interface for an object that loads Persistent Topology Information.
 *
 * A persistent store must provide an implemention this interface.
 *
 */
class TAO_Notify_Serv_Export Topology_Loader
{
public:
  /// The constructor.
  Topology_Loader ();

  /// The destructor.
  virtual ~Topology_Loader ();

  /** \brief Begin the restore process.
   *
   * Call this function to start the reload of data from a persistent store.
   * When the Topology_Loader detects a child object, it should
   * call the load_child method of the object passed in, then do the same
   * loading process on the returned object.
   */
  virtual void load (Topology_Object *root) = 0;

  /**
   * \brief Close the loader after loading is complete.
   *
   * This is not pure virtual because some loaders may not need to be closed.
   * The default implementation does nothing.
   *
   * There should be a corresponding open, but the signature may
   * vary based on the type of loader, so we can't include it in the
   * interface.
   */
  virtual void close (void);
};

} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TOPOLOGY_LOADER */
