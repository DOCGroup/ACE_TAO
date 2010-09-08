// $Id$

// ============================================================================
//
// = FILENAME
//    Logging_Sup.cpp
//
// = DESCRIPTION
//   Event Supplier for visualizing scheduling behavior, using arrival
//   and dispatch data logged by an event channel dispatch command object
//
// = AUTHOR
//    Chris Gill (cdgill@cs.wustl.edu)
//
//    Adapted from the DOVE simulation event supplier
//    originally
//    David Levine (levine@cs.wustl.edu) and
//    Tim Harrison (harrison@cs.wustl.edu)
//    modified
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "Logging_Sup.h"
#include "NavWeapC.h"

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#include "tao/Utils/ORB_Manager.h"

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"

#include "ace/os_include/os_ctype.h"

#if defined (ACE_OPENVMS)
// need this to circumvent link error on OpenVMS
// has to do with interference in template instantiations
// for the server build by previous compilation of
// components which are reused here without recompilation
ACE_Time_Value dum = ACE_Time_Value::zero;
#endif

static const char usage [] =
"[[-?]\n"
"                 [-O[RBport] ORB port number]\n"
"                 [-m <count> of messages to send [100]\n"
"                 [-f name of scheduler input data file]\n"
"                 [-d name of scheduler header dump file]\n"
"                 [-s to suppress data updates by EC]]\n";


Logging_Supplier::Logging_Supplier (int argc, ACE_TCHAR** argv)
: argc_(argc),
  argv_(argv),
  total_messages_(10),
  input_file_name_(0),
  update_data_ (1),
  schedule_file_name_(0)
{
  navigation_.roll = navigation_.pitch = 0;
}

Logging_Supplier::~Logging_Supplier ()
{
  this->dOVE_Supplier_.disconnect ();
}

int
Logging_Supplier::init ()
{
  this->get_options (argc_, argv_);
  return this->dOVE_Supplier_.connect ();
}

void
Logging_Supplier::start_generating_events (void)
{
  unsigned long total_sent = 0;

  // Load the scheduling data for the simulation.
  ACE_Unbounded_Queue<Schedule_Viewer_Data *> schedule_data;
  this->load_schedule_data (schedule_data);
  ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *>
    schedule_iter (schedule_data);

  if (schedule_iter.done ())
    {
      ACE_ERROR ((LM_ERROR,
                  "DOVE_Supplier::start_generating_events: "
                  "there is no scheduling data\n"));
      return;
    }

  CORBA::Any any;

  do
  {

  // Insert the event data
    this->insert_event_data (any,
                             schedule_iter);

    // deliver it over the wire
    dOVE_Supplier_.notify (any);

    if (total_sent < 5)
      ACE_DEBUG ((LM_DEBUG,
                  "Pushing event data.\n"));
    else if (total_sent == 5)
      ACE_DEBUG ((LM_DEBUG,
                  "Everything is running. Going to be mute.\n"));
  }
  while (++total_sent < this->total_messages_);

  // clean up the scheduling data
  Schedule_Viewer_Data **data_temp;
  for (schedule_iter.first ();
       schedule_iter.done () == 0;
       schedule_iter.advance ())
    if (schedule_iter.next (data_temp) && data_temp)
      delete (*data_temp);
}

