// $Id$

#include "Activation_Manager.h"

ACE_RCSID(lib, TAO_Activation_Manager, "$id$")

#include "LookupManager.h"
#include "Command_Builder.h"
#include "Name.h"

TAO_NS_Activation_Manager::TAO_NS_Activation_Manager (void)
  : active_suppliers_ (0)
    , active_consumers_ (0)
    , active_done_ (lock_)
    , ior_output_file_ (0)
    , started_ (0)
    , started_condition_ (lock_)
{
  LOOKUP_MANAGER->_register (this);
}

TAO_NS_Activation_Manager::~TAO_NS_Activation_Manager ()
{
  delete this->barrier_;

  if (this->ior_output_file_)
    ACE_OS::fclose(this->ior_output_file_);
}

int
TAO_NS_Activation_Manager::ior_output_file (const ACE_TCHAR *file_name)
{
  this->ior_output_file_ = ACE_OS::fopen (file_name, ACE_LIB_TEXT("w"));

  if (this->ior_output_file_ == 0)
    return -1;
  else
    return 0;
}

int
TAO_NS_Activation_Manager::ior_input_file (const ACE_TCHAR *file_name)
{
  this->ior_input_file_ = file_name;
  return 0;
}

void
TAO_NS_Activation_Manager::done (TAO_NS_Periodic_Supplier* /*supplier*/)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  --active_suppliers_;

  if (active_suppliers_ == 0)
    this->active_done_.broadcast ();
}

void
TAO_NS_Activation_Manager::done (TAO_NS_Periodic_Consumer* /*consumer*/)
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
TAO_NS_Activation_Manager::_register (TAO_NS_Periodic_Supplier* supplier, const char* obj_name ACE_ENV_ARG_DECL_NOT_USED)
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
TAO_NS_Activation_Manager::_register (TAO_NS_Periodic_Consumer* consumer, const char* obj_name ACE_ENV_ARG_DECL_NOT_USED)
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
TAO_NS_Activation_Manager::resolve (TAO_NS_Periodic_Supplier*& supplier, const char* obj_name ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  ACE_CString name (obj_name);

  if (this->supplier_map_.find (name, supplier) == -1)
    ACE_ERROR ((LM_ERROR, "Failed to find supplier %s in LookupMap\n", obj_name));
}

void
TAO_NS_Activation_Manager::resolve (TAO_NS_Periodic_Consumer*& consumer, const char* obj_name ACE_ENV_ARG_DECL_NOT_USED)
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

          if (supplier->activate_task (this->barrier_) != 0)
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
TAO_NS_Activation_Manager::dump_stats (int dump_samples)
{
  char hostname[MAXHOSTNAMELEN];
  ACE_OS::hostname (hostname, MAXHOSTNAMELEN);

  /// dump suppliers:
  char msg[BUFSIZ];
  ACE_OS::sprintf (msg, "# Stats generated on %s\n", hostname);

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
          supplier->dump_stats (msg, dump_samples);
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
          consumer->dump_stats (msg, dump_samples);
        }
    }
}

void
TAO_NS_Activation_Manager::write_ior (ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::ServantBase_var servant_var (this);

  Notify_Test::Activation_Manager_var am_object = _this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ORB_var orb;
  LOOKUP_MANAGER->resolve (orb);

  // Write IOR to a file, if asked.
  CORBA::String_var str =
    orb->object_to_string (am_object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       str.in ());
      ACE_OS::fclose (this->ior_output_file_);
      this->ior_output_file_ = 0;
  }
}

void
TAO_NS_Activation_Manager::wait_for_start_signal (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  while (this->started_ == 0)
    this->started_condition_.wait (); //@@ use timed wait.
}

void
TAO_NS_Activation_Manager::start (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  this->started_ = 1;

  this->started_condition_.signal (); //@@ use timed wait.
}

void
TAO_NS_Activation_Manager::signal_peer (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::ORB_var orb;
  LOOKUP_MANAGER->resolve (orb);

  CORBA::Object_var object =
    orb->string_to_object (this->ior_input_file_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  Notify_Test::Activation_Manager_var peer = Notify_Test::Activation_Manager::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (peer.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Object reference <%s> is nil\n",
                  this->ior_input_file_.c_str ()));
    }

  peer->start (ACE_ENV_SINGLE_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager<ACE_CString, TAO_NS_Periodic_Supplier*, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Manager<ACE_CString, TAO_NS_Periodic_Consumer*, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator<ACE_CString, TAO_NS_Periodic_Supplier*, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator<ACE_CString, TAO_NS_Periodic_Consumer*, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Entry<ACE_CString, TAO_NS_Periodic_Supplier*>;
template class ACE_Hash_Map_Entry<ACE_CString, TAO_NS_Periodic_Consumer*>;

template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_NS_Periodic_Supplier*, ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_NS_Periodic_Consumer*, ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>, ACE_SYNCH_NULL_MUTEX>;

template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_NS_Periodic_Supplier*, ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_NS_Periodic_Consumer*, ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>, ACE_SYNCH_NULL_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, TAO_NS_Periodic_Supplier*, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, TAO_NS_Periodic_Consumer*, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString, TAO_NS_Periodic_Supplier*, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString, TAO_NS_Periodic_Consumer*, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, TAO_NS_Periodic_Supplier*>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, TAO_NS_Periodic_Consumer*>

#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_NS_Periodic_Supplier*, ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_NS_Periodic_Consumer*, ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>, ACE_SYNCH_NULL_MUTEX>

#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_NS_Periodic_Supplier*, ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_NS_Periodic_Consumer*, ACE_Hash<ACE_CString>,ACE_Equal_To<ACE_CString>, ACE_SYNCH_NULL_MUTEX>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
