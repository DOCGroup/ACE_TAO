/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Active_Object_Map.h
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#ifndef TAO_ACTIVE_OBJECT_MAP_H
#define TAO_ACTIVE_OBJECT_MAP_H
#include "ace/pre.h"

#include "Key_Adapters.h"
#include "Servant_Base.h"
#include "tao/Server_Strategy_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declarations.
class TAO_Id_Uniqueness_Strategy;
class TAO_Lifespan_Strategy;
class TAO_Id_Assignment_Strategy;
class TAO_Id_Hint_Strategy;

class TAO_PortableServer_Export TAO_Active_Object_Map
{
  // = TITLE
  //     Map of object ids to servants.
  //
  // = DESCRIPTION
  //     Implementation to be used by the POA.
public:

  struct Map_Entry
  {
    // = TITLE
    //     Value field of the active object map.
    //
    // = DESCRIPTION
    //     We need a mapping from and to all of the following fields:
    //     <user_id>, <system_id>, and <servant>.  Therefore, we keep
    //     all the fields together in the map.

    Map_Entry (void);
    // Default constructor.

    PortableServer::ObjectId user_id_;
    // User id.

    PortableServer::ObjectId system_id_;
    // System id.

    PortableServer::Servant servant_;
    // Servant.

    CORBA::UShort reference_count_;
    // Reference count on outstanding requests on this servant.

    CORBA::Boolean deactivated_;
    // Has this servant been deactivated already?

    CORBA::Short priority_;
    // Priority of this servant.
  };

  TAO_Active_Object_Map (int user_id_policy,
                         int unique_id_policy,
                         int persistent_id_policy,
                         const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters
                         ACE_ENV_ARG_DECL);
  // Constructor.

  ~TAO_Active_Object_Map (void);
  // Destructor.

  int is_servant_in_map (PortableServer::Servant servant,
                         int &deactivated);
  // Must be used with UNIQUE_ID policy.

  int is_user_id_in_map (const PortableServer::ObjectId &user_id,
                         CORBA::Short priority,
                         int &priorities_match,
                         int &deactivated);
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  int bind_using_system_id_returning_system_id (PortableServer::Servant servant,
                                                CORBA::Short priority,
                                                PortableServer::ObjectId_out system_id);
  // Must be used with SYSTEM_ID policy.

  int bind_using_system_id_returning_user_id (PortableServer::Servant servant,
                                              CORBA::Short priority,
                                              PortableServer::ObjectId_out user_id);
  // Must be used with SYSTEM_ID policy.

  int bind_using_user_id (PortableServer::Servant servant,
                          const PortableServer::ObjectId &user_id,
                          CORBA::Short priority);
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  int find_system_id_using_user_id (const PortableServer::ObjectId &user_id,
                                    CORBA::Short priority,
                                    PortableServer::ObjectId_out system_id);
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  int rebind_using_user_id_and_system_id (PortableServer::Servant servant,
                                          const PortableServer::ObjectId &user_id,
                                          const PortableServer::ObjectId &system_id,
                                          TAO_Active_Object_Map::Map_Entry *&entry);
  // Can be used with any policy.

