// -*- C++ -*-

#ifndef _CUTS_PORT_AGENT_H_
#define _CUTS_PORT_AGENT_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "cuts/ActivationRecord.h"
#include "cuts/Port_Measurement_Pool.h"
#include "ace/Condition_Thread_Mutex.h"
#include "ace/Message_Queue_T.h"
#include "ace/Free_List.h"

#include <string>
#include <map>

//=============================================================================
/**
 * @class CUTS_Port_Agent
 */
//=============================================================================

class CUTS_Export CUTS_Port_Agent
{
public:
  /// Constructor.
  CUTS_Port_Agent (void);

  /// Destructor.
  virtual ~CUTS_Port_Agent (void);

  /// Set the name of the port agent.
  void name (const char * name);

  /// Get the name of the port agent.
  const char * name (void) const;

  /// Activate the port agent.
  virtual void activate (void);

  /// Deactivate the port agent.
  virtual void deactivate (void);

  /**
   * Create a new activation record from the port agent. This is
   * a factory method for all port agents.
   *
   * @return    Pointer to a clean activation record.
   */
  CUTS_Activation_Record * create_activation_record (void);

  /**
   * Close an activation record. This causes the benchmark agent
   * to consolidate the record contents into a single location.
   */
  void destroy_activation_record (CUTS_Activation_Record * record);

  /**
   * Get the current port measurement map. This will call the
   * port agent to switch to a new map for metrics collection.
   *
   * @return      Reference to the lastest port measurement map.
   */
  CUTS_Port_Measurement_Map & port_measurements (void);

protected:
  /// Service handler for the port agent.
  static ACE_THR_FUNC_RETURN event_loop (void * param);

  /// Name of the port.
  std::string name_;

  /// The active state of the port agent.
  bool active_;

  /// Collection of free activation records.
  ACE_Locked_Free_List <
    CUTS_Cached_Activation_Record,
    ACE_SYNCH_RW_MUTEX> free_list_;

  /// Collection of port measurements used by the port agent.
  CUTS_Port_Measurement_Pool measurement_pool_;

private:
  /// Helper method to recored the methics in a record.
  void record_metrics (CUTS_Activation_Record * record);

  /// Helper method to add an activation record to the free list.
  void add_to_free_list (CUTS_Activation_Record * record);

  ACE_Message_Queue_Ex <CUTS_Activation_Record,
                        ACE_MT_SYNCH> closed_list_;

  /// The group id for the port agent's threads.
  long grp_id_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/PortAgent.inl"
#endif

#endif  // !defined _CUTS_PORT_AGENT_H_
