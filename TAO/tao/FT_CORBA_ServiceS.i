//$Id$
ACE_INLINE void
FT_POA_ReplicationManager::set_default_properties_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_PropertyManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_PropertyManager::set_default_properties_skel (
      req,
      (FT_POA_PropertyManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}

ACE_INLINE void
FT_POA_ReplicationManager::get_default_properties_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_PropertyManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_PropertyManager::get_default_properties_skel (
      req,
      (FT_POA_PropertyManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}


ACE_INLINE void
FT_POA_ReplicationManager::remove_default_properties_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_PropertyManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_PropertyManager::remove_default_properties_skel (
      req,
      (FT_POA_PropertyManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}


ACE_INLINE void
FT_POA_ReplicationManager::set_type_properties_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_PropertyManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_PropertyManager::set_type_properties_skel (
      req,
      (FT_POA_PropertyManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}

ACE_INLINE void
FT_POA_ReplicationManager::get_type_properties_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_PropertyManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_PropertyManager::get_type_properties_skel (
      req,
      (FT_POA_PropertyManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}


ACE_INLINE void
FT_POA_ReplicationManager::remove_type_properties_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_PropertyManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_PropertyManager::remove_type_properties_skel (
      req,
      (FT_POA_PropertyManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}


ACE_INLINE void
FT_POA_ReplicationManager::set_properties_dynamically_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_PropertyManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_PropertyManager::set_properties_dynamically_skel (
      req,
      (FT_POA_PropertyManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}

ACE_INLINE void FT_POA_ReplicationManager::get_properties_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_PropertyManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_PropertyManager::get_properties_skel (
      req,
      (FT_POA_PropertyManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_ReplicationManager::create_member_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_ObjectGroupManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_ObjectGroupManager::create_member_skel (
      req,
      (FT_POA_ObjectGroupManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_ReplicationManager::add_member_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_ObjectGroupManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_ObjectGroupManager::add_member_skel (
      req,
      (FT_POA_ObjectGroupManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_ReplicationManager::remove_member_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_ObjectGroupManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_ObjectGroupManager::remove_member_skel (
      req,
      (FT_POA_ObjectGroupManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_ReplicationManager::set_primary_member_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_ObjectGroupManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_ObjectGroupManager::set_primary_member_skel (
      req,
      (FT_POA_ObjectGroupManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_ReplicationManager::locations_of_members_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_ObjectGroupManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_ObjectGroupManager::locations_of_members_skel (
      req,
      (FT_POA_ObjectGroupManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_ReplicationManager::get_object_group_id_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_ObjectGroupManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_ObjectGroupManager::get_object_group_id_skel (
      req,
      (FT_POA_ObjectGroupManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_ReplicationManager::get_object_group_ref_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_ObjectGroupManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_ObjectGroupManager::get_object_group_ref_skel (
      req,
      (FT_POA_ObjectGroupManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_ReplicationManager::get_member_ref_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_ObjectGroupManager_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_ObjectGroupManager::get_member_ref_skel (
      req,
      (FT_POA_ObjectGroupManager_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_ReplicationManager::create_object_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_GenericFactory_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_GenericFactory::create_object_skel (
      req,
      (FT_POA_GenericFactory_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_ReplicationManager::delete_object_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_GenericFactory_ptr impl = (FT_POA_ReplicationManager_ptr) obj;
  FT_POA_GenericFactory::delete_object_skel (
      req,
      (FT_POA_GenericFactory_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_Updateable::get_state_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_Checkpointable_ptr impl = (FT_POA_Updateable_ptr) obj;
  FT_POA_Checkpointable::get_state_skel (
      req,
      (FT_POA_Checkpointable_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
ACE_INLINE void FT_POA_Updateable::set_state_skel (
    CORBA::ServerRequest &req,
    void *obj,
    void *context,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_Checkpointable_ptr impl = (FT_POA_Updateable_ptr) obj;
  FT_POA_Checkpointable::set_state_skel (
      req,
      (FT_POA_Checkpointable_ptr) impl,
      context,
       ACE_TRY_ENV
    );
}
