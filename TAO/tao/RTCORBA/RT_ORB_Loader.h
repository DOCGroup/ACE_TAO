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

#include "rtcorba_export.h"

#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORB_Core;

class TAO_RTCORBA_Export TAO_RT_ORB_Loader : public TAO_Object_Loader
{
public:
  // = Priority mapping types
  enum
  {
    TAO_PRIORITY_MAPPING_LINEAR,
    TAO_PRIORITY_MAPPING_DIRECT
  };

  /// Constructor.
  TAO_RT_ORB_Loader (void);

  /// Destructor.
  virtual ~TAO_RT_ORB_Loader (void);

  /// Initialize the RT ORB loader hooks.
  virtual int init (int argc,
                    char* []);

  /// Creates a RT_ORB and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           char *argv [],
                                           CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_RTCORBA, TAO_RT_ORB_Loader)
ACE_FACTORY_DECLARE (TAO_RTCORBA, TAO_RT_ORB_Loader)


class TAO_RTCORBA_Export TAO_RT_Current_Loader : public TAO_Object_Loader
{
public:
  /// Creates an RT_Current flyweight object and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           char *argv [],
                                           CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_RTCORBA, TAO_RT_Current_Loader)
ACE_FACTORY_DECLARE (TAO_RTCORBA, TAO_RT_Current_Loader)

// #if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

// typedef int (*TAO_Module_Initializer) (void);

// static TAO_Module_Initializer
// TAO_Requires_RTCORBA_Initializer = &TAO_RT_ORB_Loader::Initializer;

// #else

// static int
// TAO_Requires_RTCORBA_Initializer = TAO_RT_ORB_Loader::Initializer ();

// #endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#include "ace/post.h"
#endif /* TAO_RT_ORB_LOADER_H */
