// -*- C++ -*-

//=============================================================================
/**
 * @file  FT_PG_Group_Factory.h
 *
 * $Id$
 *
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#ifndef FT_TAO_PG_GROUP_FACTORY_H
#define FT_TAO_PG_GROUP_FACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Naming/FaultTolerant/ftnaming_export.h"

#include "orbsvcs/PortableGroup/PG_Group_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace FT_Naming
{
  struct ObjectGroupUpdate;
}

namespace TAO
{

  /**
   * class FT_PG_Group_Factory
   */
  class TAO_FtNaming_Export FT_PG_Group_Factory : public PG_Group_Factory
  {
  public:

    /// Constructor.
    FT_PG_Group_Factory ();

    /// Destructor.
    ~FT_PG_Group_Factory ();

    /**
     * indicate the object group state is stale.
     * only valid when object group persistence is enabled.
     */
    void set_object_group_stale (const FT_Naming::ObjectGroupUpdate & group_info);

  protected:

    virtual PG_Object_Group_Storable * create_persistent_group (
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      CORBA::Object_ptr empty_group,
      const PortableGroup::TagGroupTaggedComponent & tagged_component,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      TAO::PG_Property_Set * type_properties,
      TAO::Storable_Factory & storable_factory);

    virtual PG_Object_Group_Storable * restore_persistent_group (
      PortableGroup::ObjectGroupId group_id,
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      TAO::Storable_Factory & storable_factory);

  };
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* FT_TAO_PG_GROUP_FACTORY_H */
