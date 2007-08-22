// $Id$

// ============================================================================
//
// = FILENAME
//    Event_Sup.h
//
// = DESCRIPTION
//   Event Supplier for the simulation
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

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

class Event_Supplier
{
  // = TITLE
  //    Event Supplier.
  //
  // = DESCRIPTION
  //    Reads Scheduler information out of a file and
  //    feeds it with additional navigation and
  //    weapon data into the dove_supplier, which
  //    is connected to the event channel
public:

  Event_Supplier (int argc, char** argv);

  ~Event_Supplier (void);

  int init (void);
  // connect the DOVE_Supplier

  void start_generating_events (void);
  // here is really something going on,
  // here we deliver the messages

  void load_schedule_data (ACE_Unbounded_Queue<Schedule_Viewer_Data *> &schedule_data);
  // Load the scheduling information into memory


private:
  void insert_event_data (CORBA::Any &data,
                          ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *> &schedule_iter);
  // just a helper to put all the information into the CORBA::Any

  unsigned int get_options (int argc, char *argv []);

  DOVE_Supplier dOVE_Supplier_;
  Navigation navigation_;
  Weapons weapons_;

  int argc_;
  char **argv_;
  unsigned long total_messages_;
  char *input_file_name_;
};

#endif /* EVENT_SUP_H */
