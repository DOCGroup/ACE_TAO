// -*- C++ -*-

//=============================================================================
/**
 *  @fileIdAssignmentStrategyUser.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IDASSIGNMENTSTRATEGYUSER_H
#define TAO_IDASSIGNMENTSTRATEGYUSER_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/IdAssignmentStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class IdAssignmentStrategyUser : public IdAssignmentStrategy
    {
    public:
      char id_assignment_key_type () const override;

      bool has_system_id () const override;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_IDASSIGNMENTSTRATEGYUSER_H */
