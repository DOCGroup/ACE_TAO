// $Id$

#include "tao/PortableServer/Active_Object_Map.h"
#include "tao/PortableServer/Active_Object_Map_Entry.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/Active_Object_Map.inl"
#endif /* __ACE_INLINE__ */

#include "tao/SystemException.h"

#include "ace/Auto_Ptr.h"
#include "ace/CORBA_macros.h"
#include "tao/debug.h"
#include "PortableServer_Functions.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/* static */
size_t TAO_Active_Object_Map::system_id_size_ = 0;

static void
hexstring (ACE_CString& hexstr, const char* s, size_t l)
{
  char buf[3] = {0};

  hexstr.fast_resize (2 + l * 2);
  hexstr.append ("0x", 2);
  while (--l)
    {
      ACE_OS::sprintf (buf, "%02x", (unsigned int)(unsigned char)*s);
      hexstr.append (buf, 2);
      ++s;
    }
}

void
TAO_Active_Object_Map::set_system_id_size (
  const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters)
{
  if (TAO_Active_Object_Map::system_id_size_ == 0)
    {
      if (creation_parameters.allow_reactivation_of_system_ids_)
        {
          switch (creation_parameters.object_lookup_strategy_for_system_id_policy_)
            {
#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
            case TAO_LINEAR:
              TAO_Active_Object_Map::system_id_size_ = sizeof (CORBA::ULong);
              break;
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

            case TAO_DYNAMIC_HASH:
            default:
              TAO_Active_Object_Map::system_id_size_ = sizeof (CORBA::ULong);
              break;
            }

          size_t hint_size = 0;

          if (creation_parameters.use_active_hint_in_ids_)
            hint_size = ACE_Active_Map_Manager_Key::size ();

          TAO_Active_Object_Map::system_id_size_ += hint_size;
        }
      else
        {
          switch (creation_parameters.object_lookup_strategy_for_system_id_policy_)
            {
#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
            case TAO_LINEAR:
              TAO_Active_Object_Map::system_id_size_ = sizeof (CORBA::ULong);
              break;

            case TAO_DYNAMIC_HASH:
              TAO_Active_Object_Map::system_id_size_ = sizeof (CORBA::ULong);
              break;
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

            case TAO_ACTIVE_DEMUX:
            default:
              TAO_Active_Object_Map::system_id_size_ =
                ACE_Active_Map_Manager_Key::size ();
              break;
            }
        }
    }
}

