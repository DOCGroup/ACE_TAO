// $Id$

#include "TimeoutConsumer.h"

#include <sstream> //for ostringstream

#include "orbsvcs/Event_Utilities.h" //for ACE_Supplier/ConsumerQOS_Factory
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/RtecEventCommC.h"
#include "ace/RW_Mutex.h"

ACE_RCSID(EC_Examples, Consumer, "$Id$")

TimeoutConsumer::TimeoutConsumer (void)
  : _to_send(0)
  , _num_sent(0)
  , _hold_mtx(0)
  , _done(0)
  , _supplier(this)
  , _consumer(this)
  , _events(0)
{
}

TimeoutConsumer::~TimeoutConsumer (void)
{
  // TODO this->disconnect() ???

  if (this->_hold_mtx && this->_done!=0)
    {
      this->_done->release();
      this->_hold_mtx = 0;
    }
}

void
TimeoutConsumer::connect (ACE_RW_Mutex* done,
                          RtecScheduler::Scheduler_ptr scheduler,
                          const char *entry_prefix,
                          TimeBase::TimeT period,
                          RtecScheduler::Importance_t importance,
                          RtecScheduler::Criticality_t criticality,
                          RtecEventComm::EventSourceID supplier_id,
                          size_t to_send,
                          const RtecEventComm::EventSet& events,
                          RtecEventChannelAdmin::EventChannel_ptr ec
                          ACE_ENV_ARG_DECL)
{
  this->_supplier_id = supplier_id;
  this->_to_send = to_send;
  this->_num_sent = 0;
  this->_hold_mtx = 0;
  this->_done = done;
  if (this->_done!= 0 && this->_num_sent<this->_to_send)
    {
      int ret = done->acquire_read();
      if (ret == -1)
        {
          ACE_DEBUG((LM_DEBUG,"ERROR: Could not acquire read lock for TimeoutConsumer: %s\n",
                     ACE_OS::strerror(errno)));
        } else
        {
          ACE_DEBUG((LM_DEBUG,"ACQUIRED read lock for TimeoutConsumer %d\n",this->_supplier_id));
          this->_hold_mtx = 1;
        }
    } else
    {
      ACE_DEBUG((LM_DEBUG,"Already done; did not grab read lock for TimeoutConsumer\n"));
    }

  this->_events.length(events.length());
  for (size_t i=0; i<events.length(); ++i)
    {
      this->_events[i] = events[i]; //copy event to local set
      this->_events[i].header.source = this->_supplier_id; //make sure event source is this
    }

  //create supplier RT_Info
  std::ostringstream supp_entry_pt;
  supp_entry_pt << entry_prefix << " Supplier " << this->_supplier_id; //unique RT_Info entry point
  ACE_DEBUG((LM_DEBUG,"Creating %s\n",supp_entry_pt.str().c_str()));
  RtecScheduler::handle_t rt_info = scheduler->create (supp_entry_pt.str().c_str()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_Time_Value tv (0,0);
  TimeBase::TimeT tmp;
  ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);
  scheduler->set (rt_info,
                  criticality,
                  tmp,tmp,tmp,
                  period,
                  importance,
                  tmp,
                  0,
                  RtecScheduler::OPERATION
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Register as supplier of events
  ACE_SupplierQOS_Factory supplierQOS;
  for (size_t i=0; i<events.length(); ++i)
    {
      //insert type for each event
      supplierQOS.insert (this->_supplier_id,
                          events[i].header.type,
                          rt_info,
                          1);
    }

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->_consumer_proxy =
    supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventComm::PushSupplier_var supplierv =
    this->_supplier._this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->_consumer_proxy->connect_push_supplier (supplierv.in (),
                                                supplierQOS.get_SupplierQOS ()
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG,"TimeoutConsumer connected as event supplier\n"));
  for (size_t i=0; i<events.length(); ++i)
    {
      ACE_DEBUG((LM_DEBUG,"\tEvent Type: %d\n",events[i].header.type));
    }

  //create consumer RT_Info
  std::ostringstream cons_entry_pt;
  cons_entry_pt << entry_prefix << " Consumer"; //unique RT_Info entry point
  ACE_DEBUG((LM_DEBUG,"Creating %s\n",cons_entry_pt.str().c_str()));
  rt_info = scheduler->create (cons_entry_pt.str().c_str() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  scheduler->set (rt_info,
                  criticality,
                  period, period, period,
                  period,
                  importance,
                  period,
                  0,
                  RtecScheduler::OPERATION
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Register as consumer of timeout events
  ACE_ConsumerQOS_Factory timeoutQOS;
  timeoutQOS.insert_time(ACE_ES_EVENT_INTERVAL_TIMEOUT /*??*/,
                         period, //TimeBase::TimeT
                         rt_info);

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->_supplier_proxy =
    consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventComm::PushConsumer_var consumerv =
    this->_consumer._this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->_supplier_proxy->connect_push_consumer (consumerv.in (),
                                                timeoutQOS.get_ConsumerQOS ()
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG,"TimeoutConsumer connected as timeout consumer\n"));
  std::ostringstream prd;
  prd << period;
  ACE_DEBUG((LM_DEBUG,"\tTimeout period: %s\n",prd.str().c_str()));
}

void
TimeoutConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  if (! CORBA::is_nil (this->_consumer_proxy.in ()))
    {
      this->_consumer_proxy->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->_consumer_proxy =
        RtecEventChannelAdmin::ProxyPushConsumer::_nil ();

      // Deactivate the servant
      PortableServer::POA_var poa =
        this->_supplier._default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      PortableServer::ObjectId_var id =
        poa->servant_to_id (&this->_supplier ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  //disconnect consumer ???
  if (! CORBA::is_nil (this->_supplier_proxy.in()))
    {
      this->_supplier_proxy->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->_supplier_proxy = RtecEventChannelAdmin::ProxyPushSupplier::_nil();

      //Deactivate the servant
      PortableServer::POA_var poa =
        this->_consumer._default_POA(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      PortableServer::ObjectId_var id =
        poa->servant_to_id (&this->_consumer ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      poa->deactivate_object(id.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (this->_hold_mtx && this->_done!=0)
    {
      this->_done->release();
      this->_hold_mtx = 0;
    }
}

void
TimeoutConsumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"TimeoutConsumer (%P|%t) push but no events\n"));
      return;
    }

  ACE_DEBUG((LM_DEBUG,"TimeoutConsumer %d (%P|%t) received %d events:\n",this->_supplier_id,
             events.length()));
  if (this->_num_sent < this->_to_send)
    {
      for (size_t i=0; i<events.length(); ++i)
        {
          if (ACE_ES_EVENT_INTERVAL_TIMEOUT == events[i].header.type)
            {
              ACE_DEBUG((LM_DEBUG,"TimeoutConsumer %d (%P|%t) received timeout event\n",this->_supplier_id));

              //TODO send this->_events
              ++this->_num_sent;
              ACE_DEBUG((LM_DEBUG,"Sent events; %d sent\t%d total\n",this->_num_sent,this->_to_send));
              if (this->_num_sent >= this->_to_send)
                {
                  //done
                  ACE_DEBUG((LM_DEBUG,"RELEASE read lock from TimeoutConsumer %d\n",
                             this->_supplier_id));
                  this->_done->release();
                  this->_hold_mtx = 0;
                }
            }
          else
            {
              int prio = -1;
              ACE_hthread_t handle;
              ACE_Thread::self(handle);
              ACE_Thread::getprio(handle,prio);
              //ACE_thread_t tid = ACE_Thread::self();
              ACE_DEBUG ((LM_DEBUG, "TimeoutConsumer @%d (%P|%t) we received event type %d\n",
                          prio,events[0].header.type));
            }
        }
    } else
    {
      //do nothing
    }
}

void
TimeoutConsumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TimeoutConsumer::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
