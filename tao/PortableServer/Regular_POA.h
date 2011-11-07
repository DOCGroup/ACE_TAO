// -*- C++ -*-

//=============================================================================
/**
 *  @file    Regular_POA.h
 *
 *  $Id$
 *
 *  Header file for CORBA's ORB type.
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_REGULAR_POA_H
#define TAO_REGULAR_POA_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Root_POA.h"

#if !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Regular_POA
 *
 * @brief Implementation of the PortableServer::POA interface.
 *
 * Implementation of the PortableServer::POA interface.
 */
class TAO_PortableServer_Export TAO_Regular_POA
  : public TAO_Root_POA
{
public:
  TAO_Regular_POA (const String &name,
                   PortableServer::POAManager_ptr poa_manager,
                   const TAO_POA_Policy_Set &policies,
                   TAO_Root_POA *parent,
                   ACE_Lock &lock,
                   TAO_SYNCH_MUTEX &thread_lock,
                   TAO_ORB_Core &orb_core,
                   TAO_Object_Adapter *object_adapter);

  virtual ~TAO_Regular_POA (void);

  PortableServer::POA_ptr the_parent (void);

protected:
  virtual void remove_from_parent_i (void);

  virtual CORBA::Boolean root (void) const;

  virtual char root_key_type (void);

  /// The parent of this POA, be aware that in case this pointer is nill,
  /// we are a parent. This can be achieved by deriving from this Regular_POA
  /// and pass a nill pointer as parent with the constructor.
  TAO_Root_POA *parent_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif

// ****************************************************************

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/Regular_POA.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_REGULAR_POA_H */
