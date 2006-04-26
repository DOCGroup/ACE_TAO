/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Fault_Detector_i.h
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file declares the Fault_Detector_i class.
 *  The class implements the FaultDetectors as defined
 *  in the specification.
 *  A FaultDetector monitors the health of replicas.
 *  It is *NOT* a CORBA object and does not implement an IDL interface.
 *  All CORBA interaction with a FaultDetector is via a FaultDetectorFactory.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================


#ifndef FAULT_DETECTOR_I_H_
#define FAULT_DETECTOR_I_H_
#include /**/ <ace/pre.h>
#include <ace/ACE.h>
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <ace/Manual_Event.h>


#include "orbsvcs/FT_NotifierC.h"
#include "orbsvcs/FT_ReplicaC.h"
#include "ace/Time_Value.h"

// Classes declared in this header
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  //////////////////////
  // Forward references

  class FT_FaultDetectorFactory_i;

  //////////////////////
  // Class declarations

  class Fault_Detector_i
  {
    ///////////////////
    // Public interface
  public:
    /**
     * Construct FaultDetector.
     * @param factory to be notified when this detector goes away.
     * @param id to use when notifyting the factory of detector departure.
     * @param notifier to receive the fault notification if our monitorable faults.
     * @param monitorable object to be monitored.
     * @param domain_id data to include in fault notification.
     * @param object_location data to include in fault notification.
     * @param object_type data to include in fault notification.
     * @param group_id data to include in fault notification.
     */
    Fault_Detector_i (
      FT_FaultDetectorFactory_i & factory,
      CORBA::ULong id,
      FT::FaultNotifier_ptr & notifier,
      FT::PullMonitorable_ptr & monitorable,
      FT::FTDomainId domain_id,
      const PortableGroup::Location & object_location,
      PortableGroup::TypeId object_type,
      PortableGroup::ObjectGroupId group_id
      );
    /**
     * destructor.
     * Non-virtual because this class does not take part in
     * inheritence.
     */
    ~Fault_Detector_i ();

    /**
     * Start the thread associated with this fault detector.
     * @param threadManager to track this thread.
     */
    void start(ACE_Thread_Manager & threadManager);

    /**
     * Request that this detector shut itself down.
     */
    void request_quit();

    ////////////////////////
    // Static public methods
  public:
    /**
     * Set the polling time for all FaultDetectors in this process.
     * @param value the time between polls.
     */
    static void set_time_for_all_detectors (ACE_Time_Value value);

    /////////////////////////
    // implementation methods
  private:
    /**
     * Send the notification message.
     */
    void notify();

    /**
     * The method to be run in the fault detector thread.
     */
    void run();

    /**
     * The startup function for the fault detector thread.
     */
    static ACE_THR_FUNC_RETURN thr_func (void * arg);

    ////////////////////
    // Forbidden methods
  private:
    Fault_Detector_i ();
    Fault_Detector_i (Fault_Detector_i & rhs);
    Fault_Detector_i & operator = (const Fault_Detector_i & rhs);

    ///////////////
    // Static data
  private:
    /**
     * Time between polls for all fault detectors in this process.
     */
    static ACE_Time_Value sleep_time_;

    ///////////////
    // Data members
  private:
    /**
     * The factory that "owns" us."
     */
    FT_FaultDetectorFactory_i & factory_;

    /**
     * How the factory knows who we are.
     */
    CORBA::ULong id_;

    /**
     * Where to send fault notification messages.
     */
    FT::FaultNotifier_var notifier_;

    /**
     * What to monitor.
     */
    FT::PullMonitorable_var monitorable_;

    /**
     * Data for the notification message.
     */
    FT::FTDomainId domain_id_;
    PortableGroup::Location object_location_;
    PortableGroup::TypeId object_type_;
    PortableGroup::ObjectGroupId group_id_;

    /**
     * An Event (in the Win32 sense) to implement interruptable sleep.
     * Manual rather than auto because once quit is requested we never
     * want to sleep again.
     */
    ACE_Manual_Event sleep_;

    /**
     * A boolean flag.  If true, this fault detector should leave.
     */
    int quit_requested_;
  };
}   // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ <ace/post.h>
#endif // FAULT_DETECTOR_I_H_
