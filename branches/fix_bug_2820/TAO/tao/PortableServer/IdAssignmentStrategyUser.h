// -*- C++ -*-

//=============================================================================
/**
 *  @fileIdAssignmentStrategyUser.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IDASSIGNMENTSTRATEGYUSER_H
#define TAO_IDASSIGNMENTSTRATEGYUSER_H
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
    class IdAssignmentStrategyUser
      : public IdAssignmentStrategy
    {
    public:
      virtual char id_assignment_key_type (void) const;

      virtual bool has_system_id (void) const;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, IdAssignmentStrategyUser)
ACE_FACTORY_DECLARE (TAO_PortableServer, IdAssignmentStrategyUser)

#include /**/ "ace/post.h"
#endif /* TAO_IDASSIGNMENTSTRATEGYUSER_H */
