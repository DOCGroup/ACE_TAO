//=============================================================================
/**
 *  @file    RT_Stub_Factory.h
 *
 *  $Id$
 *
 *   Defines the a factory interface for creating Stubs.
 *   This class creates the default stub, that is used in
 *   plain CORBA.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 *  @author  Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_RT_STUB_FACTORY_H_
#define TAO_RT_STUB_FACTORY_H_

#include "ace/pre.h"
#include "tao/corbafwd.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Stub_Factory.h"
#include "ace/Service_Config.h"

/**
 * @class TAO_RT_Stub_Factory
 *
 * This class is a factory whose product is the TAO_RT_Stub used by
 * TAO.  This factory, or one of its subclasses is dynamically
 * plugged into the ORB_Core, and is used by the ORB_Core to create
 * Stub Objects.
 */
class TAO_RTCORBA_Export TAO_RT_Stub_Factory : public TAO_Stub_Factory
{
public:
  // -- Ctor/Dtor --
  virtual ~TAO_RT_Stub_Factory (void);

  /// Creates a Stub Object.
  virtual TAO_Stub *create_stub (const char *repository_id,
                                 const TAO_MProfile &profiles,
                                 TAO_ORB_Core *orb_core
                                 ACE_ENV_ARG_DECL);
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_RTCORBA, TAO_RT_Stub_Factory)
ACE_FACTORY_DECLARE (TAO_RTCORBA, TAO_RT_Stub_Factory)

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include "ace/post.h"
#endif /* TAO_RT_STUB_FACTORY_H_ */
