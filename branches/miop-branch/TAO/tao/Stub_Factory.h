//=============================================================================
/**
 *  @file    Stub_Factory.h
 *
 *  $Id$
 *
 *   Defines the a factory interface for creating Stubs.
 *   This class creates the default stub, that is used in
 *   plain CORBA.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_STUB_FACTORY_H_
#define TAO_STUB_FACTORY_H_

#include "ace/pre.h"
#include "tao/corbafwd.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward references
class TAO_MProfile;
class TAO_ORB_Core;

/**
 * @class TAO_Stub_Factory
 *
 * This class is a factory whose product is the plain TAO_Stub used by
 * TAO. Its subclasses build Stub object that are specialization of
 * the TAO_Stub. As an example TAO_RT_Stub_Factory creates
 * RT_TAO_Stub. This factory, or one of its subclasses is dynamically
 * plugged into the ORB_Core, and is used by the ORB_Core to create
 * Stub Objects.
 */
class TAO_Export TAO_Stub_Factory
  : public ACE_Service_Object
{
public:
  // -- Ctor/Dtor --
  virtual ~TAO_Stub_Factory (void);

  /// Creates a Stub Object.
  virtual TAO_Stub *create_stub (const char *repository_id,
                                 const TAO_MProfile &profiles,
                                 TAO_ORB_Core *orb_core
                                 TAO_ENV_ARG_DECL) = 0;
};

#include "ace/post.h"
#endif /* TAO_STUB_FACTORY_H_ */
