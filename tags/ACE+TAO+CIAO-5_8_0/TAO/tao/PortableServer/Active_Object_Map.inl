// -*- C++ -*-
//
// $Id$

#include "tao/PortableServer/Active_Object_Map_Entry.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
TAO_Active_Object_Map::is_servant_in_map (PortableServer::Servant servant,
                                          bool &deactivated)
{
  return this->id_uniqueness_strategy_->is_servant_in_map (servant,
                                                           deactivated);
}

ACE_INLINE int
TAO_Active_Object_Map::bind_using_system_id_returning_system_id (
  PortableServer::Servant servant,
  CORBA::Short priority,
  PortableServer::ObjectId_out system_id)
{
  if (servant == 0 && !this->using_active_maps_)
    {
      PortableServer::ObjectId id;

      int result = this->user_id_map_->create_key (id);

      if (result == 0)
        {
          ACE_NEW_RETURN (system_id,
                          PortableServer::ObjectId (id),
                          -1);
        }

      return result;
    }

  TAO_Active_Object_Map_Entry *entry = 0;

  int result =
    this->id_assignment_strategy_->bind_using_system_id (servant,
                                                         priority,
                                                         entry);

  if (result == 0)
    {
      result = this->id_hint_strategy_->system_id (system_id,
                                                   *entry);
    }

  return result;
}

ACE_INLINE int
TAO_Active_Object_Map::bind_using_system_id_returning_user_id (
  PortableServer::Servant servant,
  CORBA::Short priority,
  PortableServer::ObjectId_out user_id)
{
  TAO_Active_Object_Map_Entry *entry = 0;

  int result =
    this->id_assignment_strategy_->bind_using_system_id (servant,
                                                         priority,
                                                         entry);
  if (result == 0)
    ACE_NEW_RETURN (user_id,
                    PortableServer::ObjectId (entry->user_id_),
                    -1);
  return result;
}

ACE_INLINE int
TAO_Active_Object_Map::bind_using_user_id (
  PortableServer::Servant servant,
  const PortableServer::ObjectId &user_id,
  CORBA::Short priority)
{
  TAO_Active_Object_Map_Entry *entry = 0;
  return this->id_uniqueness_strategy_->bind_using_user_id (servant,
                                                            user_id,
                                                            priority,
                                                            entry);
}

ACE_INLINE int
TAO_Active_Object_Map::find_system_id_using_user_id (
  const PortableServer::ObjectId &user_id,
  CORBA::Short priority,
  PortableServer::ObjectId_out system_id)
{
  if (!this->using_active_maps_)
    {
      ACE_NEW_RETURN (system_id,
                      PortableServer::ObjectId (user_id),
                      -1);

      return 0;
    }

  TAO_Active_Object_Map_Entry *entry = 0;
  int result =
    this->id_uniqueness_strategy_->bind_using_user_id (0,
                                                       user_id,
                                                       priority,
                                                       entry);
  if (result == 0)
    {
      result =
        this->id_hint_strategy_->system_id (system_id, *entry);
    }

  return result;
}

ACE_INLINE int
TAO_Active_Object_Map::rebind_using_user_id_and_system_id (
  PortableServer::Servant servant,
  const PortableServer::ObjectId &user_id,
  const PortableServer::ObjectId &,
  TAO_Active_Object_Map_Entry *&entry)
{
  return this->id_uniqueness_strategy_->bind_using_user_id (servant,
                                                            user_id,
                                                            -1,
                                                            entry);
}

ACE_INLINE int
TAO_Active_Object_Map::unbind_using_user_id (
  const PortableServer::ObjectId &user_id)
{
  return this->id_uniqueness_strategy_->unbind_using_user_id (user_id);
}

ACE_INLINE int
TAO_Active_Object_Map::find_user_id_using_servant (
  PortableServer::Servant servant,
  PortableServer::ObjectId_out user_id)
{
  return
    this->id_uniqueness_strategy_->find_user_id_using_servant (servant,
                                                               user_id);
}

ACE_INLINE int
TAO_Active_Object_Map::find_system_id_using_servant (
  PortableServer::Servant servant,
  PortableServer::ObjectId_out system_id,
  CORBA::Short &priority)
{
  return
    this->id_uniqueness_strategy_->find_system_id_using_servant (servant,
                                                                 system_id,
                                                                 priority);
}

ACE_INLINE int
TAO_Active_Object_Map::find_servant_using_user_id (
  const PortableServer::ObjectId &user_id,
  PortableServer::Servant &servant)
{
  TAO_Active_Object_Map_Entry *entry = 0;
  int result = this->user_id_map_->find (user_id, entry);

  if (result == 0)
    {
      if (entry->deactivated_)
        {
          result = -1;
        }
      else if (entry->servant_ == 0)
        {
          result = -1;
        }
      else
        {
          servant = entry->servant_;
        }
    }

  return result;
}

ACE_INLINE int
TAO_Active_Object_Map::find_servant_using_system_id_and_user_id (
  const PortableServer::ObjectId &system_id,
  const PortableServer::ObjectId &user_id,
  PortableServer::Servant &servant,
  TAO_Active_Object_Map_Entry *&entry)
{
  return
    this->lifespan_strategy_->find_servant_using_system_id_and_user_id (
      system_id,
      user_id,
      servant,
      entry);
}

ACE_INLINE int
TAO_Active_Object_Map::find_entry_using_user_id (
  const PortableServer::ObjectId &user_id,
  TAO_Active_Object_Map_Entry *&entry)
{
  int result = this->user_id_map_->find (user_id, entry);

  if (result == 0)
    {
      if (entry->deactivated_)
        {
          result = -1;
        }
    }

  return result;
}

ACE_INLINE int
TAO_Active_Object_Map::find_servant_and_system_id_using_user_id (
  const PortableServer::ObjectId &user_id,
  PortableServer::Servant &servant,
  PortableServer::ObjectId_out system_id,
  CORBA::Short &priority)
{
  TAO_Active_Object_Map_Entry *entry = 0;
  int result = this->find_entry_using_user_id (user_id, entry);

  if (result == 0)
    {
      if (entry->servant_ == 0)
        {
          result = -1;
        }
      else
        {
          result = this->id_hint_strategy_->system_id (system_id,
                                                       *entry);
          if (result == 0)
            {
              servant = entry->servant_;
              priority = entry->priority_;
            }
        }
    }

  return result;
}

ACE_INLINE int
TAO_Active_Object_Map::find_user_id_using_system_id (
  const PortableServer::ObjectId &system_id,
  PortableServer::ObjectId_out user_id)
{
  PortableServer::ObjectId id;
  if (this->id_hint_strategy_->recover_key (system_id, id) == 0)
    {
      ACE_NEW_RETURN (user_id,
                      PortableServer::ObjectId (id),
                      -1);
    }

  return 0;
}

ACE_INLINE int
TAO_Active_Object_Map::find_user_id_using_system_id (
  const PortableServer::ObjectId &system_id,
  PortableServer::ObjectId &user_id)
{
  return this->id_hint_strategy_->recover_key (system_id, user_id);
}

ACE_INLINE CORBA::Boolean
TAO_Active_Object_Map::remaining_activations (PortableServer::Servant servant)
{
  return this->id_uniqueness_strategy_->remaining_activations (servant);
}

ACE_INLINE size_t
TAO_Active_Object_Map::current_size (void)
{
  return this->user_id_map_->current_size ();
}

/* static */
ACE_INLINE size_t
TAO_Active_Object_Map::system_id_size (void)
{
  return TAO_Active_Object_Map::system_id_size_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
