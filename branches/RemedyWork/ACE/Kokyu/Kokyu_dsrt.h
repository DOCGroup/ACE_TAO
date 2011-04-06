/* -*- C++ -*- */
/**
 *  @file   Kokyu_dsrt.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 */

#ifndef KOKYU_DSRT_H
#define KOKYU_DSRT_H
#include /**/ "ace/pre.h"
#include "ace/Copy_Disabled.h"

#include "kokyu_export.h"
#include "Kokyu_defs.h"

namespace Kokyu
{

  template <class DSRT_Scheduler_Traits> class DSRT_Dispatcher_Impl;

  /**
   * @class DSRT_Dispatcher
   *
   * @brief Interface class for dynamic scheduling of threads
   *
   * The responsibility of this class is to forward all methods to
   * its delegation/implementation class, e.g.,
   * @c Default_DSRT_Dispatcher_Impl. This class follows the pImpl idiom
   * or the bridge pattern to separate the implementation from the interface.
   * DSRT_Dispatcher is the class that users will be using to achieve
   * dynamic scheduling of threads.
   */
  template <class DSRT_Scheduler_Traits>
  class DSRT_Dispatcher : private ACE_Copy_Disabled
  {
  public:
    typedef typename DSRT_Scheduler_Traits::Guid_t Guid_t;
    typedef typename DSRT_Scheduler_Traits::QoSDescriptor_t DSRT_QoSDescriptor;

    // = Scheduling methods.

    /// Schedule a thread dynamically based on the qos info supplied.
    int schedule (Guid_t guid, const DSRT_QoSDescriptor&);

    /// Update the schedule for a thread. This could alter the current schedule.
    int update_schedule (Guid_t guid, const DSRT_QoSDescriptor&);

    /// Inform the scheduler that the caller thread is about to
    /// block. This could alter the current schedule.
    int update_schedule (Guid_t guid, Kokyu::Block_Flag_t flag);

    /// Cancel the schedule for a thread. This could alter the current schedule.
    int cancel_schedule (Guid_t guid);

    /// Supply this interface with an appropriate implementation.
    void implementation (DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>*);

    // = Termination methods.

    /// Shut down the dispatcher. The dispatcher will stop processing requests.
    int shutdown ();

    /// Non virtual destructor. Read as <b><i>this class not available
    /// for inheritance<i></b>.
    ~DSRT_Dispatcher ();

  private:
    /// Auto ptr to the implementation. Implementation will be created on the
    /// heap and deleted automatically when the dispatcher object is destructed.
    auto_ptr<DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits> > dispatcher_impl_;
  };


  /**
   * @class DSRT_Dispatcher_Factory
   *
   * @brief Factory class to create one of the dispatcher interface
   * objects - for events or DSRT threads.
   *
   * Factory class creates a dispatcher or DSRT dispatcher and configures
   * the interface object with the appropriate implementation.
   */

  template <class DSRT_Scheduler_Traits>
  class DSRT_Dispatcher_Factory : private ACE_Copy_Disabled
    {
    public:
      typedef auto_ptr<DSRT_Dispatcher<DSRT_Scheduler_Traits> > DSRT_Dispatcher_Auto_Ptr;

      /**
       * Create a dispatcher for dynamic dispatching of threads.
       * This will be used to dynamic scheduling of distributable threads for
       * DSRTCORBA. The caller is responsible for freeing the memory.
       *
       * @param config Configuration information for the DSRT dispatcher.
       *
       * @return pointer to the DSRT dispatcher.
       */
      static DSRT_Dispatcher<DSRT_Scheduler_Traits>* create_DSRT_dispatcher (const DSRT_ConfigInfo&);
    };

  /**
   * @class MIF_Sched_Strategy
   *
   * @brief Strategy class implementing Maximum Importance First
   * reordering strategy.
   *
   */
  template <class QoSDesc>
  class MIF_Comparator
  {
  public:
    typedef typename QoSDesc::Importance_t Importance_t;

    int operator ()(const QoSDesc& qos1,
                    const QoSDesc& qos2);
  };

  /**
   * @class Fixed_Priority_Sched_Strategy
   *
   * @brief Strategy class implementing Fixed Priority reordering
   * strategy.
   *
   */
  template <class QoSDesc>
  class Fixed_Priority_Comparator
  {
  public:
    typedef typename QoSDesc::Priority_t Priority_t;

    int operator ()(const QoSDesc& qos1,
                    const QoSDesc& qos2);
  };

  /**
   * @class MUF_Sched_Strategy
   *
   * @brief Strategy class implementing Maximum Urgency First
   * reordering strategy.
   *
   */
  template <class QoSDesc>
  class MUF_Comparator
  {
    public:
    typedef typename QoSDesc::Criticality_t Criticality_t;
    typedef typename QoSDesc::Time_t Time_t;

    int operator ()(const QoSDesc& qos1,
                    const QoSDesc& qos2);
  };


} //end of namespace

#if defined (__ACE_INLINE__)
#include "Kokyu_dsrt.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Kokyu_dsrt.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Kokyu_dsrt.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* KOKYU_DSRT_H */