TAO_Active_Object_Map::TAO_Active_Object_Map (
    int user_id_policy,
    int unique_id_policy,
    int persistent_id_policy,
    const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &
      creation_parameters)
  : user_id_map_ (0)
  , servant_map_ (0)
  , id_uniqueness_strategy_ (0)
  , lifespan_strategy_ (0)
  , id_assignment_strategy_ (0)
  , id_hint_strategy_ (0)
  , using_active_maps_ (false)
{
  TAO_Active_Object_Map::set_system_id_size (creation_parameters);

  TAO_Id_Uniqueness_Strategy *id_uniqueness_strategy = 0;

  if (unique_id_policy)
    {
      ACE_NEW_THROW_EX (id_uniqueness_strategy,
                        TAO_Unique_Id_Strategy,
                        CORBA::NO_MEMORY ());
    }
  else
    {
#if !defined (CORBA_E_MICRO)
      ACE_NEW_THROW_EX (id_uniqueness_strategy,
                        TAO_Multiple_Id_Strategy,
                        CORBA::NO_MEMORY ());
#else
      ACE_ERROR ((LM_ERROR,
                  "Multiple Id Strategy not supported with CORBA/e\n"));
#endif
    }

  // Give ownership to the auto pointer.
  auto_ptr<TAO_Id_Uniqueness_Strategy>
    new_id_uniqueness_strategy (id_uniqueness_strategy);

  TAO_Lifespan_Strategy *lifespan_strategy = 0;

  if (persistent_id_policy)
    {
#if !defined (CORBA_E_MICRO)
      ACE_NEW_THROW_EX (lifespan_strategy,
                        TAO_Persistent_Strategy,
                        CORBA::NO_MEMORY ());
#else
      ACE_UNUSED_ARG (persistent_id_policy);
      ACE_ERROR ((LM_ERROR,
                  "Persistent Id Strategy not supported with CORBA/e\n"));
#endif
    }
  else
    {
      ACE_NEW_THROW_EX (lifespan_strategy,
                        TAO_Transient_Strategy,
                        CORBA::NO_MEMORY ());
    }

  // Give ownership to the auto pointer.
  auto_ptr<TAO_Lifespan_Strategy> new_lifespan_strategy (lifespan_strategy);

  TAO_Id_Assignment_Strategy *id_assignment_strategy = 0;

  if (user_id_policy)
    {
#if !defined (CORBA_E_MICRO)
      ACE_NEW_THROW_EX (id_assignment_strategy,
                        TAO_User_Id_Strategy,
                        CORBA::NO_MEMORY ());
#else
      ACE_ERROR ((LM_ERROR,
                  "User Id Assignment not supported with CORBA/e\n"));
#endif
    }
  else if (unique_id_policy)
    {
      ACE_NEW_THROW_EX (id_assignment_strategy,
                        TAO_System_Id_With_Unique_Id_Strategy,
                        CORBA::NO_MEMORY ());
    }
  else
    {
#if !defined (CORBA_E_MICRO)
      ACE_NEW_THROW_EX (id_assignment_strategy,
                        TAO_System_Id_With_Multiple_Id_Strategy,
                        CORBA::NO_MEMORY ());
#else
      ACE_ERROR ((LM_ERROR,
                  "System Id Assignment with multiple "
                  "not supported with CORBA/e\n"));
#endif
    }

  // Give ownership to the auto pointer.
  auto_ptr<TAO_Id_Assignment_Strategy>
    new_id_assignment_strategy (id_assignment_strategy);

  TAO_Id_Hint_Strategy *id_hint_strategy = 0;
  if ((user_id_policy
       || creation_parameters.allow_reactivation_of_system_ids_)
      && creation_parameters.use_active_hint_in_ids_)
    {
      this->using_active_maps_ = true;

      ACE_NEW_THROW_EX (id_hint_strategy,
                        TAO_Active_Hint_Strategy (
                          creation_parameters.active_object_map_size_),
                        CORBA::NO_MEMORY ());
    }
  else
    {
      ACE_NEW_THROW_EX (id_hint_strategy,
                        TAO_No_Hint_Strategy,
                        CORBA::NO_MEMORY ());
    }

  // Give ownership to the auto pointer.
  auto_ptr<TAO_Id_Hint_Strategy> new_id_hint_strategy (id_hint_strategy);

  servant_map *sm = 0;
  if (unique_id_policy)
    {
      switch (creation_parameters.reverse_object_lookup_strategy_for_unique_id_policy_)
        {
        case TAO_LINEAR:
#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
          ACE_NEW_THROW_EX (sm,
                            servant_linear_map (
                              creation_parameters.active_object_map_size_),
                            CORBA::NO_MEMORY ());
          break;
#else
          ACE_ERROR ((LM_ERROR,
                      "linear option for "
                      "-ORBUniqueidPolicyReverseDemuxStrategy "
                      "not supported with minimum POA maps. "
                      "Ingoring option to use default...\n"));
          /* FALL THROUGH */
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

        case TAO_DYNAMIC_HASH:
        default:
          ACE_NEW_THROW_EX (sm,
                            servant_hash_map (
                              creation_parameters.active_object_map_size_),
                            CORBA::NO_MEMORY ());
          break;
        }
    }

  // Give ownership to the auto pointer.
  auto_ptr<servant_map> new_servant_map (sm);

  user_id_map *uim = 0;
  if (user_id_policy
      || creation_parameters.allow_reactivation_of_system_ids_)
    {
      switch (creation_parameters.object_lookup_strategy_for_user_id_policy_)
        {
        case TAO_LINEAR:
#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
          ACE_NEW_THROW_EX (uim,
                            user_id_linear_map (
                              creation_parameters.active_object_map_size_),
                            CORBA::NO_MEMORY ());
          break;
#else
          ACE_ERROR ((LM_ERROR,
                      "linear option for -ORBUseridPolicyDemuxStrategy "
                      "not supported with minimum POA maps. "
                      "Ingoring option to use default...\n"));
          /* FALL THROUGH */
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

        case TAO_DYNAMIC_HASH:
        default:
          ACE_NEW_THROW_EX (uim,
                            user_id_hash_map (creation_parameters.active_object_map_size_),
                            CORBA::NO_MEMORY ());
          break;
        }
    }
  else
    {
      switch (creation_parameters.object_lookup_strategy_for_system_id_policy_)
        {
#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
        case TAO_LINEAR:
          ACE_NEW_THROW_EX (uim,
                            user_id_linear_map (creation_parameters.active_object_map_size_),
                            CORBA::NO_MEMORY ());
          break;

        case TAO_DYNAMIC_HASH:
          ACE_NEW_THROW_EX (uim,
                            user_id_hash_map (creation_parameters.active_object_map_size_),
                            CORBA::NO_MEMORY ());
          break;
#else
        case TAO_LINEAR:
        case TAO_DYNAMIC_HASH:
          ACE_ERROR ((LM_ERROR,
                      "linear and dynamic options for -ORBSystemidPolicyDemuxStrategy "
                      "are not supported with minimum POA maps. "
                      "Ingoring option to use default...\n"));
          /* FALL THROUGH */
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

        case TAO_ACTIVE_DEMUX:
        default:

          this->using_active_maps_ = true;

          ACE_NEW_THROW_EX (uim,
                            user_id_active_map (creation_parameters.active_object_map_size_),
                            CORBA::NO_MEMORY ());
          break;
        }
    }

  // Give ownership to the auto pointer.
  auto_ptr<user_id_map> new_user_id_map (uim);

  id_uniqueness_strategy->set_active_object_map (this);
  lifespan_strategy->set_active_object_map (this);
  id_assignment_strategy->set_active_object_map (this);

  // Finally everything is fine.  Make sure to take ownership away
  // from the auto pointer.
  this->id_uniqueness_strategy_ = new_id_uniqueness_strategy;
  this->lifespan_strategy_ =  new_lifespan_strategy;
  this->id_assignment_strategy_ = new_id_assignment_strategy;
  this->id_hint_strategy_ = new_id_hint_strategy;
  this->servant_map_ = new_servant_map;
  this->user_id_map_ = new_user_id_map;

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
  ACE_NEW (this->monitor_,
           ACE::Monitor_Control::Size_Monitor);
#endif /* TAO_HAS_MONITOR_POINTS==1 */
}