void
Logging_Supplier::load_schedule_data
      (ACE_Unbounded_Queue<Schedule_Viewer_Data *> &schedule_data)
{
  Schedule_Viewer_Data *data = 0;

  if (this->input_file_name_)
    {
      // Open the scheduler data input file and read its contents into
      // a queue.
      FILE *input_file;

      int scan_count = 0;
      input_file = ACE_OS::fopen(this->input_file_name_, "r");

      if (input_file)
        {
          // Get a line at a time from the data file and parse it.
          char input_buf[BUFSIZ];
          while (ACE_OS::fgets (input_buf, BUFSIZ, input_file))
            {
              // Run through leading whitespace.
              char *temp = input_buf;
              while (*temp && ACE_OS::ace_isspace (*temp))
                ++temp;

              // If there is anything besides whitespace in the line
              // read, scan its fields into the scheduling data
              // structure.
              if (ACE_OS::strlen (temp) > 0)
                {
                  ACE_NEW (data, Schedule_Viewer_Data);
                  scan_count = sscanf (temp, "%s %lf %lf %lu %lu %lu %lu",
                                       data->operation_name,
                                       &data->utilitzation,
                                       &data->overhead,
                                       &data->arrival_time,
                                       &data->deadline_time,
                                       &data->completion_time,
                                       &data->computation_time);
                  if (scan_count != 7)
                    {
                      ACE_ERROR ((LM_ERROR,
                                  "DOVE_Supplier::start_generating_events: "
                                  "scanned incorrect number of data elements: %d\n", scan_count));

                      delete data;
                      return;
                    }

                  // Insert the data into the queue.
                  schedule_data.enqueue_tail (data);
                }
            }
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "DOVE_Supplier::start_generating_events: "
                      "could not open input file [%s].\n",
                      this->input_file_name_));
          return;
        }
    }
  else
  {
    u_long last_completion = 0;

    // Just create 10 dummy scheduling records and use them.
    for (int i = 0; i < 10; ++i)
    {
      ACE_NEW (data, Schedule_Viewer_Data);

      const char *oper_name = 0;
      switch (i % 4)
      {
      case 0:
        oper_name = "high_20";
        break;

      case 1:
        oper_name = "low_20";
        break;

      case 2:
        oper_name = "high_10";
        break;

      case 3:
      default:
        oper_name = "low_10";
        break;
      }

      ACE_OS::strncpy (data->operation_name,
                       oper_name,
                       BUFSIZ-1);

      data->utilitzation = (double)(20.0+ACE_OS::rand() %10);
      data->overhead = (double)(ACE_OS::rand() %20);

      data->arrival_time = ACE_OS::rand() % 200;
      data->computation_time = (ACE_OS::rand() % 100) + 10;

      data->completion_time = last_completion + (ACE_OS::rand() % 100) + 100;
      data->completion_time =
        data->completion_time <  data->arrival_time + data->computation_time
        ? data->arrival_time + data->computation_time
        : data->completion_time;

      last_completion = data->completion_time;

      data->deadline_time = data->completion_time + (ACE_OS::rand() % 200) - 50;

      // insert the data into the queue.
      schedule_data.enqueue_tail (data);
    }
  }
}

// This function fills in the random data into the anys transported by
// the event channel.

