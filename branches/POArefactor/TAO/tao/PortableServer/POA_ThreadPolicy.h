// -*- C++ -*-

//=============================================================================
/**
 *  @file POA_ThreadPolicy.h
 *
 *  $Id$
 *
 *
 *
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_POA_THREADPOLICY_H
#define TAO_POA_THREADPOLICY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ThreadPolicyC.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace TAO
{
  class TAO_PortableServer_Export POA_ThreadPolicy :
    public virtual PortableServer::ThreadPolicy,
    public virtual CORBA::LocalObject
    {
    public:
     POA_ThreadPolicy (PortableServer::ThreadPolicyValue v)
     {
       if (v == ORB_CTRL_MODEL)
         {
           this->value_ =
           ACE_Dynamic_Service<ORB_CTRL_MODEL_Policy>::instance ();
         }
         else if (// for other stuff)
         {
         }
     }
     // Methods on the policy interface.
     // todo
     private:
       PortableServer::ThreadPolicyValue *value_;
    };

}

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_POA_THREADPOLICY_H */


