// -*- C++ -*-

//=============================================================================
/**
 *  @file Id_Assignment_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IDASSIGNMENTSTRATEGYUSER_H
#define TAO_IDASSIGNMENTSTRATEGYUSER_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "IdAssignmentStrategy.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export User_Id_Assignment_Strategy :
       public virtual IdAssignmentStrategy
    {
    public:
      virtual ~User_Id_Assignment_Strategy (void);

      virtual char id_assignment_key_type (void) const;

    private:
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_IDASSIGNMENTSTRATEGYUSER_H */
