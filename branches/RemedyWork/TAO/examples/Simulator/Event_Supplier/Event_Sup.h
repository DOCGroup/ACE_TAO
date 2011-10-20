
//=============================================================================
/**
 *  @file    Event_Sup.h
 *
 *  $Id$
 *
 * Event Supplier for the simulation
 *
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#ifndef EVENT_SUP_H
#define EVENT_SUP_H

//#include <ctype.h>
//#include <stdio.h>
#include "NavWeapC.h"
#include "DOVE_Supplier.h"

struct Schedule_Viewer_Data
{
  char operation_name [BUFSIZ];
  double utilitzation;
  double overhead;
  u_long arrival_time;
  u_long deadline_time;
  u_long completion_time;
  u_long computation_time;
};

/**
 * @class Event_Supplier
 *
 * @brief Event Supplier.
 *
 * Reads Scheduler information out of a file and
 * feeds it with additional navigation and
 * weapon data into the dove_supplier, which
 * is connected to the event channel
 */
class Event_Supplier
{
public:

  Event_Supplier (int argc, ACE_TCHAR** argv);

  ~Event_Supplier (void);

  /// connect the DOVE_Supplier
  int init (void);

  /// here is really something going on,
  /// here we deliver the messages
  void start_generating_events (void);

  /// Load the scheduling information into memory
  void load_schedule_data (ACE_Unbounded_Queue<Schedule_Viewer_Data *> &schedule_data);


private:
  /// just a helper to put all the information into the CORBA::Any
  void insert_event_data (CORBA::Any &data,
                          ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *> &schedule_iter);

  unsigned int get_options (int argc, ACE_TCHAR *argv []);

  DOVE_Supplier dOVE_Supplier_;
  Navigation navigation_;
  Weapons weapons_;

  int argc_;
  ACE_TCHAR **argv_;
  unsigned long total_messages_;
  ACE_TCHAR *input_file_name_;
};

#endif /* EVENT_SUP_H */
