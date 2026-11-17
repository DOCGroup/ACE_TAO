#include "Activation_Manager.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_netdb.h"
#include "LookupManager.h"
#include "Command_Builder.h"
#include "Name.h"
#include "ace/Barrier.h"

void
TAO_Notify_Tests_Activation_Manager::File_Closer::operator() (FILE *file) const noexcept
{
  ACE_OS::fclose (file);
}

TAO_Notify_Tests_Activation_Manager::TAO_Notify_Tests_Activation_Manager ()
  : active_suppliers_ {}
  , active_consumers_ {}
  , active_done_ (lock_)
  , started_ {}
  , started_condition_ (lock_)
{
  LOOKUP_MANAGER->_register (this);
}

TAO_Notify_Tests_Activation_Manager::~TAO_Notify_Tests_Activation_Manager ()
= default;

int
TAO_Notify_Tests_Activation_Manager::ior_output_file (ACE_TCHAR const *file_name)
{
  FILE * const output = ACE_OS::fopen (file_name, ACE_TEXT ("w"));
  if (output == nullptr)
    {
      return -1;
    }

  this->ior_output_file_.reset (output);
  return 0;
}

int
TAO_Notify_Tests_Activation_Manager::ior_input_file (ACE_TCHAR const *file_name)
{
  this->ior_input_file_ = file_name;
  return 0;
}

void
TAO_Notify_Tests_Activation_Manager::done (TAO_Notify_Tests_Periodic_Supplier * /*supplier*/)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  --active_suppliers_;

  if (active_suppliers_ == 0)
    {
      this->active_done_.broadcast ();
    }
}

void
TAO_Notify_Tests_Activation_Manager::done (TAO_Notify_Tests_Periodic_Consumer * /*consumer*/)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  --active_consumers_;

  if (active_consumers_ == 0)
    {
      this->active_done_.broadcast ();
    }
}

void
TAO_Notify_Tests_Activation_Manager::wait_for_completion ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  while (active_suppliers_ != 0 || active_consumers_ != 0)
    {
      this->active_done_.wait (); //@@ use timed wait.
    }
}

int
TAO_Notify_Tests_Activation_Manager::supplier_count ()
{
  return ACE_Utils::truncate_cast<int> (this->supplier_map_.current_size ());
}

int
TAO_Notify_Tests_Activation_Manager::consumer_count ()
{
  return ACE_Utils::truncate_cast<int> (this->consumer_map_.current_size ());
}

void
TAO_Notify_Tests_Activation_Manager::_register (TAO_Notify_Tests_Periodic_Supplier *supplier,
                                                char const *obj_name)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  // Register in supplier map.
  ACE_CString const name (obj_name);

  if (this->supplier_map_.bind (name, supplier) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Failed to bind supplier %s in LookupMap\n", obj_name));
    }

  // Designate application manager as the callback object.
  supplier->task_callback (this);

  ++active_suppliers_;
}

void
TAO_Notify_Tests_Activation_Manager::_register (TAO_Notify_Tests_Periodic_Consumer *consumer,
                                                char const *obj_name)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  // Register in consumer map.
  ACE_CString const name (obj_name);

  if (this->consumer_map_.bind (name, consumer) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Failed to bind consumer %s in LookupMap\n", obj_name));
    }

  // Designate application manager as the callback object.
  consumer->task_callback (this);

  ++active_consumers_;
}

void
TAO_Notify_Tests_Activation_Manager::resolve (TAO_Notify_Tests_Periodic_Supplier *&supplier,
                                              char const *obj_name)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  ACE_CString const name (obj_name);

  if (this->supplier_map_.find (name, supplier) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Failed to find supplier %s in LookupMap\n", obj_name));
    }
}

void
TAO_Notify_Tests_Activation_Manager::resolve (TAO_Notify_Tests_Periodic_Consumer *&consumer,
                                              char const *obj_name)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  ACE_CString const name (obj_name);

  if (this->consumer_map_.find (name, consumer) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Failed to find consumer %s in LookupMap\n", obj_name));
    }
}

