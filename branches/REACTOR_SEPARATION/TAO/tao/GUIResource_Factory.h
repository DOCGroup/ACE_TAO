// -*- C++ -*-

//=============================================================================
/**
 *  @file   GUI_Resource_Factory.h
 *
 *  $Id$
 *
 *  @author Hamish Friedlander <ullexco@wave.co.nz>
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_GUI_RESOURCE_FACTORY_H
#define TAO_GUI_RESOURCE_FACTORY_H
#include /**/ "ace/pre.h"
#include /**/ "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//forwards
class ACE_Reactor_Impl;
class ACE_Reactor;

namespace TAO
{
  /** Abstract base class for the GUI resource factory, which will be held in TSS by the ORB Core.
   *
   * This base class for resource factories which provide GUIReactors integrated with specific GUI
   * event loop. In general, children of this class reimplement reactor_impl method where specific
   * GUIReactor is created. GUIResource_Factory are usually created by GUIResource_Loader
   * subclasses.
   */
  class TAO_Export GUIResource_Factory
  {
  public:
    GUIResource_Factory ();
    virtual ~GUIResource_Factory ();

    /** Create or return current reactor instance.
     * Please note that this call is NOT synchronized. Left to the
     * higher level versions to synchronize access.
     */
    virtual ACE_Reactor_Impl *reactor_impl (void) = 0;

    /** Create ACE_Reactor using allocate_reactor_impl.
     * Please note that this call is NOT synchronized. Left to the
     * higher level versions to synchronize access.
     */
    virtual ACE_Reactor *get_reactor (void) ;

    /** Reclaim the reactor if allocated by this factory.
     * Please note that this call is NOT synchronized. Left to the
     * higher level versions to synchronize access.
     */
    virtual void reclaim_reactor (ACE_Reactor *);

  private:
    /**
     * Flag that is set to 1 if the reactor obtained from the
     * get_reactor() method is dynamically allocated.  If this flag is
     * set to 1, then the reclaim_reactor() method with call the delete
     * operator on the given reactor.  This flag is necessary to make
     * sure that a reactor not allocated by the default resource factory
     * is not reclaimed by the default resource factory.  Such a
     * situation can occur when a resource factory derived from the
     * default one overrides the get_reactor() method but does not
     * override the reclaim_reactor() method.
     */
    int dynamically_allocated_reactor_;
  };
}

#include /**/ "ace/post.h"
#endif /*TAO_GUI_RESOURCE_FACTORY_H*/
