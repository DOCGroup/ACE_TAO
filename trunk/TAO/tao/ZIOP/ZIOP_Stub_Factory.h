// -*- C++ -*-

//=============================================================================
/**
 *  @file    ZIOP_Stub_Factory.h
 *
 *  $Id$
 *
 *   Defines the a factory interface for creating Stubs.
 *   This class creates the ZIOP stub, that is used in
 *   the ZIOP library
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ZIOP_STUB_FACTORY_H_
#define TAO_ZIOP_STUB_FACTORY_H_

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#include "tao/ZIOP/ziop_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Stub_Factory.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ZIOP_Stub_Factory
 *
 * This class is a factory whose product is the TAO_ZIOP_Stub used by
 * TAO.  This factory, or one of its subclasses is dynamically
 * plugged into the ORB_Core, and is used by the ORB_Core to create
 * Stub Objects.
 */
class TAO_ZIOP_Export TAO_ZIOP_Stub_Factory : public TAO_Stub_Factory
{
public:
  // -- Ctor/Dtor --
  virtual ~TAO_ZIOP_Stub_Factory (void);

  /// Creates a Stub Object.
  virtual TAO_Stub *create_stub (const char *repository_id,
                                 const TAO_MProfile &profiles,
                                 TAO_ORB_Core *orb_core);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_ZIOP, TAO_ZIOP_Stub_Factory)
ACE_FACTORY_DECLARE (TAO_ZIOP, TAO_ZIOP_Stub_Factory)

#endif

#include /**/ "ace/post.h"
#endif /* TAO_ZIOP_STUB_FACTORY_H_ */
