/* -*- C++ -*- */
/**
 *  @file Driver_Base.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_APPLICATION_STARTER_H
#define TAO_Notify_Tests_APPLICATION_STARTER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Notify_Tests_Driver_Base
 *
 * @brief Base class for Driver.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Driver_Base
{
public:
  virtual ~TAO_Notify_Tests_Driver_Base () {};

  /// Shutdown
  virtual void shutdown (void) = 0;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_APPLICATION_STARTER_H */