  int unbind_using_user_id (const PortableServer::ObjectId &user_id);
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  int find_user_id_using_servant (PortableServer::Servant servant,
                                  PortableServer::ObjectId_out user_id);
  // Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  int find_system_id_using_servant (PortableServer::Servant servant,
                                    PortableServer::ObjectId_out system_id,
                                    CORBA::Short &priority);
  // Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  int find_servant_using_user_id (const PortableServer::ObjectId &user_id,
                                  PortableServer::Servant &servant);
  // Can be used with any policy. With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  int find_servant_using_system_id_and_user_id (const PortableServer::ObjectId &system_id,
                                                const PortableServer::ObjectId &user_id,
                                                PortableServer::Servant &servant,
                                                TAO_Active_Object_Map::Map_Entry *&entry);
  // Can be used with any policy.

  int find_servant_and_system_id_using_user_id (const PortableServer::ObjectId &user_id,
                                                PortableServer::Servant &servant,
                                                PortableServer::ObjectId_out system_id,
                                                CORBA::Short &priority);
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is identical to <system_id>.

  int find_servant_and_system_id_using_user_id (const PortableServer::ObjectId &user_id,
                                                TAO_Active_Object_Map::Map_Entry *&entry);
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is identical to <system_id>.

  int find_user_id_using_system_id (const PortableServer::ObjectId &system_id,
                                    PortableServer::ObjectId_out user_id);
  // Can be used with any policy.  When the SYSTEM_ID policy is used,
  // the <system_id> is identical to <user_id>.

  int find_user_id_using_system_id (const PortableServer::ObjectId &system_id,
                                    PortableServer::ObjectId &user_id);
  // Can be used with any policy.  When the SYSTEM_ID policy is used,
  // the <system_id> is identical to <user_id>.

  CORBA::Boolean remaining_activations (PortableServer::Servant servant);
  // Are there any remaining activations of <servant> in the active
  // object map?  Can be used with any policy.

  size_t current_size (void);
  // Size of the map.

  static size_t system_id_size (void);
  // Can be used with any policy.

  static void set_system_id_size (const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters);
  // Set the system id size.

  typedef ACE_Map<
  PortableServer::ObjectId,
    Map_Entry *> user_id_map;
  // Base class of the id map.

  typedef ACE_Hash_Map_Manager_Ex_Adapter<
  PortableServer::ObjectId,
    Map_Entry *,
    TAO_ObjectId_Hash,
    ACE_Equal_To<PortableServer::ObjectId>,
    TAO_Incremental_Key_Generator> user_id_hash_map;
  // Id hash map.

#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
  typedef ACE_Map_Manager_Adapter<
  PortableServer::ObjectId,
    Map_Entry *,
    TAO_Incremental_Key_Generator> user_id_linear_map;
  // Id linear map.
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

  typedef ACE_Active_Map_Manager_Adapter<
  PortableServer::ObjectId,
    Map_Entry *,
    TAO_Ignore_Original_Key_Adapter> user_id_active_map;
  // Id active map.

  typedef ACE_Map<
  PortableServer::Servant,
    Map_Entry *> servant_map;
  // Base class of the servant map.

  typedef ACE_Hash_Map_Manager_Ex_Adapter<
  PortableServer::Servant,
    Map_Entry *,
    TAO_Servant_Hash,
    ACE_Equal_To<PortableServer::Servant>,
    ACE_Noop_Key_Generator<PortableServer::Servant> > servant_hash_map;
  // Servant hash map.

#if (TAO_HAS_MINIMUM_POA_MAPS == 0)
  typedef ACE_Map_Manager_Adapter<
  PortableServer::Servant,
    Map_Entry *,
    ACE_Noop_Key_Generator<PortableServer::Servant> > servant_linear_map;
  // Servant linear map.
#endif /* TAO_HAS_MINIMUM_POA_MAPS == 0 */

  user_id_map *user_id_map_;
  // Id map.

  servant_map *servant_map_;
  // Servant map.

  TAO_Id_Uniqueness_Strategy *id_uniqueness_strategy_;
  // Id uniqueness strategy.

  TAO_Lifespan_Strategy *lifespan_strategy_;
  // Lifespan strategy.

  TAO_Id_Assignment_Strategy *id_assignment_strategy_;
  // Id assignment strategy.

  TAO_Id_Hint_Strategy *id_hint_strategy_;
  // Id hint strategy.

  int using_active_maps_;
  // Flag to see if we are using active maps in this active object
  // map.

  static size_t system_id_size_;
  // Size of the system id produced by the map.
};

