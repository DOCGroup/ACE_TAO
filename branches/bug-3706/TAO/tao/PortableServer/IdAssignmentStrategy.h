// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentStrategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ID_ASSIGNMENT_STRATEGY_H
#define TAO_ID_ASSIGNMENT_STRATEGY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/Policy_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class IdAssignmentStrategy
      : public Policy_Strategy
    {
    public:
      virtual void strategy_init(TAO_Root_POA *poa);

      virtual void strategy_cleanup(void);

      /**
       * Returns the key type the says which specific policy we have
       */
      virtual char id_assignment_key_type (void) const = 0;

      /**
       * Returns the length of the id_assignment type
       */
      char key_type_length (void) const;

      void create_key (CORBA::Octet *buffer, CORBA::ULong& starting_at);

      virtual bool has_system_id (void) const = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ID_ASSIGNMENT_STRATEGY_H */