TAO_Active_Object_Map::~TAO_Active_Object_Map (void)
{
  user_id_map::iterator iterator = this->user_id_map_->begin ();
  user_id_map::iterator end = this->user_id_map_->end ();

  for (;
       iterator != end;
       ++iterator)
    {
      user_id_map::value_type map_entry = *iterator;
      delete map_entry.second ();
    }

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
  this->monitor_->remove_from_registry ();
  this->monitor_->remove_ref ();
#endif /* TAO_HAS_MONITOR_POINTS==1 */
}

bool
TAO_Active_Object_Map::is_user_id_in_map (
  const PortableServer::ObjectId &user_id,
  CORBA::Short priority,
  bool &priorities_match,
  bool &deactivated)
{
  TAO_Active_Object_Map_Entry *entry = 0;
  bool result = false;
  int const find_result = this->user_id_map_->find (user_id, entry);

  if (find_result == 0)
    {
      if (entry->servant_ == 0)
        {
          if (entry->priority_ != priority)
            {
              priorities_match = false;
            }
        }
      else
        {
          result = true;

          if (entry->deactivated_)
            {
              deactivated = true;
            }
        }
    }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

TAO_Id_Uniqueness_Strategy::~TAO_Id_Uniqueness_Strategy (void)
{
}

void
TAO_Id_Uniqueness_Strategy::set_active_object_map (
  TAO_Active_Object_Map *active_object_map)
{
  this->active_object_map_ = active_object_map;
}

int
TAO_Unique_Id_Strategy::is_servant_in_map (PortableServer::Servant servant,
                                           bool &deactivated)
{
  TAO_Active_Object_Map_Entry *entry = 0;
  int result = this->active_object_map_->servant_map_->find (servant,
                                                             entry);
  if (result == 0)
    {
      result = 1;

      if (entry->deactivated_)
        {
          deactivated = true;
        }
    }
  else
    {
      result = 0;
    }

  return result;
}

int
TAO_Unique_Id_Strategy::bind_using_user_id (
  PortableServer::Servant servant,
  const PortableServer::ObjectId &user_id,
  CORBA::Short priority,
  TAO_Active_Object_Map_Entry *&entry)
{
  int result =
    this->active_object_map_->user_id_map_->find (user_id, entry);

  if (result == 0)
    {
      if (servant != 0)
        {
          entry->servant_ = servant;

          result =
            this->active_object_map_->servant_map_->bind (entry->servant_,
                                                          entry);

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
          if (result == 0)
            {
              this->active_object_map_->monitor_->receive (
                this->active_object_map_->servant_map_->current_size ());
            }
#endif /* TAO_HAS_MONITOR_POINTS==1 */
        }
    }
  else
    {
      ACE_NEW_RETURN (entry,
                      TAO_Active_Object_Map_Entry,
                      -1);
      entry->user_id_ = user_id;
      entry->servant_ = servant;
      entry->priority_ = priority;

      result = this->active_object_map_->id_hint_strategy_->bind (*entry);

      if (result == 0)
        {
          result =
            this->active_object_map_->user_id_map_->bind (entry->user_id_,
                                                          entry);
          if (result == 0)
            {
              if (servant != 0)
                {
                  result =
                    this->active_object_map_->servant_map_->bind (
                      entry->servant_,
                      entry);
                }

              if (result != 0)
                {
                  this->active_object_map_->user_id_map_->unbind (
                    entry->user_id_);
                  this->active_object_map_->id_hint_strategy_->unbind (
                    *entry);
                  delete entry;
                }
              else
                {
#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
                  this->active_object_map_->monitor_->receive (
                    this->active_object_map_->servant_map_->current_size ());
#endif /* TAO_HAS_MONITOR_POINTS==1 */
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

  if (result == 0 && TAO_debug_level > 7)
    {
      CORBA::String_var idstr (PortableServer::ObjectId_to_string (user_id));
      ACE_CString hex_idstr;
      hexstring (hex_idstr, idstr.in (), user_id.length ());

      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - TAO_Unique_Id_Strategy::bind_using_user_id: type=%C, id=%C\n",
                  servant ? servant->_repository_id () : "(null)",
                  hex_idstr.c_str()
                  ));
    }

  return result;
}

int
TAO_Unique_Id_Strategy::unbind_using_user_id (
  const PortableServer::ObjectId &user_id)
{
  TAO_Active_Object_Map_Entry *entry = 0;
  int result = this->active_object_map_->user_id_map_->unbind (user_id,
                                                               entry);
  if (result == 0)
    {
      if (TAO_debug_level > 7)
        {
          CORBA::String_var idstr (
              PortableServer::ObjectId_to_string (entry->user_id_));
          ACE_CString hex_idstr;
          hexstring (hex_idstr, idstr.in (), entry->user_id_.length ());

          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - TAO_Unique_Id_Strategy::unbind_using_user_id: id=%C\n",
                      hex_idstr.c_str()
                      ));
        }

      if (entry->servant_ != 0)
        {
          result =
            this->active_object_map_->servant_map_->unbind (entry->servant_);
        }

      if (result == 0)
        {
          result =
            this->active_object_map_->id_hint_strategy_->unbind (*entry);

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
          this->active_object_map_->monitor_->receive (
            this->active_object_map_->servant_map_->current_size ());
#endif /* TAO_HAS_MONITOR_POINTS==1 */
        }

      if (result == 0)
        {
          delete entry;
        }
    }

  return result;
}

int
TAO_Unique_Id_Strategy::find_user_id_using_servant (
  PortableServer::Servant servant,
  PortableServer::ObjectId_out user_id)
{
  TAO_Active_Object_Map_Entry *entry = 0;
  int result = this->active_object_map_->servant_map_->find (servant, entry);

  if (result == 0)
    {
      if (entry->deactivated_)
        {
          result = -1;
        }
      else
        {
          ACE_NEW_RETURN (user_id,
                          PortableServer::ObjectId (entry->user_id_),
                          -1);
        }
    }

  return result;
}

int
TAO_Unique_Id_Strategy::find_system_id_using_servant (
  PortableServer::Servant servant,
  PortableServer::ObjectId_out system_id,
  CORBA::Short &priority)
{
  TAO_Active_Object_Map_Entry *entry = 0;
  int result = this->active_object_map_->servant_map_->find (servant,
                                                             entry);
  if (result == 0)
    {
      if (entry->deactivated_)
        {
          result = -1;
        }
      else
        {
          result =
            this->active_object_map_->id_hint_strategy_->system_id (
              system_id,
              *entry);
          if (result == 0)
            {
              priority = entry->priority_;
            }
        }
    }

  return result;
}

CORBA::Boolean
TAO_Unique_Id_Strategy::remaining_activations (
  PortableServer::Servant servant)
{
  ACE_UNUSED_ARG (servant);

  // Since servant are always unique here, return false.
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

#if !defined (CORBA_E_MICRO)
int
TAO_Multiple_Id_Strategy::is_servant_in_map (PortableServer::Servant, bool &)
{
  return -1;
}

int
TAO_Multiple_Id_Strategy::bind_using_user_id (
  PortableServer::Servant servant,
  const PortableServer::ObjectId &user_id,
  CORBA::Short priority,
  TAO_Active_Object_Map_Entry *&entry)
{
  int result =
    this->active_object_map_->user_id_map_->find (user_id, entry);

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
                      TAO_Active_Object_Map_Entry,
                      -1);
      entry->user_id_ = user_id;
      entry->servant_ = servant;
      entry->priority_ = priority;

      result =
        this->active_object_map_->id_hint_strategy_->bind (*entry);

      if (result == 0)
        {
          result =
            this->active_object_map_->user_id_map_->bind (entry->user_id_,
                                                          entry);
          if (result != 0)
            {
              this->active_object_map_->id_hint_strategy_->unbind (*entry);
              delete entry;
            }
          else
            {
#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
              this->active_object_map_->monitor_->receive (
                this->active_object_map_->user_id_map_->current_size ());
#endif /* TAO_HAS_MONITOR_POINTS==1 */
            }
        }
      else
        {
          delete entry;
        }
    }

  if (result == 0 && TAO_debug_level > 7)
    {
      CORBA::String_var idstr (PortableServer::ObjectId_to_string (user_id));
      ACE_CString hex_idstr;
      hexstring (hex_idstr, idstr.in (), user_id.length ());

      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - TAO_Multiple_Id_Strategy::bind_using_user_id: type=%C, id=%C\n",
                  servant != 0 ? servant->_repository_id () : "(null)",
                  hex_idstr.c_str()
                  ));
    }

  return result;
}

