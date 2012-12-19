// $Id$

#include "Shared_Backing_Store.h"
#include "Server_Info.h"
#include "Activator_Info.h"
#include "utils.h"
#include "Locator_XMLHandler.h"
#include "ImR_LocatorC.h"
#include "ace/File_Lock.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_unistd.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/XML_Util.h"
#include "tao/IORManipulation/IORManip_Loader.h"

namespace {
  class Lockable_File
  {
  public:
    Lockable_File()
    : file_(0),
      flags_(0),
      locked_(false),
      unlink_in_destructor_(false)
    {
      ACE_DEBUG((LM_INFO, "(%P|%t) %d: Lockable_file\n", this));
    }

    Lockable_File(const ACE_TString& file,
                  const int flags,
                  bool unlink_in_destructor = false)
    : file_(0),
      flags_(0),
      locked_(false),
      unlink_in_destructor_(false)
    {
      ACE_DEBUG((LM_INFO, "(%P|%t) %d: Lockable_file(%s,%d,%d)\n", this, file.c_str(), flags, unlink_in_destructor));
      init_fl(file, flags, unlink_in_destructor);
//      ACE_DEBUG((LM_INFO, "(%P|%t) %d: Lockable_file done\n", this));
    }

    ~Lockable_File()
    {
      release();
    }

    void release()
    {
      if (this->file_ == 0)
        return;

//      ACE_DEBUG((LM_INFO, "(%P|%t) %d: release\n", this));
      close_file();
      ACE_DEBUG((LM_INFO, "(%P|%t) %d: release lock\n", this));
      this->file_lock_.reset();
      this->locked_ = false;
      ACE_DEBUG((LM_INFO, "(%P|%t) %d: release done\n", this));
    }

    FILE* get_file()
    {
      lock();

//      ACE_DEBUG((LM_INFO, "(%P|%t) %d: get_file %d\n", this, release_stream));
      return this->file_;
    }

    FILE* get_file(const ACE_TString& file,
                   const int flags,
                   bool unlink_in_destructor = false)
    {
      if (!locked_) ACE_DEBUG((LM_INFO, "(%P|%t) %d: get_file(%s,%d,%d)\n", this, file.c_str(), flags, unlink_in_destructor));
      init_fl(file, flags, unlink_in_destructor);
//      ACE_DEBUG((LM_INFO, "(%P|%t) %d: return get_file\n", this));
      return get_file();
    }

  private:
    void init_fl(const ACE_TString& file,
                 const int flags,
                 bool unlink_in_destructor = false)
    {
      release();

      flags_ = flags | O_CREAT;
      unlink_in_destructor_ = unlink_in_destructor;

      const ACE_TCHAR* const flags_str =
        ((flags_ & O_RDWR) != 0) ? ACE_TEXT("r+") :
        (((flags_ & O_WRONLY) != 0) ? ACE_TEXT("w") : ACE_TEXT("r"));
#ifdef ACE_WIN32
      this->filename_ = file;
      this->file_ = ACE_OS::fopen(file.c_str(), flags_str);
#else
      this->file_lock_.reset(
        new ACE_File_Lock(ACE_TEXT_CHAR_TO_TCHAR(file.c_str ()),
                          flags_,
                          0666,
                          unlink_in_destructor));
      this->file_ = ACE_OS::fdopen(this->file_lock_->get_handle(), flags_str);
#endif
      ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) %d: created file_lock_ file_=%d, mode=%C\n"), this, file_, flags_str));
    }

    void close_file()
    {
      if (this->file_ == 0)
        return;

//      ACE_DEBUG((LM_INFO, "(%P|%t) %d: close file\n", this));
      ACE_OS::fclose(this->file_);
      this->file_ = 0;
#ifdef ACE_WIN32
      if (this->unlink_in_destructor_)
        {
          ACE_OS::unlink(this->filename_.c_str());
          this->unlink_in_destructor_ = false;
        }
#endif
//      ACE_DEBUG((LM_INFO, "(%P|%t) %d: close file done\n", this));
    }

