#include "Stock_i.h"
#include <orbsvcs/Event_Utilities.h>
#include <atomic>

/// Derive a tie class that releases itself when its CORBA reference count reaches zero.
class MyTieStock : public POA_RtecEventComm::PushSupplier_tie<Quoter_Stock_i>
{
  friend class Quoter_Stock_i;
  std::atomic<long> ref_count_ {};

protected:
  MyTieStock (Quoter_Stock_i *tp,
              CORBA::Boolean release)
    : POA_RtecEventComm::PushSupplier_tie<Quoter_Stock_i> (tp, release) {}

  MyTieStock (MyTieStock const &) = delete;
  MyTieStock (MyTieStock &&) = delete;
  MyTieStock &operator= (MyTieStock const &) = delete;
  MyTieStock &operator= (MyTieStock &&) = delete;

public:
  void _add_ref () override
  {
    this->ref_count_.fetch_add (1, std::memory_order_relaxed);
  }

  void _remove_ref () override
  {
    if (this->ref_count_.fetch_sub (1, std::memory_order_acq_rel) == 1)
      delete this;
  }
};

Quoter_Stock_i::Quoter_Stock_i (std::string const &symbol,
                                std::string const &full_name,
                                CORBA::Double price)
  : supplier_personality_ (*new MyTieStock (this, false))
{
  this->data_.symbol = symbol.c_str ();
  this->data_.full_name = full_name.c_str ();
  this->data_.price = price;
}

Quoter_Stock_i::~Quoter_Stock_i ()
{
  if (!CORBA::is_nil (this->consumer_proxy_.in ()))
    this->consumer_proxy_->disconnect_push_consumer ();
}

char *
Quoter_Stock_i::symbol ()
{
  return CORBA::string_dup (this->data_.symbol.in ());
}

char *
Quoter_Stock_i::full_name ()
{
  return CORBA::string_dup (this->data_.full_name.in ());
}

CORBA::Double
Quoter_Stock_i::price ()
{
  return this->data_.price;
}

void
Quoter_Stock_i::set_price (CORBA::Double new_price)
{
  this->data_.price = new_price;
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  // Create the event
  RtecEventComm::EventSet event (1);
  event.length (1);
  RtecEventComm::Event &e = event[0];

  // Initialize the header
  char const *symbol = this->data_.symbol;
  e.header.type = static_cast<CORBA::ULong> (static_cast<unsigned char> (symbol[0])) << 24;
  e.header.type |= static_cast<CORBA::ULong> (static_cast<unsigned char> (symbol[1])) << 16;
  e.header.type |= static_cast<CORBA::ULong> (static_cast<unsigned char> (symbol[2])) << 8;
  e.header.type |= static_cast<CORBA::ULong> (static_cast<unsigned char> (symbol[3]));
  e.header.source = 1;

  // Initialize the payload
  e.data.any_value <<= this->data_;

  // Push it
  this->consumer_proxy_->push (event);
}

void
Quoter_Stock_i::disconnect_push_supplier ()
{
  // Forget about the consumer it is not there anymore
  this->consumer_proxy_ = RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
Quoter_Stock_i::connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin)
{
  this->consumer_proxy_ = supplier_admin->obtain_push_consumer ();
  RtecEventComm::PushSupplier_var supplier (this->supplier_personality_._this ());

  char const *symbol = this->data_.symbol;
  CORBA::ULong const type =
    static_cast<CORBA::ULong> (static_cast<unsigned char> (symbol[0])) << 24 |
    static_cast<CORBA::ULong> (static_cast<unsigned char> (symbol[1])) << 16 |
    static_cast<CORBA::ULong> (static_cast<unsigned char> (symbol[2])) << 8 |
    static_cast<CORBA::ULong> (static_cast<unsigned char> (symbol[3]));

  ACE_SupplierQOS_Factory publications;
  publications.insert (1, type, 0, 1);

  this->consumer_proxy_->connect_push_supplier (supplier.in (),
                                                publications.get_SupplierQOS ());
}

