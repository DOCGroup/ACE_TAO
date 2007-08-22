// -*- C++ -*-

//=============================================================================
/**
 *  @file    Endpoint_Acceptor_Filter_Factory.h
 *
 *  $Id$
 *
 *  The factory of the TAO_Endpoint_Acceptor_Filter objects.
 *
 *  @author  Yan Dai <dai_y@ociweb.com>
 */
//=============================================================================

#ifndef TAO_ENDPOINT_ACCEPTOR_FILTER_FACTORY_H
#define TAO_ENDPOINT_ACCEPTOR_FILTER_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"

#include "tao/EndpointPolicy/EndpointPolicy_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Acceptor_Filter_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Endpoint_Acceptor_Filter_Factory
 *
 * @brief The factory of the TAO_Endpoint_Acceptor_Filter objects.
 *
 * Factory service object that creates the TAO_Endpoint_Acceptor_Filter objects.
 */
class TAO_EndpointPolicy_Export TAO_Endpoint_Acceptor_Filter_Factory
  : public TAO_Acceptor_Filter_Factory
{
public:
  virtual ~TAO_Endpoint_Acceptor_Filter_Factory(void);

  /// Create a new TAO_Endpoint_Acceptor_Filter object.
  TAO_Acceptor_Filter* create_object (TAO_POA_Manager& poamanager);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_EndpointPolicy, TAO_Endpoint_Acceptor_Filter_Factory)
ACE_FACTORY_DECLARE (TAO_EndpointPolicy, TAO_Endpoint_Acceptor_Filter_Factory)


#include /**/ "ace/post.h"

#endif /* TAO_ENDPOINT_ACCEPTOR_FILTER_FACTORY_H */
