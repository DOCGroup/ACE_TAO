// -*- C++ -*-

//=============================================================================
/**
 *  @file    Profile_Transport_Resolver.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_PROFILE_TRANSPORT_RESOLVER_H
#define TAO_PROFILE_TRANSPORT_RESOLVER_H

#include /**/ "ace/pre.h"
#include "ace/Global_Macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Transport_Selection_Guard.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Time_Value;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Stub;
class TAO_Profile;
class TAO_Transport;
class TAO_Endpoint;
class TAO_Transport_Descriptor_Interface;

namespace CORBA
{
  class Object;
  class PolicyList;

  class Object;
  typedef Object *Object_ptr;
}

namespace TAO
{
  class Synch_Twoway_Invocation;

  /**
   * @class Profile_Transport_Resolver
   *
   * @brief Chooses the profile and a transport for a target object on
   * which an invocation can be made.
   *
   * A target object is represented by multiple profiles, which
   * theoretically provides multiple paths to the same target
   * object. This class helps in choosing the right profile, and pick
   * a transport from cache (or create a new transport if needed) that
   * represents the profile.
   */
  class TAO_Export Profile_Transport_Resolver
  {
  public:
    /// Constructor
    /**
     * With @a block we tell whether this resolved should always deliver
     * a connection by blocking or unblock before the connection is
     * completely established. Please note that this has *nothing* to
     * do with the synchronous or asynch connect strategy used for
     * making connections. This is a local flag used by the clients of
     * this to dictate some local behavior.
     */
    Profile_Transport_Resolver (CORBA::Object *p,
                                TAO_Stub *stub,
                                bool block = true);

    ~Profile_Transport_Resolver (void);

    /// Method where the bunch of the action takes place.
    /**
     * This is the entry point from the TAO::Invocation_Adapter
     * class. This method accesses the endpoint selector factory from
     * the ORB_Core to decide on the strategy to be used for selecting
     * the profile.
     */
    void resolve (ACE_Time_Value *val);

    //@{
    /**
     * @name Accessors and mutators for this class. The following methods
     * are used by the clients of this class to access strategies and
     * other internal workings.
     */

    /// Mutator for profile.
    void profile (TAO_Profile *pfile);

    /// Accessor for profile.
    TAO_Profile *profile (void) const;

    /// Accessor for TAO_Stub
    TAO_Stub *stub (void) const;

    /// Accessor for the target in use
    CORBA::Object *object (void) const;

    /// Accessor for the transport reserved for this invocation.
    TAO_Transport *transport (void) const;

    /// See if the transport cache has an available transport and
    /// use that one rather than trying to connect via the connector.
    /// Separating this functionality enables the look up of many
    /// endpoints before trying the more time-consuming trip through
    /// the actual connector.
    int find_transport (TAO_Transport_Descriptor_Interface *);

    /// Accessor to indicate whether we should block while
    /// establishing a connection.
    bool blocked_connect (void) const;
    //@}

    /// Signal to let the resolver know that the transport has been
    /// released back to the cache.
    void transport_released (void) const;

    /// This is a callback method used by the endpoint selectors to
    /// delegate the responsibility of reserving a transport from the
    /// connection cache for this invocation. When the descriptor
    /// contains more than one endpoint (as part of a linked list) and
    /// the parallel flag is true then the connector will look for a
    /// connection on any of the endpoints if it supports that
    /// behavior, otherwise an ENOTSUP errno will be set and the
    /// method will return false.
    bool try_connect (TAO_Transport_Descriptor_Interface *desc,
                      ACE_Time_Value *val);

    bool try_parallel_connect (TAO_Transport_Descriptor_Interface *desc,
                               ACE_Time_Value *val);

    /// This method wraps a call to the orb core to see if parallel
    /// connection attempts are even desired. This is controlled by
    /// the -ORBUseParallelConnects 1|0 commandline option.
    bool use_parallel_connect (void) const;

    /// Initialize the inconsistent policy list that this object has
    /// cached.
    void init_inconsistent_policies (void);

    CORBA::PolicyList *inconsistent_policies (void) const;

    CORBA::PolicyList *steal_inconsistent_policies (void);
  private:

    /// Helper method to access get the connection timeout from the
    /// ORB.
    bool get_connection_timeout (ACE_Time_Value &max_wait_time);

    void operator= (const Profile_Transport_Resolver &);
    Profile_Transport_Resolver (const Profile_Transport_Resolver &);

  private:
    bool try_connect_i (TAO_Transport_Descriptor_Interface *desc,
                        ACE_Time_Value *val,
                        bool parallel);

    /// Target object
    mutable CORBA::Object *obj_;

    /// Stub object for the target object.
    TAO_Stub *stub_;

    /// The transport selected for this invocation.

    /// Using the wrapper guard ensures it is available for use with
    /// the Transport Current interfaces.
    TAO::Transport_Selection_Guard transport_;

    /// The profile that has been selected for this invocation.
    TAO_Profile *profile_;

    /// List of inconsistent policies
    /**
     * If current effective policies cause the invocation to raise
     * CORBA::INV_POLICY exception, the conflicting/problematic policies
     * are stored in this list.  This is used by \param
     * Object::_validate_connection method to inform clients about
     * causes of invocation failure.
     * @par
     * Conflicting policies are only stored in this list if \param
     * init_inconsistent_policies method has been called prior to the
     * beginning of invocation.  This saves extra work of conflicting
     * policies 'logging' when it's not needed.
     *
     * @note We don't use _var with a reason. Using _var would
     * involve including the  header file for at least
     * Policy_ForwardC.h, and that is what we precisely want to
     * avoid.
     */
    CORBA::PolicyList *inconsistent_policies_;

    /// Has the transport been idle?
    mutable bool is_released_;

    /// Should we block while trying to make a connection
    bool const blocked_;
  };
} // TAO namespace end

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Profile_Transport_Resolver.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /*TAO_PROFILE_TRANSPORT_RESOLVER_H*/
