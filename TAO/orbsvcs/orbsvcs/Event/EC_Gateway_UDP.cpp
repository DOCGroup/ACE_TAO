//
// $Id$
//

#include "orbsvcs/Event/EC_Gateway_UDP.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Time_Utilities.h"

// ****************************************************************

TAO_ECG_UDP_Sender::TAO_ECG_UDP_Sender (void)
{
}

int
TAO_ECG_UDP_Sender::get_local_addr (ACE_INET_Addr& addr)
{
  return this->dgram_.get_local_addr (addr);
}

void 
TAO_ECG_UDP_Sender::init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
			  RtecScheduler::Scheduler_ptr lcl_sched,
			  const char* lcl_name,
			  const ACE_INET_Addr& ipaddr,
			  CORBA::Environment &_env)
{
  this->lcl_ec_ = 
    RtecEventChannelAdmin::EventChannel::_duplicate (lcl_ec);

  this->lcl_info_ =
    lcl_sched->create (lcl_name, _env);
  if (_env.exception () != 0) return;

  ACE_Time_Value tv (0, 500);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  lcl_sched->set (this->lcl_info_,
		  RtecScheduler::VERY_HIGH_CRITICALITY,
		  time, time, time,
		  25000 * 10,
		  RtecScheduler::VERY_LOW_IMPORTANCE,
		  time,
		  0,
		  RtecScheduler::OPERATION,
		  _env);
  if (_env.exception () != 0) return;

  if (this->dgram_.open (ipaddr) == -1)
    {
      // @@ TODO Use a Event Channel specific exception
      ACE_ERROR ((LM_ERROR, "ECG_UDP::init - Dgram open failed\n"));
      _env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));
    }
  if (_env.exception () != 0) return;
}

void
TAO_ECG_UDP_Sender::shutdown (CORBA::Environment& _env)
{
  this->close (_env);
  if (_env.exception () == 0) return;
  this->lcl_ec_ = RtecEventChannelAdmin::EventChannel::_nil ();

  if (this->dgram_.close () == -1)
    {
      // @@ TODO Use a Event Channel specific exception
      ACE_ERROR ((LM_ERROR, "ECG_UDP::init - Dgram close failed\n"));
      _env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));
    }
  if (_env.exception () != 0) return;
}

void
TAO_ECG_UDP_Sender::open (RtecEventChannelAdmin::ConsumerQOS& sub,
			  CORBA::Environment &_env)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Open gateway\n"));
  if (CORBA::is_nil (this->lcl_ec_.in ()))
    return;

  if (!CORBA::is_nil (this->supplier_proxy_.in ()))
    this->close (_env);
  if (_env.exception () != 0) return;

  if (sub.dependencies.length () == 0)
    return;
  for (CORBA::ULong j = 0; j < sub.dependencies.length (); ++j)
    {
      sub.dependencies[j].rt_info = this->lcl_info_;
    }

  //ACE_DEBUG ((LM_DEBUG, "ECG (%t) Gateway/Supplier "));
  //ACE_SupplierQOS_Factory::debug (pub);

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->lcl_ec_->for_consumers (_env);
  if (_env.exception () != 0) return;

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (_env);
  if (_env.exception () != 0) return;

  RtecEventComm::PushConsumer_var consumer_ref =
    this->_this (_env);
  if (_env.exception () != 0) return;

  //ACE_DEBUG ((LM_DEBUG, "ECG (%t) Gateway/Consumer "));
  //ACE_ConsumerQOS_Factory::debug (sub);

  this->supplier_proxy_->connect_push_consumer (consumer_ref.in (),
						sub,
						_env);
  if (_env.exception () != 0) return;
}

void
TAO_ECG_UDP_Sender::close (CORBA::Environment &env)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Closing gateway\n"));
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (env);
  if (env.exception () != 0) return;
  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
TAO_ECG_UDP_Sender::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "ECG (%t): Supplier-consumer received "
	      "disconnect from channel.\n"));
}

