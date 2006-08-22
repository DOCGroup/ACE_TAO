// -*- C++ -*-

//=============================================================================
/**
 *  @file    DII_Arguments_Converter_Impl.h
 *
 *  $Id$
 *
 *  @author  Yan Dai <dai_y@ociweb.com>
 */
//=============================================================================


#ifndef TAO_DII_ARGUMENTS_CONVERTER_IMPL_H
#define TAO_DII_ARGUMENTS_CONVERTER_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/DynamicInterface/dynamicinterface_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/DII_Arguments_Converter.h"

#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


/**
 * @class DII_Arguments_Converter_Impl
 *
 * @brief DII_Arguments_Converter_Impl.
 *
 * This class provides the implementation to expand the DII arguments in NVList 
 * to the list of arguments. 
 */
class TAO_DynamicInterface_Export TAO_DII_Arguments_Converter_Impl
  : public TAO_DII_Arguments_Converter
{
public:

  virtual void convert ( TAO_ServerRequest & server_request,
                         TAO::Argument * const args[],
                         size_t nargs
                         ACE_ENV_ARG_DECL );
 
  // Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_DII_Arguments_Converter_Impl_Initializer =
  TAO_DII_Arguments_Converter_Impl::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_DII_Arguments_Converter_Impl)
ACE_FACTORY_DECLARE (TAO_DynamicInterface, TAO_DII_Arguments_Converter_Impl)


#include /**/ "ace/post.h"

#endif /* TAO_DII_ARGUMENTS_CONVERTER_IMPL_H */
