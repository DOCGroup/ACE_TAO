/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = FILENAME
//   Load_Balancer_i.h
//
// = DESCRIPTION
//    Defines classes that implement interfaces in Load_Balancer.idl
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    with modifications by Bala Natarajan <bala@cs.wustl.edu>
// ============================================================================

#ifndef LOAD_BALANCER_I_H_
#define LOAD_BALANCER_I_H_

#include "Load_BalancerS.h"
#include "ace/Hash_Map_With_Allocator_T.h"
#include "ace/SString.h"
#include "ace/Synch.h"
#include "ace/Containers.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is to remove "inherits via dominance" warnings from MSVC.
#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

typedef ACE_Allocator_Adapter <ACE_Malloc<ACE_MMAP_MEMORY_POOL,
  TAO_SYNCH_MUTEX> > ALLOCATOR;

typedef ACE_Hash_Map_With_Allocator<char *, char *> HASH_MAP;

class  Object_Group_Factory_i :
  public virtual POA_Load_Balancer::Object_Group_Factory,
  public virtual PortableServer::RefCountServantBase
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

  Object_Group_Factory_i (CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa);
  // Constructor.

  ~Object_Group_Factory_i (void);
  // Destructor.


  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException));
  // Method for the POA that will return the persistent POA_ptr stored
  // in here..

  // = Load_Balancer::Object_Group_Factory idl methods.

  Load_Balancer::Object_Group_ptr make_round_robin (const char * id
                                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::duplicate_group));
  // Creates an <Object_Group> that resolves requests for arbitrary
  // members in round robin order.  If an <Object_Group>, of any
  // type, with Group_ID <id> has already been created by this
  // factory, and hasn't been destroyed, a <duplicate_group>
  // exception is thrown.

  void unbind_round_robin (const char *id
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_group));


  Load_Balancer::Object_Group_ptr make_random (const char * id
                                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::duplicate_group));
  // Creates an <Object_Group> that resolves requests for arbitrary
  // members in random order.  If an <Object_Group>, of any
  // type, with Group_ID <id> has already been created by this
  // factory, and hasn't been destroyed, a <duplicate_group>
  // exception is thrown.

  void unbind_random (const char *id
                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_group));

  Load_Balancer::Object_Group_ptr resolve (const char * id
                                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_group));
  // Locates and returns an <Object_Group IOR> by its <Group_ID>.   If
  // no <Object_Group> has <Group_ID> of <id>, throw a
  // <no_such_group> exception.

  Load_Balancer::Group_List * round_robin_groups (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Lists all the round robin <Object_Group>s which were created
  // by this factory, and haven't been destroyed yet, i.e., return
  // a sequence of <Group_ID>s of all existing round robin
  // <Object_Group>s created by this factory.

  Load_Balancer::Group_List * random_groups (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Lists all the random <Object_Group>s which were created
  // by this factory, and haven't been destroyed yet, i.e., return
  // a sequence of <Group_ID>s of all existing random
  // <Object_Group>s created by this factory.

private:

  CORBA::ORB_var orb_;
  // Our ORB

  PortableServer::POA_var poa_;
  // Our POA

  // = Helper methods.

  Load_Balancer::Object_Group_ptr make_group (int random,
                                              const char * id
                                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::duplicate_group));
  // This function factors out common code in <make_round_robin> and
  // <make_random>.  Creates a random <Object_Group> if <random> parameter is
  // set to 1 and round robin <Object_Group> if it is 0.

  Load_Balancer::Group_List * list_groups (int random
                                           ACE_ENV_ARG_DECL);
  // This function factors out common code in <random_groups> and
  // <round_robin_groups>.  Returns a sequence of its random
  // groups if <random> parameter is set to 1 and a sequence of its
  // round robin groups if it is 0.

  void update_flags (int random
                     ACE_ENV_ARG_DECL);
  // The helper that updates the vlaue of the variable flags_

  void update_objects (ACE_ENV_SINGLE_ARG_DECL);
  // This rolls back the status of the objects in the POA if the
  // service had failed..

  HASH_MAP *random_groups_;
  // Map containing all random <Object_Group>s created by this factory.

  HASH_MAP *rr_groups_;
  // Map containing all round robin <Object_Group>s created by this factory.

  ALLOCATOR *mem_pool_;
  // Memory pool that will have the data

  CORBA::Short *flags_;
  // This would be kind of a hack.. As I am not able to think of
  // anything at present let us live with this.. OK.. Here is how it
  // works.. This value will be stored in the MMAP file. If the value
  // is 1 then the Round Robin group object is registered with the
  // Services POA. If the value is 2 then the Random group object is
  // registered with the POA. If the value is 3 both of them are
  // registered with the POA.. The initial value would be 0 when this
  // object initialises and binded as "FLAGS"..

  ACE_Throughput_Stats throughput_;

};