    void lock()
    {
#ifndef ACE_WIN32
      if (this->locked_)
        return;

      ACE_DEBUG((LM_INFO, "(%P|%t) %d: acquire %d\n", this, flags_));
      int ret = -1;
      if ((this->flags_ & O_RDWR) != 0)
        ret = file_lock_->acquire();
      if ((this->flags_ & O_WRONLY) != 0)
        ret = file_lock_->acquire_write();
      else
        ret = file_lock_->acquire_read();

      ACE_DEBUG((LM_INFO, "(%P|%t) %d: acquired %d\n", this, flags_));
      this->locked_ = true;
#endif
    }

    auto_ptr<ACE_File_Lock> file_lock_;
    FILE* file_;
    int flags_;
    bool locked_;
    bool unlink_in_destructor_;
    ACE_TString filename_;
  };
}

Shared_Backing_Store::Shared_Backing_Store(const Options& opts,
                                           CORBA::ORB_ptr orb)
: XML_Backing_Store(opts, orb, true),
  listing_file_(opts.persist_file_name() + ACE_TEXT("imr_listing.xml")),
  seq_num_(0),
  replica_seq_num_(0),
  imr_type_(opts.imr_type())
{
  IMR_REPLICA[Options::PRIMARY_IMR] = "ImR_ReplicaPrimary";
  IMR_REPLICA[Options::BACKUP_IMR] = "ImR_ReplicaBackup";
  IMR_REPLICA[Options::STANDALONE_IMR] = "ImR_NoReplica";
}

Shared_Backing_Store::~Shared_Backing_Store()
{
}


static void replicate(
  Shared_Backing_Store::Replica_ptr replica,
  const ImplementationRepository::ServerUpdate& update)
{
  // replicate the ServerUpdate to our replicated locator
  replica->notify_updated_server(update);
}

static void replicate(
  Shared_Backing_Store::Replica_ptr replica,
  const ImplementationRepository::ActivatorUpdate& update)
{
  // replicate the ActivatorUpdate to our replicated locator
  replica->notify_updated_activator(update);
}

template< typename Update>
static void replicate(
  Shared_Backing_Store::Replica_ptr replica,
  const ACE_CString& name,
  const ImplementationRepository::UpdateType type,
  const ImplementationRepository::SequenceNum seq_num)
{
  if (CORBA::is_nil (replica))
    {
      return;
    }

  ACE_DEBUG((LM_INFO, "(%P|%t) REPLICATE!!!!!!!!!!!!!!!!!!\n"));
  try
    {
      Update update;
      update.name = name.c_str();
      update.type = type;
      update.seq_num = seq_num;
      replicate(replica, update);
    }
  catch (const CORBA::COMM_FAILURE&)
    {
      ACE_DEBUG ((LM_DEBUG,
        "(%P|%t) Replicated ImR: COMM_FAILURE Exception\n"));
    }
  catch (const CORBA::TRANSIENT&)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Replicated ImR: TRANSIENT Exception\n"));
    }
  catch (const CORBA::OBJECT_NOT_EXIST&)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Replicated ImR: OBJECT_NOT_EXIST "
        "Exception, dropping replica\n"));
      replica = TAO::Objref_Traits
        <ImplementationRepository::UpdatePushNotification>::nil ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT("Replicated ImR: notify update"));
      replica = 0;
    }
  ACE_DEBUG((LM_INFO, "(%P|%t) REPLICATED!!!!!!!!!!!!!!!!!!\n"));
}