int
TAO_Multiple_Id_Strategy::unbind_using_user_id (
  const PortableServer::ObjectId &user_id)
{
  TAO_Active_Object_Map_Entry *entry = 0;
  int result = this->active_object_map_->user_id_map_->unbind (user_id,
                                                               entry);
  if (result == 0)
    {
      if (TAO_debug_level > 7)
        {
          CORBA::String_var idstr (
              PortableServer::ObjectId_to_string (entry->user_id_));
          ACE_CString hex_idstr;
          hexstring (hex_idstr, idstr.in (), entry->user_id_.length ());

          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - TAO_Multiple_Id_Strategy::unbind_using_user_id: id=%C\n",
                      hex_idstr.c_str()
                      ));
        }

      result = this->active_object_map_->id_hint_strategy_->unbind (*entry);

      if (result == 0)
        {
          delete entry;
        }

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
              this->active_object_map_->monitor_->receive (
                this->active_object_map_->user_id_map_->current_size ());
#endif /* TAO_HAS_MONITOR_POINTS==1 */
    }

  return result;
}

int
TAO_Multiple_Id_Strategy::find_user_id_using_servant (
  PortableServer::Servant,
  PortableServer::ObjectId_out)
{
  return -1;
}

int
TAO_Multiple_Id_Strategy::find_system_id_using_servant (
  PortableServer::Servant,
  PortableServer::ObjectId_out,
  CORBA::Short &)
{
  return -1;
}