class TAO_Id_Uniqueness_Strategy
{
  // = TITLE
  //     Id uniqueness strategy.
  //
  // = DESCRIPTION
  //     Strategy for implementing points of variation between the
  //     UNIQUE_ID and the MULTIPLE_ID policies.
public:
  virtual ~TAO_Id_Uniqueness_Strategy (void);
  // Virtual destructor.

  virtual int is_servant_in_map (PortableServer::Servant servant,
                                 int &deactivated) = 0;
  // Must be used with UNIQUE_ID policy.

  virtual int unbind_using_user_id (const PortableServer::ObjectId &user_id) = 0;
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual int find_user_id_using_servant (PortableServer::Servant servant,
                                          PortableServer::ObjectId_out user_id) = 0;
  // Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual int find_system_id_using_servant (PortableServer::Servant servant,
                                            PortableServer::ObjectId_out system_id,
                                            CORBA::Short &priority) = 0;
  // Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual int bind_using_user_id (PortableServer::Servant servant,
                                  const PortableServer::ObjectId &user_id,
                                  CORBA::Short priority,
                                  TAO_Active_Object_Map::Map_Entry *&entry) = 0;
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual CORBA::Boolean remaining_activations (PortableServer::Servant servant) = 0;
  // Are there any remaining activations of <servant> in the active
  // object map?  Can be used with any policy.

  void set_active_object_map (TAO_Active_Object_Map *active_object_map);
  // Set the active map.

protected:

  TAO_Active_Object_Map *active_object_map_;
  // Pointer to the active map.
};

class TAO_Unique_Id_Strategy : public TAO_Id_Uniqueness_Strategy
{
  // = TITLE
  //     Unique id strategy.
  //
  // = DESCRIPTION
  //     Strategy for the UNIQUE_ID policy.
public:

  virtual int is_servant_in_map (PortableServer::Servant servant,
                                 int &deactivated);
  // Must be used with UNIQUE_ID policy.

  virtual int unbind_using_user_id (const PortableServer::ObjectId &user_id);
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual int find_user_id_using_servant (PortableServer::Servant servant,
                                          PortableServer::ObjectId_out user_id);
  // Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual int find_system_id_using_servant (PortableServer::Servant servant,
                                            PortableServer::ObjectId_out system_id,
                                            CORBA::Short &priority);
  // Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual int bind_using_user_id (PortableServer::Servant servant,
                                  const PortableServer::ObjectId &user_id,
                                  CORBA::Short priority,
                                  TAO_Active_Object_Map::Map_Entry *&entry);
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual CORBA::Boolean remaining_activations (PortableServer::Servant servant);
  // Are there any remaining activations of <servant> in the active
  // object map?  Can be used with any policy.
};

class TAO_Multiple_Id_Strategy : public TAO_Id_Uniqueness_Strategy
{
  // = TITLE
  //     Multiple id strategy.
  //
  // = DESCRIPTION
  //     Strategy for the MULTIPLE_ID policy.
public:

  virtual int is_servant_in_map (PortableServer::Servant servant,
                                 int &deactivated);
  // Must be used with UNIQUE_ID policy.

  virtual int unbind_using_user_id (const PortableServer::ObjectId &user_id);
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual int find_user_id_using_servant (PortableServer::Servant servant,
                                          PortableServer::ObjectId_out user_id);
  // Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual int find_system_id_using_servant (PortableServer::Servant servant,
                                            PortableServer::ObjectId_out system_id,
                                            CORBA::Short &priority);
  // Must be used with UNIQUE_ID policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual int bind_using_user_id (PortableServer::Servant servant,
                                  const PortableServer::ObjectId &user_id,
                                  CORBA::Short priority,
                                  TAO_Active_Object_Map::Map_Entry *&entry);
  // Can be used with any policy.  With the SYSTEM_ID policy,
  // <user_id> is actually <system_id>.

  virtual CORBA::Boolean remaining_activations (PortableServer::Servant servant);
  // Are there any remaining activations of <servant> in the active
  // object map?  Can be used with any policy.
};