int
Shared_Backing_Store::persistent_remove (const ACE_CString& name,
                                         bool activator)
{
  Lockable_File listing_lf;
  const int err = persist_listings(listing_lf);
  if (err != 0)
    {
      return err;
    }

  const ACE_TString fname = make_filename(name, activator);

  {
    // take the lock, then remove the file
    Lockable_File file(fname, O_WRONLY, true);
  }
  listing_lf.release();

  ++seq_num_;
  if (activator)
    {
      replicate<ImplementationRepository::ActivatorUpdate>
        (replica_, name, ImplementationRepository::repo_remove, seq_num_);
    }
  else
    {
      replicate<ImplementationRepository::ServerUpdate>
        (replica_, name, ImplementationRepository::repo_remove, seq_num_);
    }
  return 0;
}

ACE_TString
Shared_Backing_Store::make_filename(const ACE_CString& name,
                                    bool activator,
                                    bool relative) const
{
  const char* const prefix = (activator ? "a_" : "s_");
  ACE_CString file = prefix + name + ".xml";
  if (!relative)
    {
      file = this->filename_ + file;
    }
  return ACE_TEXT_CHAR_TO_TCHAR(file);
}

int
Shared_Backing_Store::persistent_update(const Server_Info_Ptr& info, bool add)
{
  Lockable_File listing_lf;
  if (add)
    {
      const int err = persist_listings(listing_lf);
      if (err != 0)
        {
          return err;
        }
    }

  ACE_CString name = ACEXML_escape_string (info->name);

  const ACE_TString fname = make_filename(name, false);
  if (this->opts_.debug() > 9)
    {
      ACE_DEBUG((LM_INFO, ACE_TEXT ("Persisting to %s(%C)\n"),
        fname.c_str(), info->name.c_str()));
    }
  Lockable_File server_file(fname, O_WRONLY);
  FILE* fp = server_file.get_file();
  if (fp == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Couldn't write to file %s\n"),
        fname.c_str()));
      return -1;
    }
  // successfully added file (if adding), so release the listing file lock
  listing_lf.release();
  ACE_OS::fprintf (fp,"<?xml version=\"1.0\"?>\n");
  persist(fp, *info, "");
  server_file.release();

  const ImplementationRepository::UpdateType type = add ?
    ImplementationRepository::repo_add :
    ImplementationRepository::repo_update;
  replicate<ImplementationRepository::ServerUpdate>
    (replica_, name, type, ++seq_num_);
  return 0;
}


int
Shared_Backing_Store::persistent_update(const Activator_Info_Ptr& info,
                                        bool add)
{
  Lockable_File listing_lf;
  if (add)
    {
      const int err = persist_listings(listing_lf);
      if (err != 0)
        {
          return err;
        }
    }

  ACE_CString name = lcase (info->name);

  const ACE_TString fname = make_filename(name, true);
  if (this->opts_.debug() > 9)
    {
      ACE_DEBUG((LM_INFO, ACE_TEXT ("Persisting to %s(%s)\n"),
        fname.c_str(), info->name.c_str()));
    }
  Lockable_File activator_file(fname, O_WRONLY);
  FILE* fp = activator_file.get_file();
  if (fp == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Couldn't write to file %s\n"),
                  fname.c_str()));
      return -1;
    }
  // successfully added file (if adding), so release the listing file lock
  listing_lf.release();
  ACE_OS::fprintf (fp,"<?xml version=\"1.0\"?>\n");
  persist(fp, *info, "");
  activator_file.release();

  const ImplementationRepository::UpdateType type = add ?
    ImplementationRepository::repo_add :
    ImplementationRepository::repo_update;
  replicate<ImplementationRepository::ActivatorUpdate>
    (replica_, name, type, ++seq_num_);
  return 0;
}

const char*
Shared_Backing_Store::repo_mode() const
{
  return this->listing_file_.c_str();
}

