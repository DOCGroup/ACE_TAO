// $Id$

#include "Machine_Properties.h"

const int DEFAULT_TIMEOUT_SEC = 10;
const int DEFAULT_TIMEOUT_USEC = 0;

const char* TAO_Machine_Properties::PROP_NAMES[] = 
{
  "CPU", 
  "DISK",
  "PAGES",
  "SWAPS",
  "PACKETS",
  "CONTEXT_SWITCHES",
  "COLLISIONS",
  "INTERRUPTS",
  "LOAD"
};

const CORBA::TypeCode_ptr
TAO_Machine_Properties::PROP_TYPE = CORBA::_tc_float;

TAO_Machine_Properties::
TAO_Machine_Properties (void)
  : timeout_ (ACE_Time_Value (DEFAULT_TIMEOUT_SEC, DEFAULT_TIMEOUT_USEC)),
    timestamp_ (0)
{
  this->init ();
}


TAO_Machine_Properties::
TAO_Machine_Properties (const ACE_Time_Value& timeout)
  : timeout_ (timeout),
    timestamp_ (0)
{
  this->init ();
}

void
TAO_Machine_Properties::init (void)
{
  this->rstat_client_ =
    ::clnt_create("localhost", RSTATPROG, RSTATVERS_TIME, "udp");

  if (this->rstat_client_ == 0)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) %s\n",
		  ::clnt_spcreateerror ("localhost")));
    }
  
  if (this->retrieve_stats () == -1)
    ::memset (&this->old_stats_, 0, sizeof (statstime));
  else
    this->timestamp_.sec (0);
}

int
TAO_Machine_Properties::retrieve_stats (void)
{
  if (this->rstat_client_ == 0)
    return -1;
  
  static struct timeval timeout = {25, 0};
  if (clnt_call (this->rstat_client_,
		 RSTATPROC_STATS,
		 xdr_void,
		 0,
		 (xdrproc_t) xdr_statstime,
		 (caddr_t) &this->stats_,
		 timeout) != RPC_SUCCESS)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %s\n",
			 ::clnt_sperror (this->rstat_client_, "localhost")), -1);
    }

  this->sample_time_ = ACE_OS::gettimeofday () - this->timestamp_;
  this->timestamp_ = ACE_OS::gettimeofday ();
}

CORBA::Any*
TAO_Machine_Properties::evalDP(const CORBA::Any& extra_info,
			       CORBA::TypeCode_ptr returned_type,
			       CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure))
{
  CORBA::Any* return_value = 0;
  
  if (ACE_OS::gettimeofday () - this->timestamp_ > this->timeout_)
    {
      if (this->retrieve_stats () == -1)
	return return_value;
    }

  char* prop_name = 0;
  extra_info >>= prop_name;

  if (prop_name == 0)
    return return_value;

  ACE_NEW_RETURN (return_value, CORBA::Any, 0);

  int elapsed_seconds = this->sample_time_.sec () +
    (this->sample_time_.usec () > 500000) ? 1 : 0;
  
  if (ACE_OS::strcmp (prop_name, PROP_NAMES[CPU]) == 0)
    this->compute_cpu (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[DISK]) == 0)
    this->compute_disk (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[PAGES]) == 0)
    this->compute_pages (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[SWAPS]) == 0)
    this->compute_swaps (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[PACKETS]) == 0)
    this->compute_packets (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[CONTEXT_SWITCHES]) == 0)
    this->compute_context (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[COLLISIONS]) == 0)
    this->compute_collisions (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[INTERRUPTS]) == 0)
    this->compute_interrupts (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[LOAD]) == 0)
    this->compute_load (*return_value, elapsed_seconds);

  ::memcpy (&this->old_stats_, &this->stats_, sizeof (statstime));
}
 
void
TAO_Machine_Properties::compute_cpu (CORBA::Any& value, int elapsed_seconds)
{
  // The first three cpu stats are for user, kernal, iowait
  CORBA::Float used = 0.0;
  for (int i = 0; i < RSTAT_CPUSTATES -1; i++)
    used += (this->stats_.cp_time[i] - this->old_stats_.cp_time[i]);
  
  // The last is the amount idle.
  CORBA::Float idle =
    this->stats_.cp_time[RSTAT_CPUSTATES - 1] -
    this->old_stats_.cp_time[RSTAT_CPUSTATES - 1];
  
  // The CPU usage is the amount used over the total available.
  value <<= (CORBA::Float) (used / (used + idle) * 100.0);
}

void
TAO_Machine_Properties::compute_disk (CORBA::Any& value, int elapsed_seconds)
{
  CORBA::Float used = 0.0;
  for (int i = 0; i < RSTAT_DK_NDRIVE; i++)
    used += (this->stats_.dk_xfer[i] - this->old_stats_.dk_xfer[i]);
  
  value <<= (CORBA::Float)(used / (float) elapsed_seconds);
}

void
TAO_Machine_Properties::compute_pages (CORBA::Any& value, int elapsed_seconds)
{
  CORBA::Float pages =
    (this->stats_.v_pgpgin - this->old_stats_.v_pgpgin) +
    (this->stats_.v_pgpgout - this->old_stats_.v_pgpgout);
  
  value <<= (CORBA::Float) (pages / elapsed_seconds);
}

void
TAO_Machine_Properties::compute_swaps (CORBA::Any& value, int elapsed_seconds)
{
  CORBA::Float swaps =
    (this->stats_.v_pswpin - this->old_stats_.v_pswpin) +
    (this->stats_.v_pswpout - this->old_stats_.v_pswpout);

  value <<= (CORBA::Float) (swaps / elapsed_seconds);
}

void
TAO_Machine_Properties::compute_packets (CORBA::Any& value, int elapsed_seconds)
{
  CORBA::Float packets =
    (this->stats_.if_ipackets - this->old_stats_.if_ipackets) +
    (this->stats_.if_opackets - this->old_stats_.if_opackets);

  value <<= (CORBA::Float) (packets / elapsed_seconds);
}

void
TAO_Machine_Properties::compute_errors (CORBA::Any& value, int elapsed_seconds)
{
  CORBA::Float packets =
    (this->stats_.if_ipackets - this->old_stats_.if_ipackets) +
    (this->stats_.if_opackets - this->old_stats_.if_opackets);

  value <<= (CORBA::Float) (packets / elapsed_seconds);
}

void
TAO_Machine_Properties::
compute_context (CORBA::Any& value, int elapsed_seconds)
{
  CORBA::Float switches = (this->stats_.v_swtch - this->old_stats_.v_swtch);
  value <<= (CORBA::Float) (switches / elapsed_seconds);
}

void
TAO_Machine_Properties::
compute_collisions (CORBA::Any& value, int elapsed_seconds)
{
  CORBA::Float cols =
    (this->stats_.if_collisions - this->old_stats_.if_collisions);
  value <<= (CORBA::Float) (cols / elapsed_seconds);
}

void
TAO_Machine_Properties::
compute_interrupts (CORBA::Any& value, int elapsed_seconds)
{
  CORBA::Float intr = (this->stats_.v_intr - this->old_stats_.v_intr);
  value <<= (CORBA::Float) (intr / elapsed_seconds);
}

void
TAO_Machine_Properties::compute_load (CORBA::Any& value, int elapsed_seconds)
{
  CORBA::Float load = (float) this->stats_.avenrun[0] / FSCALE;
  value <<= load;
}
