// -*- C++ -*-

//=============================================================================
/**
 *  @file
 *
 *  $Id$
 *
 *   Header file for Loading DynamicTP Configurations.
 *
 *
 *  @author
 */
//=============================================================================

#ifndef TAO_DYNAMIC_TP_CONFIG_H
#define TAO_DYNAMIC_TP_CONFIG_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/Dynamic_TP/dynamic_tp_export.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

class TAO_Dynamic_TP_Export TAO_Dynamic_TP_Config : public ACE_Service_Object
{
public:

  /// Constructor.
  TAO_Dynamic_TP_Config (void);

  /// Destructor.
  virtual ~TAO_Dynamic_TP_Config (void);

  /// Initialize the DynamicTP loader hooks.
  virtual int init (int argc, ACE_TCHAR* []);

private:
  /// Set to true after init is called.
  bool initialized_;
};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Dynamic_TP, TAO_Dynamic_TP_Config)
ACE_FACTORY_DECLARE (TAO_Dynamic_TP, TAO_Dynamic_TP_Config)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_DYNAMIC_TP_CONFIG_H */
