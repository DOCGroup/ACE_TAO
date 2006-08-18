// -*- C++ -*-

//=============================================================================
/**
 *  @file    DII_Arguments_Converter.h
 *
 *  $Id$
 *
 *  @author  Yan Dai <dai_y@ociweb.com>
 */
//=============================================================================


#ifndef TAO_DII_ARGUMENTS_CONVERTER_H
#define TAO_DII_ARGUMENTS_CONVERTER_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Server_Request.h"
#include "ace/Service_Object.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DII_Arguments_Converter
 *
 * @brief TAO_DII_Arguments_Converter.
 *
 * Abstract class that convert the DII arguments in NVList to the
 * list of arguments. This is a base class for the actual implementation 
 * in the DynamicInterface library.
 */
class TAO_PortableServer_Export TAO_DII_Arguments_Converter : public ACE_Service_Object
{
public:
  
  virtual void convert ( TAO_ServerRequest & server_request,
                         TAO::Argument * const args[],
                         size_t nargs
                         ACE_ENV_ARG_DECL ) = 0;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DII_ARGUMENTS_CONVERTER_H */
