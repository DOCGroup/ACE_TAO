/**
 *  @file    TAO_Object_Group_Creator.h
 *
 *  This file is part of Fault Tolerant CORBA.
 *  Utility to Create Object Group
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */

#ifndef TAO_OBJECT_GROUP_CREATOR_H
#define TAO_OBJECT_GROUP_CREATOR_H
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <orbsvcs/FT_ReplicationManagerC.h>
#include <orbsvcs/PortableGroupC.h>
#include <ace/SString.h>

#include <vector>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace TAO
{
  class Object_Group_Creator
  {
    using StringVec = std::vector<ACE_CString>;
   public:
    ///////////////////////////
    // construction/destruction
    Object_Group_Creator ();

    ~Object_Group_Creator ();

    /////////////////
    // pre-init methods
    int set_factory_registry (PortableGroup::FactoryRegistry_ptr factory);

    /////////////////
    // initialization
    int init (CORBA::ORB_ptr orb);

    /////////////////
    // functionality
    CORBA::Object_ptr create_group(
        char const *role,
        int write_iors);

    /////////////////
    // functionality
    CORBA::Object_ptr create_infrastructure_managed_group(
        char const *type_id);

    int unregister_role(char const *role);

    ////////////
    // shut down
    int fini ();

    /////////////////
    // implementation
  private:
    int write_ior_file(char const *outputFile, char const *ior);

  int create_detector_for_replica (
    CORBA::Object_ptr replica,
    char const *role,
    char const *type_id,
    PortableGroup::ObjectGroupId group_id,
    PortableGroup::Location const & location);

    ////////////////////
    // forbidden methods
   private:
      Object_Group_Creator (Object_Group_Creator const & rhs);
      Object_Group_Creator & operator = (Object_Group_Creator const & rhs);

    /// @name Data members
    /// @{
   private:
    CORBA::ORB_var orb_;
    PortableGroup::FactoryRegistry_var registry_;

    ::FT::ReplicationManager_var replication_manager_;

    ::PortableGroup::FactoryInfos_var detector_infos_;
    CORBA::String_var detector_type_id_;

    /**
     * bool: true if we have a real replication manager
     */
    int have_replication_manager_;
    /// @}
  };
} // namespace TAO
TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAO_OBJECT_GROUP_CREATOR_H
