/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Default_POA_Factory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_DEFAULT_POA_FACTORY_H
#define TAO_NOTIFY_DEFAULT_POA_FACTORY_H

#include /**/ "ace/pre.h"
#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Notify_Default_POA_Factory
 *
 * @brief Maintained for backward compatibility.
 */
class TAO_Notify_Export TAO_Notify_Default_POA_Factory
{
public:
  /// Helper function to register the default factory into the service
  /// configurator.
  static int init_svc (void);
};

#include /**/ "ace/post.h"
#endif /* TAO_NOTIFY_DEFAULT_POA_FACTORY_H */
