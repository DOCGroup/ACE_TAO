// -*- C++ -*-

//=============================================================================
/**
 *  @file    Active_Object_Map.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#ifndef TAO_ACTIVE_OBJECT_MAP_H
#define TAO_ACTIVE_OBJECT_MAP_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/Key_Adapters.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Base.h"
#include "tao/Server_Strategy_Factory.h"
#include "ace/Map_T.h"
#include "ace/Auto_Ptr.h"

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
#include "ace/Monitor_Size.h"
#endif /* TAO_HAS_MONITOR_POINTS==1 */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class TAO_Id_Uniqueness_Strategy;
class TAO_Lifespan_Strategy;
class TAO_Id_Assignment_Strategy;
class TAO_Id_Hint_Strategy;
struct TAO_Active_Object_Map_Entry;

/**
 * @class TAO_Active_Object_Map
 *
 * @brief Map of object ids to servants.
 *
 * Implementation to be used by the POA.
 */
class TAO_Active_Object_Map
{
public:

  /// Constructor.
  TAO_Active_Object_Map (
    int user_id_policy,
    int unique_id_policy,
    int persistent_id_policy,
    const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters);

  /// Destructor.
  ~TAO_Active_Object_Map (void);

  /// Must be used with UNIQUE_ID policy.
  int
  is_servant_in_map (PortableServer::Servant servant,
                     bool &deactivated);

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// user_id is actually system_id.
  bool
  is_user_id_in_map (const PortableServer::ObjectId &user_id,
                     CORBA::Short priority,
                     bool &priorities_match,
                     bool &deactivated);

  /// Must be used with SYSTEM_ID policy.
  int
  bind_using_system_id_returning_system_id (
    PortableServer::Servant servant,
    CORBA::Short priority,
    PortableServer::ObjectId_out system_id);

  /// Must be used with SYSTEM_ID policy.
  int
  bind_using_system_id_returning_user_id (
    PortableServer::Servant servant,
    CORBA::Short priority,
    PortableServer::ObjectId_out user_id);

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// user_id is actually system_id.
  int
  bind_using_user_id (PortableServer::Servant servant,
                      const PortableServer::ObjectId &user_id,
                      CORBA::Short priority);

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// user_id is actually system_id.
  int
  find_system_id_using_user_id (const PortableServer::ObjectId &user_id,
                                CORBA::Short priority,
                                PortableServer::ObjectId_out system_id);

  /// Can be used with any policy.
  int
  rebind_using_user_id_and_system_id (
    PortableServer::Servant servant,
    const PortableServer::ObjectId &user_id,
    const PortableServer::ObjectId &system_id,
    TAO_Active_Object_Map_Entry *&entry);

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// user_id is actually system_id.
  int
  unbind_using_user_id (const PortableServer::ObjectId &user_id);

  /// Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  /// user_id is actually system_id.
  int
  find_user_id_using_servant (PortableServer::Servant servant,
                              PortableServer::ObjectId_out user_id);

  /// Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  /// user_id is actually system_id.
  int
  find_system_id_using_servant (PortableServer::Servant servant,
                                PortableServer::ObjectId_out system_id,
                                CORBA::Short &priority);

  /// Can be used with any policy. With the SYSTEM_ID policy,
  /// user_id is actually system_id.
  int
  find_servant_using_user_id (const PortableServer::ObjectId &user_id,
                              PortableServer::Servant &servant);


  /// Can be used with any policy.
  int
  find_servant_using_system_id_and_user_id (
    const PortableServer::ObjectId &system_id,
    const PortableServer::ObjectId &user_id,
    PortableServer::Servant &servant,
    TAO_Active_Object_Map_Entry *&entry);

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// @a user_id is identical to @a system_id.
  int
  find_servant_and_system_id_using_user_id (
    const PortableServer::ObjectId &user_id,
    PortableServer::Servant &servant,
    PortableServer::ObjectId_out system_id,
    CORBA::Short &priority);

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// @a user_id is identical to @c system_id.
  /**
   * @retval -1 Entry is not found or is deactivated.
   * @retval 0 Entry is found.
   */
  int
  find_entry_using_user_id (const PortableServer::ObjectId &user_id,
                            TAO_Active_Object_Map_Entry *&entry);