int
TAO_Notify_Tests_Activation_Manager::activate_suppliers ()
{
  TAO_Notify_Tests_PeriodicSupplier_Iterator iter(this->supplier_map_);

  // Create the barrier that synchronizes activated suppliers.
  this->barrier_ = std::make_unique<ACE_Barrier> (this->supplier_count () + 1);

  for (; iter.done () == 0; iter.advance ())
    {
      TAO_Notify_Tests_PeriodicSupplier_Entry *entry {};
      if (iter.next (entry) != 0)
        {
          TAO_Notify_Tests_Periodic_Supplier * const supplier = entry->int_id_;
          if (supplier->activate_task (this->barrier_.get ()) != 0)
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
TAO_Notify_Tests_Activation_Manager::dump_stats (int dump_samples)
{
  char hostname[MAXHOSTNAMELEN] {};
  ACE_OS::hostname (hostname, MAXHOSTNAMELEN);

  ACE_TCHAR msg[BUFSIZ] {};
  ACE_OS::sprintf (msg, ACE_TEXT("# Stats generated on %s\n"), hostname);

  TAO_Notify_Tests_PeriodicSupplier_Iterator sup_iter(this->supplier_map_);

  for (; sup_iter.done () == 0; sup_iter.advance ())
    {
      TAO_Notify_Tests_PeriodicSupplier_Entry *sup_entry {};
      if (sup_iter.next (sup_entry) != 0)
        {
          TAO_Notify_Tests_Periodic_Supplier * const supplier = sup_entry->int_id_;
          supplier->dump_stats (msg, dump_samples);
        }
    }

  TAO_Notify_Tests_PeriodicConsumer_Iterator cons_iter(this->consumer_map_);

  for (; cons_iter.done () == 0; cons_iter.advance ())
    {
      TAO_Notify_Tests_PeriodicConsumer_Entry *cons_entry {};
      if (cons_iter.next (cons_entry) != 0)
        {
          TAO_Notify_Tests_Periodic_Consumer * const consumer = cons_entry->int_id_;
          consumer->dump_stats (msg, dump_samples);
        }
    }
}

void
TAO_Notify_Tests_Activation_Manager::write_ior ()
{
  PortableServer::ServantBase_var const servant_var (this);

  Notify_Test::Activation_Manager_var const am_object = _this ();

  CORBA::ORB_var orb;
  LOOKUP_MANAGER->resolve (orb);

  // Write IOR to a file, if asked.
  CORBA::String_var const str =
    orb->object_to_string (am_object.in ());

  if (this->ior_output_file_)
    {
      FILE * const output = this->ior_output_file_.release ();
      int const write_result = ACE_OS::fprintf (output, "%s", str.in ());
      int const close_result = ACE_OS::fclose (output);
      if (write_result < 0 || close_result != 0)
        {
          ACE_ERROR ((LM_ERROR, "Failed to write activation manager IOR\n"));
        }
    }
}

void
TAO_Notify_Tests_Activation_Manager::wait_for_start_signal ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  while (!this->started_)
    {
      this->started_condition_.wait (); //@@ use timed wait.
    }
}

void
TAO_Notify_Tests_Activation_Manager::start ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  this->started_ = true;

  this->started_condition_.signal (); //@@ use timed wait.
}

void
TAO_Notify_Tests_Activation_Manager::signal_peer ()
{
  CORBA::ORB_var orb;
  LOOKUP_MANAGER->resolve (orb);

  CORBA::Object_var const object =
    orb->string_to_object (this->ior_input_file_.c_str ());

  Notify_Test::Activation_Manager_var const peer = Notify_Test::Activation_Manager::_narrow (object.in ());

  if (CORBA::is_nil (peer.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Object reference <%s> is nil\n",
                  this->ior_input_file_.c_str ()));
      return;
    }

  peer->start ();
}
