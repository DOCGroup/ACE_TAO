/**
 *  @file   Load_Balancer_i.h
 *
 *  Defines classes that implement interfaces in Load_Balancer.idl
 *
 *  @author Marina Spivak <marina@cs.wustl.edu> with modifications by Bala Natarajan <bala@cs.wustl.edu>
 */

#ifndef LOAD_BALANCER_I_H_
#define LOAD_BALANCER_I_H_

#include "Load_BalancerS.h"
#include "ace/Hash_Map_With_Allocator_T.h"
#include "ace/SString.h"
#include "ace/Synch.h"
#include "ace/Containers.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/High_Res_Timer.h"
#include "ace/Memory_Pool.h"
#include "ace/Malloc_T.h"

#include <memory>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

using ALLOCATOR = ACE_Allocator_Adapter <ACE_Malloc<ACE_MMAP_MEMORY_POOL,
  TAO_SYNCH_MUTEX> >;

using HASH_MAP = ACE_Hash_Map_With_Allocator<char *, char *>;

/**
 * Implements the Load_Balancer::Object_Group_Factory interface.
 *
 * This implementation uses two maps to store Group_ID to Object_Group
 * associations for all load-balancing groups created by the factory. One
 * map tracks random groups, and the other tracks round-robin groups.
 */
class Object_Group_Factory_i :
  public virtual POA_Load_Balancer::Object_Group_Factory
{
public:
  Object_Group_Factory_i (CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa);

  ~Object_Group_Factory_i () override = default;

  Object_Group_Factory_i (Object_Group_Factory_i const &) = delete;
  Object_Group_Factory_i (Object_Group_Factory_i &&) = delete;
  Object_Group_Factory_i &operator= (Object_Group_Factory_i const &) = delete;
  Object_Group_Factory_i &operator= (Object_Group_Factory_i &&) = delete;

  /// Returns the persistent POA reference stored in this servant.
  PortableServer::POA_ptr _default_POA () override;

  /**
   * Creates an @c Object_Group that resolves requests for arbitrary
   * members in round robin order.  If an @c Object_Group, of any
   * type, with Group_ID @c id has already been created by this
   * factory, and hasn't been destroyed, a @c duplicate_group
   * exception is thrown.
   */
  Load_Balancer::Object_Group_ptr make_round_robin (char const *id) override;

  void unbind_round_robin (char const *id) override;

  /**
   * Creates an @c Object_Group that resolves requests for arbitrary
   * members in random order.  If an @c Object_Group, of any
   * type, with Group_ID @c id has already been created by this
   * factory, and hasn't been destroyed, a @c duplicate_group
   * exception is thrown.
   */
  Load_Balancer::Object_Group_ptr make_random (char const *id) override;

  void unbind_random (char const *id) override;

  /**
   * Locates and returns an @c Object_Group IOR by its @c Group_ID.   If
   * no @c Object_Group has @c Group_ID of @c id, throw a
   * @c no_such_group exception.
   */
  Load_Balancer::Object_Group_ptr resolve (char const *id) override;

  /**
   * Lists all the round robin @c Object_Group objects which were created
   * by this factory, and haven't been destroyed yet, i.e., return
   * a sequence of @c Group_ID values of all existing round robin
   * @c Object_Group objects created by this factory.
   */
  Load_Balancer::Group_List * round_robin_groups () override;

  /**
   * Lists all the random @c Object_Group objects which were created
   * by this factory, and haven't been destroyed yet, i.e., return
   * a sequence of @c Group_ID values of all existing random
   * @c Object_Group objects created by this factory.
   */
  Load_Balancer::Group_List * random_groups () override;

private:
  /// Our ORB
  CORBA::ORB_var orb_;

  /// Our POA
  PortableServer::POA_var poa_;

  /// @name Helper methods
  /// @{
  /**
   * This function factors out common code in @c make_round_robin and
   * @c make_random.  Creates a random @c Object_Group if @c random parameter is
   * true and round robin @c Object_Group otherwise.
   */
  Load_Balancer::Object_Group_ptr make_group (bool random,
                                              char const *id);

  /**
   * This function factors out common code in @c random_groups and
   * @c round_robin_groups.  Returns a sequence of its random
   * groups if @c random is true and a sequence of its round robin
   * groups otherwise.
   */
  Load_Balancer::Group_List * list_groups (bool random);

  /// The helper that updates the vlaue of the variable flags_
  void update_flags (bool random);

  /// This rolls back the status of the objects in the POA if the
  /// service had failed..
  void update_objects ();
  /// @}

  /// Raw pointer to persistent map storage owned by the memory pool.
  HASH_MAP *random_groups_ {};

  /// Raw pointer to persistent map storage owned by the memory pool.
  HASH_MAP *rr_groups_ {};

  /// Memory pool that will have the data
  std::unique_ptr<ALLOCATOR> mem_pool_;

  /**
   * This would be kind of a hack.. As I am not able to think of
   * anything at present let us live with this.. OK.. Here is how it
   * works.. This value will be stored in the MMAP file. If the value
   * is 1 then the Round Robin group object is registered with the
   * Services POA. If the value is 2 then the Random group object is
   * registered with the POA. If the value is 3 both of them are
   * registered with the POA.. The initial value would be 0 when this
   * object initialises and binded as "FLAGS"..
   */
  CORBA::Short *flags_ {};

  ACE_Throughput_Stats throughput_;
};