int
Shared_Backing_Store::connect_replicas (Replica_ptr this_replica)
{
  const ACE_TString& replica_ior = replica_ior_filename(true);
  if (this->opts_.debug() > 1)
    {
      ACE_DEBUG((LM_INFO,
        "Resolving ImR replica %s\n", replica_ior.c_str()));
    }

  CORBA::Object_var obj =
    this->orb_->string_to_object (replica_ior.c_str());

  if (CORBA::is_nil (obj.in ()))
    {
      if (this->imr_type_ == Options::BACKUP_IMR)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
            "Error: No primary ImR replica file found <%s>\n",
            replica_ior.c_str()), -1);
        }

      // no connection currently, just wait for backup
      return 0;
    }

  ACE_DEBUG((LM_INFO, "narrowing replica\n"));
  this->replica_ =
    ImplementationRepository::UpdatePushNotification::_unchecked_narrow (obj.in());
  if (this->replica_->_non_existent() == 1)
    {
      this->replica_ = ImplementationRepository::UpdatePushNotification::_nil();
    }

  ACE_DEBUG((LM_INFO, "check replica\n"));
  if (CORBA::is_nil (this->replica_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
      "Error: obj key <%s> not an ImR replica\n",
      replica_ior.c_str()), -1);

  if (opts_.debug() > 1)
    {
      ACE_DEBUG((LM_INFO,
        "Registering with previously running ImR replica\n"));
    }

  try
    {
      this->replica_->register_replica(this_replica,
                                       this->imr_ior_.inout(),
                                       this->replica_seq_num_);
      ACE_DEBUG((LM_INFO, "(%P|%t) ???????? Registered with replica <%C>\n", this->imr_ior_.in()));
    }
  catch (const ImplementationRepository::InvalidPeer& ip)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT("Error: obj key <%s> is an invalid ImR replica because %s\n"),
        replica_ior.c_str(), ip.reason.in()), -1);
    }

  if (opts_.debug() > 9)
    {
      ACE_DEBUG((LM_INFO,
        ACE_TEXT("Initializing repository with ft ior=<%s> and replica seq number %d\n"),
        this->imr_ior_.in(), replica_seq_num_));
    }

  return 0;
}

int
Shared_Backing_Store::init_repo(PortableServer::POA_ptr imr_poa)
{
  this->non_ft_imr_ior_ = this->imr_ior_;
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId ("ImR_Replica");
  imr_poa->activate_object_with_id (id.in (), this);

  if (this->imr_type_ != Options::STANDALONE_IMR)
    {
      CORBA::Object_var obj = imr_poa->id_to_reference (id.in ());

      Replica_var this_replica =
        ImplementationRepository::UpdatePushNotification::_narrow (obj.in());
      const int err = connect_replicas(this_replica.in());
      if (err != 0)
        {
          return err;
        }
    }

  // only start the repo clean if no replica is running
  if (this->opts_.repository_erase() && CORBA::is_nil (this->replica_.in ()))
    {
      Lockable_File listing_lf;
      const XMLHandler_Ptr listings = get_listings(listing_lf, false);
      if (listings.null())
        {
          if (this->opts_.debug() > 9)
            {
              ACE_DEBUG((LM_INFO,
                ACE_TEXT ("Persisted Repository already empty\n")));
            }
        }
      else
        {
          const ACE_Vector<ACE_TString>& filenames = listings->filenames();
          CORBA::ULong sz = filenames.size ();
          for (CORBA::ULong i = 0; i < sz; ++i)
            {
              if (this->opts_.debug() > 9)
                {
                  ACE_DEBUG((LM_INFO, ACE_TEXT ("Removing %s\n"),
                    filenames[i].c_str()));
                }
              ACE_OS::unlink ( filenames[i].c_str () );
            }

          if (this->opts_.debug() > 9)
            {
              ACE_DEBUG((LM_INFO, ACE_TEXT ("Removing %s\n"),
                this->listing_file_.c_str()));
            }
          ACE_OS::unlink ( this->listing_file_.c_str () );
        }
    }

  // ignore persistent_load return since files don't have to exist
  persistent_load(false);

  if (this->opts_.debug() > 9)
    {
      ACE_DEBUG((LM_INFO,
        ACE_TEXT ("ImR Repository initialized\n")));
    }

  return 0;
}

