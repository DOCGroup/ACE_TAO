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

#include "tao/CSD_Framework/CSD_FW_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_CSD_Framework_Loader
 *
 * @brief TAO_CSD_Framework_Loader.
 *
 * This class acts as a facade for the CSD_Framework library to the
 * ORB.
 */
class TAO_CSD_FW_Export TAO_CSD_Framework_Loader : public ACE_Service_Object
{
 public:
  /// Constructor.
  TAO_CSD_Framework_Loader (void);

  /// Destructor.
  virtual ~TAO_CSD_Framework_Loader (void);

  /// Used to force the initialization of the ORB code.
  static int static_init (void);

  /// Initialize the PI_Server loader hooks.
  virtual int init (int argc, ACE_TCHAR* []);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_CSD_FW, TAO_CSD_Framework_Loader)
ACE_FACTORY_DECLARE (TAO_CSD_FW, TAO_CSD_Framework_Loader)

#include /**/ "ace/post.h"
#endif /* TAO_CSD_FRAMEWORK_LOADER_H */
