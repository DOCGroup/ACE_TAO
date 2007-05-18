// -*- C++ -*-

//=============================================================================
/**
 *  @file    Compression_Adapter.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================
#ifndef TAO_COMPRESSION_ADAPTER_H
#define TAO_COMPRESSION_ADAPTER_H

#include /**/ "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Compression_Adapter
 *
 * @brief TAO_Compression_Adapter.
 *
 * Class that offers an interface to the ORB to load and manipulate
 * Compression library.
 */
class TAO_Export TAO_Compression_Adapter : public ACE_Service_Object
{
public:

  /// The virtual destructor
  virtual ~TAO_Compression_Adapter (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_COMPRESSION_ADAPTER_H */
