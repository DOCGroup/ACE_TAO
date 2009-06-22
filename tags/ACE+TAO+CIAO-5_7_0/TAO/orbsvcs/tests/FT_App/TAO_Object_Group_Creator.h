/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    TAO_Object_Group_Creator.h
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  Utility to Create Object Group
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#ifndef TAO_OBJECT_GROUP_CREATOR_H
#define TAO_OBJECT_GROUP_CREATOR_H
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <orbsvcs/FT_ReplicationManagerC.h>
#include <orbsvcs/PortableGroupC.h>
#include <ace/SString.h>
#include <ace/Vector_T.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace TAO
{
  class Object_Group_Creator
  {
    typedef ACE_Vector<ACE_CString> StringVec;
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
        const char * role,
        int write_iors);

    /////////////////
    // functionality
    CORBA::Object_ptr create_infrastructure_managed_group(
        const char * type_id);

    int unregister_role(const char * role);

    ////////////
    // shut down
    int fini ();

    /////////////////
    // implementation
  private:
    int write_ior_file(const char * outputFile, const char * ior);

  int create_detector_for_replica (
    CORBA::Object_ptr replica,
    const char * role,
    const char * type_id,
    PortableGroup::ObjectGroupId group_id,
    const PortableGroup::Location & location);

    ////////////////////
    // forbidden methods
   private:
      Object_Group_Creator (const Object_Group_Creator & rhs);
      Object_Group_Creator & operator = (const Object_Group_Creator & rhs);

    ////////////////
    // Data members
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
  };

} // namespace TAO
TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAO_OBJECT_GROUP_CREATOR_H
