// -*- C++ -*-

//=============================================================================
/**
 *  @file    POA.h
 *
 *  $Id$
 *
 *  Header file for CORBA's ORB type.
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_POA_H
#define TAO_POA_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/Root_POA.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_POA
 *
 * @brief Implementation of the PortableServer::POA interface.
 *
 * Implementation of the PortableServer::POA interface.
 */
class TAO_PortableServer_Export TAO_POA
  : public TAO_Root_POA
{
public:
  TAO_POA (const String &name,
           TAO_POA_Manager &poa_manager,
           const TAO_POA_Policy_Set &policies,
           TAO_Root_POA *parent,
           ACE_Lock &lock,
           TAO_SYNCH_MUTEX &thread_lock,
           TAO_ORB_Core &orb_core,
           TAO_Object_Adapter *object_adapter
           ACE_ENV_ARG_DECL);

  virtual ~TAO_POA (void);

protected:

};


// ****************************************************************

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "POA.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_POA_H */