/**
 * Partially implements the Load_Balancer::Object_Group interface.
 *
 * resolve() is the only abstract method. Subclasses define it to implement
 * a load-balancing policy; other methods can be overridden as needed. This
 * class factors out code shared by Object_Group implementations with
 * different policies.
 */
class Object_Group_i
  : public virtual POA_Load_Balancer::Object_Group
{
public:
  Object_Group_i (char const *id,
                  PortableServer::POA_ptr poa);

  ~Object_Group_i () override;

  Object_Group_i (Object_Group_i const &) = delete;
  Object_Group_i (Object_Group_i &&) = delete;
  Object_Group_i &operator= (Object_Group_i const &) = delete;
  Object_Group_i &operator= (Object_Group_i &&) = delete;

  /// Returns the persistent POA used by this servant.
  PortableServer::POA_ptr _default_POA () override;

  /// Get group's id.
  char * id () override;

  /**
   * Adds a new @c member to the @c Object_Group.  Note that each
   * @c Member_ID in an @c Object_Group must be unique.  If the
   * group already contains a member with the same @c Member_ID, a
   * @c duplicate_member exceptions is thrown.
   */
  void bind (Load_Balancer::Member const &member) override;

  /**
   * Removes a member with the specified @c Member_ID from the
   * @c Object_Group.  If none of the group's members have a
   * Member_ID of @c id, @c no_such_member exception is thrown.
   */
  void unbind (char const *id) override;

  /**
   * Returns a member object from this @c Object_Group in accordance with
   * load balancing policy it implements, i.e., ``random'' or
   * ``round robin.''  If the group contains no members, @c no_such_member
   * exception is thrown.
   */
  char * resolve () override = 0;

  /**
   * Returns an object with the specified @c Member_ID.  If this
   * @c Object_Group contains no members with the specified
   * @c Member_ID, @c no_such_member exception is thrown.
   */
  char * resolve_with_id (char const *id) override;

  /// Return a sequence of @c Member_ID values for all members.
  Load_Balancer::Member_ID_List * members () override;

  /**
   * Cleanup the resources associated with this @c Object_Group.
   * Subsequent calls to this @c Object_Group should fail, and its
   * @c id should become available.  @c Object_Group_Factory
   * should no longer list this @c Object_Group.
   */
  void destroy () override;

protected:
  /// This will replenish all the pointers that could have been lost
  /// because of failure
  void read_from_memory ();

  /// Our POA
  PortableServer::POA_var poa_;

  /// Typedefs for ease of use.
  using LIST = ACE_DLList<char *>;
  using ITERATOR = ACE_DLList_Iterator<char *>;

  /**
   * Member IDs are stored in both the list and map: the list supports
   * policy-specific resolve operations, while the map supports lookup by ID.
   */
  /// These raw pointers refer to map/list structures in the persistent allocator.
  /// They are rebound from the memory map after restart and do not own that storage.
  /// List of ids of all the members of this group.
  LIST *member_id_list_ {};

  /// Mapping of member_id to obj for all the members of this group.
  HASH_MAP *members_ {};

  /// This group's id.
  ACE_CString id_;

  /// Owns the process-local allocator wrapper for persistent storage.
  std::unique_ptr<ALLOCATOR> allocator_;
};

/**
 * @class Random_Object_Group
 *
 * @brief This class implements @c Object_Group idl interface with the
 * random policy for @c resolve.
 */
class Random_Object_Group : public Object_Group_i
{
public:
  Random_Object_Group (char const *id,
                       PortableServer::POA_ptr poa);

  Random_Object_Group (Random_Object_Group const &) = delete;
  Random_Object_Group (Random_Object_Group &&) = delete;
  Random_Object_Group &operator= (Random_Object_Group const &) = delete;
  Random_Object_Group &operator= (Random_Object_Group &&) = delete;

  /// Returns a member object from this @c Object_Group in accordance with
  /// the "random" load balancing policy.
  char * resolve () override;
};

/**
 * @class RR_Object_Group:
 *
 * @brief This class implements @c Object_Group idl interface with the
 * round robin policy for @c resolve.
 */
class RR_Object_Group: public Object_Group_i
{
public:
  RR_Object_Group (char const *id,
                   PortableServer::POA_ptr poa);

  RR_Object_Group (RR_Object_Group const &) = delete;
  RR_Object_Group (RR_Object_Group &&) = delete;
  RR_Object_Group &operator= (RR_Object_Group const &) = delete;
  RR_Object_Group &operator= (RR_Object_Group &&) = delete;

  /**
   * We need to override the implementation of @c unbind from
   * Object_Group_i to make sure @c resolve
   * works correctly.
   */
  void unbind (char const *id) override;

  /// Returns a member object from this @c Object_Group in accordance with
  /// the "round robin" load balancing policy.
  char * resolve () override;

private:
  /// Index into the Object_Group_i::member_id_list_: keeps track of
  /// the member_id to return on the next invocation of @c resolve.
  size_t next_;
};

#endif /* LOAD_BALANCER_I_H_  */
