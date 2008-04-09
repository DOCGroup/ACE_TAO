// -*- C++ -*-

//=============================================================================
/**
*  @file    Topology_Object.h
*
*  $Id$
*
*  @author Jonathan Pollack <pollack_j@ociweb.com>
*/
//=============================================================================

#ifndef TOPOLOGY_OBJECT_H
#define TOPOLOGY_OBJECT_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/Object.h"
#include "orbsvcs/Notify/Name_Value_Pair.h"

#include "ace/SString.h"
#include "ace/Vector_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// \namespace TAO_Notify
/// \brief A namespace to be used by all of TAO's Notification Service
/// implementation.
///
/// The initial implementation used the TAO_Notify_ prefix rather than
/// a namespace.  As part of the reliable Notification Service project
/// we started using this TAO_Notify namespace,  but there are still
/// many parts of the Notification Service that are in the global
/// namespace with a TAO_NS prefix.

// @@ Wouldn't it be better to use something like
//
// namespace TAO
// {
//   namespace Notify {}
//
// }
//
namespace TAO_Notify
{
  class Topology_Saver;
  class Topology_Parent;

  /// A vector of IDS.  Used as a path from the EventChannelFactory to a proxy.
  typedef ACE_Vector <TAO_Notify_Object::ID> IdVec;

  /// \brief Interface to be implemented by savable topology objects.
  class TAO_Notify_Serv_Export Topology_Savable
  {
   public:
    /// Destructor.
    virtual ~Topology_Savable (void);

    /// Save our state to a Topology_Saver.
    ///
    /// Use the methods of a Topology_Saver to store all information we want
    /// persisted.  This function is called by our parent, which gives us a
    /// saver to use.  In turn, we must call this function on all of our
    /// children.
    /// The implementation should look like:
    ///   bool change = this->self_changed_;
    ///   this->self_changed_ = false;
    ///   this->children_changed_ = false;
    ///   if (is_persistent ())
    ///   {
    ///     bool want_all_children = saver.begin_object(
    ///       this->id(), type, attrs, change);
    ///     for all children
    ///     {
    ///       if (want_all_children || child.is_changed())
    ///       {
    ///         child.save_persistent(saver);
    ///       }
    ///     }
    ///     for all deleted children
    ///     {
    ///       saver.delete_child(child_type, child_id);
    ///     }
    ///     saver.end_object(this->id(), type);
    ///   )
    virtual void save_persistent (Topology_Saver& saver) = 0;

    /// Re-establish connections that we had before a shutdown.
    ///
    /// After a topology restore, this method is called so we can reconnect
    /// to any external objects with whom we were interacting.  We should
    /// call the reconnect() method on all of our children to give them
    /// the chance to do the same.
    virtual void reconnect (void);

  };

  /// \brief Base class for Persistent Topology Objects.
  ///
  /// Topology objects must be derived from this class to allow themselves
  /// to be persisted.
  /// Note: virtual inheritance from TopologySavable is unnecessary,
  /// but HP ACC compiler warns if it's not there.
  class TAO_Notify_Serv_Export Topology_Object :
    public virtual TAO_Notify_Object,
    public virtual Topology_Savable
  {
  public:
    /// The constructor.
    Topology_Object ();

    /// The destructor.
    virtual ~Topology_Object ();

    /// Init this object with data from <rhs>.
    virtual void initialize (Topology_Parent* topology_parent);

    /// \brief Create a child of the appropriate type and return it.
    ///
    /// Use "type" as passed in to determine what kind of child (supporting
    /// the Topology_Object interface) to create and return.  Inform it of
    /// its new ID.
    virtual Topology_Object* load_child (const ACE_CString & /*type*/,
      CORBA::Long /* id */,
      const NVPList& /* attrs */);

    /// \brief Find the id associated with topology object.
    ///
    /// A bit of a hack because id is unknown to Topology_Object
    /// the get_id returns the same thing as id -- we just need someone
    /// to find it for us.
    virtual TAO_Notify_Object::ID get_id () const;

    /// \brief Get the path of id's from the root to this object.
    void get_id_path (IdVec & id_path) const;

    /// \brief Is there an unsaved change for this object or its children?
    bool is_changed () const;

  protected:
    /// \brief Should this object be saved?
    ///
    /// This is a way for send_change() and save_persistent() to find out
    /// if this object has a persistent QoS connection property.
    /// \return true (default) if object should be saved.
    virtual bool is_persistent () const;

    /// \brief Method to report change in this object
    ///
    ///  see also Topology_Parent::child_change ()
    /// \return false if save will never happen
    bool self_change (void);

    /// \brief pointer to our topological parent
    ///
    /// \return 0 if none
    Topology_Parent * topology_parent () const;

    /// \brief Handle details of propagating change
    ///
    /// \return false if save will never happen
    bool send_change (void);

    /// \brief Handle details of propagating change
    ///  for a deleted object.
    ///
    /// \return false if save will never happen
    bool send_deletion_change ();

  private:
    /// \brief Send change to parent.
    ///
    /// Override this if you don't expect to have a parent
    /// (top level of tree)
    /// private virtual because this should only be called from send_change()
    /// \return false if save will never happen
    virtual bool change_to_parent (void);

  protected:
    /// true if this object changed since last save_persistent
    bool self_changed_;
    /// true of any of this object's children changed since last save_persistent
    bool children_changed_;

    /// A safely-typed copy of parent_;
    Topology_Parent * topology_parent_;
  };

  /// \brief Interface for topology objects that act as parents.
  ///
  /// Any topology object which contains other topology objects
  /// must implement this interface so that it's children can signal
  /// that they have changed.
  class TAO_Notify_Serv_Export Topology_Parent : public Topology_Object
  {
  public:
    /// Called by a child that has changed.
    /// A child calls this method to report that it has changed.
    /// \return false if save will never happen
    bool child_change (void);
  };

} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Topology_Object.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TOPOLOGY_OBJECT */
