// $Id$

#include "Activation_Manager.h"

#if ! defined (__ACE_INLINE__)
#include "Activation_Manager.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_Activation_Manager, "$id$")

#include "LookupManager.h"
#include "Command_Builder.h"
#include "Name.h"

TAO_NS_Activation_Manager::TAO_NS_Activation_Manager (void)
  :active_suppliers_ (0), active_consumers_ (0), active_done_ (lock_)
{
  LOOKUP_MANAGER->_register (this);
}

TAO_NS_Activation_Manager::~TAO_NS_Activation_Manager ()
{
  delete this->barrier_;
}

void
TAO_NS_Activation_Manager::done (TAO_NS_Periodic_Supplier* supplier)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  --active_suppliers_;

  if (active_suppliers_ == 0)
    this->active_done_.broadcast ();
}

void
TAO_NS_Activation_Manager::done (TAO_NS_Periodic_Consumer* consumer)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  --active_consumers_;

  if (active_consumers_ == 0)
    this->active_done_.broadcast ();
}

void
TAO_NS_Activation_Manager::wait_for_completion (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  while (active_suppliers_ != 0 || active_consumers_ != 0)
    this->active_done_.wait (); //@@ use timed wait.
}

int
TAO_NS_Activation_Manager::supplier_count (void)
{
  return this->supplier_map_.current_size ();
}

int
TAO_NS_Activation_Manager::consumer_count (void)
{
  return this->consumer_map_.current_size ();
}

void
TAO_NS_Activation_Manager::_register (TAO_NS_Periodic_Supplier* supplier, const char* obj_name ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  // Register in supplier map.
  ACE_CString name (obj_name);

  if (this->supplier_map_.bind (name, supplier) == -1)
    ACE_ERROR ((LM_ERROR, "Failed to bind supplier %s in LookupMap\n", obj_name));

  // Designate application manager as the callback object.
  supplier->task_callback (this);

  active_suppliers_++;
}

void
TAO_NS_Activation_Manager::_register (TAO_NS_Periodic_Consumer* consumer, const char* obj_name ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  // Register in consumer map.
  ACE_CString name (obj_name);

  if (this->consumer_map_.bind (name, consumer) == -1)
    ACE_ERROR ((LM_ERROR, "Failed to bind consumer %s in LookupMap\n", obj_name));

  // Designate application manager as the callback object.
  consumer->task_callback (this);

  active_consumers_++;
}

void
TAO_NS_Activation_Manager::resolve (TAO_NS_Periodic_Supplier*& supplier, const char* obj_name ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  ACE_CString name (obj_name);

  if (this->supplier_map_.find (name, supplier) == -1)
    ACE_ERROR ((LM_ERROR, "Failed to find supplier %s in LookupMap\n", obj_name));
}

void
TAO_NS_Activation_Manager::resolve (TAO_NS_Periodic_Consumer*& consumer, const char* obj_name ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  ACE_CString name (obj_name);

  if (this->consumer_map_.find (name, consumer) == -1)
    ACE_ERROR ((LM_ERROR, "Failed to find consumer %s in LookupMap\n", obj_name));
}

int
TAO_NS_Activation_Manager::activate_suppliers (void)
{
  TAO_NS_PeriodicSupplier_Iterator iter(this->supplier_map_);

  TAO_NS_PeriodicSupplier_Entry* entry;

  // Create the barrier to synch activated auppiers.
  this->barrier_ = new ACE_Barrier (this->supplier_count () + 1);

  // For each supplier : activate
  TAO_NS_Periodic_Supplier* supplier = 0;

  for (u_int index = 0; iter.done () == 0; iter.advance (), ++index)
    {
      if (iter.next (entry) != 0)
        {
          supplier = entry->int_id_;

          if (supplier->activate (this->barrier_) != 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Supplier list activation failed\n"));
              return -1;
            }
        }
    }

  this->barrier_->wait ();
  return 0;
}

void
TAO_NS_Activation_Manager::dump_stats (void)
{
  /// dump suppliers:
  char msg[BUFSIZ];
  ACE_OS::sprintf (msg, "# Stats generated on --\n");

  TAO_NS_PeriodicSupplier_Iterator sup_iter(this->supplier_map_);

  TAO_NS_PeriodicSupplier_Entry* sup_entry;

  // For each supplier
  TAO_NS_Periodic_Supplier* supplier = 0;

  u_int index = 0;
  for (; sup_iter.done () == 0; sup_iter.advance (), ++index)
    {
      if (sup_iter.next (sup_entry) != 0)
        {
          supplier = sup_entry->int_id_;
          supplier->dump_stats (msg);
        }
    }

  // dump consumers
  TAO_NS_PeriodicConsumer_Iterator cons_iter(this->consumer_map_);

  TAO_NS_PeriodicConsumer_Entry* cons_entry;

  TAO_NS_Periodic_Consumer* consumer = 0;
  for (index = 0; cons_iter.done () == 0; cons_iter.advance (), ++index)
    {
      if (cons_iter.next (cons_entry) != 0)
        {
          consumer = cons_entry->int_id_;
          consumer->dump_stats (msg);
        }
    }
}
