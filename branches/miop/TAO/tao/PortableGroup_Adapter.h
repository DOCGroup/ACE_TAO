// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    PortableGroup_Adapter.h
 *
 *  $Id$
 *
 *  @author  Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_PORTABLEGROUP_ADAPTER_H
#define TAO_PORTABLEGROUP_ADAPTER_H
#include "ace/pre.h"

#include "tao/Exception.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

class TAO_ORB_Core;
class TAO_POA_PortableGroup_Hooks;

/**
 * @class TAO_PortableGroup_Adapter
 *
 * @brief TAO_PortableGroup_Adapter.
 *
 * Class that offers an interface to the ORB to load and manipulate
 * PortableGroup library.
 */

class TAO_Export TAO_PortableGroup_Adapter : public ACE_Service_Object
{
public:

  /// The virtual destructor
  virtual ~TAO_PortableGroup_Adapter (void);

  /// Activate the PortableGroup library
  virtual void activate (CORBA::ORB_ptr,
                         int argc,
                         char *argv [],
                         CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Create a Miop object and return it.
  virtual CORBA::Object_ptr miop (CORBA::Environment &)
    ACE_THROW_SPEC (());

  /// Retrieve the hooks for interfacing with the POA.
  virtual TAO_POA_PortableGroup_Hooks *poa_hooks (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#include "ace/post.h"
#endif /* TAO_PORTABLEGROUP_ADAPTER_H */