CORBA::Boolean
TAO_Multiple_Id_Strategy::remaining_activations (
  PortableServer::Servant servant)
{
  TAO_Active_Object_Map::user_id_map::iterator end =
    this->active_object_map_->user_id_map_->end ();

  for (TAO_Active_Object_Map::user_id_map::iterator iter =
         this->active_object_map_->user_id_map_->begin ();
       iter != end;
       ++iter)
    {
      TAO_Active_Object_Map::user_id_map::value_type map_pair = *iter;
      TAO_Active_Object_Map_Entry *entry = map_pair.second ();

      if (entry->servant_ == servant)
        {
          return 1;
        }
    }

  return 0;
}
#endif

TAO_Lifespan_Strategy::~TAO_Lifespan_Strategy (void)
{
}

void
TAO_Lifespan_Strategy::set_active_object_map (
  TAO_Active_Object_Map *active_object_map)
{
  this->active_object_map_ = active_object_map;
}

int
TAO_Transient_Strategy::find_servant_using_system_id_and_user_id (
  const PortableServer::ObjectId &system_id,
  const PortableServer::ObjectId &user_id,
  PortableServer::Servant &servant,
  TAO_Active_Object_Map_Entry *&entry)
{
  int result = this->active_object_map_->id_hint_strategy_->find (system_id,
                                                                  entry);
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
  else
    {
      result = this->active_object_map_->user_id_map_->find (user_id,
                                                             entry);
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
    }

  if (result == -1)
    {
      entry = 0;
    }

  return result;
}

