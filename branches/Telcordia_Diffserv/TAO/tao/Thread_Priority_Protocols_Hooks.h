// -*- C++ -*-

// ===================================================================
/**
 *  @file   Thread_Priority_Protocols_Hooks.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *          Johnny Willemsen <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_THREAD_PRIORITY_PROTOCOLS_HOOKS_H
#define TAO_THREAD_PRIORITY_PROTOCOLS_HOOKS_H

#include /**/ "ace/pre.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include /**/ "tao/TAO_Export.h"
#include "tao/Basic_Types.h"
#include "tao/IOP_IORC.h"
#include "ace/SString.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Policy;
  class Environment;
}

class TAO_ORB_Core;
class TAO_Service_Context;
class TAO_Connection_Handler;
class TAO_Stub;

class TAO_Export TAO_Thread_Priority_Protocols_Hooks 
      : public ACE_Service_Object
{
public:
  /// destructor
  virtual ~TAO_Thread_Priority_Protocols_Hooks (void);

  virtual void init_hooks (TAO_ORB_Core *orb_core
                           ACE_ENV_ARG_DECL) = 0;
  
  virtual int get_thread_CORBA_priority (CORBA::Short &
                                         ACE_ENV_ARG_DECL) = 0;

  virtual int get_thread_native_priority (CORBA::Short &
                                          ACE_ENV_ARG_DECL) = 0;

  virtual int get_thread_CORBA_and_native_priority (
    CORBA::Short &,
    CORBA::Short &
    ACE_ENV_ARG_DECL) = 0;

  virtual int set_thread_CORBA_priority (CORBA::Short
                                         ACE_ENV_ARG_DECL) = 0;

  virtual int set_thread_native_priority (CORBA::Short
                                          ACE_ENV_ARG_DECL) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_THREAD_PRIORITY_PROTOCOLS_HOOKS_H */
