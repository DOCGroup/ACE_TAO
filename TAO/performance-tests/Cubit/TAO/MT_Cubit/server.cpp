
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  @author Andy Gokhale
 *  @author Sumedh Mungee
 *  @author Sergio Flores-Gaitan
 *  @author and Nagarajan Surendran
 */
//=============================================================================


#include "ace/config-all.h"

#if defined (ACE_VXWORKS) && !defined (__RTP__)
# undef ACE_MAIN
# define ACE_MAIN server
#endif /* ACE_VXWORKS && !__RTP__ */

#include "server.h"
#include "ace/OS_NS_string.h"
#include "ace/Sched_Params.h"
#include "tao/Strategies/advanced_resource.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Barrier.h"

#if defined (ACE_HAS_QUANTIFY)
# include "quantify.h"
#endif /* ACE_HAS_QUANTIFY */

Server::Server (void)
  :argc_ (0),
   argv_ (0),
   cubits_ (0),
   high_priority_task_ (0),
   low_priority_tasks_ (0),
   high_argv_ (0),
   low_argv_ (0)
{
}

int
Server::init (int argc, ACE_TCHAR **argv)
{
  int result;

  result = GLOBALS::instance ()->sched_fifo_init ();
  if (result == -1)
    return result;

  this->argc_ = argc;
  this->argv_ = argv;

  VX_VME_INIT;
  FORCE_ARGV (this->argc_,this->argv_);
  // Make sure we've got plenty of socket handles.  This call will use
  // the default maximum.
  ACE::set_handle_limit ();
  return 0;
}

int
Server::run (void)
{
  STOP_QUANTIFY;
  CLEAR_QUANTIFY;
  START_QUANTIFY;

  if (this->start_servants () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error creating the servants\n"),
                      -1);
  ACE_DEBUG ((LM_DEBUG,
              "Wait for all the threads to exit\n"));
  // Wait for all the threads to exit.
  this->servant_manager_.wait ();
  STOP_QUANTIFY;
  return 0;
}

void
Server::prelim_args_process (void)
{
  int i;

  for (i = 0; i < this->argc_ ; i++)
    {
      if (ACE_OS::strcmp (this->argv_[i], ACE_TEXT("-e")) == 0 &&
          i - 1 < this->argc_)
        ACE_OS::strcpy (GLOBALS::instance ()->endpoint,
                        this->argv_[i+1]);
    }
}

void
Server::init_low_priority (void)
{
  ACE_Sched_Priority prev_priority = this->high_priority_;

  // Drop the priority.
  if (GLOBALS::instance ()->thread_per_rate == 1
      || GLOBALS::instance ()->use_multiple_priority == 1)
    this->low_priority_ =
      this->priority_.get_low_priority (this->num_low_priority_,
                                        prev_priority,
                                        1);
  else
    this->low_priority_ =
      this->priority_.get_low_priority (this->num_low_priority_,
                                        prev_priority,
                                        0);

  this->num_priorities_ = this->priority_.number_of_priorities ();
  this->grain_ = this->priority_.grain ();
  this->counter_ = 0;
}

// Write the ior's to a file so the client can read them.

int
Server::write_iors (void)
{
  u_int j;

  // By this time the num of objs should be set properly.
  ACE_NEW_RETURN (this->cubits_,
                  char* [GLOBALS::instance ()->num_of_objs],
                  -1);

  this->cubits_[0] = ACE_OS::strdup (this->high_priority_task_->get_servant_ior (0));

  for (j = 1;
       j < GLOBALS::instance ()->num_of_objs;
       ++j)
    this->cubits_[j] =
      ACE_OS::strdup (this->low_priority_tasks_[j-1]->get_servant_ior (0));

  FILE *ior_f = 0;

  if (GLOBALS::instance ()->ior_file != 0)
    ior_f = ACE_OS::fopen (GLOBALS::instance ()->ior_file,
                           "w");

  for (j = 0;
       j < GLOBALS::instance ()->num_of_objs;
       ++j)
    {
      if (ior_f != 0)
        {
          ACE_OS::fprintf (ior_f,
                           "%s\n",
                           this->cubits_[j]);
          ACE_DEBUG ((LM_DEBUG,
                      "this->cubits_[%d] ior = %s\n",
                      j,
                      this->cubits_[j]));
        }
    }

  if (ior_f != 0)
    ACE_OS::fclose (ior_f);

  return 0;
}

int
Server::activate_high_servant (void)
{
  ACE_TCHAR orbendpoint[BUFSIZ];

  ACE_OS::strcpy (orbendpoint, ACE_TEXT ("-ORBEndpoint "));
  ACE_OS::strcat (orbendpoint, GLOBALS::instance ()->endpoint);

  ACE_TCHAR *high_second_argv[] =
    { orbendpoint,
      const_cast<ACE_TCHAR *> (ACE_TEXT ("-ORBSndSock 32768 ")),
      const_cast<ACE_TCHAR *> (ACE_TEXT ("-ORBRcvSock 32768 ")),
      0
    };
  ACE_NEW_RETURN (this->high_argv_,
                  ACE_ARGV (this->argv_, high_second_argv),
                  -1);
  ACE_NEW_RETURN (this->high_priority_task_,
                  Cubit_Task (this->high_argv_->buf (),
                              "internet",
                              1,
                              &this->servant_manager_,
                              0), //task id 0.
                  -1);

  // Make the high priority task an active object.
  if (this->high_priority_task_->activate
      (GLOBALS::instance ()->thr_create_flags,
       1,
       0,
       this->high_priority_) == -1)
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("(%P|%t) task activation at priority %d ")
                ACE_TEXT (" failed, exiting!\n%a"),
                this->high_priority_,
                -1));

  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                            ready_mon,
                            GLOBALS::instance ()->ready_mtx_,
                            -1));

  // Wait on the condition variable for the high priority client to
  // finish parsing the arguments.

  while (!GLOBALS::instance ()->ready_)
    GLOBALS::instance ()->ready_cnd_.wait ();

  // Default return success.
  return 0;
}

