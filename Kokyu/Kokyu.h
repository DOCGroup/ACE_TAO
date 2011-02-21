/* -*- C++ -*- */
/**
 *  @file   Kokyu.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison Chris Gill,
 * Carlos O'Ryan and other members of the DOC group.
 */

#ifndef KOKYU_H
#define KOKYU_H
#include /**/ "ace/pre.h"
#include "ace/Copy_Disabled.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "kokyu_export.h"
#include "Kokyu_defs.h"

//Currently I am not seeing a way to avoid including these here. The
//whole purpose of the pImpl idiom is to avoid this dependency. But
//using the auto_ptr<> to store the implementation causes a compile
//error (in the destructor) that the implementation definition is not
//found. Note that the auto-ptr<T>::~auto_ptr() calls delete on the
//internal pointer and at this point the class definition needs to be
//visible. Need to revisit this and see whether there is a work
//around.
#include "Dispatcher_Impl.h"

//#################################################################
//Beware that this interface will be subject to change in the future
//since this is the very initial release. We will be working on
//trying to merge the DSRT and EC mechanisms in the future. If you
//are a user of this interface, kindly let us know so that we can
//coordinate with you when we refactor this interface.
//##################################################################

namespace Kokyu
{
  //class Dispatcher_Impl;

  /**
   * @class Dispatcher
   *
   * @brief Interface class for dynamic scheduling of events
   *
   * The responsibility of this class is to forward all methods to
   * its delegation/implementation class, e.g.,
   * @c Default_Dispatcher_Impl. This class follows the pImpl idiom
   * or the bridge pattern to separate the implementation from the interface.
   * Dispatcher is the class that users will be using to achieve
   * dynamic dispatching of events in an event channel.
   */
  class Kokyu_Export Dispatcher : private ACE_Copy_Disabled
  {
  public:
    /// Dispatch a command object based on the qos info supplied.
    int dispatch (const Dispatch_Command*, const QoSDescriptor&);

    /// Shut down the dispatcher. The dispatcher will stop processing requests.
    int shutdown ();

    /// Supply this interface with an appripriate implementation.
    void implementation (Dispatcher_Impl*);

    int activate ();

    /// Non virtual destructor. Read as <b><i>this class not available
    /// for inheritance<i></b>.
    ~Dispatcher ();
  private:
    /// Auto ptr to the implementation. Implementation will be created on the
    /// heap and deleted automatically when the dispatcher object is destructed.
    auto_ptr<Dispatcher_Impl> dispatcher_impl_;
  };

  typedef auto_ptr<Dispatcher> Dispatcher_Auto_Ptr;

  /**
   * @class Dispatcher_Factory
   *
   * @brief Factory class to create one of the dispatcher interface
   * objects - for events.
   *
   * Factory class creates a dispatcher for EC and configures the
   * interface object with the appropriate implementation.
   */
  class Kokyu_Export Dispatcher_Factory : private ACE_Copy_Disabled
    {
    public:
      /**
       * Create a dispatcher for dynamic dispatching of commands
       * (eg. events). The caller is responsible for freeing the
       * returned dynamically allocated memory.
       *
       * @param config Configuration information for the dispatcher.
       *
       * @return pointer to the dispatcher.
       */
      static Dispatcher*
      create_dispatcher (const Dispatcher_Attributes& attr);
    };
} //end of namespace

#if defined (__ACE_INLINE__)
#include "Kokyu.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* KOKYU_H */