class TAO_Lifespan_Strategy
{
  // = TITLE
  //     Lifespan strategy.
  //
  // = DESCRIPTION
  //     Strategy for implementing points of variation between the
  //     TRANSIENT and the PERSISTENT policies.
public:

  virtual ~TAO_Lifespan_Strategy (void);
  // Virtual destructor.

  virtual int find_servant_using_system_id_and_user_id (const PortableServer::ObjectId &system_id,
                                                        const PortableServer::ObjectId &user_id,
                                                        PortableServer::Servant &servant,
                                                        TAO_Active_Object_Map::Map_Entry *&entry) = 0;
  // Can be used with any policy.

  void set_active_object_map (TAO_Active_Object_Map *active_object_map);
  // Set the active map.

protected:

  TAO_Active_Object_Map *active_object_map_;
  // Pointer to the active map.
};

class TAO_Transient_Strategy : public TAO_Lifespan_Strategy
{
  // = TITLE
  //     Transient strategy.
  //
  // = DESCRIPTION
  //     Strategy for the TRANSIENT policy.
public:
  virtual int find_servant_using_system_id_and_user_id (const PortableServer::ObjectId &system_id,
                                                        const PortableServer::ObjectId &user_id,
                                                        PortableServer::Servant &servant,
                                                        TAO_Active_Object_Map::Map_Entry *&entry);
  // Can be used with any policy.
};

class TAO_Persistent_Strategy : public TAO_Lifespan_Strategy
{
  // = TITLE
  //     Persistent strategy.
  //
  // = DESCRIPTION
  //     Strategy for the PERSISTENT policy.
public:
  virtual int find_servant_using_system_id_and_user_id (const PortableServer::ObjectId &system_id,
                                                        const PortableServer::ObjectId &user_id,
                                                        PortableServer::Servant &servant,
                                                        TAO_Active_Object_Map::Map_Entry *&entry);
  // Can be used with any policy.

};

class TAO_Id_Assignment_Strategy
{
  // = TITLE
  //     Id uniqueness strategy.
  //
  // = DESCRIPTION
  //     Strategy for implementing points of variation between the
  //     USER_ID and the SYSTEM_ID policies.
public:
  virtual ~TAO_Id_Assignment_Strategy (void);
  // Virtual destructor.

  virtual int bind_using_system_id (PortableServer::Servant servant,
                                    CORBA::Short priority,
                                    TAO_Active_Object_Map::Map_Entry *&entry) = 0;
  // Must be used with SYSTEM_ID policy.

  void set_active_object_map (TAO_Active_Object_Map *active_object_map);
  // Set the active map.

protected:

  TAO_Active_Object_Map *active_object_map_;
  // Pointer to the active map.
};

class TAO_User_Id_Strategy : public TAO_Id_Assignment_Strategy
{
  // = TITLE
  //     User id strategy.
  //
  // = DESCRIPTION
  //     Strategy for the USER_ID policy.
public:
  virtual int bind_using_system_id (PortableServer::Servant servant,
                                    CORBA::Short priority,
                                    TAO_Active_Object_Map::Map_Entry *&entry);
  // Must be used with SYSTEM_ID policy.
};

class TAO_System_Id_With_Unique_Id_Strategy : public TAO_Id_Assignment_Strategy
{
  // = TITLE
  //     System id strategy.
  //
  // = DESCRIPTION
  //     Strategy for the SYSTEM_ID policy (with UNIQUE_ID policy).
public:
  virtual int bind_using_system_id (PortableServer::Servant servant,
                                    CORBA::Short priority,
                                    TAO_Active_Object_Map::Map_Entry *&entry);
  // Must be used with SYSTEM_ID policy.
};

