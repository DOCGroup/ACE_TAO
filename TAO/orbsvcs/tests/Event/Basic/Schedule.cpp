// $Id$

#include "Schedule.h"
#include "Consumer.h"
#include "Supplier.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"



int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  EC_Schedule driver;
  return driver.run (argc, argv);
}

// ****************************************************************

EC_Schedule::EC_Schedule (void)
  :  scheduler_impl_ (0)
{
}

int
EC_Schedule::parse_args (int& argc, ACE_TCHAR* argv[])
{
  if (this->EC_Driver::parse_args (argc, argv) != 0)
    return -1;

  return 0;
}

void
EC_Schedule::print_args (void) const
{
  this->EC_Driver::print_args ();
}

void
EC_Schedule::print_usage (void)
{
  this->EC_Driver::print_usage ();
}

void
EC_Schedule::initialize_ec_impl (void)
{
  this->scheduler_impl_ = new ACE_Config_Scheduler;
  this->scheduler_ = this->scheduler_impl_->_this ();

  this->EC_Driver::initialize_ec_impl ();
}

void
EC_Schedule::modify_attributes (TAO_EC_Event_Channel_Attributes& attr)
{
  attr.scheduler = this->scheduler_.in (); // no need to dup
}

void
EC_Schedule::cleanup_ec (void)
{
  this->EC_Driver::cleanup_ec ();
  delete this->scheduler_impl_;
}

void
EC_Schedule::execute_test (void)
{
  CORBA::Long min_priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  CORBA::Long max_priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG,
                "EC_Schedule (%P|%t) computing schedule\n"));

  RtecScheduler::RT_Info_Set_var infos;
  RtecScheduler::Dependency_Set_var deps;
  RtecScheduler::Config_Info_Set_var configs;
  RtecScheduler::Scheduling_Anomaly_Set_var anomalies;
  this->scheduler_->compute_scheduling (min_priority, max_priority,
                                        infos.out (),
                                        deps.out (),
                                        configs.out (),
                                        anomalies.out ());

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG,
                "EC_Schedule (%P|%t) schedule prepared\n"));

  ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                        deps.in (),
                                        configs.in (),
                                        anomalies.in ());

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG,
                "EC_Schedule (%P|%t) schedule dumped\n"));

}

void
EC_Schedule::build_consumer_qos (
  int i,
  RtecEventChannelAdmin::ConsumerQOS& qos,
  int& shutdown_event_type)
{
  char name[128];
  ACE_OS::sprintf (name, "EC_Schedule::Consumer::%04x", i);

  RtecScheduler::handle_t rt_info =
    this->scheduler_->create (name);

  // The worst case execution time is far less than 2
  // milliseconds, but that is a safe estimate....
  ACE_Time_Value tv (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  this->scheduler_->set (rt_info,
                         RtecScheduler::VERY_HIGH_CRITICALITY,
                         time, time, time,
                         0,
                         RtecScheduler::VERY_LOW_IMPORTANCE,
                         time,
                         0,
                         RtecScheduler::OPERATION);

  int type_start =
    this->consumer_type_start_
    + i * this->consumer_type_shift_;

  shutdown_event_type = type_start + this->consumer_type_count_;

  ACE_ConsumerQOS_Factory qos_factory;
  qos_factory.start_disjunction_group ();
  qos_factory.insert_type (shutdown_event_type, rt_info);

  for (int j = 0; j != this->consumer_type_count_; ++j)
    qos_factory.insert_type (type_start + j, rt_info);

  qos = qos_factory.get_ConsumerQOS ();
}

void
EC_Schedule::build_supplier_qos (
      int i,
      RtecEventChannelAdmin::SupplierQOS& qos,
      int& shutdown_event_type)
{
  char name[128];
  ACE_OS::sprintf (name, "EC_Schedule::Supplier::%04x", i);

  RtecScheduler::handle_t rt_info =
    this->scheduler_->create (name);

  ACE_Time_Value tv (0, this->burst_pause_);
  RtecScheduler::Period_t rate = tv.usec () * 10;

  // The execution times are set to reasonable values, but
  // actually they are changed on the real execution, i.e. we
  // lie to the scheduler to obtain right priorities; but we
  // don't care if the set is schedulable.
  tv.set (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  this->scheduler_->set (rt_info,
                         RtecScheduler::VERY_HIGH_CRITICALITY,
                         time, time, time,
                         rate,
                         RtecScheduler::VERY_LOW_IMPORTANCE,
                         time,
                         1,
                         RtecScheduler::OPERATION);

  int type_start = this->supplier_type_start_ + i*this->supplier_type_shift_;
  int supplier_id = i + 1;
  shutdown_event_type = type_start + this->supplier_type_count_;

  ACE_SupplierQOS_Factory qos_factory;
  for (int j = 0; j != this->supplier_type_count_; ++j)
    qos_factory.insert (supplier_id,
                        type_start + j,
                        rt_info, 1);

  qos_factory.insert (supplier_id,
                      shutdown_event_type,
                      rt_info, 1);

  qos = qos_factory.get_SupplierQOS ();
}

void
EC_Schedule::dump_results (void)
{
}