void
TAO_ECG_UDP_Sender::push (const RtecEventComm::EventSet &events,
			  CORBA::Environment & _env)
{
  ACE_DEBUG ((LM_DEBUG, "ECG_UDP_Sender::push - "));

  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  ACE_DEBUG ((LM_DEBUG, "%d event(s) - ", events.length ()));

  // @@ TODO, there is an extra data copy here, we should do the event
  // modification without it and only compact the necessary events.
  int count = 0;
  RtecEventComm::EventSet out (events.length ());
  for (u_int i = 0; i < events.length (); ++i)
    {
      //ACE_DEBUG ((LM_DEBUG, "type = %d ", events[i].type_));
      if (events[i].ttl_ > 0)
        {
          count++;
          out.length (count);
          out[count - 1] = events[i];
          out[count - 1].ttl_--;
        }
    }
  ACE_DEBUG ((LM_DEBUG, "count = %d\n", count));

  if (count > 0)
    {
      TAO_OutputCDR cdr;
      cdr.write_boolean (TAO_ENCAP_BYTE_ORDER);
      cdr.write_ulong (0); // Place holder for size...

      cdr.encode (RtecEventComm::_tc_EventSet, &out, 0, _env);
      if (_env.exception () != 0) return;

      CORBA::ULong bodylen = cdr.total_length ();
      char* buf = ACE_const_cast(char*,cdr.buffer ());
      buf += 4;
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
      *ACE_reinterpret_cast(CORBA::ULong*,buf) = bodylen;
#else
      if (!cdr.do_byte_swap ())
	{
	  *ACE_reinterpret_cast(CORBA::ULong*, buf) = bodylen;
	}
      else
	{
	  CDR::swap_4 (ACE_reinterpret_cast(char*,&bodylen), buf);
	}
#endif

      // This is a good maximum, because Dgrams cannot be longer than
      // 64K and the usual size for a CDR fragment is 512 bytes, still
      // if this is not enough we allocate memory from the heap.
      const int TAO_WRITEV_MAX = 128;
      ACE_IO_Vector iov[TAO_WRITEV_MAX];

      int iovcnt = 0;
      for (const ACE_Message_Block* i = cdr.begin ();
	   i != cdr.end () && iovcnt < TAO_WRITEV_MAX;
	   i = i->cont ())
	{
	  iov[iovcnt].buffer (i->rd_ptr ());
	  iov[iovcnt].length (i->length ());
	  iovcnt++;
	}

      ssize_t n = this->dgram_.send (iov, iovcnt);
      if (n == -1)
	{
	  // @@ TODO Use a Event Channel specific exception
	  ACE_DEBUG ((LM_DEBUG,
		      "ECG_UDP (%t) send failed %p\n", ""));
	  _env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));
	}
      else if (n == 0)
	{
	  // @@ TODO Use a Event Channel specific exception
	  ACE_DEBUG ((LM_DEBUG,
		      "ECG_UDP (%t) EOF on send \n"));
	  _env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));
	}
    }
}

// ****************************************************************

TAO_ECG_UDP_Receiver::TAO_ECG_UDP_Receiver (void)
{
}

void 
TAO_ECG_UDP_Receiver::init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
			    RtecScheduler::Scheduler_ptr lcl_sched,
			    const char* lcl_name,
			    const ACE_INET_Addr& ignore_from,
			    CORBA::Environment &_env)
{
  this->ignore_from_ = ignore_from;

  this->lcl_ec_ = 
    RtecEventChannelAdmin::EventChannel::_duplicate (lcl_ec);

  this->lcl_info_ =
    lcl_sched->create (lcl_name, _env);
  if (_env.exception () != 0) return;

  ACE_Time_Value tv (0, 500);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  lcl_sched->set (this->lcl_info_,
		  RtecScheduler::VERY_HIGH_CRITICALITY,
		  time, time, time,
		  25000 * 10,
		  RtecScheduler::VERY_LOW_IMPORTANCE,
		  time,
		  1,
		  RtecScheduler::OPERATION,
		  _env);
  if (_env.exception () != 0) return;
}

void
TAO_ECG_UDP_Receiver::open (RtecEventChannelAdmin::SupplierQOS& pub,
			    CORBA::Environment &_env)
{
  if (CORBA::is_nil (this->lcl_ec_.in ()))
    return;

  if (!CORBA::is_nil (this->consumer_proxy_.in ()))
    this->close (_env);
  if (_env.exception () != 0) return;

  if (pub.publications.length () == 0)
    return;

  for (CORBA::ULong i = 0; i < pub.publications.length (); ++i)
    {
      pub.publications[i].dependency_info.rt_info = this->lcl_info_;
    }

  // = Connect as a supplier to the local EC
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->lcl_ec_->for_suppliers (_env);
  if (_env.exception () != 0) return;

  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer (_env);
  if (_env.exception () != 0) return;

  RtecEventComm::PushSupplier_var supplier_ref =
    this->_this (_env);
  if (_env.exception () != 0) return;

  ACE_DEBUG ((LM_DEBUG, "ECG_UDP_Receiver (%t) Gateway/Supplier "));
  ACE_SupplierQOS_Factory::debug (pub);

  this->consumer_proxy_->connect_push_supplier (supplier_ref.in (),
						pub,
						_env);
  if (_env.exception () != 0) return;
}

