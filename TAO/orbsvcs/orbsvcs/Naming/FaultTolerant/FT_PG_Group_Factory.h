// -*- C++ -*-

//=============================================================================
/**
 * @file  FT_PG_Group_Factory.h
 *
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

namespace FT_Naming
{
  struct ObjectGroupUpdate;
}

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FT_Naming_Replication_Manager;

namespace TAO
{
  /**
   * class FT_PG_Group_Factory
   */
  class TAO_FtNaming_Export FT_PG_Group_Factory : public PG_Group_Factory
  {
  public:
    /// Constructor.
    FT_PG_Group_Factory () = default;

    /// Destructor.
    ~FT_PG_Group_Factory () override = default;

    /**
     * indicate the object group state is stale.
     * only valid when object group persistence is enabled.
     */
    void set_object_group_stale (const FT_Naming::ObjectGroupUpdate & group_info);

    void set_replicator (TAO_FT_Naming_Replication_Manager *repl);

  protected:
    PG_Object_Group_Storable * create_persistent_group (
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      CORBA::Object_ptr empty_group,
      const PortableGroup::TagGroupTaggedComponent & tagged_component,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      const TAO::PG_Property_Set_var & type_properties,
      TAO::Storable_Factory & storable_factory) override;

    PG_Object_Group_Storable * restore_persistent_group (
      PortableGroup::ObjectGroupId group_id,
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      TAO::Storable_Factory & storable_factory) override;

    TAO_FT_Naming_Replication_Manager *replicator_ {};
  };
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* FT_TAO_PG_GROUP_FACTORY_H */