int
Server::activate_low_servants (void)
{
  if (static_cast<int> (this->num_low_priority_) > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Creating %d servants starting at priority %d\n",
                  this->num_low_priority_,
                  this->low_priority_));

      // Create the low priority servants.
      ACE_NEW_RETURN (this->low_priority_tasks_,
                      Cubit_Task *[GLOBALS::instance ()->num_of_objs],
                      -1);
    }

  // Strip the provided addr from the endpoint and make the ORB
  // choose the remaining endpoints based on the protocol used in
  // the endpoint.
  //
  // e.g.: orignal endpoint:    iiop://foobar:10001
  //       random endpoint      iiop://

  const ACE_TCHAR protocol_delimiter[] = ACE_TEXT("://");

  ACE_TCHAR *addr = ACE_OS::strstr (GLOBALS::instance ()->endpoint,
                                    protocol_delimiter);

  size_t random_endpoint_length =
    ACE_OS::strlen (GLOBALS::instance ()->endpoint) -
    ACE_OS::strlen (addr) +
    ACE_OS::strlen (protocol_delimiter);

  ACE_TCHAR random_endpoint[BUFSIZ];

  ACE_OS::sprintf (random_endpoint, ACE_TEXT("-ORBEndpoint "));

  ACE_OS::strncat (random_endpoint,
                   GLOBALS::instance ()->endpoint,
                   random_endpoint_length);

  for (int i = this->num_low_priority_;
       i > 0;
       i--)
    {
      ACE_TCHAR *low_second_argv[] =
        {random_endpoint,
         const_cast<ACE_TCHAR *> (ACE_TEXT("-ORBSndSock 32768 ")),
         const_cast<ACE_TCHAR *> (ACE_TEXT("-ORBRcvSock 32768 ")),
         0};
      ACE_NEW_RETURN (this->low_argv_,
                      ACE_ARGV (this->argv_,
                                low_second_argv),
                      -1);

      ACE_NEW_RETURN (this->low_priority_tasks_ [i - 1],
                      Cubit_Task (this->low_argv_->buf (),
                                  "internet",
                                  1,
                                  &this->servant_manager_,
                                  i),
                      -1);

      // Make the low priority task an active object.
      if (this->low_priority_tasks_[i - 1]->activate
          (GLOBALS::instance ()->thr_create_flags,
           1,
           0,
           this->low_priority_) == -1)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) task activation at priority %d ")
                    ACE_TEXT (" failed, exiting!\n%a"),
                    this->low_priority_,
                    -1));

      ACE_DEBUG ((LM_DEBUG,
                  "Created servant %d with priority %d\n",
                  i,
                  this->low_priority_));

      // Use different priorities on thread per rate or multiple
      // priority.
      if (GLOBALS::instance ()->use_multiple_priority == 1
          || GLOBALS::instance ()->thread_per_rate == 1)
        {
          this->counter_ = (this->counter_ + 1) % this->grain_;

          if (this->counter_ == 0
               //Just so when we distribute the priorities among the
               //threads, we make sure we don't go overboard.
              && this->num_priorities_ * this->grain_ > this->num_low_priority_ - (i - 1))
            // Get the next higher priority.
            this->low_priority_ = ACE_Sched_Params::next_priority
              (ACE_SCHED_FIFO,
               this->low_priority_,
               ACE_SCOPE_THREAD);
        }
    } /* end of for() */

  // default return success.
  return 0;
}

int
Server::start_servants (void)
{
  // Do the preliminary argument processing for options -p and -h.
  this->prelim_args_process ();

  // Find the priority for the high priority servant.
  this->high_priority_ = this->priority_.get_high_priority ();

  ACE_DEBUG ((LM_DEBUG,
              "Creating servant 0 with high priority %d\n",
              this->high_priority_));

  // Activate the high priority servant task
  if (this->activate_high_servant () < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Failure in activating high priority servant\n"),
                      -1);

  this->num_low_priority_ =
    GLOBALS::instance ()->num_of_objs - 1;

  // Initialize the priority of the low priority servants.
  this->init_low_priority ();

  // Activate the low priority servants.
  if (this->activate_low_servants () < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Failure in activating low priority servant\n"),
                      -1);

  // Wait in the barrier.
  GLOBALS::instance ()->barrier_->wait ();

  int result = this->write_iors ();
  if (result != 0)
    return result;
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result;

  Server server;

  result = server.init (argc, argv);
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in Initialization\n"),
                      1);

  // run the server.
  result = server.run ();
  ACE_DEBUG ((LM_DEBUG,
              "%s:%d\n", __FILE__, __LINE__));
  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error while running the servants\n"),
                      2);
  return 0;
}
