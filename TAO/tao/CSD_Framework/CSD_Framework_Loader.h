/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    CSD_Framework_Loader.h
 *
 *  $Id$
 *
 *   Header file for loading CSD framework service objects.
 *
 *  @author Yan Dai (dai_y@ociweb.com)
 */
//=============================================================================

#ifndef TAO_CSD_FRAMEWORK_LOADER_H
#define TAO_CSD_FRAMEWORK_LOADER_H
#include /**/ "ace/pre.h"

#include "CSD_FW_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_CSD_Framework_Loader
 *
 * @brief TAO_CSD_Framework_Loader.
 *
 * This class acts as a facade for the CSD_Framework library to the
 * ORB.
 */
class TAO_CSD_FW_Export TAO_CSD_Framework_Loader
{
 public:
  /// Used to force the initialization of the ORB code.
  static int init (void);
};

#include /**/ "ace/post.h"
#endif /* TAO_CSD_FRAMEWORK_LOADER_H */