int
Shared_Backing_Store::persistent_load (bool only_changes)
{
  Lockable_File listing_lf;
  const XMLHandler_Ptr listings = get_listings(listing_lf, only_changes);
  if (listings.null())
    {
      // failed to retrieve listings
      return -1;
    }

  if (only_changes)
    {
      listings->remove_unmatched(this->servers(), this->activators());
    }

  const ACE_Vector<ACE_TString>& filenames = listings->filenames();
  CORBA::ULong sz = filenames.size ();
  if (this->opts_.debug() > 9)
    {
      ACE_DEBUG((LM_INFO, ACE_TEXT ("persistent_load %d files\n"), sz));
    }
  for (CORBA::ULong i = 0; i < sz; ++i)
    {
      const ACE_TString& fname = filenames[i];
      Lockable_File listing_lf(fname, O_RDONLY);
      load(fname, listing_lf.get_file());
    }

  return 0;
}

Shared_Backing_Store::XMLHandler_Ptr
Shared_Backing_Store::get_listings(Lockable_File& listing_lf,
                                   bool only_changes) const
{
  XMLHandler_Ptr listings_handler;
  if (only_changes)
    {
      listings_handler.reset(new LocatorListings_XMLHandler(
        this->filename_, servers(), activators()));
    }
  else
    {
      listings_handler.reset(new LocatorListings_XMLHandler(this->filename_));
    }

  if (load(this->listing_file_,
           *listings_handler,
           this->opts_.debug(),
           listing_lf.get_file(this->listing_file_, O_RDONLY)) != 0)
    {
      ACE_DEBUG((LM_INFO, "load failed\n"));
      listings_handler.reset();
      ACE_DEBUG((LM_INFO, "reset handler\n"));
    }

  return listings_handler;
}

int
Shared_Backing_Store::sync_load (const ACE_CString& /*name*/,
                                 SyncOp /*sync_op*/,
                                 bool /*activator*/)
{
  return 0;
}

static void write_listing(FILE* list, const ACE_TString& fname,
                          const ACE_CString& name, const ACE_TCHAR* tag)
{
  ACE_OS::fprintf (list, "\t<%s", tag);
  ACE_OS::fprintf (list, " fname=\"%s\"", fname.c_str ());
  ACE_OS::fprintf (list, " name=\"%s\" />\n", name.c_str ());
}

int
Shared_Backing_Store::persist_listings (Lockable_File& listing_lf) const
{
  FILE* list = listing_lf.get_file(this->listing_file_, O_WRONLY);
  if (list == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Couldn't write to file %s\n"),
                  this->listing_file_.c_str()));
      return -1;
    }
  ACE_OS::fprintf (list,"<?xml version=\"1.0\"?>\n");
  ACE_OS::fprintf (list,"<ImRListing>\n");

  // Save servers
  Locator_Repository::SIMap::ENTRY* sientry = 0;
  Locator_Repository::SIMap::CONST_ITERATOR siit (this->servers ());
  for (; siit.next (sientry); siit.advance() )
    {
      Server_Info_Ptr& info = sientry->int_id_;

      const ACE_TString fname = make_filename(info->name, false, true);
      ACE_CString listing_name = ACEXML_escape_string (info->name);
      write_listing(list, fname, listing_name,
        Locator_XMLHandler::SERVER_INFO_TAG);
    }

  // Save Activators
  Locator_Repository::AIMap::ENTRY* aientry = 0;
  Locator_Repository::AIMap::CONST_ITERATOR aiit (this->activators ());
  for (; aiit.next (aientry); aiit.advance ())
    {
      const ACE_CString& aname = aientry->ext_id_;
      const ACE_TString fname = make_filename(aname, true, true);
      write_listing(list, fname, aname,
        Locator_XMLHandler::ACTIVATOR_INFO_TAG);
    }

  ACE_OS::fprintf (list,"</ImRListing>\n");
  return 0;
}

