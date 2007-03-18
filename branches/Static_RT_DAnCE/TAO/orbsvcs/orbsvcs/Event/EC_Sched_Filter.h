// -*- C++ -*-
/**
 *  @file   EC_Sched_Filter.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_SCHED_FILTER_H
#define TAO_EC_SCHED_FILTER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/Event/sched_event_export.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_EC_Sched_Filter
 *
 * @brief Decorate a filter with scheduling information
 *
 * This filter decorates a regular filter with scheduling
 * information. It creates a new RT_Info entry for the filter and
 * it adds the dependencies between the filter and any childrens
 * it may have.
 *
 * <H2>Memory Management</H2>
 * It assumes ownership of the children.
 */
class TAO_RTSchedEvent_Export TAO_EC_Sched_Filter : public TAO_EC_Filter
{
public:
  /**
   * Constructor.
   * It assumes ownership of the <body>, makes a copy of the other
   * parameters
   */
  TAO_EC_Sched_Filter (const char* name,
                       RtecScheduler::handle_t rt_info,
                       RtecScheduler::Scheduler_ptr scheduler,
                       TAO_EC_Filter* body,
                       RtecScheduler::handle_t body_info,
                       RtecScheduler::handle_t parent_info,
                       RtecScheduler::Info_Type_t info_type);

  /// Destructor
  virtual ~TAO_EC_Sched_Filter (void);

  // = The TAO_EC_Filter methods, please check the documentation in
  // TAO_EC_Filter.
  virtual ChildrenIterator begin (void) const;
  virtual ChildrenIterator end (void) const;
  virtual int size (void) const;
  virtual int filter (const RtecEventComm::EventSet& event,
                      TAO_EC_QOS_Info& qos_info);
  virtual int filter_nocopy (RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info);
  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info);
  virtual void push_nocopy (RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info);
  virtual void clear (void);
  virtual CORBA::ULong max_event_size (void) const;
  virtual int can_match (const RtecEventComm::EventHeader& header) const;
  virtual int add_dependencies (const RtecEventComm::EventHeader& header,
                                const TAO_EC_QOS_Info &qos_info);
  virtual void get_qos_info (TAO_EC_QOS_Info& qos_info);

private:
  TAO_EC_Sched_Filter (const TAO_EC_Sched_Filter&);
  TAO_EC_Sched_Filter& operator= (const TAO_EC_Sched_Filter&);

  /// Initialize our RT_Info handle and dependencies
  void init_rt_info (void);

  /// Compute a new qos_info to push up.
  void compute_qos_info (TAO_EC_QOS_Info& qos_info);

private:
  /// The RT_Info handle for this object
  RtecScheduler::handle_t rt_info_;

  /// Has the Scheduler been updated?
  int rt_info_computed_;

  /// Our operation name
  ACE_CString name_;

  /// The scheduler we are going to use
  RtecScheduler::Scheduler_var scheduler_;

  /// The implementation
  TAO_EC_Filter* body_;

  /// The RT_Info handle for the body
  RtecScheduler::handle_t body_info_;

  /// The RT_Info handle for the parent
  RtecScheduler::handle_t parent_info_;

  /// Required for the scheduling service
  RtecScheduler::Info_Type_t info_type_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_EC_SCHED_FILTER_H */
