// $Id$

#include "Machine_Properties.h"

ACE_RCSID(server, Machine_Properties, "$Id$")

const int TAO_Machine_Properties::NUM_PROPERTIES = 10;

const int DEFAULT_TIMEOUT_SEC = 1;
const int DEFAULT_TIMEOUT_USEC = 0;

const char* TAO_Machine_Properties::PROP_NAMES[] =
{
  "CPU",
  "Disk",
  "Pages",
  "Swaps",
  "Packets",
  "Errors",
  "Context_Switches",
  "Collisions",
  "Interrupts",
  "Load"
};

// XXX:hack to avoid CC errors. 
// Redefine xdr_void. Always return true.
int xdr_void (XDR *,void *)
{
  return 1;
}


TAO_Machine_Properties::TAO_Machine_Properties (void)
  : timeout_ (ACE_Time_Value (DEFAULT_TIMEOUT_SEC, DEFAULT_TIMEOUT_USEC)),
    timestamp_ (0),
    sample_time_ (0)
{
  this->init ();
}


TAO_Machine_Properties::
TAO_Machine_Properties (const ACE_Time_Value& timeout)
  : timeout_ (timeout),
    timestamp_ (0),
    sample_time_ (0)
{
  this->init ();
}

void
TAO_Machine_Properties::init (void)
{
  /*
  this->rstat_client_ =
    ::clnt_create("localhost", RSTATPROG, RSTATVERS_TIME, "udp");

  if (this->rstat_client_ == 0)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) %s\n",
                  ::clnt_spcreateerror ("localhost")));
    }

  ::memset (&this->old_stats_, 0, sizeof (statstime));
  */
  this->retrieve_stats ();
}

int
TAO_Machine_Properties::retrieve_stats (void)
{
  // initialize the handle everytime ??
  this->rstat_client_ =
    ::clnt_create("localhost", RSTATPROG, RSTATVERS_TIME, "udp");

  if (this->rstat_client_ == 0)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) %s\n",
                  ::clnt_spcreateerror ("localhost")));
    }

  ::memset (&this->old_stats_, 0, sizeof (statstime));

  //  if (this->rstat_client_ == 0)
  //    return -1;

  static struct timeval timeout = {25, 0};
  u_int result;
  if ((result =clnt_call (this->rstat_client_,
                 RSTATPROC_STATS,
                 xdr_void,
                 0,
                 (xdrproc_t) xdr_statstime,
                 (caddr_t) &this->stats_,
                 timeout)) != RPC_SUCCESS)
    {
      ACE_DEBUG ((LM_DEBUG,"rpc-error:%d\n",result));
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %s\n",
                         ::clnt_sperror (this->rstat_client_, "localhost")), -1);
    }
  else
      ACE_DEBUG ((LM_DEBUG,"rpc client call worked\n"));

  this->sample_time_ = ACE_OS::gettimeofday () - this->timestamp_;
  this->timestamp_ = ACE_OS::gettimeofday ();
  return 0;
}

CORBA::Any*
TAO_Machine_Properties::evalDP (const char* prop_name,
                                CORBA::TypeCode_ptr returned_type,
                                const CORBA::Any& extra_info,
                                CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure))
{
  CORBA::Any* return_value;

  ACE_DEBUG ((LM_DEBUG, "Evaluating machine properties.\n"));

  ACE_NEW_RETURN (return_value, CORBA::Any, 0);

  if (ACE_OS::gettimeofday () - this->timestamp_ > this->timeout_)
    {
      if (this->retrieve_stats () == -1)
        return return_value;
    }

  CORBA::String_var prop_name_var (prop_name);
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
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[ERRORS]) == 0)
    this->compute_errors (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[CONTEXT_SWITCHES]) == 0)
    this->compute_context (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[COLLISIONS]) == 0)
    this->compute_collisions (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[INTERRUPTS]) == 0)
    this->compute_interrupts (*return_value, elapsed_seconds);
  else if (ACE_OS::strcmp (prop_name, PROP_NAMES[LOAD]) == 0)
    this->compute_load (*return_value, elapsed_seconds);

  ::memcpy (&this->old_stats_, &this->stats_, sizeof (statstime));

  return return_value;
}

void
TAO_Machine_Properties::
export_properties (TAO_Property_Exporter& prop_exporter)
{
  ACE_DEBUG ((LM_ERROR, "Adding machine properties.\n"));
  for (int i = 0; i < NUM_PROPERTIES; i++)
    {
      CORBA::Any extra_info;
      const char* name = PROP_NAMES[i];
      const CORBA::TypeCode_ptr prop_type = CORBA::_tc_float;

      CosTradingDynamic::DynamicProp* dp_struct =
        this->construct_dynamic_prop (name, prop_type, extra_info);

      prop_exporter.add_dynamic_property (name, dp_struct);
    }
}

int
TAO_Machine_Properties::
define_properties (CosTradingRepos::ServiceTypeRepository::PropStructSeq& prop_seq,
                   CORBA::ULong offset) const
{
  prop_seq.length (NUM_PROPERTIES + offset);
  for (int j = prop_seq.length () - offset - 1, i = offset; j >= 0; j--, i++)
    {
      prop_seq[i].name = PROP_NAMES[i - offset];
      prop_seq[i].value_type = CORBA::TypeCode::_duplicate (CORBA::_tc_float);
      prop_seq[i].mode = CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
    }

  return NUM_PROPERTIES;
}

void
TAO_Machine_Properties::compute_cpu (CORBA::Any& value, int elapsed_seconds)
{

  // The first three cpu stats are for user, kernal, iowait
  CORBA::ULong used = 0.0;
  for (int i = 0; i < RSTAT_CPUSTATES - 1; i++)
    used += (this->stats_.cp_time[i] - this->old_stats_.cp_time[i]);

  // The last is the amount idle.
  CORBA::ULong idle =
    this->stats_.cp_time[RSTAT_CPUSTATES - 1] -
    this->old_stats_.cp_time[RSTAT_CPUSTATES - 1];

  // The CPU usage is the amount used over the total available.
  value <<= (CORBA::Float) ((((CORBA::Float) used) / (used + idle)) * 100.0);
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
  CORBA::Float load = ((CORBA::Float) this->stats_.avenrun[0]) / FSCALE;
  value <<= load;
}
