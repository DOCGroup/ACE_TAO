/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Load_Balancer_i.h
 *
 *  $Id$
 *
 *  Defines classes that implement interfaces in Load_Balancer.idl
 *
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef LOAD_BALANCER_I_H_
#define LOAD_BALANCER_I_H_

#include "Load_BalancerS.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"
#include "ace/Synch.h"
#include "ace/Containers.h"
#include "ace/Null_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Object_Group_Factory_i :
  public virtual POA_Load_Balancer::Object_Group_Factory
{
  // = TITLE
  //    This class implements Load_Balancer::Object_Group_Factory idl
  //    interface.
  //
  // = DESCRIPTION
  //    This implementation uses two <ACE_Hash_Map_Manager>s
  //    to store <Group_ID> to <Object_Group> associations for all
  //    load balancing groups created by this factory (one map keeps
  //    track of all random groups, and the other keeps track of all
  //    round robin groups).
  //
public:

  // = Initialization and termination methods.

  /// Constructor.
  Object_Group_Factory_i (void);

  /// Destructor.
  ~Object_Group_Factory_i (void);

  // = Load_Balancer::Object_Group_Factory idl methods.

  /**
   * Creates an <Object_Group> that resolves requests for arbitrary
   * members in round robin order.  If an <Object_Group>, of any
   * type, with Group_ID <id> has already been created by this
   * factory, and hasn't been destroyed, a <duplicate_group>
   * exception is thrown.
   */
  Load_Balancer::Object_Group_ptr make_round_robin (const char * id);

  /**
   * Creates an <Object_Group> that resolves requests for arbitrary
   * members in random order.  If an <Object_Group>, of any
   * type, with Group_ID <id> has already been created by this
   * factory, and hasn't been destroyed, a <duplicate_group>
   * exception is thrown.
   */
  Load_Balancer::Object_Group_ptr make_random (const char * id);

  /**
   * Locates and returns an <Object_Group> by its <Group_ID>.   If
   * no <Object_Group> has <Group_ID> of <id>, throw a
   * <no_such_group> exception.
   */
  Load_Balancer::Object_Group_ptr resolve (const char * id);

  /**
   * Lists all the round robin <Object_Group>s which were created
   * by this factory, and haven't been destroyed yet, i.e., return
   * a sequence of <Group_ID>s of all existing round robin
   * <Object_Group>s created by this factory.
   */
  Load_Balancer::Group_List * round_robin_groups (void);

  /**
   * Lists all the random <Object_Group>s which were created
   * by this factory, and haven't been destroyed yet, i.e., return
   * a sequence of <Group_ID>s of all existing random
   * <Object_Group>s created by this factory.
   */
  Load_Balancer::Group_List * random_groups (void);

  // = Implementation detail methods.

  /**
   * This method is invoked by an <Object_Group> with group id <id> when it
   * is being destroyed.  The method removes entry corresponding to
   * group <id> from <random_groups_> if <random> is 1 or from
   * <rr_groups_> if <random> is 0.  This recycles <id>, allowing it
   * to be used for new <Object_Group>, and prevents the destroyed
   * group from being included in lists returned from <random_groups>
   * and <round_robin_groups> methods.
   */
  void remove_group (const ACE_CString &id, int random);

private:
  // = Helper methods.

  /**
   * This function factors out common code in <make_round_robin> and
   * <make_random>.  Creates a random <Object_Group> if <random> parameter is
   * set to 1 and round robin <Object_Group> if it is 0.
   */
  Load_Balancer::Object_Group_ptr make_group (int random,
                                              const char * id);

  /**
   * This function factors out common code in <random_groups> and
   * <round_robin_groups>.  Returns a sequence of its random
   * groups if <random> parameter is set to 1 and a sequence of its
   * round robin groups if it is 0.
   */
  Load_Balancer::Group_List * list_groups (int random);

  /// Typedef for ease of use: hash map associating group ids to
  /// <Object_Group> references.
  typedef ACE_Hash_Map_Manager<ACE_CString,
    Load_Balancer::Object_Group_var, ACE_Null_Mutex> HASH_MAP;

  /// Map containing all random <Object_Group>s created by this factory.
  HASH_MAP random_groups_;

  /// Map containing all round robin <Object_Group>s created by this factory.
  HASH_MAP rr_groups_;
};

class Object_Group_i : public virtual POA_Load_Balancer::Object_Group

{
  // = TITLE
  //    This abstract class partially implements
  //    Load_Balancer::Object_Group idl interface.
  //
  // = DESCRIPTION
  //   <Resolve> is the only abstract method - subclasses should
  //   define it in order to implement an appropriate load balancing
  //   policy.  Other methods can be overridden as needed.  This class
  //   factors out code common to <Object_Group> implementations with
  //   different load balancing policies.
  //
public:

  // = Initialization and termination methods.