int
Shared_Backing_Store::report_ior(PortableServer::POA_ptr root_poa,
                                 PortableServer::POA_ptr imr_poa)
{
  if (this->imr_type_ == Options::STANDALONE_IMR)
    {
      return Locator_Repository::report_ior(root_poa, imr_poa);
    }

  CORBA::Object_var obj = this->orb_->resolve_initial_references ("IORTable");
  IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in ());
  ACE_ASSERT (! CORBA::is_nil (ior_table.in ()));

  const char* const replica_name(IMR_REPLICA[this->imr_type_]);
  ACE_TString replica_filename = replica_ior_filename(false);
  FILE* fp = ACE_OS::fopen (replica_filename.c_str (), "w");
  if (fp == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT("ImR: Could not open file: %s\n"),
        replica_filename.c_str ()), -1);
    }
  obj = imr_poa->servant_to_reference (this);
  const CORBA::String_var replica_ior = this->orb_->object_to_string (obj.in ());
  report(ior_table.in(), replica_name, replica_ior.in());
  ACE_OS::fprintf (fp, "%s", replica_ior.in ());
  ACE_OS::fclose (fp);

  int err = 0;
  // only report the imr ior if the fault tolerant ImR is complete
  if (!CORBA::is_nil (this->replica_.in()))
    {
      ACE_DEBUG((LM_INFO, "(%P|%t) ???????? Registered <%C>\n", this->imr_ior_.in()));
      err = Locator_Repository::report_ior(root_poa, imr_poa);
    }
  else
    ACE_DEBUG((LM_INFO, "(%P|%t) ???????? Not Registered yet \n"));


  return err;
}

char*
Shared_Backing_Store::locator_service_ior(const char* peer_ior) const
{
  const CORBA::Object_ptr this_obj =
    this->orb_->string_to_object(this->non_ft_imr_ior_.in());
  const CORBA::Object_ptr peer_obj =
    this->orb_->string_to_object(peer_ior);
  const CORBA::Object_ptr& obj1 =
    (this->imr_type_ == Options::PRIMARY_IMR) ? this_obj : peer_obj;
  const CORBA::Object_ptr& obj2 =
    (this->imr_type_ != Options::PRIMARY_IMR) ? this_obj : peer_obj;

  CORBA::Object_var IORM =
    this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION, 0);

  TAO_IOP::TAO_IOR_Manipulation_var iorm =
    TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in());

  CORBA::Object_var locator_service = iorm->add_profiles(obj1, obj2);

  char* const combined_ior =
    this->orb_->object_to_string(locator_service.in());
  ACE_DEBUG((LM_INFO, "(%P|%t) ????? Combined: %d and %d to make %d\n", obj1, obj2, locator_service.in()));
  ACE_DEBUG((LM_INFO, "(%P|%t) ????? Combined: <%C>\nand <%C>\nto make <%C>\n", non_ft_imr_ior_.in(), peer_ior, combined_ior));
  return combined_ior;

}

bool
Shared_Backing_Store::sync_repo(
  ImplementationRepository::SequenceNum new_seq_num,
  const ImplementationRepository::UpdateType& update_type)
{
  // if the sequence number is the next expected sequence number and it is an
  // update, then only need to update the indicated changes, otherwise the
  // whole repo needs to be reloaded
  if (++replica_seq_num_ != new_seq_num)
    {
      replica_seq_num_ = new_seq_num;
      persistent_load(false);
      // repository is now up to date, no incremental sync needed
      return false;
    }

  if (update_type != ImplementationRepository::repo_update)
    {
      persistent_load(true);
      // repository is now up to date, no incremental sync needed
      return false;
    }

  // the update can be performed by the caller
  return true;
}

