/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    Task_Client.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#if !defined (TASK_CLIENT_H)
#define TASK_CLIENT_H

#include <math.h>

#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Profile_Timer.h"
#include "ace/ARGV.h"

#include "orbsvcs/CosNamingC.h"
#include "cubitC.h"

// @@ Should we put this into a more general file, e.g., OS.h?
#if defined (ACE_LACKS_FLOATING_POINT)
#define double ACE_UINT32
#define fabs(X) ((X) >= 0 ? (X) : -(X))
#endif /* ACE_LACKS_FLOATING_POINT */

#if !defined (ACE_HAS_THREADS)
class NOOP_ACE_Barrier
{
public:
  NOOP_ACE_Barrier (int) {}
  void wait (void) {}
};
#define ACE_Barrier NOOP_ACE_Barrier
#endif /* ACE_HAS_THREADS */

// @@ Make sure not to rely on static constructors being called...  
// In general, there should be no statics in this program.
// Please use a Singleton instead...
static CORBA::String key = CORBA::String ("Cubit");

// Arbitrary generator used by the client to create the numbers to be
// cubed.
static inline int 
func (u_i) 
{ 
  return i - 117; 
}

enum Cubit_Datatypes
{
  // = The various datatypes which the client and the server can
  // exchange.
  
  CB_OCTET,
  CB_SHORT,
  CB_LONG,
  CB_STRUCT,

  // = Rate constants.
  CB_40HZ_CONSUMER = 0,
  CB_20HZ_CONSUMER = 1,
  CB_10HZ_CONSUMER = 2,
  CB_5HZ_CONSUMER = 3,
  CB_1HZ_CONSUMER = 4,

  CB_40HZ_CONSUMER_RATE = 40,
  CB_20HZ_CONSUMER_RATE = 20,
  CB_10HZ_CONSUMER_RATE = 10,
  CB_5HZ_CONSUMER_RATE = 5,
  CB_1HZ_CONSUMER_RATE = 1,

  CB_HIGH_PRIORITY_RATE = 20,
  CB_LOW_PRIORITY_RATE = 10
};

class Task_State
{
  // = TITLE
  //     Maintains state common to multiple Cubit clients.
  //
  // = DESCRIPTION
  //     This class maintains state which is common to the potentially
  //     multiple concurrent clients.
public:
  ACE_Barrier *barrier_;
  // Barrier for the multiple clients to synchronize after binding to
  // the servants.

  Task_State (int argc, char **argv);
  // Constructor. Takes the command line arguments, which are later
  // passed into ORB_init.

  u_int start_count_;
  // Keeps a count of the number of clients started.  This count also
  // serves as a thread-id.  The first thread created gets an ID of 0,
  // then 1, and so on.

  u_int loop_count_;
  // Number of times to loop, making calls.

  u_int thread_count_;
  // Number of concurrent clients to create.

  u_int base_port_;
  // This is the port at which the high priority servant is
  // listening. lower priority ports begin at base_port_ + 1.

  char *server_host_;
  // Server hostname.

  double *latency_;
  // Array to store the latency for every client, indexed by
  // thread-id.

  int *ave_latency_;
  // Int array to store the latencies.

  Cubit_Datatypes datatype_;
  // Which datatype to use to make the calls.

  ACE_SYNCH_MUTEX lock_;
  // Lock to protect access to this object.

  // = Command line arguments.
  int argc_;
  char **argv_;

  u_int thread_per_rate_;
  // Flag for the thread_per_rate test.

  double **global_jitter_array_;
  // This array stores the latency seen by each client for each
  // request, to be used later to compute jitter.

  char *factory_ior_;
  // Object reference string for the cubit factory.

  u_int shutdown_;
  // Flag that indicates if we are going to call the shutdown methos
  // for the servant.

  u_int oneway_;
  // Flag that indicates if we are going to use oneway calls instead
  // of two-way.

  u_int use_name_service_;
  // Flag that say if we are using the or not the name service.

  // @@ Please document me.  Also, please don't use magic numbers like
  // 50...
  char *iors_[50];
  char *ior_file_;
};

class Client : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //     The Cubit client.
  // 
  // = DESCRIPTION
  //     @@ Please elaborate...
public:
  Client (Task_State *ts);
  // constructor, with a pointer to the common task state.

  virtual int svc (void);
  // The thread function.

  double get_high_priority_latency (void);
  double get_low_priority_latency (void);
  double get_high_priority_jitter (void);
  double get_low_priority_jitter (void);
  int get_latency (u_int thread_id);
  // Accessors to get the various measured quantities.

private:
  int run_tests (Cubit_ptr,
                 u_int,
                 u_int,
                 Cubit_Datatypes,
                 double frequency);
  // Makes the calls to the servant.

  void put_latency (double *jitter,
                    double latency,
                    u_int);
  // Records the latencies in the <Task_State>.

  void put_ave_latency (int latency, u_int);
  // Records the latencies in the <Task_State>.

  int parse_args (int, char **);
  // Parses the arguments.

  Task_State *ts_;
  // Pointer to shared state.

  CosNaming::NamingContext_var naming_context_;
  // Object reference to the naming service.

  CosNaming::NamingContext_var mt_cubit_context_;
  // Object reference to the cubit context "MT_Cubit".
};

#endif /* !defined (TASK_CLIENT_H) */
