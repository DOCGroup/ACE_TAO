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

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Policy_Strategy.h"
#include "tao/Basic_Types.h"

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdAssignmentStrategy :
       public virtual Policy_Strategy
    {
    public:
      virtual ~IdAssignmentStrategy (void);

      virtual void strategy_init(TAO_Root_POA *poa ACE_ENV_ARG_DECL);

      virtual void strategy_cleanup(ACE_ENV_SINGLE_ARG_DECL);

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

#include /**/ "ace/post.h"
#endif /* TAO_ID_ASSIGNMENT_STRATEGY_H */
