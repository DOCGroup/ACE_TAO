// -*- C++ -*-

//=============================================================================
/**
 *  @file    Acceptor_Filter_Factory.h
 *
 *  $Id$
 *
 *  Provides default acceptor filters for use by the POA.
 *
 *  @author  Yan Dai <dai_y@ociweb.com>
 *  @author  Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#ifndef TAO_ACCEPTOR_FILTER_FACTORY_H
#define TAO_ACCEPTOR_FILTER_FACTORY_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"
#include "tao/orbconf.h"

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_POA_Manager;
class TAO_Acceptor_Filter;

/**
 * @class TAO_Acceptor_Filter_Factory
 *
 * @brief Default class for factories providing the endpoint acceptor
 *        filter objects.
 *
 * The TAO_Acceptor_Filter_Factory is a loadable service object. It exists
 * to allow external application of filters used to control the assembly of
 * object references and how endpoints are assigned.
 */
class TAO_PortableServer_Export TAO_Acceptor_Filter_Factory
  : public ACE_Service_Object
{
public:
  virtual ~TAO_Acceptor_Filter_Factory(void);

  virtual TAO_Acceptor_Filter* create_object (TAO_POA_Manager& poamanager);

  /// Static initializer ensures the factory is loaded
  static int initialize (void);
};


static int
TAO_Requires_Acceptor_Filter_Factory =
TAO_Acceptor_Filter_Factory::initialize ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_Acceptor_Filter_Factory)
ACE_FACTORY_DECLARE (TAO_PortableServer, TAO_Acceptor_Filter_Factory)

#endif /* #if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)*/

#include /**/ "ace/post.h"

#endif /* TAO_ACCEPTOR_FILTER_FACTORY_H */
