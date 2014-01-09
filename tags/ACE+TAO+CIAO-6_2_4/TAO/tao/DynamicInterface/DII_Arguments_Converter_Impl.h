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

#include "tao/PortableServer/Collocated_Arguments_Converter.h"

#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


/**
 * @class TAO_DII_Arguments_Converter_Impl
 *
 * @brief TAO_DII_Arguments_Converter_Impl.
 *
 * This class provides the implementation to expand the DII arguments in NVList
 * to the list of arguments.
 */
class TAO_DynamicInterface_Export TAO_DII_Arguments_Converter_Impl
  : public TAO::Collocated_Arguments_Converter
{
public:

  virtual void convert_request (TAO_ServerRequest & server_request,
                                TAO::Argument * const args[],
                                size_t nargs);

  virtual void dsi_convert_request (TAO_ServerRequest & server_request,
                                    TAO_OutputCDR & output);

  virtual void convert_reply (TAO_ServerRequest & server_request,
                              TAO::Argument * const args[],
                              size_t nargs);

  virtual void dsi_convert_reply (TAO_ServerRequest & server_request,
                                  TAO_InputCDR & input);

  virtual void handle_corba_exception (TAO_ServerRequest & server_request,
                                       CORBA::Exception *exception);

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