void
Logging_Supplier::insert_event_data (CORBA::Any &data,
                                  ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *> &schedule_iter)
{
  static u_long last_completion = 0;

  // constants for periods (in units of one hundred nanoseconds)
  const TimeBase::TimeT ONE_HZ_PERIOD = 10000000;
  const TimeBase::TimeT FIVE_HZ_PERIOD = ONE_HZ_PERIOD / 5 ;
  const TimeBase::TimeT TEN_HZ_PERIOD = ONE_HZ_PERIOD / 10;
  const TimeBase::TimeT TWENTY_HZ_PERIOD = ONE_HZ_PERIOD / 20;

  try
  {
    Schedule_Viewer_Data **sched_data;

    if ((schedule_iter.next (sched_data)) && (sched_data) && (*sched_data))
    {
      if ((ACE_OS::strcmp((*sched_data)->operation_name, "high_20") == 0) ||
           (ACE_OS::strcmp((*sched_data)->operation_name, "low_20") == 0)  ||
           (ACE_OS::strcmp((*sched_data)->operation_name, "high_1") == 0)  ||
           (ACE_OS::strcmp((*sched_data)->operation_name, "low_1") == 0))
      {
        if ((ACE_OS::strcmp((*sched_data)->operation_name, "high_20") == 0) ||
            (ACE_OS::strcmp((*sched_data)->operation_name, "high_1") == 0))
          {
            navigation_.criticality = 1;
          }
        else
          {
            navigation_.criticality = 0;
          }

        if ((ACE_OS::strcmp((*sched_data)->operation_name, "high_20") == 0) ||
            (ACE_OS::strcmp((*sched_data)->operation_name, "low_20") == 0))
          {
            navigation_.deadline_time = TWENTY_HZ_PERIOD;
          }
        else
          {
            navigation_.deadline_time = ONE_HZ_PERIOD;
          }

        navigation_.position_latitude = ACE_OS::rand() % 90;
        navigation_.position_longitude = ACE_OS::rand() % 180;
        navigation_.altitude = ACE_OS::rand() % 100;
        navigation_.heading = ACE_OS::rand() % 180;
        navigation_.roll = (navigation_.roll >= 180) ? -180 : navigation_.roll + 1;
        navigation_.pitch =  (navigation_.pitch >= 90) ? -90 : navigation_.pitch + 1;

        navigation_.utilization =       0.0;
        navigation_.overhead =          0.0;
        navigation_.arrival_time =      ORBSVCS_Time::zero ();
        navigation_.completion_time =   ORBSVCS_Time::zero ();
        navigation_.computation_time =  ORBSVCS_Time::zero ();
        navigation_.update_data =       update_data_;


        // because the scheduler data does not supply these values
        navigation_.utilization = (double) (20.0 + ACE_OS::rand() % 10);
        navigation_.overhead = (double) (ACE_OS::rand() % 10);

        data <<= navigation_;
      }
      else if ((ACE_OS::strcmp((*sched_data)->operation_name, "high_10") == 0) ||
               (ACE_OS::strcmp((*sched_data)->operation_name, "low_10") == 0)  ||
                (ACE_OS::strcmp((*sched_data)->operation_name, "high_5") == 0)  ||
                (ACE_OS::strcmp((*sched_data)->operation_name, "low_5") == 0))
      {
        if ((ACE_OS::strcmp((*sched_data)->operation_name, "high_10") == 0) ||
            (ACE_OS::strcmp((*sched_data)->operation_name, "high_5") == 0))
          {
            weapons_.criticality = 1;
          }
        else
          {
            weapons_.criticality = 0;
          }

        if ((ACE_OS::strcmp((*sched_data)->operation_name, "high_10") == 0) ||
            (ACE_OS::strcmp((*sched_data)->operation_name, "low_10") == 0))
          {
            weapons_.deadline_time = TEN_HZ_PERIOD;
          }
        else
          {
            weapons_.deadline_time = FIVE_HZ_PERIOD;
          }


        weapons_.number_of_weapons = 2;
        weapons_.weapon1_identifier = CORBA::string_alloc (30);
        ACE_OS::strcpy (weapons_.weapon1_identifier.inout (),"Photon Torpedoes");
        weapons_.weapon1_status =(ACE_OS::rand() % 4) == 0 ? 0 : 1 ;
        weapons_.weapon2_identifier = CORBA::string_alloc (30);
        ACE_OS::strcpy (weapons_.weapon2_identifier.inout (),"Quantum Torpedoes");
        weapons_.weapon2_status = (ACE_OS::rand() % 4) == 0 ? 0 : 1;
        weapons_.weapon3_identifier = CORBA::string_alloc (1);
        ACE_OS::strcpy (weapons_.weapon3_identifier.inout (), "");
        weapons_.weapon3_status = 0;
        weapons_.weapon4_identifier = CORBA::string_alloc (1);
        ACE_OS::strcpy (weapons_.weapon4_identifier.inout (), "");
        weapons_.weapon4_status = 0;
        weapons_.weapon5_identifier = CORBA::string_alloc (1);
        ACE_OS::strcpy (weapons_.weapon5_identifier.inout (), "");
        weapons_.weapon5_status = 0;
        weapons_.utilization =       0.0;
        weapons_.overhead =          0.0;
        weapons_.arrival_time =      ORBSVCS_Time::zero ();
        weapons_.completion_time  =  ORBSVCS_Time::zero ();
        weapons_.computation_time =  ORBSVCS_Time::zero ();
        weapons_.update_data =       update_data_;


        data <<= weapons_;
      }
      else {
        ACE_ERROR ((LM_ERROR,
                    "Logging_Supplier::insert_event_data:"
                    "unrecognized operation name [%s]",
                    (*sched_data)->operation_name));
      }



            if (last_completion > (*sched_data)->completion_time)
              last_completion = 0;

      if ((*sched_data)->completion_time >= last_completion)
      {
              ACE_Time_Value pause (0,
                                    (*sched_data)->completion_time -
                                      last_completion);
              ACE_OS::sleep (pause);
              last_completion = (*sched_data)->completion_time;
      }
    }
    else
      ACE_ERROR ((LM_ERROR,
                  "Logging_Supplier::insert_event_data:"
                  "Could Not access scheduling data"));

    schedule_iter.advance ();

    if (schedule_iter.done ())
      schedule_iter.first ();
  }
  catch (const CORBA::Exception&)
  {
    ACE_ERROR ((LM_ERROR,
                "(%t)Error in Logging_Supplier::insert_event_data.\n"));
  }
}