  /// Can be used with any policy.  When the SYSTEM_ID policy is used,
  /// the @a system_id is identical to @a user_id.
  int
  find_user_id_using_system_id (const PortableServer::ObjectId &system_id,
                                PortableServer::ObjectId_out user_id);

  /// Can be used with any policy.  When the SYSTEM_ID policy is used,
  /// the @a system_id is identical to @a user_id.
  int
  find_user_id_using_system_id (const PortableServer::ObjectId &system_id,
                                 PortableServer::ObjectId &user_id);

  /// Are there any remaining activations of @a servant in the active
  /// object map?  Can be used with any policy.
  CORBA::Boolean
  remaining_activations (PortableServer::Servant servant);

  /// Size of the map.
  size_t
  current_size (void);

  /// Can be used with any policy.
  static size_t
  system_id_size (void);

  /// Set the system id size.
  static void
  set_system_id_size (
    const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters);

  /// Base class of the id map.
  typedef ACE_Map<
  PortableServer::ObjectId,
    TAO_Active_Object_Map_Entry *> user_id_map;

  /// Id hash map.
  typedef ACE_Hash_Map_Manager_Ex_Adapter<
  PortableServer::ObjectId,
    TAO_Active_Object_Map_Entry *,
    TAO_ObjectId_Hash,
    ACE_Equal_To<PortableServer::ObjectId>,
    TAO_Incremental_Key_Generator> user_id_hash_map;

  /// Id linear map.
  typedef ACE_Map_Manager_Adapter<
  PortableServer::ObjectId,
    TAO_Active_Object_Map_Entry *,
    TAO_Incremental_Key_Generator> user_id_linear_map;

  /// Id active map.
  typedef ACE_Active_Map_Manager_Adapter<
  PortableServer::ObjectId,
    TAO_Active_Object_Map_Entry *,
    TAO_Ignore_Original_Key_Adapter> user_id_active_map;

  /// Base class of the servant map.
  typedef ACE_Map<
  PortableServer::Servant,
    TAO_Active_Object_Map_Entry *> servant_map;

  /// Servant hash map.
  typedef ACE_Hash_Map_Manager_Ex_Adapter<
  PortableServer::Servant,
    TAO_Active_Object_Map_Entry *,
    TAO_Servant_Hash,
    ACE_Equal_To<PortableServer::Servant>,
    ACE_Noop_Key_Generator<PortableServer::Servant> > servant_hash_map;

#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
  /// Servant linear map.
  typedef ACE_Map_Manager_Adapter<
  PortableServer::Servant,
    TAO_Active_Object_Map_Entry *,
    ACE_Noop_Key_Generator<PortableServer::Servant> > servant_linear_map;
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

  /// Id map.
  auto_ptr<user_id_map> user_id_map_;

  /// Servant map.
  auto_ptr<servant_map> servant_map_;

  /// Id uniqueness strategy.
  auto_ptr<TAO_Id_Uniqueness_Strategy> id_uniqueness_strategy_;

  /// Lifespan strategy.
  auto_ptr<TAO_Lifespan_Strategy> lifespan_strategy_;

  /// Id assignment strategy.
  auto_ptr<TAO_Id_Assignment_Strategy> id_assignment_strategy_;

  /// Id hint strategy.
  auto_ptr<TAO_Id_Hint_Strategy> id_hint_strategy_;

  /// Flag to see if we are using active maps in this active object
  /// map.
  bool using_active_maps_;

  /// Size of the system id produced by the map.
  static size_t system_id_size_;

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
  ACE::Monitor_Control::Size_Monitor *monitor_;
#endif /* TAO_HAS_MONITOR_POINTS==1 */
};

/**
 * @class TAO_Id_Uniqueness_Strategy
 *
 * @brief Id uniqueness strategy.
 *
 * Strategy for implementing points of variation between the
 * UNIQUE_ID and the MULTIPLE_ID policies.
 */
class TAO_Id_Uniqueness_Strategy
{
public:
  /// Virtual destructor.
  virtual ~TAO_Id_Uniqueness_Strategy (void);

