// -*- C++ -*-

//=============================================================================
/**
 *  @file    Messaging_Loader.h
 *
 *  $Id$
 */
//=============================================================================

#ifndef TAO_MESSAGING_LOADER_H
#define TAO_MESSAGING_LOADER_H

#include /**/ "ace/pre.h"

#include "tao/Messaging/messaging_export.h"

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Messaging_Export TAO_Messaging_Loader : public ACE_Service_Object
{
public:
  /// Constructor.
  TAO_Messaging_Loader (void);

  /// Destructor.
  virtual ~TAO_Messaging_Loader (void);

  /// Initialize the Messaging loader hooks.
  virtual int init (int argc, ACE_TCHAR* []);
};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Messaging, TAO_Messaging_Loader)
ACE_FACTORY_DECLARE (TAO_Messaging, TAO_Messaging_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_MESSAGING_LOADER_H */