class Object_Group_i : public virtual POA_Load_Balancer::Object_Group,
                       public virtual PortableServer::RefCountServantBase

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

  Object_Group_i (const char * id,
                  PortableServer::POA_ptr poa);
  // Constructor.

  ~Object_Group_i (void);
  // Destructor.

  // Persistent POA
  // Method for the POA
  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // = Load_Balancer::Object_Group idl methods.

  char * id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get group's id.

  void bind (const Load_Balancer::Member & member
             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::duplicate_member));
  // Adds a new <member> to the <Object_Group>.  Note that each
  // <Member_ID> in an <Object_Group> must be unique.  If the
  // group already contains a member with the same <Member_ID>, a
  // <duplicate_member> exceptions is thrown.

  void unbind (const char * id
               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_member));
  // Removes a member with the specified <Member_ID> from the
  // <Object_Group>.  If none of the group's members have a
  // Member_ID of <id>, <no_such_member> exception is thrown.

  char * resolve (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_member)) = 0;
  // Returns a member object from this <Object_Group> in accordance with
  // load balancing policy it implements, i.e., ``random'' or
  // ``round robin.''  If the group contains no members, <no_such_member>
  // exception is thrown.

  char * resolve_with_id (const char * id
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_member));
  // Returns an object with the specified <Member_ID>.  If this
  // <Object_Group> contains no members with the specified
  // <Member_ID>, <no_such_member> exception is thrown.

  Load_Balancer::Member_ID_List * members (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return a sequence of <Member_ID>s of all of its members.

  void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cleanup the resources associated with this <Object_Group>.
  // Subsequent calls to this <Object_Group> should fail, and its
  // <id> should become available.  <Object_Group_Factory>
  // should no longer list this <Object_Group>.

protected:

  void read_from_memory (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);
  // This will replenish all the pointers that could have been lost
  // because of failure

  PortableServer::POA_var poa_;
  // Our POA

  typedef ACE_DLList<char *> LIST;
  typedef ACE_DLList_Iterator<char *> ITERATOR;
  // Typedefs for ease of use.

  LIST *member_id_list_;
  // List of ids of all the members of this group.

  HASH_MAP *members_;
  // Mapping of member_id to obj for all the members of this group.

  // Note, we store information redundantly in this implementation,
  // i.e., both <member_id_list_> and <members_> store member ids.
  // However, this redundancy eases/speeds up the implementation of
  // certain operations.  <member_id_list_> is useful for implementing
  // variations of <resolve> method to implement different policies.
  // <members_> is useful for doing id-based look-up.

  ACE_CString id_;
  // This group's id.

  ALLOCATOR *allocator_;
  // Pointer to the location where I can allocate memory...
};


class Random_Object_Group : public Object_Group_i
{
  // = TITLE
  //    This class implements <Object_Group> idl interface with the
  //    random policy for <resolve>.
  //
public:
  Random_Object_Group (const char *id,
                       PortableServer::POA_ptr poa);
  // Constructor.

  char * resolve (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_member));
  // Returns a member object from this <Object_Group> in accordance with
  // the "random" load balancing policy.

};

class RR_Object_Group: public Object_Group_i
{
  // = TITLE
  //    This class implements <Object_Group> idl interface with the
  //    round robin policy for <resolve>.
  //
public:

  RR_Object_Group (const char *id,
                   PortableServer::POA_ptr poa);
  // Constructor.

  void unbind (const char * id
               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_member));
  // We need to override the implementation of <unbind> from
  // Object_Group_i to make sure <resolve>
  // works correctly.

  char * resolve (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_member));
  // Returns a member object from this <Object_Group> in accordance with
  // the "round robin" load balancing policy.

private:

  size_t next_;
  // Index into the Object_Group_i::member_id_list_: keeps track of
  // the member_id to return on the next invocation of <resolve>.
};

#endif /* LOAD_BALANCER_I_H_  */
