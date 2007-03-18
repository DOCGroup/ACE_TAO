// -*- C++ -*-

//=============================================================================
/**
 *  @file Compression.h
 *
 *  $Id$
 *
 *  Dynamic loader object for Compression
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_COMPRESSION_GIOP_H
#define TAO_COMPRESSION_GIOP_H
#include /**/ "ace/pre.h"

#include "tao/Compression/compression_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"
#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Compression_Loader
 *
 * @brief Class that loads the Compression library.
 */

class TAO_Compression_Export TAO_Compression_Loader
  : public TAO_Object_Loader
{
public:
  /// Creates a Compression Manager and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb, int argc,
                                           ACE_TCHAR *argv []);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
  TAO_Requires_Compression_Initializer = TAO_Compression_Loader::Initializer
  ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_Compression_Loader)
ACE_FACTORY_DECLARE (TAO_Compression, TAO_Compression_Loader)

#define TAO_COMPRESSION_SAFE_INCLUDE
#include "tao/Compression/CompressionC.h"
#undef TAO_COMPRESSION_SAFE_INCLUDE

#include /**/ "ace/post.h"
#endif /* TAO_COMPRESSION_GIOP_H */
