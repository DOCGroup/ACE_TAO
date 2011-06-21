// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// $Id$

#include "Gate.h"
#include "ace/Truncate.h"

using namespace CORBA;
using namespace CosNotifyComm;
using namespace CosNotification;
using namespace CosNotifyChannelAdmin;

Gate::~Gate ()
{
  // Stop tracker thread.
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    stop_ = true;
  }

  thread_mgr_.wait ();
}


Gate::
Gate (ACE_INET_Addr const& group, EventChannel_ptr ch)
    : socket_ (group, false),
      stop_ (false)
{
  init (ch->default_consumer_admin (),
        ch->default_supplier_admin ());
}

Gate::
Gate (ACE_INET_Addr const& group,
      ConsumerAdmin_ptr consumer_admin,
      SupplierAdmin_ptr supplier_admin)
    : socket_ (group, false),
      stop_ (false)
{
  init (consumer_admin, supplier_admin);
}

void Gate::
init (ConsumerAdmin_ptr consumer_admin,
      SupplierAdmin_ptr supplier_admin)
{
  // Generate unique id. It is used to prevent event looping.
  //
  ACE_Utils::UUID uuid;
  ACE_Utils::UUID_GENERATOR::instance ()->init ();
  ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);

  id_ = string_alloc (ACE_Utils::truncate_cast<CORBA::ULong> (
                        uuid.to_string ()->length () + 2));
  ACE_OS::strcpy (id_.inout (), "_");
  ACE_OS::strcpy (id_.inout () + 1, uuid.to_string ()->rep ());

  // ACE_DEBUG ((LM_DEBUG, "ID: %s\n", id_.in ()));


  // Obtain proxy consumer.
  //
  ProxyConsumer_var pc (
    supplier_admin->obtain_notification_push_consumer (
      STRUCTURED_EVENT, consumer_id_));

  consumer_ = StructuredProxyPushConsumer::_narrow (pc.in ());

  consumer_->connect_structured_push_supplier (
    StructuredPushSupplier::_nil ());


  // Register as consumer.
  //
  StructuredPushConsumer_var ref (_this ()); // Activate on default POA.

  ProxySupplier_var ps (
    consumer_admin->obtain_notification_push_supplier (
      STRUCTURED_EVENT, supplier_id_));

  supplier_ = StructuredProxyPushSupplier::_narrow (ps.in ());

  supplier_->connect_structured_push_consumer (ref.in ());


  // Create tracker thread.
  //
  thread_mgr_.spawn (tracker_thunk, this);
}

ACE_THR_FUNC_RETURN Gate::
tracker_thunk (void* arg)
{
  Gate* a = reinterpret_cast<Gate*> (arg);
  a->tracker ();
  return 0;
}

void Gate::
tracker ()
{
  // Time period after which a manual cancellation request is
  // checked for.
  //
  ACE_Time_Value const timeout (0, 500);

  while (true)
  {
    ssize_t n;

    while (true)
    {
      n = socket_.size (timeout);

      // Check for cancellation request.
      //
      {
        ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

        if (stop_)
          return;
      }

      if (n == -1)
      {
        if (errno != ETIME)
          ACE_OS::abort ();
      }
      else
        break;
    }

    CORBA::ULong seqn = ACE_Utils::truncate_cast<CORBA::ULong> (n);
    OctetSeq seq (seqn);
    seq.length (seqn);

    char* buffer = reinterpret_cast<char*> (seq.get_buffer ());

    if (socket_.recv (buffer, n) != n)
    {
      ACE_ERROR ((LM_ERROR,
                  "recv() reported different size than size()\n"));
      continue;
    }

    TAO_InputCDR cdr (buffer, n);

    StructuredEvent e;

    cdr >> e;

    // Add TTL header to prevent infinite message looping.
    //
    ULong i (0);

    for (; i < e.header.variable_header.length (); ++i)
    {
      if (ACE_OS::strcmp (e.header.variable_header[i].name.in (), id_.in ()) == 0)
        break;
    }

    if (i == e.header.variable_header.length ())
    {
      e.header.variable_header.length (i + 1);

      e.header.variable_header[i].name = string_dup (id_.in ());
    }

    //ACE_DEBUG ((LM_DEBUG,
    //            "adding %s as header #%d\n",
    //            e.header.variable_header[i].name.in (), i));

    e.header.variable_header[i].value <<= ULong (1);

    /*
    cerr << "IN: "
         << e.header.fixed_header.event_type.domain_name << "::"
         << e.header.fixed_header.event_type.type_name << " "
         << e.header.fixed_header.event_name << endl;
    */

    consumer_->push_structured_event (e);
  }
}

void Gate::
push_structured_event (StructuredEvent const& e)
{
  for (ULong i (0); i < e.header.variable_header.length (); ++i)
  {
    if (ACE_OS::strcmp (e.header.variable_header[i].name.in (), id_.in ()) == 0)
    {
      ULong ttl;

      e.header.variable_header[i].value >>= ttl;

      if (ttl <= 1)
      {
        //ACE_DEBUG ((LM_DEBUG,
        //            "DROPPED\n"));
        return;
      }

      break;
    }
  }

  /*
  cerr << "OUT: "
       << e.header.fixed_header.event_type.domain_name << "::"
       << e.header.fixed_header.event_type.type_name << " "
       << e.header.fixed_header.event_name << endl;
  */

  TAO_OutputCDR cdr;

  cdr << e;

  CORBA::ULong size (ACE_Utils::truncate_cast<CORBA::ULong> (cdr.total_length ()));

  OctetSeq seq (size);
  seq.length (size);

  char* buffer = reinterpret_cast<char*> (seq.get_buffer ());

  {
    char* buf = buffer;

    for (ACE_Message_Block const* mb = cdr.begin ();
         mb != 0;
         mb = mb->cont ())
    {
      ACE_OS::memcpy (buf, mb->rd_ptr (), mb->length ());
      buf += mb->length ();
    }
  }

  socket_.send (buffer, size);
}


void
Gate::disconnect_structured_push_consumer (void)
{
  // We don't care.
}

void
Gate::offer_change (EventTypeSeq const&, EventTypeSeq const&)
{
  // We don't care.
}
