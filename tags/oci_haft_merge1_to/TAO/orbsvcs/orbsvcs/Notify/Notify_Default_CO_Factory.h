/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Default_CO_Factory.h
 *
 *  $Id$
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_DEFAULT_CO_FACTORY_H
#define NOTIFY_DEFAULT_CO_FACTORY_H

#include "ace/pre.h"
#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Notify_Default_CO_Factory
 *
 * @brief TAO_Notify_Default_CO_Factory
 *
 * Maintained for backward compatibility.
 */
class TAO_Notify_Export TAO_Notify_Default_CO_Factory
{
public:
  /// Helper function to register the default factory into the service
  /// configurator.
  static int init_svc (void);

};

#include "ace/post.h"

#endif /* NOTIFY_DEFAULT_CO_FACTORY_H */