void
Shared_Backing_Store::notify_updated_server(
  const ImplementationRepository::ServerUpdate& server)
{
  ACE_DEBUG((LM_INFO, "(%P|%t) notify_updated_server=%s\n", server.name.in()));
  if (sync_repo(server.seq_num, server.type))
    {
      const ACE_TString fname = make_filename(server.name.in(), false);
      Lockable_File listing_lf(fname, O_RDONLY);
      load(fname, listing_lf.get_file());
    }
}

void
Shared_Backing_Store::notify_updated_activator(
  const ImplementationRepository::ActivatorUpdate& activator)
{
  ACE_DEBUG((LM_INFO, "(%P|%t) notify_updated_activator=%s\n", activator.name.in()));
  if (sync_repo(activator.seq_num, activator.type))
    {
      const ACE_TString fname = make_filename(activator.name.in(), false);
      Lockable_File listing_lf(fname, O_RDONLY);
      load(fname, listing_lf.get_file());
    }
}

void
Shared_Backing_Store::register_replica(
  ImplementationRepository::UpdatePushNotification_ptr replica,
  char*& ft_imr_ior,
  ImplementationRepository::SequenceNum_out seq_num)
{
  ACE_ASSERT (! CORBA::is_nil (replica));
  this->replica_ =
    ImplementationRepository::UpdatePushNotification::_duplicate (replica);

  seq_num = this->seq_num_;
  if (this->imr_type_ == Options::STANDALONE_IMR)
    {
      ACE_ERROR((LM_ERROR,
        "Error: Non-replicated ImR receiving replica registration <%s>\n",
        ft_imr_ior));
      return;
    }

  this->replica_seq_num_ = 0;

  // store off original char* to ensure memory cleanup
  CORBA::String_var ior = ft_imr_ior;

  // if we already have the fault tolerant ImR ior
  // then just copy it
  if (registered())
    {
      ACE_DEBUG((LM_INFO, "(%P|%t) ???????? Already registered <%C>\n", this->imr_ior_.in()));
      // make a copy
      ior = this->imr_ior_.in();
      // handoff memory
      ft_imr_ior = ior._retn();
      return;
    }

  // otherwise we need to combine the primary and backup ior to make
  // the fault tolerant ImR ior
  char* combined_ior = 0;
  CORBA::String_var reason;
  try
    {
      combined_ior = locator_service_ior(ft_imr_ior);
    }
  catch (const TAO_IOP::Invalid_IOR& )
    {
      reason = "invalid ior";
    }
  catch (const TAO_IOP::EmptyProfileList& )
    {
      reason = "no profiles";
    }
  catch (const TAO_IOP::Duplicate& )
    {
      reason = "duplicate profile";
    }

  if (combined_ior == 0)
    {
      // give back the original pointer and don't clean it up
      ft_imr_ior = ior._retn();
      ACE_ERROR((LM_ERROR,
        "ERROR: Failed to create Fault Tolerant ImR, reason=%s\n",
        reason.in()));
      throw ImplementationRepository::InvalidPeer(reason.in());
    }

  ft_imr_ior = combined_ior;
  // pass as const char* to make sure string is copied
  this->imr_ior_ = (const char*)ft_imr_ior;
  ACE_DEBUG((LM_INFO, "(%P|%t) ???????? Registering <%C>\n", this->imr_ior_.in()));

  PortableServer::POA_var null_poa;
  Locator_Repository::report_ior(null_poa, null_poa);
}

ACE_CString
Shared_Backing_Store::replica_ior_filename(bool peer_ior_file) const
{
  Options::ImrType desired_type = this->imr_type_;
  if (peer_ior_file)
    {
      desired_type = (desired_type == Options::PRIMARY_IMR) ?
        Options::BACKUP_IMR :
        Options::PRIMARY_IMR;
    }
  ACE_CString ior =
    this->filename_ + IMR_REPLICA[desired_type] + ACE_TEXT(".ior");
  if (peer_ior_file)
    {
      // the peer ior file needs the file prefix
      ior = "file://" + ior;
    }

  return ior;
}

