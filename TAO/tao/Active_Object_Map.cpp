// $Id$

#include "tao/Active_Object_Map.h"
#include "tao/params.h"

#if !defined (__ACE_INLINE__)
# include "tao/Active_Object_Map.i"
#endif /* __ACE_INLINE__ */

////////////////////////////////////////////////////////////////////////////////

TAO_Incremental_Key_Generator::TAO_Incremental_Key_Generator (void)
  : counter_ (0)
{
}

int
TAO_Incremental_Key_Generator::operator() (PortableServer::ObjectId &id)
{
  // Resize to accommodate the counter.
  id.length (sizeof this->counter_);

  // Add new key data.
  ACE_OS::memcpy (id.get_buffer (),
                  &++this->counter_,
                  sizeof this->counter_);

  // Success.
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

u_long
TAO_ObjectId_Hash::operator () (const PortableServer::ObjectId &id) const
{
  return ACE::hash_pjw ((const char *) id.get_buffer (),
                        id.length ());
}

////////////////////////////////////////////////////////////////////////////////

int
TAO_Ignore_Original_Key_Adapter::encode (const PortableServer::ObjectId &original_key,
                                         const ACE_Active_Map_Manager_Key &active_key,
                                         PortableServer::ObjectId &modified_key)
{
  // Size of active key.
  size_t active_key_size = active_key.size ();

  // Resize to accommodate both the original data and the new active key.
  modified_key.length (active_key_size);

  // Copy active key data into user key.
  active_key.encode (modified_key.get_buffer ());

  // Success.
  return 0;
}

int
TAO_Ignore_Original_Key_Adapter::decode (const PortableServer::ObjectId &modified_key,
                                         ACE_Active_Map_Manager_Key &active_key)
{
  // Read off value of index and generation.
  active_key.decode (modified_key.get_buffer ());

  // Success.
  return 0;
}

int
TAO_Ignore_Original_Key_Adapter::decode (const PortableServer::ObjectId &modified_key,
                                         PortableServer::ObjectId &original_key)
{
  // Smartly copy all the data; <original_key does not own the data>.
  original_key.replace (modified_key.maximum (),
                        modified_key.length (),
                        ACE_const_cast (CORBA::Octet *,
                                        modified_key.get_buffer ()),
                        0);

  // Success.
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

int
TAO_Preserve_Original_Key_Adapter::encode (const PortableServer::ObjectId &original_key,
                                           const ACE_Active_Map_Manager_Key &active_key,
                                           PortableServer::ObjectId &modified_key)
{
  // Size of active key.
  size_t active_key_size = active_key.size ();

  // Resize to accommodate both the original data and the new active key.
  modified_key.length (active_key_size + original_key.length ());

  // Copy active key data into user key.
  active_key.encode (modified_key.get_buffer ());

  // Copy the original key after the active key.
  ACE_OS::memcpy (modified_key.get_buffer () + active_key_size,
                  original_key.get_buffer (),
                  original_key.length ());

  // Success.
  return 0;
}

int
TAO_Preserve_Original_Key_Adapter::decode (const PortableServer::ObjectId &modified_key,
                                           ACE_Active_Map_Manager_Key &active_key)
{
  // Read off value of index and generation.
  active_key.decode (modified_key.get_buffer ());

  // Success.
  return 0;
}

int
TAO_Preserve_Original_Key_Adapter::decode (const PortableServer::ObjectId &modified_key,
                                           PortableServer::ObjectId &original_key)
{
  // Size of active key.
  size_t active_key_size = ACE_Active_Map_Manager_Key::size ();

  // Smartly copy all the data; <original_key does not own the data>.
  original_key.replace (modified_key.maximum () - active_key_size,
                        modified_key.length () - active_key_size,
                        ACE_const_cast (CORBA::Octet *,
                                        modified_key.get_buffer ()) + active_key_size,
                        0);

  // Success.
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

TAO_Active_Object_Map::TAO_Active_Object_Map (int user_id_policy,
                                              int unique_id_policy,
                                              int persistent_id_policy,
                                              const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters)
  : user_id_map_ (0),
    servant_map_ (0),
    id_uniqueness_strategy_ (0),
    lifespan_strategy_ (0),
    id_assignment_strategy_ (0),
    id_hint_strategy_ (0),
    system_id_size_ (0)
{
  if (unique_id_policy)
    {
      ACE_NEW (this->id_uniqueness_strategy_,
               TAO_Unique_Id_Strategy);
    }
  else
    {
      ACE_NEW (this->id_uniqueness_strategy_,
               TAO_Multiple_Id_Strategy);
    }

  if (persistent_id_policy)
    {
      ACE_NEW (this->lifespan_strategy_,
               TAO_Persistent_Strategy);
    }
  else
    {
      ACE_NEW (this->lifespan_strategy_,
               TAO_Transient_Strategy);
    }

  if (user_id_policy)
    {
      ACE_NEW (this->id_assignment_strategy_,
               TAO_User_Id_Strategy);
    }
  else
    {
      if (unique_id_policy)
        {
          ACE_NEW (this->id_assignment_strategy_,
                   TAO_System_Id_With_Unique_Id_Strategy);
        }
      else
        {
          ACE_NEW (this->id_assignment_strategy_,
                   TAO_System_Id_With_Multiple_Id_Strategy);
        }
    }

  if ((user_id_policy ||
       creation_parameters.allow_reactivation_of_system_ids_) &&
      creation_parameters.use_active_hint_in_ids_)
    {
      ACE_NEW (this->id_hint_strategy_,
               TAO_Active_Hint_Strategy);
    }
  else
    {
      ACE_NEW (this->id_hint_strategy_,
               TAO_No_Hint_Strategy);
    }

  if (unique_id_policy)
    {
      switch (creation_parameters.reverse_object_lookup_strategy_for_unique_id_policy_)
        {
        case TAO_LINEAR:
          ACE_NEW (this->servant_map_,
                   servant_linear_map (creation_parameters.active_object_map_size_));
          break;
        case TAO_DYNAMIC_HASH:
        default:
          ACE_NEW (this->servant_map_,
                   servant_hash_map (creation_parameters.active_object_map_size_));
          break;
        }
    }

  if (user_id_policy ||
      creation_parameters.allow_reactivation_of_system_ids_)
    {
      switch (creation_parameters.object_lookup_strategy_for_user_id_policy_)
        {
        case TAO_LINEAR:

          ACE_NEW (this->user_id_map_,
                   user_id_linear_map (creation_parameters.active_object_map_size_));

          this->system_id_size_ = sizeof (CORBA::ULong);

          break;

        case TAO_DYNAMIC_HASH:
        default:

          ACE_NEW (this->user_id_map_,
                   user_id_hash_map (creation_parameters.active_object_map_size_));

          this->system_id_size_ = sizeof (CORBA::ULong);

          break;
        }
    }
  else
    {
      switch (creation_parameters.object_lookup_strategy_for_system_id_policy_)
        {
        case TAO_LINEAR:

          ACE_NEW (this->user_id_map_,
                   user_id_linear_map (creation_parameters.active_object_map_size_));

          this->system_id_size_ = sizeof (CORBA::ULong);

          break;

        case TAO_DYNAMIC_HASH:

          ACE_NEW (this->user_id_map_,
                   user_id_hash_map (creation_parameters.active_object_map_size_));

          this->system_id_size_ = sizeof (CORBA::ULong);

          break;

        case TAO_ACTIVE_DEMUX:
        default:

          ACE_NEW (this->user_id_map_,
                   user_id_active_map (creation_parameters.active_object_map_size_));

          this->system_id_size_ = ACE_Active_Map_Manager_Key::size ();

          break;
        }
    }

  this->id_uniqueness_strategy_->set_active_object_map (this);
  this->lifespan_strategy_->set_active_object_map (this);
  this->id_assignment_strategy_->set_active_object_map (this);

  this->system_id_size_ += this->id_hint_strategy_->hint_size ();
}

TAO_Active_Object_Map::~TAO_Active_Object_Map (void)
{
  delete this->id_uniqueness_strategy_;
  delete this->lifespan_strategy_;
  delete this->id_assignment_strategy_;
  delete this->id_hint_strategy_;
  delete this->servant_map_;
  delete this->user_id_map_;
}

int
TAO_Active_Object_Map::is_servant_in_map (PortableServer::Servant servant)
{
  return this->id_uniqueness_strategy_->is_servant_in_map (servant);
}

int
TAO_Active_Object_Map::is_user_id_in_map (const PortableServer::ObjectId &user_id)
{
  Map_Entry *entry = 0;
  int result = this->user_id_map_->find (user_id,
                                         entry);
  if (result == 0)
    {
      if (entry->servant_ == 0)
        {
          result = 0;
        }
      else
        {
          result = 1;
        }
    }
  else
    {
      result = 0;
    }

  return result;
}

int
TAO_Active_Object_Map::bind_using_system_id_returning_system_id (PortableServer::Servant servant,
                                                                 PortableServer::ObjectId_out system_id)
{
  Map_Entry *entry = 0;

  int result = this->id_assignment_strategy_->bind_using_system_id (servant,
                                                                    entry);

  if (result == 0)
    {
      result = this->id_hint_strategy_->system_id (system_id,
                                                   *entry);
    }

  return result;
}

int
TAO_Active_Object_Map::bind_using_system_id_returning_user_id (PortableServer::Servant servant,
                                                               PortableServer::ObjectId_out user_id)
{
  Map_Entry *entry = 0;

  int result = this->id_assignment_strategy_->bind_using_system_id (servant,
                                                                    entry);

  if (result == 0)
    {
      ACE_NEW_RETURN (user_id,
                      PortableServer::ObjectId (entry->user_id_),
                      -1);
    }

  return result;
}

int
TAO_Active_Object_Map::bind_using_user_id (PortableServer::Servant servant,
                                           const PortableServer::ObjectId &user_id)
{
  Map_Entry *entry = 0;
  return this->id_uniqueness_strategy_->bind_using_user_id (servant,
                                                            user_id,
                                                            entry);
}

int
TAO_Active_Object_Map::find_system_id_using_user_id (const PortableServer::ObjectId &user_id,
                                                     PortableServer::ObjectId_out system_id)
{
  Map_Entry *entry = 0;
  int result = this->id_uniqueness_strategy_->bind_using_user_id (0,
                                                                  user_id,
                                                                  entry);
  if (result == 0)
    {
      result = this->id_hint_strategy_->system_id (system_id,
                                                   *entry);
    }

  return result;
}

int
TAO_Active_Object_Map::rebind_using_user_id_and_system_id (PortableServer::Servant servant,
                                                           const PortableServer::ObjectId &user_id,
                                                           const PortableServer::ObjectId &system_id)
{
  ACE_UNUSED_ARG (system_id);

  Map_Entry *entry = 0;
  return this->id_uniqueness_strategy_->bind_using_user_id (servant,
                                                            user_id,
                                                            entry);
}

int
TAO_Active_Object_Map::unbind_using_user_id (const PortableServer::ObjectId &user_id)
{
  return this->id_uniqueness_strategy_->unbind_using_user_id (user_id);
}

int
TAO_Active_Object_Map::find_user_id_using_servant (PortableServer::Servant servant,
                                                   PortableServer::ObjectId_out user_id)
{
  return this->id_uniqueness_strategy_->find_user_id_using_servant (servant,
                                                                    user_id);
}

int
TAO_Active_Object_Map::find_system_id_using_servant (PortableServer::Servant servant,
                                                     PortableServer::ObjectId_out system_id)
{
  return this->id_uniqueness_strategy_->find_system_id_using_servant (servant,
                                                                      system_id);
}

int
TAO_Active_Object_Map::find_servant_using_user_id (const PortableServer::ObjectId &user_id,
                                                   PortableServer::Servant &servant)
{
  Map_Entry *entry = 0;
  int result = this->user_id_map_->find (user_id,
                                         entry);
  if (result == 0)
    {
      if (entry->servant_ == 0)
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

int
TAO_Active_Object_Map::find_servant_using_system_id (const PortableServer::ObjectId &system_id,
                                                     PortableServer::Servant &servant)
{
  return this->lifespan_strategy_->find_servant_using_system_id (system_id,
                                                                 servant);
}

int
TAO_Active_Object_Map::find_servant_and_system_id_using_user_id (const PortableServer::ObjectId &user_id,
                                                                 PortableServer::Servant &servant,
                                                                 PortableServer::ObjectId_out system_id)
{
  Map_Entry *entry = 0;
  int result = this->user_id_map_->find (user_id,
                                         entry);

  if (result == 0)
    {
      if (entry->servant_ == 0)
        {
          result = -1;
        }
      else
        {
          servant = entry->servant_;
          result = this->id_hint_strategy_->system_id (system_id,
                                                       *entry);
        }
    }

  return result;
}

int
TAO_Active_Object_Map::find_user_id_using_system_id (const PortableServer::ObjectId &system_id,
                                                     PortableServer::ObjectId_out user_id)
{
  PortableServer::ObjectId id;
  int result = this->id_hint_strategy_->recover_key (system_id,
                                                     id);

  if (result == 0)
    {
      ACE_NEW_RETURN (user_id,
                      PortableServer::ObjectId (id),
                      -1);
    }

  return 0;
}

size_t
TAO_Active_Object_Map::system_id_size (void)
{
  return this->system_id_size_;
}

////////////////////////////////////////////////////////////////////////////////

TAO_Id_Uniqueness_Strategy::~TAO_Id_Uniqueness_Strategy (void)
{
}

void
TAO_Id_Uniqueness_Strategy::set_active_object_map (TAO_Active_Object_Map *active_object_map)
{
  this->active_object_map_ = active_object_map;
}

////////////////////////////////////////////////////////////////////////////////

int
TAO_Unique_Id_Strategy::is_servant_in_map (PortableServer::Servant servant)
{
  int result = this->active_object_map_->servant_map_->find (servant);
  if (result == 0)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

int
TAO_Unique_Id_Strategy::bind_using_user_id (PortableServer::Servant servant,
                                            const PortableServer::ObjectId &user_id,
                                            TAO_Active_Object_Map::Map_Entry *&entry)
{
  int result = this->active_object_map_->user_id_map_->find (user_id,
                                                             entry);

  if (result == 0)
    {
      if (servant != 0)
        {
          entry->servant_ = servant;

          result = this->active_object_map_->servant_map_->bind (entry->servant_,
                                                                 entry);
        }
    }
  else
    {
      ACE_NEW_RETURN (entry,
                      TAO_Active_Object_Map::Map_Entry,
                      -1);

      entry->user_id_ = user_id;
      entry->servant_ = servant;

      result = this->active_object_map_->id_hint_strategy_->bind (*entry);

      if (result == 0)
        {
          result = this->active_object_map_->user_id_map_->bind (entry->user_id_,
                                                                 entry);

          if (result == 0)
            {
              if (servant != 0)
                {
                  result = this->active_object_map_->servant_map_->bind (entry->servant_,
                                                                         entry);
                }

              if (result != 0)
                {
                  this->active_object_map_->user_id_map_->unbind (entry->user_id_);
                  this->active_object_map_->id_hint_strategy_->unbind (*entry);
                  delete entry;
                }
            }
          else
            {
              this->active_object_map_->id_hint_strategy_->unbind (*entry);
              delete entry;
            }
        }
      else
        {
          delete entry;
        }
    }

  return result;
}

int
TAO_Unique_Id_Strategy::unbind_using_user_id (const PortableServer::ObjectId &user_id)
{
  TAO_Active_Object_Map::Map_Entry *entry = 0;
  int result = this->active_object_map_->user_id_map_->unbind (user_id,
                                                               entry);

  if (result == 0)
    {
      result = this->active_object_map_->servant_map_->unbind (entry->servant_);

      if (result == 0)
        {
          result = this->active_object_map_->id_hint_strategy_->unbind (*entry);
        }

      if (result == 0)
        {
          delete entry;
        }
    }

  return result;
}

int
TAO_Unique_Id_Strategy::find_user_id_using_servant (PortableServer::Servant servant,
                                                    PortableServer::ObjectId_out user_id)
{
  TAO_Active_Object_Map::Map_Entry *entry = 0;
  int result = this->active_object_map_->servant_map_->find (servant,
                                                             entry);

  if (result == 0)
    {
      ACE_NEW_RETURN (user_id,
                      PortableServer::ObjectId (entry->user_id_),
                      -1);
    }

  return result;
}

int
TAO_Unique_Id_Strategy::find_system_id_using_servant (PortableServer::Servant servant,
                                                      PortableServer::ObjectId_out system_id)
{
  TAO_Active_Object_Map::Map_Entry *entry = 0;
  int result = this->active_object_map_->servant_map_->find (servant,
                                                             entry);

  if (result == 0)
    {
      result = this->active_object_map_->id_hint_strategy_->system_id (system_id,
                                                                       *entry);
    }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

int
TAO_Multiple_Id_Strategy::is_servant_in_map (PortableServer::Servant servant)
{
  return -1;
}

int
TAO_Multiple_Id_Strategy::bind_using_user_id (PortableServer::Servant servant,
                                              const PortableServer::ObjectId &user_id,
                                              TAO_Active_Object_Map::Map_Entry *&entry)
{
  int result = this->active_object_map_->user_id_map_->find (user_id,
                                                             entry);

  if (result == 0)
    {
      if (servant != 0)
        {
          entry->servant_ = servant;
        }
    }
  else
    {
      ACE_NEW_RETURN (entry,
                      TAO_Active_Object_Map::Map_Entry,
                      -1);

      entry->user_id_ = user_id;
      entry->servant_ = servant;

      result = this->active_object_map_->id_hint_strategy_->bind (*entry);

      if (result == 0)
        {
          result = this->active_object_map_->user_id_map_->bind (entry->user_id_,
                                                                 entry);

          if (result != 0)
            {
              this->active_object_map_->id_hint_strategy_->unbind (*entry);
              delete entry;
            }
        }
      else
        {
          delete entry;
        }
    }

  return result;
}

int
TAO_Multiple_Id_Strategy::unbind_using_user_id (const PortableServer::ObjectId &user_id)
{
  TAO_Active_Object_Map::Map_Entry *entry = 0;
  int result = this->active_object_map_->user_id_map_->unbind (user_id,
                                                               entry);

  if (result == 0)
    {
      result = this->active_object_map_->id_hint_strategy_->unbind (*entry);

      if (result == 0)
        {
          delete entry;
        }
    }

  return result;
}

int
TAO_Multiple_Id_Strategy::find_user_id_using_servant (PortableServer::Servant servant,
                                                      PortableServer::ObjectId_out user_id)
{
  return -1;
}

int
TAO_Multiple_Id_Strategy::find_system_id_using_servant (PortableServer::Servant servant,
                                                        PortableServer::ObjectId_out system_id)
{
  return -1;
}

////////////////////////////////////////////////////////////////////////////////

TAO_Lifespan_Strategy::~TAO_Lifespan_Strategy (void)
{
}

void
TAO_Lifespan_Strategy::set_active_object_map (TAO_Active_Object_Map *active_object_map)
{
  this->active_object_map_ = active_object_map;
}

////////////////////////////////////////////////////////////////////////////////

int
TAO_Transient_Strategy::find_servant_using_system_id (const PortableServer::ObjectId &system_id,
                                                      PortableServer::Servant &servant)
{
  TAO_Active_Object_Map::Map_Entry *entry = 0;
  int result = this->active_object_map_->id_hint_strategy_->find (system_id,
                                                                  entry);
  if (result == 0)
    {
      if (entry->servant_ == 0)
        {
          result = -1;
        }
      else
        {
          servant = entry->servant_;
        }
    }
  else
    {
      PortableServer::ObjectId user_id;
      result = this->active_object_map_->id_hint_strategy_->recover_key (system_id,
                                                                         user_id);

      if (result == 0)
        {
          result = this->active_object_map_->user_id_map_->find (user_id,
                                                                 entry);
          if (result == 0)
            {
              if (entry->servant_ == 0)
                {
                  result = -1;
                }
              else
                {
                  servant = entry->servant_;
                }
            }
        }
    }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

int
TAO_Persistent_Strategy::find_servant_using_system_id (const PortableServer::ObjectId &system_id,
                                                       PortableServer::Servant &servant)
{
  PortableServer::ObjectId user_id;
  int result = this->active_object_map_->id_hint_strategy_->recover_key (system_id,
                                                                         user_id);

  if (result == 0)
    {
      TAO_Active_Object_Map::Map_Entry *entry = 0;
      result = this->active_object_map_->id_hint_strategy_->find (system_id,
                                                                  entry);
      if (result == 0 &&
          user_id == entry->user_id_)
        {
          if (entry->servant_ == 0)
            {
              result = -1;
            }
          else
            {
              servant = entry->servant_;
            }
        }
      else
        {
          result = this->active_object_map_->user_id_map_->find (user_id,
                                                                 entry);
          if (result == 0)
            {
              if (entry->servant_ == 0)
                {
                  result = -1;
                }
              else
                {
                  servant = entry->servant_;
                }
            }
        }
    }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

TAO_Id_Assignment_Strategy::~TAO_Id_Assignment_Strategy (void)
{
}

void
TAO_Id_Assignment_Strategy::set_active_object_map (TAO_Active_Object_Map *active_object_map)
{
  this->active_object_map_ = active_object_map;
}

////////////////////////////////////////////////////////////////////////////////

int
TAO_User_Id_Strategy::bind_using_system_id (PortableServer::Servant servant,
                                            TAO_Active_Object_Map::Map_Entry *&entry)
{
  return -1;
}

////////////////////////////////////////////////////////////////////////////////

int
TAO_System_Id_With_Unique_Id_Strategy::bind_using_system_id (PortableServer::Servant servant,
                                                             TAO_Active_Object_Map::Map_Entry *&entry)
{
  ACE_NEW_RETURN (entry,
                  TAO_Active_Object_Map::Map_Entry,
                  -1);

  int result = this->active_object_map_->user_id_map_->bind_create_key (entry,
                                                                        entry->user_id_);
  if (result == 0)
    {
      entry->servant_ = servant;

      result = this->active_object_map_->id_hint_strategy_->bind (*entry);

      if (result == 0)
        {
          if (servant != 0)
            {
              result = this->active_object_map_->servant_map_->bind (entry->servant_,
                                                                     entry);
            }

          if (result != 0)
            {
              this->active_object_map_->user_id_map_->unbind (entry->user_id_);
              this->active_object_map_->id_hint_strategy_->unbind (*entry);
              delete entry;
            }
        }
      else
        {
          this->active_object_map_->user_id_map_->unbind (entry->user_id_);
          delete entry;
        }
    }
  else
    {
      delete entry;
    }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

int
TAO_System_Id_With_Multiple_Id_Strategy::bind_using_system_id (PortableServer::Servant servant,
                                                               TAO_Active_Object_Map::Map_Entry *&entry)
{
  ACE_NEW_RETURN (entry,
                  TAO_Active_Object_Map::Map_Entry,
                  -1);

  int result = this->active_object_map_->user_id_map_->bind_create_key (entry,
                                                                        entry->user_id_);
  if (result == 0)
    {
      entry->system_id_ = entry->user_id_;
      entry->servant_ = servant;

      result = this->active_object_map_->id_hint_strategy_->bind (*entry);

      if (result != 0)
        {
          this->active_object_map_->user_id_map_->unbind (entry->user_id_);
          delete entry;
        }
    }
  else
    {
      delete entry;
    }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

TAO_Id_Hint_Strategy::~TAO_Id_Hint_Strategy (void)
{
}

////////////////////////////////////////////////////////////////////////////////

TAO_Active_Hint_Strategy::~TAO_Active_Hint_Strategy (void)
{
}

int
TAO_Active_Hint_Strategy::recover_key (const PortableServer::ObjectId &system_id,
                                       PortableServer::ObjectId &user_id)
{
  return this->system_id_map_.recover_key (system_id,
                                           user_id);
}

int
TAO_Active_Hint_Strategy::bind (TAO_Active_Object_Map::Map_Entry &entry)
{
  entry.system_id_ = entry.user_id_;

  return this->system_id_map_.bind_modify_key (&entry,
                                               entry.system_id_);
}

int
TAO_Active_Hint_Strategy::unbind (TAO_Active_Object_Map::Map_Entry &entry)
{
  return this->system_id_map_.unbind (entry.system_id_);
}

int
TAO_Active_Hint_Strategy::find (const PortableServer::ObjectId &system_id,
                                TAO_Active_Object_Map::Map_Entry *&entry)
{
  return this->system_id_map_.find (system_id,
                                    entry);
}

size_t
TAO_Active_Hint_Strategy::hint_size (void)
{
  return ACE_Active_Map_Manager_Key::size ();
}

int
TAO_Active_Hint_Strategy::system_id (PortableServer::ObjectId_out system_id,
                                     TAO_Active_Object_Map::Map_Entry &entry)
{
  ACE_NEW_RETURN (system_id,
                  PortableServer::ObjectId (entry.system_id_),
                  -1);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

TAO_No_Hint_Strategy::~TAO_No_Hint_Strategy (void)
{
}

int
TAO_No_Hint_Strategy::recover_key (const PortableServer::ObjectId &system_id,
                                   PortableServer::ObjectId &user_id)
{
  // Smartly copy all the data; <user_id does not own the data>.
  user_id.replace (system_id.maximum (),
                   system_id.length (),
                   ACE_const_cast (CORBA::Octet *,
                                   system_id.get_buffer ()),
                   0);

  return 0;
}

int
TAO_No_Hint_Strategy::bind (TAO_Active_Object_Map::Map_Entry &entry)
{
  return 0;
}

int
TAO_No_Hint_Strategy::unbind (TAO_Active_Object_Map::Map_Entry &entry)
{
  return 0;
}

int
TAO_No_Hint_Strategy::find (const PortableServer::ObjectId &system_id,
                            TAO_Active_Object_Map::Map_Entry *&entry)
{
  return -1;
}

size_t
TAO_No_Hint_Strategy::hint_size (void)
{
  return 0;
}

int
TAO_No_Hint_Strategy::system_id (PortableServer::ObjectId_out system_id,
                                 TAO_Active_Object_Map::Map_Entry &entry)
{
  ACE_NEW_RETURN (system_id,
                  PortableServer::ObjectId (entry.user_id_),
                  -1);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

// Common typedefs.
typedef PortableServer::ObjectId id;
typedef PortableServer::Servant servant;
typedef TAO_Active_Object_Map::Map_Entry * value;

typedef ACE_Pair<id, value> id_expanded_value;
typedef ACE_Reference_Pair<const id, value> id_value_type;
typedef ACE_Reference_Pair<const servant, value> servant_value_type;
typedef ACE_Equal_To<id> id_compare_keys;
typedef ACE_Equal_To<servant> servant_compare_keys;
typedef TAO_ObjectId_Hash id_hash;
typedef TAO_Servant_Hash servant_hash;
typedef ACE_Noop_Key_Generator<PortableServer::Servant> noop_servant_key_generator;

// Common
template class ACE_Reference_Pair<const id, value>;
template class ACE_Reference_Pair<const servant, value>;

// Map and iterator classes.
template class ACE_Map<id, value>;
template class ACE_Map<servant, value>;
template class ACE_Iterator<id_value_type>;
template class ACE_Iterator<servant_value_type>;
template class ACE_Reverse_Iterator<id_value_type>;
template class ACE_Reverse_Iterator<servant_value_type>;

// Iterator base classes.
template class ACE_Iterator_Impl<id_value_type>;
template class ACE_Iterator_Impl<servant_value_type>;
template class ACE_Reverse_Iterator_Impl<id_value_type>;
template class ACE_Reverse_Iterator_Impl<servant_value_type>;

// Active Map Manager related.
template class ACE_Pair<id, value>;
template class ACE_Active_Map_Manager_Adapter<id, value, TAO_Ignore_Original_Key_Adapter>;
template class ACE_Active_Map_Manager_Adapter<id, value, TAO_Preserve_Original_Key_Adapter>;
template class ACE_Active_Map_Manager_Iterator_Adapter<id_value_type, id_expanded_value>;
template class ACE_Active_Map_Manager_Reverse_Iterator_Adapter<id_value_type, id_expanded_value>;
template class ACE_Active_Map_Manager<id_expanded_value>;
template class ACE_Map_Manager<ACE_Active_Map_Manager_Key, id_expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_Active_Map_Manager_Key, id_expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_Active_Map_Manager_Key, id_expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, id_expanded_value, ACE_Null_Mutex>;
template class ACE_Map_Entry<ACE_Active_Map_Manager_Key, id_expanded_value>;

// Hash Map Manager related.
template class ACE_Hash_Map_Manager_Ex_Adapter<id, value, id_hash, id_compare_keys, TAO_Incremental_Key_Generator>;
template class ACE_Hash_Map_Manager_Ex_Adapter<servant, value, servant_hash, servant_compare_keys, noop_servant_key_generator>;
template class ACE_Hash_Map_Manager_Ex_Iterator_Adapter<id_value_type, id, value, id_hash, id_compare_keys>;
template class ACE_Hash_Map_Manager_Ex_Iterator_Adapter<servant_value_type, servant, value, servant_hash, servant_compare_keys>;
template class ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<id_value_type, id, value, id_hash, id_compare_keys>;
template class ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<servant_value_type, servant, value, servant_hash, servant_compare_keys>;
template class ACE_Hash_Map_Manager_Ex<id, value, id_hash, id_compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<servant, value, servant_hash, servant_compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<id, value, id_hash, id_compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<servant, value, servant_hash, servant_compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<id, value, id_hash, id_compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<servant, value, servant_hash, servant_compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<id, value, id_hash, id_compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<servant, value, servant_hash, servant_compare_keys, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<id, value>;
template class ACE_Hash_Map_Entry<servant, value>;
template class ACE_Equal_To<id>;
template class ACE_Equal_To<servant>;

// Map Manager related.
template class ACE_Map_Manager_Iterator_Adapter<id_value_type, id, value>;
template class ACE_Map_Manager_Iterator_Adapter<servant_value_type, servant, value>;
template class ACE_Map_Manager_Reverse_Iterator_Adapter<id_value_type, id, value>;
template class ACE_Map_Manager_Reverse_Iterator_Adapter<servant_value_type, servant, value>;
template class ACE_Map_Manager_Adapter<id, value, TAO_Incremental_Key_Generator>;
template class ACE_Map_Manager_Adapter<servant, value, noop_servant_key_generator>;
template class ACE_Map_Manager<id, value, ACE_Null_Mutex>;
template class ACE_Map_Manager<servant, value, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<id, value, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<servant, value, ACE_Null_Mutex>;
template class ACE_Map_Iterator<id, value, ACE_Null_Mutex>;
template class ACE_Map_Iterator<servant, value, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<id, value, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<servant, value, ACE_Null_Mutex>;
template class ACE_Map_Entry<id, value>;
template class ACE_Map_Entry<servant, value>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// Common typedefs.
typedef PortableServer::ObjectId id;
typedef PortableServer::Servant servant;
typedef TAO_Active_Object_Map::Map_Entry * value;

typedef ACE_Pair<id, value> id_expanded_value;
typedef ACE_Reference_Pair<const id, value> id_value_type;
typedef ACE_Reference_Pair<const servant, value> servant_value_type;
typedef ACE_Equal_To<id> id_compare_keys;
typedef ACE_Equal_To<servant> servant_compare_keys;
typedef TAO_ObjectId_Hash id_hash;
typedef TAO_Servant_Hash servant_hash;
typedef ACE_Noop_Key_Generator<PortableServer::Servant> noop_servant_key_generator;

// Common
#pragma instantiate ACE_Reference_Pair<const id, value>
#pragma instantiate ACE_Reference_Pair<const servant, value>

// Map and iterator classes.
#pragma instantiate ACE_Map<id, value>
#pragma instantiate ACE_Map<servant, value>
#pragma instantiate ACE_Iterator<id_value_type>
#pragma instantiate ACE_Iterator<servant_value_type>
#pragma instantiate ACE_Reverse_Iterator<id_value_type>
#pragma instantiate ACE_Reverse_Iterator<servant_value_type>

// Iterator base classes.
#pragma instantiate ACE_Iterator_Impl<id_value_type>
#pragma instantiate ACE_Iterator_Impl<servant_value_type>
#pragma instantiate ACE_Reverse_Iterator_Impl<id_value_type>
#pragma instantiate ACE_Reverse_Iterator_Impl<servant_value_type>

// Active Map Manager related.
#pragma instantiate ACE_Pair<id, value>
#pragma instantiate ACE_Active_Map_Manager_Adapter<id, value, TAO_Ignore_Original_Key_Adapter>
#pragma instantiate ACE_Active_Map_Manager_Adapter<id, value, TAO_Preserve_Original_Key_Adapter>
#pragma instantiate ACE_Active_Map_Manager_Iterator_Adapter<id_value_type, id_expanded_value>
#pragma instantiate ACE_Active_Map_Manager_Reverse_Iterator_Adapter<id_value_type, id_expanded_value>
#pragma instantiate ACE_Active_Map_Manager<id_expanded_value>
#pragma instantiate ACE_Map_Manager<ACE_Active_Map_Manager_Key, id_expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_Active_Map_Manager_Key, id_expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<ACE_Active_Map_Manager_Key, id_expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, id_expanded_value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<ACE_Active_Map_Manager_Key, id_expanded_value>

// Hash Map Manager related.
#pragma instantiate ACE_Hash_Map_Manager_Ex_Adapter<id, value, id_hash, id_compare_keys, TAO_Incremental_Key_Generator>
#pragma instantiate ACE_Hash_Map_Manager_Ex_Adapter<servant, value, servant_hash, servant_compare_keys, noop_servant_key_generator>
#pragma instantiate ACE_Hash_Map_Manager_Ex_Iterator_Adapter<id_value_type, id, value, id_hash, id_compare_keys>
#pragma instantiate ACE_Hash_Map_Manager_Ex_Iterator_Adapter<servant_value_type, servant, value, servant_hash, servant_compare_keys>
#pragma instantiate ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<id_value_type, id, value, id_hash, id_compare_keys>
#pragma instantiate ACE_Hash_Map_Manager_Ex_Reverse_Iterator_Adapter<servant_value_type, servant, value, servant_hash, servant_compare_keys>
#pragma instantiate ACE_Hash_Map_Manager_Ex<id, value, id_hash, id_compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<servant, value, servant_hash, servant_compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<id, value, id_hash, id_compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<servant, value, servant_hash, servant_compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<id, value, id_hash, id_compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<servant, value, servant_hash, servant_compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<id, value, id_hash, id_compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<servant, value, servant_hash, servant_compare_keys, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<id, value>
#pragma instantiate ACE_Hash_Map_Entry<servant, value>
#pragma instantiate ACE_Equal_To<id>
#pragma instantiate ACE_Equal_To<servant>

// Map Manager related.
#pragma instantiate ACE_Map_Manager_Iterator_Adapter<id_value_type, id, value>
#pragma instantiate ACE_Map_Manager_Iterator_Adapter<servant_value_type, servant, value>
#pragma instantiate ACE_Map_Manager_Reverse_Iterator_Adapter<id_value_type, id, value>
#pragma instantiate ACE_Map_Manager_Reverse_Iterator_Adapter<servant_value_type, servant, value>
#pragma instantiate ACE_Map_Manager_Adapter<id, value, TAO_Incremental_Key_Generator>
#pragma instantiate ACE_Map_Manager_Adapter<servant, value, noop_servant_key_generator>
#pragma instantiate ACE_Map_Manager<id, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Manager<servant, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<id, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<servant, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<id, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<servant, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<id, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<servant, value, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<id, value>
#pragma instantiate ACE_Map_Entry<servant, value>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
