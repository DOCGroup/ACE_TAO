// -*- C++ -*-

//=============================================================================
/**
*  @file    Topology_Saver.h
*
*  $Id$
*
*  @author Jonathan Pollack <pollack_j@ociweb.com>
*/
//=============================================================================

#ifndef TOPOLOGY_SAVER_H
#define TOPOLOGY_SAVER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/Topology_Object.h"
#include "orbsvcs/Notify/notify_serv_export.h"

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  /**
  * \brief An interface to be implemented by objects that save Topology.
  *
  * A persistent topology store must provide an implemention this interface.
  *
  */
  class TAO_Notify_Serv_Export Topology_Saver
  {
  public:

    /// The destructor.
    virtual ~Topology_Saver ();

    /** \brief Begin the storage of an object.
    *
    * Call this function with the type and ID of an object to be stored.
    * This object may have children, and this will necessitate nested calls
    * to begin_object.
    * Design principle:
    *   Names should be descriptive enough to allow the objects' parent to create
    *   an instance of the desired class.  This instance will be registered
    *   with the poa using the id.
    *   The instance itself should will load its own attributes.
    *    Example <proxy type="push_supplier" events="any" id="20"...> is not a
    *    good design because the name "proxy" is not descriptive enough.
    *    "<structured_proxy_push_supplier id="20"...> is better because this
    *    allows the parent to create the correct type of object without decoding
    *    attributes.
    * \param id numeric id for this object
    * \param type string containing the unique type name for this class of objects
    * \param attrs a collection of name/value attributes
    * \param change true if this object's attributes have changed.
    * \return bool want_all_children.  If true even changed children should be saved.
    */
    virtual bool begin_object (CORBA::Long id,
      const ACE_CString &type,
      const NVPList& attrs,
      bool changed) = 0;

    /** \brief Report deleted children to the saver.
    *
    * Use the ID and "type" as passed in to determine which child we should
    * delete.  A parent should call this function when one of its children
    * is deleted.
    * \param id numeric id for the deleted child
    * \param type the type name for the class of the deleted child.
    *
    */
    virtual void delete_child (
      CORBA::Long id,
      const ACE_CString & type)
    {
      ACE_UNUSED_ARG (id);
      ACE_UNUSED_ARG (type);
    }

    /** \brief End the storage of an object.
    *
    * This function should be called to end the scope of the current object
    * and commit it to the persistent store.
    */
    virtual void end_object (CORBA::Long id,
      const ACE_CString &type) = 0;

    /**
     * \brief Close the saver.
     *
     * This is not pure virtual.  The default implementation does nothing.
     *
     * There should be a corresponding open, but the signature may
     * vary based on the type of saver, so we can't include it in the
     * interface.
     */
    virtual void close (void);
  };
} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TOPOLOGY_SAVER */
