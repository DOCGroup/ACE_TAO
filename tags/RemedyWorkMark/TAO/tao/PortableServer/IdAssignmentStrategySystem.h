// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentStrategySystem.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IDASSIGNMENTSTRATEGYSYSTEM_H
#define TAO_IDASSIGNMENTSTRATEGYSYSTEM_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/IdAssignmentStrategy.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class IdAssignmentStrategySystem
      : public IdAssignmentStrategy
    {
    public:
      virtual char id_assignment_key_type (void) const;

      virtual bool has_system_id (void) const;
    };
  }
}

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, IdAssignmentStrategySystem)
ACE_FACTORY_DECLARE (TAO_PortableServer, IdAssignmentStrategySystem)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_IDASSIGNMENTSTRATEGYSYSTEM_H */
