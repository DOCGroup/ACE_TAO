// -*- C++ -*-

//=============================================================================
/**
 *  @file    Collocated_Arguments_Converter.h
 *
 *  $Id$
 *
 *  @author  Yan Dai <dai_y@ociweb.com>
 *  @author  Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_COLLOCATED_ARGUMENTS_CONVERTER_H
#define TAO_COLLOCATED_ARGUMENTS_CONVERTER_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Server_Request.h"
#include "tao/CDR.h"
#include "ace/Service_Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
/**
 * @class Collocated_Arguments_Converter
 *
 * @brief Collocated_Arguments_Converter.
 *
 * Abstract class that convert arguments in the case of a collocated call.
 */
class TAO_PortableServer_Export Collocated_Arguments_Converter :
  public ACE_Service_Object
{
public:

  virtual ~Collocated_Arguments_Converter (void);

  virtual void convert_request (TAO_ServerRequest & server_request,
                                TAO::Argument * const args[],
                                size_t nargs) = 0;

  virtual void dsi_convert_request (TAO_ServerRequest & server_request,
                                    TAO_OutputCDR & output) = 0;

  virtual void convert_reply (TAO_ServerRequest & server_request,
                              TAO::Argument * const args[],
                              size_t nargs) = 0;

  virtual void dsi_convert_reply (TAO_ServerRequest & server_request,
                                  TAO_InputCDR & input) = 0;

  virtual void handle_corba_exception (TAO_ServerRequest & server_request,
                                       CORBA::Exception *exception) = 0;
};
}
TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_COLLOCATED_ARGUMENTS_CONVERTER_H */

