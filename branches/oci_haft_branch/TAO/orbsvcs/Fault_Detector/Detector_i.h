// $Id$
//=============================================================================
/**
 *  @file    Detector_i.h
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file declares the Detector_i class.
 *  The class implements the FaultDetectors as defined
 *  in the specification.
 *  A FaultDetector monitors the health of replicas.
 *  It is *NOT* a CORBA object and does not implement an IDL interface.
 *  All CORBA interaction with a FaultDetector is via a FaultDetectorFactory.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================


#ifndef FT_DETECTOR_I_H_
#define FT_DETECTOR_I_H_
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/FT_NotifierC.h"
#include "orbsvcs/FT_ReplicaC.h"
#include "ace/Time_Value.h"

//////////////////////
// Forward references

class FT_FaultDetectorFactory_i;

//////////////////////
// Class declarations

class Detector_i
{
  ///////////////////
  // Public interface
public:
  Detector_i (
    FT_FaultDetectorFactory_i & factory,
    CORBA::ULong id,
    FT::FaultNotifier_var & notifier,
    FT::PullMonitorable_var & monitorable,
    FT::FTDomainId domain_id,
    FT::ObjectGroupId group_id,
    FT::TypeId object_type,
    FT::Location object_location
    );
  ~Detector_i ();

  void start(ACE_Thread_Manager & threadManager);

  void requestQuit();

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
  FT_FaultDetectorFactory_i & factory_;
  CORBA::ULong id_;
  FT::FaultNotifier_var notifier_;
  FT::PullMonitorable_var monitorable_;

  FT::FTDomainId domain_id_;
  FT::Location object_location_;
  FT::TypeId object_type_;
  FT::ObjectGroupId group_id_;

  /**
   * an Event (in the Win32 sense) to implement interruptable sleep.
   */
  ACE_Manual_Event sleep_;

  int quitRequested_;
};

#endif // FT_DETECTOR_I_H_