// Function get_options.
unsigned int
Logging_Supplier::get_options (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("f:m:d:s"));
  int opt;
  int temp;

  while ((opt = get_opt ()) != -1)
    {
      switch (opt)
        {
        case 'm':
          temp = ACE_OS::atoi (get_opt.opt_arg ());
          if (temp > 0)
            {
              this->total_messages_ = (u_int) temp;
               ACE_DEBUG ((LM_DEBUG,
                           "Messages to send: %d\n",
                           this->total_messages_));
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: count must be > 0",
                               argv[0]),
                               1);
          break;
        case 'f':
          this->input_file_name_ = get_opt.opt_arg ();

          if (!this->input_file_name_ || ACE_OS::strlen (this->input_file_name_) > 0)
            ACE_DEBUG ((LM_DEBUG,"Reading file!\n"));
          else
            {
              this->input_file_name_ = 0;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%s: file name must be specified with -f option",
                                 argv[0]),
                                1);
            }
          break;
        case 'd':
          this->schedule_file_name_ = get_opt.opt_arg ();

          if (!this->schedule_file_name_ || ACE_OS::strlen (this->schedule_file_name_) > 0)
            ACE_DEBUG ((LM_DEBUG,"Dumping file!\n"));
          else
            {
              this->input_file_name_ = 0;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%s: file name must be specified with -d option",
                                 argv[0]),
                                1);
            }
          break;
        case 's':
          update_data_ = 0;
          break;
        default:
        case '?':
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s %s\n",
                      argv[0], usage));
          ACE_OS::exit (0);
          break;
        }
    }

  if (argc != get_opt.opt_ind ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s: too many arguments\n"
                       "Usage: %s %s\n",
                       argv[0],
                       argv[0],
                       usage),
                      1);

  return 0;
}

// function main

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize ORB.
      TAO_ORB_Manager orb_Manager;

      orb_Manager.init (argc,
                        argv);


      // Create the demo supplier.
      Logging_Supplier *event_Supplier_ptr;

      ACE_NEW_RETURN (event_Supplier_ptr,
                      Logging_Supplier(argc, argv),
                      -1);

      // Initialize everthing
      if (event_Supplier_ptr->init () == -1)
        ACE_OS::exit (1);

      // now we can go ahead
      event_Supplier_ptr->start_generating_events ();

      // when done, we clean up
      delete event_Supplier_ptr;

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("SYS_EX");
    }

  return 0;
}