  /// Must be used with UNIQUE_ID policy.
  virtual int
  is_servant_in_map (PortableServer::Servant servant,
                     bool &deactivated) = 0;

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// @a user_id is actually @c system_id.
  virtual int
  unbind_using_user_id (const PortableServer::ObjectId &user_id) = 0;

  /// Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  /// @a user_id is actually @c system_id.
  virtual int
  find_user_id_using_servant (PortableServer::Servant servant,
                              PortableServer::ObjectId_out user_id) = 0;

  /// Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  /// <user_id> is actually @a system_id.
  virtual int
  find_system_id_using_servant (PortableServer::Servant servant,
                                PortableServer::ObjectId_out system_id,
                                CORBA::Short &priority) = 0;

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// @a user_id is actually @c system_id.
  virtual int
  bind_using_user_id (PortableServer::Servant servant,
                      const PortableServer::ObjectId &user_id,
                      CORBA::Short priority,
                      TAO_Active_Object_Map_Entry *&entry) = 0;

  /// Are there any remaining activations of @a servant in the active
  /// object map?  Can be used with any policy.
  virtual CORBA::Boolean
  remaining_activations (PortableServer::Servant servant) = 0;

  /// Set the active map.
  void
  set_active_object_map (TAO_Active_Object_Map *active_object_map);

protected:

  /// Pointer to the active map.
  TAO_Active_Object_Map *active_object_map_;
};

/**
 * @class TAO_Unique_Id_Strategy
 *
 * @brief Unique id strategy.
 *
 * Strategy for the UNIQUE_ID policy.
 */
class TAO_Unique_Id_Strategy : public TAO_Id_Uniqueness_Strategy
{
public:

  /// Must be used with UNIQUE_ID policy.
  virtual int
  is_servant_in_map (PortableServer::Servant servant,
                     bool &deactivated);

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// @a user_id is actually @c system_id.
  virtual int
  unbind_using_user_id (const PortableServer::ObjectId &user_id);

  /// Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  /// @a user_id is actually @c system_id.
  virtual int
  find_user_id_using_servant (PortableServer::Servant servant,
                              PortableServer::ObjectId_out user_id);

  /// Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  /// <user_id> is actually @a system_id.
  virtual int
  find_system_id_using_servant (PortableServer::Servant servant,
                                PortableServer::ObjectId_out system_id,
                                CORBA::Short &priority);

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// @a user_id is actually @c system_id.
  virtual int
  bind_using_user_id (PortableServer::Servant servant,
                      const PortableServer::ObjectId &user_id,
                      CORBA::Short priority,
                      TAO_Active_Object_Map_Entry *&entry);

  /// Are there any remaining activations of @a servant in the active
  /// object map?  Can be used with any policy.
  virtual CORBA::Boolean
  remaining_activations (PortableServer::Servant servant);
};

#if !defined (CORBA_E_MICRO)
/**
 * @class TAO_Multiple_Id_Strategy
 *
 * @brief Multiple id strategy.
 *
 * Strategy for the MULTIPLE_ID policy.
 */
class TAO_Multiple_Id_Strategy : public TAO_Id_Uniqueness_Strategy
{
public:

  /// Must be used with UNIQUE_ID policy.
  virtual int
  is_servant_in_map (PortableServer::Servant servant,
                     bool &deactivated);

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// @a user_id is actually @c system_id.
  virtual int
  unbind_using_user_id (const PortableServer::ObjectId &user_id);

  /// Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  /// @a user_id is actually @c system_id.
  virtual int
  find_user_id_using_servant (PortableServer::Servant servant,
                              PortableServer::ObjectId_out user_id);

  /// Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  /// <user_id> is actually @a system_id.
  virtual int
  find_system_id_using_servant (PortableServer::Servant servant,
                                PortableServer::ObjectId_out system_id,
                                CORBA::Short &priority);

  /// Can be used with any policy.  With the SYSTEM_ID policy,
  /// @a user_id is actually @c system_id.
  virtual int
  bind_using_user_id (PortableServer::Servant servant,
                      const PortableServer::ObjectId &user_id,
                      CORBA::Short priority,
                      TAO_Active_Object_Map_Entry *&entry);

