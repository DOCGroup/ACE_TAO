// -*- C++ -*-

//==========================================================================
/**
 *  @file    SchedGroup.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//==========================================================================


#ifndef SCHEDGROUP_H
#define SCHEDGROUP_H
#include <time.h>
#include <sys/types.h>

extern "C" {
#include "include/group_sched/group_sched_rr2_abi.h"
#include "include/middleware/group_sched_api.h"
}

class SchedGroup;

class SchedQoS
{
protected:
  const char* scheduler_name_;
  int param_len_;
  void* param_;
  friend class SchedGroup;
};

class FrameProgressQoS : public SchedQoS
{
public:
  FrameProgressQoS();
  int progress;
};

class RR_QoS : public SchedQoS
{
public:
  RR_QoS();
};

class RR2_QoS : public SchedQoS, public sched_rr2_data
{
public:
  RR2_QoS();
};

class Seq_QoS : public SchedQoS
{
public:
  Seq_QoS();
};

class Static_Priority_QoS : public SchedQoS
{
public:
  Static_Priority_QoS();
  int priority;
};


class SchedThread
{
public:
  /**
   * Set up the current thread to join the scheduling group \c grp. The thread
   * will be removed from the group upon the destruction of the SchedThread object.
   *
   * If the thread cannot join the scheduling group, \c is_valid() will return false.
   *
   * @param grp The scheduling group that the current running thread joins.
   * @param qos The QoS value for the thread.
   * @param thread_name The name of the thread.
   */
  SchedThread(SchedGroup* grp, const SchedQoS& qos, const char* thread_name);
  ~SchedThread();

  /**
   * Returns true if the current thread has joined to a scheduling group.
   */
  bool is_valid() const;
private:
  SchedThread(const SchedThread&);
  SchedThread& operator = (const SchedThread&);
  SchedGroup* grp_;
  int member_id_;
};

class SchedGroup
{
public:
  /**
   * Create a new Scheduling Group.
   */
  SchedGroup(const char* name,
             const SchedQoS& qos);
  /**
   * Contruct a SchedGroup object that represents
   * an existing Scheduling Group with specified name.
   */
  SchedGroup(const char* name);
  ~SchedGroup();

  /**
   * Sets the group as the top level scheduling group.
   *
   * @return -1 if unsuccessful.
   */
  int set_as_top();

   /**
   * Adds the new scheduling group \c member as a member of the scheduling group.
   *
   * @return -1 if unsuccessful.
   */
  int add_member(SchedGroup& member);

private:
  friend class SchedThread;
  int add_current_thread(const SchedQoS& qos, const char* thread_name);
  int remove_current_thread(int member_id);
  SchedGroup(const SchedGroup&);
  SchedGroup& operator = (const SchedGroup&);
  char* name_;
  int id_;
  const char* parent_name_;
};

#include "SchedGroup.inl"

#endif
