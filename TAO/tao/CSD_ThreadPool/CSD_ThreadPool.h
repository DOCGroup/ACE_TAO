// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_ThreadPool.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_THREADPOOL_H
#define TAO_CSD_THREADPOOL_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"
#include "tao/Versioned_Namespace.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_CSD_ThreadPool
 *
 * @brief the static initializer for the CSD Thread Pool library
 *
 * The loader appears to be born of the CSD_TP_Factory, which is the
 * real service object. This separation seems to be necesary to ensure
 * the CSD_Framework gets loaded prior to initializing the TP Factory.
 *
 */
class TAO_CSD_TP_Export TAO_CSD_ThreadPool
{
 public:
  /// Used to force the initialization of the ORB code.
  static int init (void);
};

static int
TAO_Requires_CSD_Threadpool =
  TAO_CSD_ThreadPool::init ();

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_CSD_THREADPOOL_H */