class TAO_System_Id_With_Multiple_Id_Strategy : public TAO_Id_Assignment_Strategy
{
  // = TITLE
  //     System id strategy.
  //
  // = DESCRIPTION
  //     Strategy for the SYSTEM_ID policy (with MULTIPLE_ID policy).
public:
  virtual int bind_using_system_id (PortableServer::Servant servant,
                                    CORBA::Short priority,
                                    TAO_Active_Object_Map::Map_Entry *&entry);
  // Must be used with SYSTEM_ID policy.
};

class TAO_Id_Hint_Strategy
{
  // = TITLE
  //     Id uniqueness strategy.
  //
  // = DESCRIPTION
  //     Strategy for implementing points of variation between the
  //     active hint and the no hint policies.
public:
  virtual ~TAO_Id_Hint_Strategy (void);
  // Virtual destructor.

  virtual int recover_key (const PortableServer::ObjectId &system_id,
                           PortableServer::ObjectId &user_id) = 0;
  // Find the user id from the system id.

  virtual int bind (TAO_Active_Object_Map::Map_Entry &entry) = 0;
  // Add to map.

  virtual int unbind (TAO_Active_Object_Map::Map_Entry &entry) = 0;
  // Remove from map.

  virtual int find (const PortableServer::ObjectId &system_id,
                    TAO_Active_Object_Map::Map_Entry *&entry) = 0;
  // Find system id.

  virtual size_t hint_size (void) = 0;
  // How big is the hint generated by this strategy?

  virtual int system_id (PortableServer::ObjectId_out system_id,
                         TAO_Active_Object_Map::Map_Entry &entry) = 0;
  // Get the system id associated with this entry.
};

class TAO_Active_Hint_Strategy : public TAO_Id_Hint_Strategy
{
  // = TITLE
  //     Active hint strategy.
  //
  // = DESCRIPTION
  //     Strategy for adding active hints to ids.
public:
  TAO_Active_Hint_Strategy (CORBA::ULong map_size);

  virtual ~TAO_Active_Hint_Strategy (void);
  // Virtual destructor.

  virtual int recover_key (const PortableServer::ObjectId &system_id,
                           PortableServer::ObjectId &user_id);

  virtual int bind (TAO_Active_Object_Map::Map_Entry &entry);

  virtual int unbind (TAO_Active_Object_Map::Map_Entry &entry);

  virtual int find (const PortableServer::ObjectId &system_id,
                    TAO_Active_Object_Map::Map_Entry *&entry);

  virtual size_t hint_size (void);

  virtual int system_id (PortableServer::ObjectId_out system_id,
                         TAO_Active_Object_Map::Map_Entry &entry);

  typedef ACE_Active_Map_Manager_Adapter<
  PortableServer::ObjectId,
    TAO_Active_Object_Map::Map_Entry *,
    TAO_Preserve_Original_Key_Adapter> system_id_map;

  system_id_map system_id_map_;
};

class TAO_No_Hint_Strategy : public TAO_Id_Hint_Strategy
{
  // = TITLE
  //     No hint strategy.
  //
  // = DESCRIPTION
  //     Strategy for not adding active hints to ids.
public:
  virtual ~TAO_No_Hint_Strategy (void);
  // Virtual destructor.

  virtual int recover_key (const PortableServer::ObjectId &system_id,
                           PortableServer::ObjectId &user_id);

  virtual int bind (TAO_Active_Object_Map::Map_Entry &entry);

  virtual int unbind (TAO_Active_Object_Map::Map_Entry &entry);

  virtual int find (const PortableServer::ObjectId &system_id,
                    TAO_Active_Object_Map::Map_Entry *&entry);

  virtual size_t hint_size (void);

  virtual int system_id (PortableServer::ObjectId_out system_id,
                         TAO_Active_Object_Map::Map_Entry &entry);
};

#if defined (__ACE_INLINE__)
# include "Active_Object_Map.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ACTIVE_OBJECT_MAP_H */