void
TAO_ECG_UDP_Receiver::close (CORBA::Environment &env)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Closing gateway\n"));
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (env);
  if (env.exception () != 0) return;
  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();

}

void
TAO_ECG_UDP_Receiver::disconnect_push_supplier (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "ECG (%t): Supplier received "
	      "disconnect from channel.\n"));
}

void
TAO_ECG_UDP_Receiver::shutdown (CORBA::Environment& _env)
{
  this->close (_env);
  if (_env.exception () == 0) return;

  this->lcl_ec_ = RtecEventChannelAdmin::EventChannel::_nil ();
}

int
TAO_ECG_UDP_Receiver::handle_input (ACE_SOCK_Dgram& dgram)
{
  // Use ULong so the alignment is right.
  CORBA::ULong header[2];
  ACE_Addr from;

  ssize_t n = dgram.recv (header, sizeof(header), from, MSG_PEEK);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "ECG_UDP_Receive_EH::handle_input - peek\n"), -1);
  else if (n == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "ECG_UDP_Receive_EH::handle_input - peek 0\n"),
		      0);

  char* buf = ACE_reinterpret_cast(char*,header);
  int byte_order = buf[0];
  CORBA::ULong length = header[1];

  if (byte_order != TAO_ENCAP_BYTE_ORDER)
    {
      CDR::swap_4 (buf + 4,
		   ACE_reinterpret_cast(char*,&length));
    }
  
  ACE_Message_Block mb (length + CDR::MAX_ALIGNMENT);
  CDR::mb_align (&mb);
  mb.wr_ptr (length);

  n = dgram.recv (mb.rd_ptr (), length, from);

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "ECG_UDP_Receive_EH::handle_input - read\n"), -1);
  else if (n == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "ECG_UDP_Receive_EH::handle_input - read 0\n"),
		      0);
  // This is to avoid receiving the events we send; notice that we
  // drop the message after reading it.
  if (from == this->ignore_from_)
    return 0;

  TAO_TRY
    {
      TAO_InputCDR cdr (&mb, byte_order);
      cdr.skip_bytes (8); // skip the header...

      RtecEventComm::EventSet events;
      cdr.decode (RtecEventComm::_tc_EventSet, &events, 0,
		  TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumer_proxy_->push (events, TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("ECG_UDP_Receive_EH::handle_input");
    }
  TAO_ENDTRY;
  return 0;
}

// ****************************************************************

TAO_ECG_UDP_EH::TAO_ECG_UDP_EH (TAO_ECG_UDP_Receiver *recv)
  :  receiver_ (recv)
{
}

int
TAO_ECG_UDP_EH::open (const ACE_INET_Addr& ipaddr)
{
  if (this->dgram_.open (ipaddr) == -1)
    return -1;
  return this->reactor ()->register_handler (this,
					     ACE_Event_Handler::READ_MASK);
}

int
TAO_ECG_UDP_EH::close (void)
{
  if (this->reactor ()->remove_handler (this,
					ACE_Event_Handler::READ_MASK) == -1)
    return -1;
      
  return this->dgram_.close ();
}

int
TAO_ECG_UDP_EH::handle_input (ACE_HANDLE)
{
  return this->receiver_->handle_input (this->dgram_);
}

ACE_HANDLE
TAO_ECG_UDP_EH::get_handle (void) const
{
  return this->dgram_.get_handle ();
}

// ****************************************************************

TAO_ECG_Mcast_EH::TAO_ECG_Mcast_EH (TAO_ECG_UDP_Receiver *recv)
  :  receiver_ (recv)
{
}

int
TAO_ECG_Mcast_EH::open (const ACE_INET_Addr& mcast_group)
{
  if (this->dgram_.subscribe (mcast_group) == -1)
    return -1;
  return this->reactor ()->register_handler (this,
					     ACE_Event_Handler::READ_MASK);
}

int
TAO_ECG_Mcast_EH::close (void)
{
  if (this->reactor ()->remove_handler (this,
					ACE_Event_Handler::READ_MASK) == -1)
    return -1;
      
  return this->dgram_.unsubscribe ();
}

int
TAO_ECG_Mcast_EH::handle_input (ACE_HANDLE)
{
  return this->receiver_->handle_input (this->dgram_);
}

ACE_HANDLE
TAO_ECG_Mcast_EH::get_handle (void) const
{
  return this->dgram_.get_handle ();
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
