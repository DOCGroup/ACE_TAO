// -*- C++ -*-

// ===================================================================
/**
 *  @file   RT_POA_Initializer.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 *  @author Angelo Corsaro <corsaro@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_RT_POA_INITIALIZER_H
#define TAO_RT_POA_INITIALIZER_H
#include "ace/pre.h"

#include "rtportableserver_export.h"
#include "tao/POA_Extension_Initializer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declarations.
class TAO_POA_Policy_Set;
class TAO_ORB_Core;

class TAO_RTPortableServer_Export TAO_RT_POA_Initializer
  : public TAO_POA_Extension_Initializer
{
public:
  /// Destructor.
  virtual ~TAO_RT_POA_Initializer (void);

protected:

  virtual void register_hooks_impl (TAO_Object_Adapter &object_adapter,
                                    CORBA::Environment &ACE_TRY_ENV);

  void init_rt_default_policies (TAO_POA_Policy_Set &policies,
                                 TAO_ORB_Core &orb_core,
                                 CORBA::Environment &ACE_TRY_ENV);
};

#include "ace/post.h"
#endif /* TAO_RT_POA_INITIALIZER_H */