  /// Are there any remaining activations of @a servant in the active
  /// object map?  Can be used with any policy.
  virtual CORBA::Boolean
  remaining_activations (PortableServer::Servant servant);
};
#endif

/**
 * @class TAO_Lifespan_Strategy
 *
 * @brief Lifespan strategy.
 *
 * Strategy for implementing points of variation between the
 * TRANSIENT and the PERSISTENT policies.
 */
class TAO_Lifespan_Strategy
{
public:

  /// Virtual destructor.
  virtual ~TAO_Lifespan_Strategy (void);

  /// Can be used with any policy.
  virtual int
  find_servant_using_system_id_and_user_id (
    const PortableServer::ObjectId &system_id,
    const PortableServer::ObjectId &user_id,
    PortableServer::Servant &servant,
    TAO_Active_Object_Map_Entry *&entry) = 0;

  /// Set the active map.
  void
  set_active_object_map (TAO_Active_Object_Map *active_object_map);

protected:

  /// Pointer to the active map.
  TAO_Active_Object_Map *active_object_map_;
};

/**
 * @class TAO_Transient_Strategy
 *
 * @brief Transient strategy.
 *
 * Strategy for the TRANSIENT policy.
 */
class TAO_Transient_Strategy : public TAO_Lifespan_Strategy
{
public:
  /// Can be used with any policy.
  virtual int
  find_servant_using_system_id_and_user_id (
    const PortableServer::ObjectId &system_id,
    const PortableServer::ObjectId &user_id,
    PortableServer::Servant &servant,
    TAO_Active_Object_Map_Entry *&entry);
};

#if !defined (CORBA_E_MICRO)
/**
 * @class TAO_Persistent_Strategy
 *
 * @brief Persistent strategy.
 *
 * Strategy for the PERSISTENT policy.
 */
class TAO_Persistent_Strategy : public TAO_Lifespan_Strategy
{
public:
  /// Can be used with any policy.
  virtual int
  find_servant_using_system_id_and_user_id (
    const PortableServer::ObjectId &system_id,
    const PortableServer::ObjectId &user_id,
    PortableServer::Servant &servant,
    TAO_Active_Object_Map_Entry *&entry);
};
#endif

/**
 * @class TAO_Id_Assignment_Strategy
 *
 * @brief Id uniqueness strategy.
 *
 * Strategy for implementing points of variation between the
 * USER_ID and the SYSTEM_ID policies.
 */
class TAO_Id_Assignment_Strategy
{
public:
  /// Virtual destructor.
  virtual ~TAO_Id_Assignment_Strategy (void);

  /// Must be used with SYSTEM_ID policy.
  virtual int
  bind_using_system_id (PortableServer::Servant servant,
                        CORBA::Short priority,
                        TAO_Active_Object_Map_Entry *&entry) = 0;

  /// Set the active map.
  void
  set_active_object_map (TAO_Active_Object_Map *active_object_map);

protected:

  /// Pointer to the active map.
  TAO_Active_Object_Map *active_object_map_;
};

#if !defined (CORBA_E_MICRO)
/**
 * @class TAO_User_Id_Strategy
 *
 * @brief User id strategy.
 *
 * Strategy for the USER_ID policy.
 */
class TAO_User_Id_Strategy : public TAO_Id_Assignment_Strategy
{
public:
  /// Must be used with SYSTEM_ID policy.
  virtual int
  bind_using_system_id (PortableServer::Servant servant,
                        CORBA::Short priority,
                        TAO_Active_Object_Map_Entry *&entry);
};
#endif

/**
 * @class TAO_System_Id_With_Unique_Id_Strategy
 *
 * @brief System id strategy.
 *
 * Strategy for the SYSTEM_ID policy (with UNIQUE_ID policy).
 */
class TAO_System_Id_With_Unique_Id_Strategy
  : public TAO_Id_Assignment_Strategy
{
public:
  /// Must be used with SYSTEM_ID policy.
  virtual int
  bind_using_system_id (PortableServer::Servant servant,
                        CORBA::Short priority,
                        TAO_Active_Object_Map_Entry *&entry);
};