////////////////////////////////////////////////////////////////////////////////

#if !defined (CORBA_E_MICRO)
int
TAO_Persistent_Strategy::find_servant_using_system_id_and_user_id (
  const PortableServer::ObjectId &system_id,
  const PortableServer::ObjectId &user_id,
  PortableServer::Servant &servant,
TAO_Active_Object_Map_Entry *&entry)
{
  int result =
    this->active_object_map_->id_hint_strategy_->find (system_id,
                                                       entry);
  if (result == 0 && user_id == entry->user_id_)
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
  else
    {
      result =
        this->active_object_map_->user_id_map_->find (user_id, entry);

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
    }

  if (result == -1)
    {
      entry = 0;
    }

  return result;
}
#endif

TAO_Id_Assignment_Strategy::~TAO_Id_Assignment_Strategy (void)
{
}

void
TAO_Id_Assignment_Strategy::set_active_object_map (
  TAO_Active_Object_Map *active_object_map)
{
  this->active_object_map_ = active_object_map;
}

#if !defined (CORBA_E_MICRO)
int
TAO_User_Id_Strategy::bind_using_system_id (PortableServer::Servant,
                                            CORBA::Short,
                                            TAO_Active_Object_Map_Entry *&)
{
  return -1;
}
#endif

int
TAO_System_Id_With_Unique_Id_Strategy::bind_using_system_id (
  PortableServer::Servant servant,
  CORBA::Short priority,
  TAO_Active_Object_Map_Entry *&entry)
{
  ACE_NEW_RETURN (entry,
                  TAO_Active_Object_Map_Entry,
                  -1);

  int result =
    this->active_object_map_->user_id_map_->bind_create_key (entry,
                                                             entry->user_id_);
  if (result == 0)
    {
      entry->servant_ = servant;
      entry->priority_ = priority;

      result = this->active_object_map_->id_hint_strategy_->bind (*entry);

      if (result == 0)
        {
          if (servant != 0)
            {
              result =
                this->active_object_map_->servant_map_->bind (entry->servant_,
                                                              entry);
            }

          if (result != 0)
            {
              this->active_object_map_->user_id_map_->unbind (entry->user_id_);
              this->active_object_map_->id_hint_strategy_->unbind (*entry);
              delete entry;
            }
          else
            {
#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
              this->active_object_map_->monitor_->receive (
                this->active_object_map_->servant_map_->current_size ());
#endif /* TAO_HAS_MONITOR_POINTS==1 */
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

  if (result == 0 && TAO_debug_level > 7)
    {
      CORBA::String_var idstr (
          PortableServer::ObjectId_to_string (entry->user_id_));
      ACE_CString hex_idstr;
      hexstring (hex_idstr, idstr.in (), entry->user_id_.length ());

      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - TAO_System_Id_With_Unique_Id_Strategy::"
                  "bind_using_system_id: type=%C, id=%C\n",
                  servant->_repository_id (),
                  hex_idstr.c_str()
                  ));
    }

  return result;
}

#if !defined (CORBA_E_MICRO)
int
TAO_System_Id_With_Multiple_Id_Strategy::bind_using_system_id (
  PortableServer::Servant servant,
  CORBA::Short priority,
  TAO_Active_Object_Map_Entry *&entry)
{
  ACE_NEW_RETURN (entry,
                  TAO_Active_Object_Map_Entry,
                  -1);
  int result =
    this->active_object_map_->user_id_map_->bind_create_key (entry,
                                                             entry->user_id_);
  if (result == 0)
    {
      entry->servant_ = servant;
      entry->priority_ = priority;

      result = this->active_object_map_->id_hint_strategy_->bind (*entry);

      if (result != 0)
        {
          this->active_object_map_->user_id_map_->unbind (entry->user_id_);
          delete entry;
        }

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
      this->active_object_map_->monitor_->receive (
        this->active_object_map_->user_id_map_->current_size ());
#endif /* TAO_HAS_MONITOR_POINTS==1 */
    }
  else
    {
      delete entry;
    }

  if (result == 0 && TAO_debug_level > 7)
    {
      CORBA::String_var idstr (
          PortableServer::ObjectId_to_string (entry->user_id_));
      ACE_CString hex_idstr;
      hexstring (hex_idstr, idstr.in (), entry->user_id_.length ());

      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - TAO_System_Id_With_Multiple_Id_Strategy::"
                  "bind_using_system_id: type=%C, id=%C\n",
                  servant->_repository_id (),
                  hex_idstr.c_str()
                  ));
    }

  return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////

