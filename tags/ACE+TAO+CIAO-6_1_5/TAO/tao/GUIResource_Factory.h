// -*- C++ -*-

//=============================================================================
/**
 *  @file   GUIResource_Factory.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 *  @author Marek Brudka <mbrudka@aster.pl>
 */
//=============================================================================

#ifndef TAO_GUI_RESOURCE_FACTORY_H
#define TAO_GUI_RESOURCE_FACTORY_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#include "ace/Thread_Mutex.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor_Impl;
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /** Abstract base class for the GUI resource factory.
   *
   * This base class for resource factories which provide
   * GUIReactors integrated with specific GUI
   * event loop. In general, children of this class reimplement
   * reactor_impl method where specific
   * GUIReactor is created. GUIResource_Factory are usually
   * created by GUIResource_Loader subclasses.
   * Children of this class will be held in TSS by the ORB Core.
   */
  class TAO_Export GUIResource_Factory
  {
  public:

    GUIResource_Factory ();

    virtual ~GUIResource_Factory ();

    /** Create ACE_Reactor using allocate_reactor_impl.
     * Please note that this call is NOT synchronized. Left to the
     * higher level versions to synchronize access.
     */
    virtual ACE_Reactor *get_reactor (void) ;

    /**
     * Reclaim the reactor if allocated by this factory.
     * Please note that this call is NOT synchronized. Left to the
     * higher level versions to synchronize access.
     */
    virtual void reclaim_reactor (ACE_Reactor *);

  protected:

    /**
     * Create or return current reactor instance.
     * Please note that this call is NOT synchronized. Left to the
     * get_reactor() to synchronize access.
     */
    virtual ACE_Reactor_Impl *reactor_impl (void) = 0;

  private:

    /**
     * Flag that is set to true if the reactor obtained from the
     * get_reactor() method is dynamically allocated.  If this flag is
     * set to true, then the reclaim_reactor() method with call the delete
     * operator on the given reactor.  This flag is necessary to make
     * sure that a reactor not allocated by the default resource factory
     * is not reclaimed by the default resource factory.  Such a
     * situation can occur when a resource factory derived from the
     * default one overrides the get_reactor() method but does not
     * override the reclaim_reactor() method.
     */
    bool dynamically_allocated_reactor_;

    /// for internal locking.
    TAO_SYNCH_MUTEX lock_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_GUI_RESOURCE_FACTORY_H */