#if !defined (CORBA_E_MICRO)
/**
 * @class TAO_System_Id_With_Multiple_Id_Strategy
 *
 * @brief System id strategy.
 *
 * Strategy for the SYSTEM_ID policy (with MULTIPLE_ID policy).
 */
class TAO_System_Id_With_Multiple_Id_Strategy
  : public TAO_Id_Assignment_Strategy
{
public:
  /// Must be used with SYSTEM_ID policy.
  virtual int
  bind_using_system_id (PortableServer::Servant servant,
                        CORBA::Short priority,
                        TAO_Active_Object_Map_Entry *&entry);
};
#endif

/**
 * @class TAO_Id_Hint_Strategy
 *
 * @brief Id uniqueness strategy.
 *
 * Strategy for implementing points of variation between the
 * active hint and the no hint policies.
 */
class TAO_Id_Hint_Strategy
{
public:
  /// Virtual destructor.
  virtual ~TAO_Id_Hint_Strategy (void);

  /// Find the user id from the system id.
  virtual int
  recover_key (const PortableServer::ObjectId &system_id,
               PortableServer::ObjectId &user_id) = 0;

  /// Add to map.
  virtual int
  bind (TAO_Active_Object_Map_Entry &entry) = 0;

  /// Remove from map.
  virtual int
  unbind (TAO_Active_Object_Map_Entry &entry) = 0;

  /// Find system id.
  virtual int
  find (const PortableServer::ObjectId &system_id,
        TAO_Active_Object_Map_Entry *&entry) = 0;

  /// How big is the hint generated by this strategy?
  virtual size_t
  hint_size (void) = 0;

  /// Get the system id associated with this entry.
  virtual int
  system_id (PortableServer::ObjectId_out system_id,
             TAO_Active_Object_Map_Entry &entry) = 0;
};

/**
 * @class TAO_Active_Hint_Strategy
 *
 * @brief Active hint strategy.
 *
 * Strategy for adding active hints to ids.
 */
class TAO_Active_Hint_Strategy : public TAO_Id_Hint_Strategy
{
public:
  TAO_Active_Hint_Strategy (CORBA::ULong map_size);

  /// Virtual destructor.
  virtual ~TAO_Active_Hint_Strategy (void);

  virtual int
  recover_key (const PortableServer::ObjectId &system_id,
               PortableServer::ObjectId &user_id);

  virtual int
  bind (TAO_Active_Object_Map_Entry &entry);

  virtual int
  unbind (TAO_Active_Object_Map_Entry &entry);

  virtual int
  find (const PortableServer::ObjectId &system_id,
        TAO_Active_Object_Map_Entry *&entry);

  virtual size_t
  hint_size (void);

  virtual int
  system_id (PortableServer::ObjectId_out system_id,
             TAO_Active_Object_Map_Entry &entry);

  typedef ACE_Active_Map_Manager_Adapter<
  PortableServer::ObjectId,
    TAO_Active_Object_Map_Entry *,
    TAO_Preserve_Original_Key_Adapter> system_id_map;

  system_id_map system_id_map_;
};

/**
 * @class TAO_No_Hint_Strategy
 *
 * @brief No hint strategy.
 *
 * Strategy for not adding active hints to ids.
 */
class TAO_No_Hint_Strategy : public TAO_Id_Hint_Strategy
{
public:
  /// Virtual destructor.
  virtual ~TAO_No_Hint_Strategy (void);

  virtual int
  recover_key (const PortableServer::ObjectId &system_id,
               PortableServer::ObjectId &user_id);

  virtual int
  bind (TAO_Active_Object_Map_Entry &entry);

  virtual int
  unbind (TAO_Active_Object_Map_Entry &entry);

  virtual int
  find (const PortableServer::ObjectId &system_id,
        TAO_Active_Object_Map_Entry *&entry);

  virtual size_t
  hint_size (void);

  virtual int
  system_id (PortableServer::ObjectId_out system_id,
             TAO_Active_Object_Map_Entry &entry);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/Active_Object_Map.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_ACTIVE_OBJECT_MAP_H */
