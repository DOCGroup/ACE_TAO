/* -*- C++ -*- */
/**
 *  @file   Kokyu.h
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison Chris Gill,
 * Carlos O'Ryan and other members of the DOC group.
 */

#ifndef KOKYU_H
#define KOKYU_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <utility>
#include <memory>
#include "kokyu_export.h"
#include "Kokyu_defs.h"

namespace Kokyu
{
  class Dispatcher_Impl;

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
  class Kokyu_Export Dispatcher
  {
  public:
    Dispatcher ();
    Dispatcher (Dispatcher const &) = delete;
    Dispatcher (Dispatcher &&) = delete;
    Dispatcher &operator= (Dispatcher const &) = delete;
    Dispatcher &operator= (Dispatcher &&) = delete;

    /// Dispatch a command object based on the qos info supplied.
    int dispatch (Dispatch_Command const *, QoSDescriptor const &);

    /// Shut down the dispatcher. The dispatcher will stop processing requests.
    int shutdown ();

    /// Takes ownership of the implementation.
    void implementation (Dispatcher_Impl *);

    int activate ();

    /// Non virtual destructor. Read as <b><i>this class not available
    /// for inheritance<i></b>.
    ~Dispatcher ();
  private:
    /// Owns the implementation created for this dispatcher.
    std::unique_ptr<Dispatcher_Impl> dispatcher_impl_;
  };

  using Dispatcher_Auto_Ptr = std::unique_ptr<Dispatcher>;

  /**
   * @class Dispatcher_Factory
   *
   * @brief Factory class to create one of the dispatcher interface
   * objects - for events.
   *
   * Factory class creates a dispatcher for EC and configures the
   * interface object with the appropriate implementation.
   */
  class Kokyu_Export Dispatcher_Factory
  {
  public:
    Dispatcher_Factory (Dispatcher_Factory const &) = delete;
    Dispatcher_Factory (Dispatcher_Factory &&) = delete;
    Dispatcher_Factory &operator= (Dispatcher_Factory const &) = delete;
    Dispatcher_Factory &operator= (Dispatcher_Factory &&) = delete;

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
    create_dispatcher (Dispatcher_Attributes const &attr);
  };
} //end of namespace

#if defined (__ACE_INLINE__)
#include "Kokyu.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* KOKYU_H */
