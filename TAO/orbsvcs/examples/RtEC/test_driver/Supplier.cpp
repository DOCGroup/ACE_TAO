// $Id$

#include "Supplier.h"
#include "orbsvcs/Event_Utilities.h" //for ACE_Supplier/ConsumerQOS_Factory
#include "orbsvcs/Event_Service_Constants.h"

ACE_RCSID(EC_Examples, Supplier, "$Id$")

Supplier::Supplier (void)
  : timeoutconsumer(this)
  , _supplier(this)
{
}

Supplier::~Supplier()
{
}

void 
Supplier::update(ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG((LM_DEBUG,"Supplier %d (%P|%t) received update\n",this->_supplier_id));
 
  if (this->_num_sent < this->_to_send)
    {
      //send this->_events
      this->_consumer_proxy->push(this->_events ACE_ENV_ARG_PARAMETER);

      ++this->_num_sent;
      ACE_DEBUG((LM_DEBUG,"Sent events; %d sent\t%d total\n",this->_num_sent,this->_to_send));
      if (this->_num_sent >= this->_to_send)
	{
	  //just finished; only want to do this once!
	  ACE_DEBUG((LM_DEBUG,"RELEASE read lock from Supplier %d\n",
		     this->_supplier_id));
	  this->_done->release();
	  this->_hold_mtx = 0;
	}
    } 
  else
    {
      //do nothing
    }
}

void 
Supplier::connect (ACE_RW_Mutex* done,
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
          ACE_DEBUG((LM_DEBUG,"ERROR: Could not acquire read lock for Supplier: %s\n",
                     ACE_OS::strerror(errno)));
        } else
        {
          ACE_DEBUG((LM_DEBUG,"ACQUIRED read lock for Supplier %d\n",this->_supplier_id));
          this->_hold_mtx = 1;
        }
    } else
    {
      ACE_DEBUG((LM_DEBUG,"Already done; did not grab read lock for Supplier %d\n",this->_supplier_id));
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
  
  ACE_DEBUG((LM_DEBUG,"Supplier %d connected\n",this->_supplier_id));
  for (size_t i=0; i<events.length(); ++i)
    {
      ACE_DEBUG((LM_DEBUG,"\tEvent Type: %d\n",events[i].header.type));
    }

  //connect TimeoutConsumer for timeouts.
  this->timeoutconsumer.connect(scheduler,supp_entry_pt.str().c_str(),period,
				importance,criticality,ec ACE_ENV_ARG_PARAMETER);

  //Add Scheduler dependency between TimeoutConsumer and Supplier
  scheduler->add_dependency (this->timeoutconsumer.get_RT_Info(),
			     rt_info,
			     1,
			     RtecBase::TWO_WAY_CALL
			     ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  
}

void 
Supplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
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

  this->timeoutconsumer.disconnect(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
Supplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
