// $Id$
#ifndef FT_DETECTOR_I_H_
#define FT_DETECTOR_I_H_
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include <orbsvcs/FT_NotifierC.h>
#include <orbsvcs/FT_ReplicaC.h>
#include <ace/Time_Value.h>

class Detector_i
{
  ///////////////////
  // Public interface
public:
  Detector_i (
    FT::FaultNotifier_var & notifier,
    FT::PullMonitorable_var & monitorable,
    FT::FTDomainId domain_id,
    FT::ObjectGroupId group_id,
    FT::Location object_location,
    FT::TypeId object_type
    );
  ~Detector_i ();

  void start(ACE_Thread_Manager & threadManager);

  ////////////////////////
  // Static public methods
public:
  static void setTimeValue (ACE_Time_Value value);

  /////////////////////////
  // implementation methods
private:
  void notify();

  void run();
  static ACE_THR_FUNC_RETURN thr_func (void * arg);


  ////////////////////
  // Forbidden methods
private:
  Detector_i ();
  Detector_i (Detector_i & rhs);
  Detector_i & operator = (const Detector_i & rhs);

  ///////////////
  // Static data
private:
  static ACE_Time_Value sleepTime_;

  ///////////////
  // Data members
private:
  FT::FaultNotifier_var notifier_;
  FT::PullMonitorable_var monitorable_;

  FT::FTDomainId domain_id_;
  FT::ObjectGroupId group_id_;
  FT::Location object_location_;
  FT::TypeId object_type_;

  int quitRequested_;
};

#endif // FT_DETECTOR_I_H_
