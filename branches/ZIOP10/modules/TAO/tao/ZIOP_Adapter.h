// -*- C++ -*-

//=============================================================================
/**
 *  @file    ZIOP_Adapter.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================
#ifndef TAO_ZIOP_ADAPTER_H
#define TAO_ZIOP_ADAPTER_H

#include /**/ "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"
#include "tao/orbconf.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Profile_Transport_Resolver.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Policy_Validator;

/**
 * @class TAO_ZIOP_Adapter
 *
 * @brief TAO_ZIOP_Adapter
 *
 * Class that offers an interface to the ORB to load and manipulate
 * ZIOP library.
 */
class TAO_Export TAO_ZIOP_Adapter : public ACE_Service_Object
{
public:
  virtual bool decompress (TAO_ServerRequest& server_request) = 0;

  virtual bool marshal_data (TAO_Operation_Details &details, TAO_OutputCDR &stream, TAO::Profile_Transport_Resolver &resolver_) = 0;

  virtual void load_policy_validators (TAO_Policy_Validator &validator) = 0;

  /// The virtual destructor
  virtual ~TAO_ZIOP_Adapter (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ZIOP_ADAPTER_H */

