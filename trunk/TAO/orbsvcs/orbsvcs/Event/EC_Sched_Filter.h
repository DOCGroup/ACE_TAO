/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Sched_Filter
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_SCHED_FILTER_H
#define TAO_EC_SCHED_FILTER_H

#include "orbsvcs/RtecSchedulerC.h"
#include "EC_Filter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_EC_Sched_Filter : public TAO_EC_Filter
{
  // = TITLE
  //   Decorate a filter with scheduling information
  //
  // = DESCRIPTION
  //   This filter decorates a regular filter with scheduling
  //   information. It creates a new RT_Info entry for the filter and
  //   it adds the dependencies between the filter and any childrens
  //   it may have.
  //
  // = MEMORY MANAGMENT
  //   It assumes ownership of the children.
  //
public:
  TAO_EC_Sched_Filter (const char* name,
                       RtecScheduler::Scheduler_ptr scheduler,
                       TAO_EC_Filter* body,
                       RtecScheduler::handle_t body_info,
                       RtecScheduler::Info_Type_t info_type);
  // Constructor.
  // It assumes ownership of the <body>, makes a copy of the other
  // parameters

  virtual ~TAO_EC_Sched_Filter (void);
  // Destructor

  // = The TAO_EC_Filter methods, please check the documentation in
  // TAO_EC_Filter.
  virtual ChildrenIterator begin (void) const;
  virtual ChildrenIterator end (void) const;
  virtual int size (void) const;
  virtual int filter (const RtecEventComm::EventSet& event,
                      TAO_EC_QOS_Info& qos_info,
                      CORBA::Environment& env);
  virtual int filter_nocopy (RtecEventComm::EventSet& event,
                             TAO_EC_QOS_Info& qos_info,
                             CORBA::Environment& env);
  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info,
                     CORBA::Environment& env);
  virtual void push_nocopy (RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info,
                            CORBA::Environment& env);
  virtual void clear (void);
  virtual CORBA::ULong max_event_size (void) const;
  virtual int can_match (const RtecEventComm::EventHeader& header) const;
  virtual int add_dependencies (const RtecEventComm::EventHeader& header,
                                const TAO_EC_QOS_Info &qos_info,
                                CORBA::Environment &ACE_TRY_ENV);
  virtual void get_qos_info (TAO_EC_QOS_Info& qos_info,
                             CORBA::Environment &ACE_TRY_ENV);

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Sched_Filter
                              (const TAO_EC_Sched_Filter&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Sched_Filter& operator=
                              (const TAO_EC_Sched_Filter&))

  void init_rt_info (CORBA::Environment& env);
  // Initialize our RT_Info handle and dependencies

  void compute_qos_info (TAO_EC_QOS_Info& qos_info,
                         CORBA::Environment &ACE_TRY_ENV);
  // Compute a new qos_info to push up.

private:
  RtecScheduler::handle_t rt_info_;
  // The RT_Info handle for this object

  ACE_CString name_;
  // Our operation name

  RtecScheduler::Scheduler_var scheduler_;
  // The scheduler we are going to use

  TAO_EC_Filter* body_;
  // The implementation

  RtecScheduler::handle_t body_info_;
  // The RT_Info handle for the body

  RtecScheduler::Info_Type_t info_type_;
  // Required for the scheduling service
};

#if defined (__ACE_INLINE__)
#include "EC_Sched_Filter.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_SCHED_FILTER_H */