TAO_Id_Hint_Strategy::~TAO_Id_Hint_Strategy (void)
{
}

////////////////////////////////////////////////////////////////////////////////

TAO_Active_Hint_Strategy::TAO_Active_Hint_Strategy (CORBA::ULong map_size)
  : system_id_map_ (map_size)
{
}

TAO_Active_Hint_Strategy::~TAO_Active_Hint_Strategy (void)
{
}

int
TAO_Active_Hint_Strategy::recover_key (
  const PortableServer::ObjectId &system_id,
  PortableServer::ObjectId &user_id)
{
  return this->system_id_map_.recover_key (system_id, user_id);
}

int
TAO_Active_Hint_Strategy::bind (TAO_Active_Object_Map_Entry &entry)
{
  entry.system_id_ = entry.user_id_;

  return this->system_id_map_.bind_modify_key (&entry,
                                               entry.system_id_);
}

int
TAO_Active_Hint_Strategy::unbind (TAO_Active_Object_Map_Entry &entry)
{
  return this->system_id_map_.unbind (entry.system_id_);
}

int
TAO_Active_Hint_Strategy::find (const PortableServer::ObjectId &system_id,
                                TAO_Active_Object_Map_Entry *&entry)
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
                                     TAO_Active_Object_Map_Entry &entry)
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
                   const_cast<CORBA::Octet *> (system_id.get_buffer ()),
                   0);

  return 0;
}

int
TAO_No_Hint_Strategy::bind (TAO_Active_Object_Map_Entry &)
{
  return 0;
}

int
TAO_No_Hint_Strategy::unbind (TAO_Active_Object_Map_Entry &)
{
  return 0;
}

int
TAO_No_Hint_Strategy::find (const PortableServer::ObjectId &,
                            TAO_Active_Object_Map_Entry *&)
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
                                 TAO_Active_Object_Map_Entry &entry)
{
  ACE_NEW_RETURN (system_id,
                  PortableServer::ObjectId (entry.user_id_),
                  -1);
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
