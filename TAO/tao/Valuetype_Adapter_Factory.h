// -*- C++ -*-

//=============================================================================
/**
 *  @file    Valuetype_Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author  Kees van Marle <kvmarle@remedy.nl>
 */
//=============================================================================


#ifndef TAO_VALUETYPE_ADAPTER_FACTORY_H
#define TAO_VALUETYPE_ADAPTER_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/TAO_Export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Valuetype_Adapter;

/**
 * @class TAO_Valuetype_Adapter_Factory
 *
 * @brief TAO_Valuetype_Adapter_Factory.
 *
 * Class that creates one instance of TAO_Valuetype_Adapter per
 * ORB on the ORB's first usage of its valuetype_adapter_. This is a base
 * class for the actual implementation in the TAO_Valuetype library.
 */
class TAO_Export TAO_Valuetype_Adapter_Factory
  : public ACE_Service_Object
{
public:
  virtual ~TAO_Valuetype_Adapter_Factory (void);

  virtual TAO_Valuetype_Adapter * create (void) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_VALUETYPE_ADAPTER_FACTORY_H */
