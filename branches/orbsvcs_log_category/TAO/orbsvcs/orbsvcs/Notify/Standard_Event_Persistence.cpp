// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Standard_Event_Persistence.h"
#include "orbsvcs/Notify/Persistent_File_Allocator.h"
#include "tao/debug.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_strings.h"
#include "ace/Truncate.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{

Standard_Event_Persistence::Standard_Event_Persistence ()
  : filename_ (ACE_TEXT ("__PERSISTENT_EVENT__.DB"))
  , block_size_ (512)
  , factory_ (0)
{
}

Standard_Event_Persistence::~Standard_Event_Persistence ()
{
}

// get the current factory, creating it if necessary
Event_Persistence_Factory *
Standard_Event_Persistence::get_factory ()
{
  //@@todo guard? ; doublecheck?
  if (this->factory_ == 0)
  {
    ACE_NEW_NORETURN (
      this->factory_,
      Standard_Event_Persistence_Factory ()
      );
    if (this->factory_ != 0)
    {
      if (!this->factory_->open (this->filename_.c_str ()))
      {
        this->factory_ = 0;
      }
    }
  }
  return this->factory_;
}

// release the current factory so a new one can be created
void
Standard_Event_Persistence::reset ()
{
  delete this->factory_;
  this->factory_ = 0;
}

int
Standard_Event_Persistence::init (int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  bool verbose = false;
  for (int narg = 0; narg < argc; ++narg)
  {
    ACE_TCHAR * av = argv[narg];
    if (ACE_OS::strcasecmp (av, ACE_TEXT ("-v")) == 0)
    {
      verbose = true;
      ORBSVCS_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Standard_Event_Persistence: -verbose\n")
        ));
    }
    else if (ACE_OS::strcasecmp (av, ACE_TEXT ("-file_path")) == 0 && narg + 1 < argc)
    {
      this->filename_ = argv[narg + 1];
      if (TAO_debug_level > 0 || verbose)
      {
        ORBSVCS_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Standard_Event_Persistence: Setting -file_path: %s\n"),
          this->filename_.c_str ()
        ));
      }
      narg += 1;
    }
    else if (ACE_OS::strcasecmp (av, ACE_TEXT ("-block_size")) == 0 && narg + 1 < argc)
    {
      this->block_size_ = ACE_OS::atoi(argv[narg + 1]);
      if (TAO_debug_level > 0 || verbose)
      {
        ORBSVCS_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Standard_Event_Persistence: Setting -block_size: %d\n"),
          this->block_size_
        ));
      }
      narg += 1;
    }
    else
    {
      ORBSVCS_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Unknown parameter to Standard Event Persistence: %s\n"),
        argv[narg]
        ));
      result = -1;
    }
  }
  return result;
}

int
Standard_Event_Persistence::fini ()
{
  delete this->factory_;
  this->factory_ = 0;
  return 0;
}

Standard_Event_Persistence_Factory::Standard_Event_Persistence_Factory ()
  : allocator_()
  , root_(this)
  , psb_(0)
  , serial_number_(ROUTING_SLIP_ROOT_SERIAL_NUMBER + 1)
  , is_reloading_ (false)
{
}

bool
Standard_Event_Persistence_Factory::open (const ACE_TCHAR* filename,
                                          ACE_UINT32 block_size)
{
  bool result = false;
  if (allocator_.open (filename, block_size))
  {
    this->is_reloading_ = this->root_.load(ROUTING_SLIP_ROOT_BLOCK_NUMBER, ROUTING_SLIP_ROOT_SERIAL_NUMBER);
    if (! this->is_reloading_)
    {
      ACE_ASSERT (this->psb_ == 0);
//      this->psb_ = this->allocator_.allocate();
      this->root_.store_root();
    }
    result = true;
  }
  return result;
}

Standard_Event_Persistence_Factory::~Standard_Event_Persistence_Factory()
{
  if (TAO_debug_level > 0)
  {
    ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Standard_Event_Persistence_Factory::~Standard_Event_Persistence_Factory\n")
    ));
  }
  this->root_.release_all ();
  delete this->psb_;
  this->psb_ = 0;
  this->allocator_.shutdown();
}

Routing_Slip_Persistence_Manager*
Standard_Event_Persistence_Factory::create_routing_slip_persistence_manager(
  Persistent_Callback* callback)
{
  Routing_Slip_Persistence_Manager* rspm = 0;
  ACE_NEW_RETURN(rspm, Routing_Slip_Persistence_Manager(this), rspm);
  rspm->set_callback(callback);
  return rspm;
}

Routing_Slip_Persistence_Manager *
Standard_Event_Persistence_Factory::first_reload_manager()
{
  Routing_Slip_Persistence_Manager * result = 0;
  if (this->is_reloading_)
  {
    result = this->root_.load_next();
  }
  return result;
}

void
Standard_Event_Persistence_Factory::done_reloading(
  Persistent_Storage_Block * next_psb,
  ACE_UINT64 current_serial_number)
{
  ACE_ASSERT (this->psb_ == 0);
  this->psb_ = next_psb;
  this->serial_number_ = current_serial_number;
  this->is_reloading_ = false;
}

void
Standard_Event_Persistence_Factory::preallocate_next_record(
  ACE_UINT64& current_serial_number,
  Persistent_Storage_Block*& current_psb,
  ACE_UINT64& next_serial_number,
  ACE_UINT32& next_block_number)
{
  // return current serial number and
  // a psb containing current record number
  current_serial_number = this->serial_number_;
  this->psb_->set_allocator_owns(false); // give up ownership
  this->psb_->set_sync();
  current_psb = this->psb_;
  this->get_preallocated_pointer (next_serial_number, next_block_number);
}

void
Standard_Event_Persistence_Factory::get_preallocated_pointer(
  ACE_UINT64& next_serial_number,
  ACE_UINT32& next_block_number)
{
  ++this->serial_number_;
  this->psb_ = this->allocator_.allocate();

  next_serial_number = this->serial_number_;
  next_block_number = ACE_Utils::truncate_cast<ACE_UINT32> (this->psb_->block_number());
}

Persistent_File_Allocator*
Standard_Event_Persistence_Factory::allocator()
{
  return &this->allocator_;
}

Routing_Slip_Persistence_Manager &
Standard_Event_Persistence_Factory::root()
{
  return this->root_;
}

} // End TAO_Notify_Namespace

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_NAMESPACE_DEFINE (TAO_Notify_Serv,
                              TAO_Notify_Standard_Event_Persistence,
                              TAO_Notify::Standard_Event_Persistence)