  /// Constructor.
  Object_Group_i (const char * id,
                  Object_Group_Factory_i * my_factory);

  /// Destructor.
  ~Object_Group_i (void);

  // = Load_Balancer::Object_Group idl methods.

  /// Get group's id.
  char * id (void);

  /**
   * Adds a new <member> to the <Object_Group>.  Note that each
   * <Member_ID> in an <Object_Group> must be unique.  If the
   * group already contains a member with the same <Member_ID>, a
   * <duplicate_member> exceptions is thrown.
   */
  void bind (const Load_Balancer::Member & member);

  /**
   * Removes a member with the specified <Member_ID> from the
   * <Object_Group>.  If none of the group's members have a
   * Member_ID of <id>, <no_such_member> exception is thrown.
   */
  void unbind (const char * id);

  /**
   * Returns a member object from this <Object_Group> in accordance with
   * load balancing policy it implements, i.e., ``random'' or
   * ``round robin.''  If the group contains no members, <no_such_member>
   * exception is thrown.
   */
  CORBA::Object_ptr resolve (void)
    = 0;

  /**
   * Returns an object with the specified <Member_ID>.  If this
   * <Object_Group> contains no members with the specified
   * <Member_ID>, <no_such_member> exception is thrown.
   */
  CORBA::Object_ptr resolve_with_id (const char * id);

  /// Return a sequence of <Member_ID>s of all of its members.
  Load_Balancer::Member_ID_List * members (void);

  /**
   * Cleanup the resources associated with this <Object_Group>.
   * Subsequent calls to this <Object_Group> should fail, and its
   * <id> should become available.  <Object_Group_Factory>
   * should no longer list this <Object_Group>.
   */
  void destroy (void);

protected:

  /// Typedefs for ease of use.
  typedef ACE_DLList<ACE_CString> LIST;
  typedef ACE_DLList_Iterator<ACE_CString> ITERATOR;
  typedef ACE_Hash_Map_Manager<ACE_CString, CORBA::Object_var,
    ACE_Null_Mutex> HASH_MAP;

  /// List of ids of all the members of this group.
  LIST member_id_list_;

  /// Mapping of member_id to obj for all the members of this group.
  HASH_MAP members_;

  // Note, we store information redundantly in this implementation,
  // i.e., both <member_id_list_> and <members_> store member ids.
  // However, this redundancy eases/speeds up the implementation of
  // certain operations.  <member_id_list_> is useful for implementing
  // variations of <resolve> method to implement different policies.
  // <members_> is useful for doing id-based look-up.

  /// This group's id.
  ACE_CString id_;

  /**
   * Pointer to the <Object_Group_Factory> servant, which created this
   * <Object_Group> servant.  We need this pointer to be able to
   * notify the factory when this <Object_Group> is destroyed.  Upon
   * notification, the factory can update its records and release
   * resources as necessary.
   */
  Object_Group_Factory_i *my_factory_;
};


/**
 * @class Random_Object_Group
 *
 * @brief This class implements <Object_Group> idl interface with the
 * random policy for <resolve>.
 */
class Random_Object_Group : public Object_Group_i
{
public:
  /// Constructor.
  Random_Object_Group (const char *id,
                       Object_Group_Factory_i *my_factory);

  /// Destructor.
  ~Random_Object_Group (void);

  /// Returns a member object from this <Object_Group> in accordance with
  /// the "random" load balancing policy.
  CORBA::Object_ptr resolve (void);

  /**
   * Cleanup the resources associated with this <Object_Group>.
   * Subsequent calls to this <Object_Group> should fail, and its
   * <id> should become available.  <Object_Group_Factory>
   * should no longer list this <Object_Group>.
   */
  void destroy (void);
};

/**
 * @class RR_Object_Group:
 *
 * @brief This class implements <Object_Group> idl interface with the
 * round robin policy for <resolve>.
 */
class RR_Object_Group: public Object_Group_i
{
public:

  /// Constructor.
  RR_Object_Group (const char *id,
                   Object_Group_Factory_i *my_factory);

  /// Destructor.
  ~RR_Object_Group (void);

  /**
   * We need to override the implementation of <unbind> from
   * Object_Group_i to make sure <resolve>
   * works correctly.
   */
  void unbind (const char * id);

  /// Returns a member object from this <Object_Group> in accordance with
  /// the "round robin" load balancing policy.
  CORBA::Object_ptr resolve (void);

  /**
   * Cleanup the resources associated with this <Object_Group>.
   * Subsequent calls to this <Object_Group> should fail, and its
   * <id> should become available.  <Object_Group_Factory>
   * should no longer list this <Object_Group>.
   */
  void destroy (void);

private:

  /// Index into the Object_Group_i::member_id_list_: keeps track of
  /// the member_id to return on the next invocation of <resolve>.
  size_t next_;
};

#endif /* LOAD_BALANCER_I_H_  */
