/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    RT_ORB_Loader.h
 *
 *  $Id$
 *
 *   Header file for Loading RT_ORB.
 *
 *
 *  @author  Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef TAO_RT_ORB_LOADER_H
#define TAO_RT_ORB_LOADER_H
#include "ace/pre.h"

#include "tao/ORB_Core.h"
#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_RT_ORB_Loader : public TAO_Object_Loader
{
public:

  /// Constructor
  TAO_RT_ORB_Loader (void);

  /// Creates a RT_ORB and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           char *argv [],
                                           CORBA::Environment &)
    ACE_THROW_SPEC (());

protected:

  /// Setup additional RTCORBA related objects.
  void set_objects (TAO_ORB_Core *orb_core,
                    CORBA::Environment &);

};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_RT_ORB_Loader)
ACE_FACTORY_DECLARE (TAO, TAO_RT_ORB_Loader)

#include "ace/post.h"
#endif /* TAO_RT_ORB_LOADER_H */
