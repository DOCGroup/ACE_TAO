// -*- C++ -*-

/**
 *  @file Topology_Factory.h
 *
 *  $Id$
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */

#ifndef TAO_NOTIFY_TOPOLOGY_FACTORY_H
#define TAO_NOTIFY_TOPOLOGY_FACTORY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  class Topology_Saver;
  class Topology_Loader;

  /**
   * @brief Service Object to allow dynamic control of the loaded saver factory.
   *
   */
  class TAO_Notify_Serv_Export Topology_Factory : public ACE_Service_Object
  {
  public:
    /// Create a Saver.
    /// \return a pointer to a Topology_Saver which must be deleted
    /// by the caller.
    virtual Topology_Saver* create_saver () = 0;
    /// Create a Loader
    /// \return a pointer to a Topology_Loader which must be deleted
    /// by the caller.
    virtual Topology_Loader* create_loader () = 0;
    virtual ~Topology_Factory ();
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif
