// $Id$

#include "Inversion.h"
#include "Consumer.h"
#include "Supplier.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event_Utilities.h"
#include "ace/Sched_Params.h"
#include "ace/Arg_Shifter.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  EC_Inversion driver;
  return driver.run (argc, argv);
}

// ****************************************************************

EC_Inversion::EC_Inversion (void)
  :  same_events_ (0)
{
}

int
EC_Inversion::parse_args (int &argc, ACE_TCHAR *argv [])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, ACE_TEXT("-same_events")) == 0)
        {
          arg_shifter.consume_arg ();
          this->same_events_ = 1;
        }

      else
        {
          arg_shifter.ignore_arg ();
        }
    }

  int r = this->EC_Driver::parse_args (argc, argv);
  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG,
                "EC_Inversion (%P|%t) "
                "adjusting number of consumers (2)\n"));
  return r;
}

void
EC_Inversion::connect_consumers (void)
{
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->event_channel_->for_consumers ();

  ACE_ConsumerQOS_Factory qos0;
  qos0.start_disjunction_group (2);
  qos0.insert_type (ACE_ES_EVENT_UNDEFINED, 0);
  qos0.insert_type (ACE_ES_EVENT_UNDEFINED + 1, 0);

  this->consumers_[0]->connect (consumer_admin.in (),
                                qos0.get_ConsumerQOS (),
                                ACE_ES_EVENT_UNDEFINED + 1);

  for (int i = 1; i < this->n_consumers_; ++i)
    {
      int base_event = ACE_ES_EVENT_UNDEFINED + 2;
      if (this->same_events_)
        base_event = ACE_ES_EVENT_UNDEFINED;

      ACE_ConsumerQOS_Factory qos1;
      qos1.start_disjunction_group (2);
      qos1.insert_type (base_event    , 0);
      qos1.insert_type (base_event + 1, 0);

      this->consumers_[i]->connect (consumer_admin.in (),
                                    qos1.get_ConsumerQOS (),
                                    base_event + 1);
    }
  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Inversion (%P|%t) connected consumer(s)\n"));
}

void
EC_Inversion::connect_suppliers (void)
{
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->event_channel_->for_suppliers ();

  ACE_SupplierQOS_Factory qos0;
  qos0.insert (1, ACE_ES_EVENT_UNDEFINED, 0, 1);
  qos0.insert (1, ACE_ES_EVENT_UNDEFINED + 1, 0, 1);

  this->suppliers_[0]->connect (supplier_admin.in (),
                                qos0.get_SupplierQOS (),
                                ACE_ES_EVENT_UNDEFINED + 1);

  for (int j = 1; j != this->n_suppliers_; ++j)
    {
      int base_event = ACE_ES_EVENT_UNDEFINED + 2;
      if (this->same_events_)
        base_event = ACE_ES_EVENT_UNDEFINED;

      ACE_SupplierQOS_Factory qos1;
      qos1.insert (1, base_event    , 0, 1);
      qos1.insert (1, base_event + 1, 0, 1);

      this->suppliers_[j]->connect (supplier_admin.in (),
                                    qos1.get_SupplierQOS (),
                                    base_event + 1);
    }

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Inversion (%P|%t) connected supplier(s)\n"));
}

void
EC_Inversion::activate_tasks (void)
{
  int priority;

  if (ACE_BIT_ENABLED (this->thr_create_flags_, THR_SCHED_FIFO))
    {
      priority =
        ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
    }
  else
    {
      priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_OTHER);
    }
  if (this->tasks_[0]->activate (this->thr_create_flags_,
                                 1, 0, priority) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "EC_Inversion (%P|%t) cannot activate high prio task\n"));
    }

  if (ACE_BIT_ENABLED (this->thr_create_flags_, THR_SCHED_FIFO))
    {
      priority =
        (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
         + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
    }
  else
    {
      priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_OTHER);
    }

  for (int i = 1; i < this->n_suppliers_; ++i)
    {
      if (this->tasks_[i]->activate (this->thr_create_flags_,
                                     1, 0, priority) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "EC_Inversion (%P|%t) Cannot activate thread "
                      "for supplier %d\n%p\n",
                      i, "EC_Inversion - OS error is:"));
        }
    }
}
