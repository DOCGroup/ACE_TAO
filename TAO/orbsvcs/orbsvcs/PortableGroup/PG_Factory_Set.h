// -*- C++ -*-

//=======================================================================
/**
 *  @file    PG_Factory_Set.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_PG_FACTORY_SET_H
#define TAO_PG_FACTORY_SET_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupC.h"

#include "ace/Array_Base.h"


/**
 * @class TAO_PG_Factory_Node
 *
 * @brief Structure that contains all factory-specific information.
 *
 * Each member created by the infrastructure instead of the
 * application will have a corresponding TAO_PG_Factory_Node structure
 * associated with it.  A list of these will be maintained by the
 * infrastructure so that it is possible for the instrastructure to
 * destroy members it created when destroying the object group.
 */
struct TAO_PG_Factory_Node
{
  /// Member factory information.
  PortableGroup::FactoryInfo factory_info;

  /// FactoryCreationId assigned to the member.
  PortableGroup::GenericFactory::FactoryCreationId_var factory_creation_id;

};

typedef ACE_Array_Base<TAO_PG_Factory_Node> TAO_PG_Factory_Set;


#include /**/ "ace/post.h"

#endif  /* TAO_PG_FACTORY_SET_H */