Shared_Backing_Store::LocatorListings_XMLHandler::LocatorListings_XMLHandler(
  const ACE_TString& dir)
: dir_(dir),
  only_changes_(false)
{
}

Shared_Backing_Store::LocatorListings_XMLHandler::LocatorListings_XMLHandler(
  const ACE_TString& dir,
  const Locator_Repository::SIMap& servers,
  const Locator_Repository::AIMap& activators)
: dir_(dir),
  only_changes_(true)
{
  Locator_Repository::SIMap::ENTRY* sientry = 0;
  Locator_Repository::SIMap::CONST_ITERATOR siit (servers);
  for (; siit.next (sientry); siit.advance() )
    {
      unmatched_servers_.bind (sientry->ext_id_, sientry->int_id_);
    }

  Locator_Repository::AIMap::ENTRY* aientry = 0;
  Locator_Repository::AIMap::CONST_ITERATOR aiit (activators);
  for (; aiit.next (aientry); aiit.advance() )
    {
      unmatched_activators_.bind (aientry->ext_id_, aientry->int_id_);
    }
}

void
Shared_Backing_Store::LocatorListings_XMLHandler::startElement (
  const ACEXML_Char* ,
  const ACEXML_Char* ,
  const ACEXML_Char* qName,
  ACEXML_Attributes* attrs)
{
  const bool server =
    (ACE_OS::strcasecmp (qName, Locator_XMLHandler::SERVER_INFO_TAG) == 0);
  if (!server &&
      (ACE_OS::strcasecmp (qName, Locator_XMLHandler::ACTIVATOR_INFO_TAG) != 0))
    {
      return;
    }

  if (attrs != 0 && attrs->getLength () == 2)
    {
      ACE_TString fname = attrs->getValue ((size_t)0);
      bool store_fname = !only_changes_;
      if (only_changes_)
        {
          ACE_CString name = ACE_TEXT_ALWAYS_CHAR(attrs->getValue ((size_t)1));
          // if the name is not present, then this is an add, so store it
          store_fname = server ?
            (unmatched_servers_.unbind (name) != 0) :
            (unmatched_activators_.unbind (name) != 0);
        }

      if (store_fname)
        {
          filenames_.push_back(dir_ + fname);
        }
    }
  else
    ACE_DEBUG((LM_INFO, ACE_TEXT ("startElement ERROR too few attrs\n")));

}

void
Shared_Backing_Store::LocatorListings_XMLHandler::endElement (
  const ACEXML_Char* ,
  const ACEXML_Char* ,
  const ACEXML_Char* )
{
}

void
Shared_Backing_Store::LocatorListings_XMLHandler::remove_unmatched(
  Locator_Repository::SIMap& servers,
  Locator_Repository::AIMap& activators)
{
  Locator_Repository::SIMap::ENTRY* sientry = 0;
  Locator_Repository::SIMap::CONST_ITERATOR siit (this->unmatched_servers_);
  for (; siit.next (sientry); siit.advance() )
    {
      int ret = servers.unbind (sientry->ext_id_);
      if (ret != 0)
        {
          ACE_ERROR((LM_ERROR,
            ACE_TEXT ("ERROR: could not remove server: %s\n"),
            sientry->int_id_->name.c_str()));
        }
    }

  Locator_Repository::AIMap::ENTRY* aientry = 0;
  Locator_Repository::AIMap::CONST_ITERATOR aiit (this->unmatched_activators_);
  for (; aiit.next (aientry); aiit.advance ())
    {
      int ret = activators.unbind (aientry->ext_id_);
      if (ret != 0)
        {
          ACE_ERROR((LM_ERROR,
            ACE_TEXT ("ERROR: could not remove activator: %s\n"),
            aientry->int_id_->name.c_str()));
        }
    }
}

const ACE_Vector<ACE_TString>&
Shared_Backing_Store::LocatorListings_XMLHandler::filenames() const
{
  return this->filenames_;
}
