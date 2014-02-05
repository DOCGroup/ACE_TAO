// $Id$

#include "orbsvcs/Log_Macros.h"
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
    }

    Lockable_File(const ACE_TString& file,
                  const int flags,
                  bool unlink_in_destructor = false)
    : file_(0),
      flags_(0),
      locked_(false),
      unlink_in_destructor_(false)
    {
      init_fl(file, flags, unlink_in_destructor);
    }

    ~Lockable_File()
    {
      release();
    }

    void release()
    {
      if (this->file_ == 0)
        return;

      close_file();
      this->file_lock_.reset();
      this->locked_ = false;
    }

    FILE* get_file()
    {
      lock();

      return this->file_;
    }

    FILE* get_file(const ACE_TString& file,
                   const int flags,
                   bool unlink_in_destructor = false)
    {
      init_fl(file, flags, unlink_in_destructor);
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

      // Truncating output so this will not allow reading then writing

      ACE_OS::ftruncate(this->file_lock_->get_handle(), 0);
      this->file_ = ACE_OS::fdopen(this->file_lock_->get_handle(), flags_str);
#endif
    }

    void close_file()
    {
      if (this->file_ == 0)
        return;

      ACE_OS::fflush(this->file_);
      ACE_OS::fclose(this->file_);
      this->file_ = 0;
#ifdef ACE_WIN32
      if (this->unlink_in_destructor_)
        {
          ACE_OS::unlink(this->filename_.c_str());
          this->unlink_in_destructor_ = false;
        }
#endif
    }

    void lock()
    {
#ifndef ACE_WIN32
      if (this->locked_)
        return;

      if (file_lock_.get () == 0)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: attempting to lock ")
                      ACE_TEXT ("an uninitialized Lockable_File.")));
          this->locked_ = false;
          return;
        }

      if ((this->flags_ & O_RDWR) != 0)
        file_lock_->acquire();
      if ((this->flags_ & O_WRONLY) != 0)
        file_lock_->acquire_write();
      else
        file_lock_->acquire_read();

      this->locked_ = true;
#endif
    }

    auto_ptr<ACE_File_Lock> file_lock_;
    FILE* file_;
    int flags_;
    bool locked_;
    bool unlink_in_destructor_;
    ACE_TString filename_;
  }; // class Lockable_File

  void replicate_i (Shared_Backing_Store::Replica_ptr replica,
                    const ImplementationRepository::ServerUpdate& update)
  {
    // replicate the ServerUpdate to our replicated locator
    replica->notify_updated_server(update);
  }

  void replicate_i (Shared_Backing_Store::Replica_ptr replica,
                    const ImplementationRepository::ActivatorUpdate& update)
  {
    // replicate the ActivatorUpdate to our replicated locator
    replica->notify_updated_activator(update);
  }

  template< typename Update >
  void replicate (Shared_Backing_Store::Replica_ptr replica,
                  const ACE_CString &key,
                  const Shared_Backing_Store::UniqueId& uid,
                  const ImplementationRepository::UpdateType type,
                  const ImplementationRepository::SequenceNum seq_num)
  {
    if (CORBA::is_nil (replica))
      {
        return;
      }

    try
      {
        Update update;
        update.name =  key.c_str();
        update.type = type;
        update.seq_num = seq_num;
        update.repo_id = uid.repo_id;
        update.repo_type = uid.repo_type;
        replicate_i (replica, update);
      }
    catch (const CORBA::COMM_FAILURE&)
      {
        ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT("(%P|%t) Replicated ImR: COMM_FAILURE Exception\n")));
      }
    catch (const CORBA::TRANSIENT&)
      {
        ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT("(%P|%t) Replicated ImR: TRANSIENT Exception\n")));
      }
    catch (const CORBA::OBJECT_NOT_EXIST&)
      {
        ORBSVCS_DEBUG ((
                    LM_DEBUG,
                    ACE_TEXT("(%P|%t) Replicated ImR: OBJECT_NOT_EXIST ")
                    ACE_TEXT("Exception, dropping replica\n")));
        replica = TAO::Objref_Traits
          <ImplementationRepository::UpdatePushNotification>::nil ();
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception (ACE_TEXT("Replicated ImR: notify update"));
        replica = 0;
      }
  }

  void
  create_uid (const Options::ImrType repo_type,
              const unsigned int repo_id,
              Shared_Backing_Store::UniqueId &id)
  {
    id.repo_id = repo_id;
    id.repo_type = repo_type;
    switch (repo_type)
      {
      case Options::BACKUP_IMR:
        id.repo_type_str = "0";
        break;
      case Options::PRIMARY_IMR:
        id.repo_type_str = "1";
        break;
      case Options::STANDALONE_IMR:
        id.repo_type_str = "2";
      }

    ACE_TCHAR tmp[20];
    ACE_OS::itoa(repo_id, tmp, 10);

    id.repo_id_str = tmp;
    id.unique_filename = id.repo_type_str + ACE_TEXT ("_") +
      id.repo_id_str + ACE_TEXT (".xml");
  }


  template< typename Update >
  int
  remove (const ACE_CString& key,
          const Shared_Backing_Store::UniqueIdMap& unique_ids,
          const ACE_TString& path,
          Lockable_File& listing_lf,
          Shared_Backing_Store::Replica_ptr peer_replica,
          ImplementationRepository::SequenceNum& seq_num)
  {
    Shared_Backing_Store::UniqueId uid;
    const int err = unique_ids.find (key, uid);
    if (err != 0)
      {
        ORBSVCS_ERROR((LM_ERROR,
                   ACE_TEXT("(%P|%t) Couldn't find unique repo id for name = %C\n"),
                   key.c_str()));
        return err;
      }

    const ACE_TString fname = path + uid.unique_filename;

    {
      // take the lock, then remove the file
      Lockable_File file(fname, O_WRONLY, true);
    }
    listing_lf.release();

    replicate<Update> (peer_replica, key, uid,
                       ImplementationRepository::repo_remove,
                       ++seq_num);

    return 0;
  }

  void write_listing_item (FILE* list,
                           const ACE_TString& fname,
                           const ACE_CString& name,
                           const ACE_TCHAR* tag)
  {
    ACE_OS::fprintf (list, "\t<%s", tag);
    ACE_OS::fprintf (list, " fname=\"%s\"", fname.c_str ());
    ACE_OS::fprintf (list, " name=\"%s\" />\n", name.c_str ());
  }

} // End anonymous namespace

//---------------------------------------------------------------------------

Shared_Backing_Store::Shared_Backing_Store(const Options& opts,
                                           CORBA::ORB_ptr orb)
: XML_Backing_Store(opts, orb, true),
  listing_file_(opts.persist_file_name() + ACE_TEXT("imr_listing.xml")),
  seq_num_(0),
  replica_seq_num_(0),
  imr_type_(opts.imr_type()),
  sync_needed_(NO_SYNC),
  repo_id_(1),
  repo_values_(2)
{
  IMR_REPLICA[Options::PRIMARY_IMR] = ACE_TEXT ("ImR_ReplicaPrimary");
  IMR_REPLICA[Options::BACKUP_IMR] = ACE_TEXT ("ImR_ReplicaBackup");
  IMR_REPLICA[Options::STANDALONE_IMR] = ACE_TEXT ("ImR_NoReplica");

  this->repo_values_[REPO_TYPE] =
    std::make_pair(ACE_CString(ACE_TEXT ("repo_type")),
                   ACE_CString());
  this->repo_values_[REPO_ID] =
    std::make_pair(ACE_CString(ACE_TEXT ("repo_id")),
                   ACE_CString());
}

Shared_Backing_Store::~Shared_Backing_Store()
{
}


void
Shared_Backing_Store::bind_unique_id (const ACE_CString& key,
                                      UniqueIdMap& unique_ids,
                                      const UniqueId& uid)
{
  unique_ids.bind (key, uid);
}

void
Shared_Backing_Store::find_unique_id (const ACE_CString& key,
                                      UniqueIdMap &unique_ids,
                                      UniqueId &uid)
{
  if (unique_ids.find (key, uid) != 0)
    {
      const unsigned int repo_id = this->repo_id_++;
      create_uid (this->imr_type_, repo_id, uid);
      bind_unique_id (key, unique_ids, uid);
    }
}

void
Shared_Backing_Store::update_unique_id (const ACE_CString &key,
                                        UniqueIdMap& unique_ids,
                                        Options::ImrType& entry_repo_type,
                                        unsigned int& entry_repo_id,
                                        UniqueId& uid)
{
  UniqueId temp_id;
  const bool found = (unique_ids.find (key, temp_id) == 0);

  create_uid (entry_repo_type, entry_repo_id, uid);
  bind_unique_id (key, unique_ids, uid);

  if (entry_repo_id == 0)
    {
      // if no repo id provided, treat it like it came from this repo
      entry_repo_id = this->repo_id_++;
      entry_repo_type = this->imr_type_;
    }
  else if (found)
    {
      if (entry_repo_id != uid.repo_id &&
          entry_repo_type != uid.repo_type)
        {
          // if already existed, replace the contents
          ORBSVCS_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) ERROR: replacing name = %C with ")
                          ACE_TEXT ("existing repo_id = %d and imr_type = %d, ")
                          ACE_TEXT ("with repo_id = %d and imr_type = %d\n"),
                          key.c_str(), uid.repo_id, uid.repo_type,
                          entry_repo_id, entry_repo_type));
        }
    }

  if (entry_repo_type == this->imr_type_ && entry_repo_id >= this->repo_id_)
    {
      // persisting existing entries for this repo, so move the repo_id past
      // the entries id
      this->repo_id_ = entry_repo_id + 1;
    }
}

void
Shared_Backing_Store::verify_unique_id (const ACE_CString& key,
                                        const XML_Backing_Store::NameValues& extra_params,
                                        UniqueIdMap& unique_ids)
  {
    const size_t size = extra_params.size();
    if ((size != 2) && (this->opts_.debug() > 4))
      {
        ORBSVCS_ERROR((
                   LM_ERROR,
                   ACE_TEXT("(%P|%t) Persisted server id=%C name=%C doesn't have all ")
                   ACE_TEXT("unique id params. (%d of 2)\n"),
                   size));
      };

    unsigned int repo_id = 0;
    // default to this repo
    Options::ImrType repo_type = this->imr_type_;
    for (unsigned int i = 0; i < size; ++i)
      {
        ORBSVCS_DEBUG((LM_INFO,
                       ACE_TEXT ("name values %C=%C (%C)\n"),
                       extra_params[i].first.c_str(),
                       extra_params[i].second.c_str(),
                       this->repo_values_[i].first.c_str()));
      }
    if ((size > Shared_Backing_Store::REPO_TYPE) &&
        (extra_params[Shared_Backing_Store::REPO_TYPE].first ==
         this->repo_values_[Shared_Backing_Store::REPO_TYPE].first))
      {
        repo_type =
          (Options::ImrType)ACE_OS::atoi(extra_params[Shared_Backing_Store::REPO_TYPE].second.c_str());
      }
    if ((size > Shared_Backing_Store::REPO_ID) &&
        (extra_params[Shared_Backing_Store::REPO_ID].first ==
         this->repo_values_[Shared_Backing_Store::REPO_ID].first))
      {
        repo_id =
          ACE_OS::atoi(extra_params[Shared_Backing_Store::REPO_ID].second.c_str());
      }
    else
      {
        ORBSVCS_ERROR((LM_ERROR,
                   ACE_TEXT("(%P|%t) Persisted name = %C did not supply a repo_id\n"),
                   key.c_str()));
      }

    UniqueId uid;
    update_unique_id (key, unique_ids, repo_type, repo_id, uid);
  }

int
Shared_Backing_Store::persistent_remove (const ACE_CString& name,
                                         bool activator)
{
  Lockable_File listing_lf;
  int err = persist_listings(listing_lf);
  if (err != 0)
    {
      return err;
    }

  if (activator)
    {
      err = remove<ImplementationRepository::ActivatorUpdate>
        (name, this->activator_uids_, this->filename_, listing_lf,
         this->peer_replica_.in (), this->seq_num_);
    }
  else
    {
      err = remove<ImplementationRepository::ServerUpdate>
        (name, this->server_uids_, this->filename_, listing_lf,
         this->peer_replica_.in (), this->seq_num_);
    }
  return err;
}

int
Shared_Backing_Store::persistent_update (const Server_Info_Ptr& info, bool add)
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

  ACE_CString name = ACEXML_escape_string (info->key_name);

  UniqueId uid;
  this->find_unique_id (info->key_name, this->server_uids_, uid);

  const ACE_TString fname = this->filename_ + uid.unique_filename;
  if (this->opts_.debug() > 9)
    {
      ORBSVCS_DEBUG((LM_INFO, ACE_TEXT ("Persisting server to %s(%C)\n"),
        fname.c_str(), info->key_name.c_str()));
    }
  Lockable_File server_file (fname, O_WRONLY);
  const ACE_TString bfname = fname.c_str() + ACE_TString(".bak");
  FILE* fp = server_file.get_file();
  if (fp == 0)
    {
      ORBSVCS_ERROR ((LM_ERROR, ACE_TEXT ("Couldn't write to file %s\n"),
        fname.c_str()));
      return -1;
    }
  // successfully added file (if adding), so release the listing file lock
  listing_lf.release();
  ACE_OS::fprintf (fp,"<?xml version=\"1.0\"?>\n");

  this->repo_values_[REPO_TYPE].second = uid.repo_type_str;
  this->repo_values_[REPO_ID].second = uid.repo_id_str;

  persist (fp, *info, "", this->repo_values_);

  // Copy the current file to a backup.
  FILE* bfp = ACE_OS::fopen (bfname.c_str(),ACE_TEXT("w"));
  ACE_OS::fprintf (bfp,"<?xml version=\"1.0\"?>\n");
  persist (bfp, *info, "", this->repo_values_);
  ACE_OS::fflush (bfp);
  ACE_OS::fclose (bfp);
  server_file.release ();

  const ImplementationRepository::UpdateType type = add ?
    ImplementationRepository::repo_add :
    ImplementationRepository::repo_update;
  replicate<ImplementationRepository::ServerUpdate> (peer_replica_.in (),
                                                     info->key_name, uid, type,
                                                     ++seq_num_);
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

  UniqueId uid;
  this->find_unique_id (name, this->activator_uids_,uid);

  const ACE_TString fname = this->filename_ + uid.unique_filename;
  if (this->opts_.debug() > 9)
    {
      ORBSVCS_DEBUG ((LM_INFO,
                      ACE_TEXT ("Persisting activator to %s(%C)\n"),
                      fname.c_str(), info->name.c_str()));
    }
  Lockable_File activator_file (fname, O_WRONLY);
  const ACE_TString bfname = fname.c_str() + ACE_TString(".bak");
  FILE* fp = activator_file.get_file();
  if (fp == 0)
    {
      ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("Couldn't write to file %s\n"),
                      fname.c_str()));
      return -1;
    }
  // successfully added file (if adding), so release the listing file lock
  listing_lf.release ();
  ACE_OS::fprintf (fp, "<?xml version=\"1.0\"?>\n");

  this->repo_values_[REPO_TYPE].second = uid.repo_type_str;
  this->repo_values_[REPO_ID].second = uid.repo_id_str;

  persist (fp, *info, "", this->repo_values_);

  // Copy the current file to a backup.
  FILE* bfp = ACE_OS::fopen (bfname.c_str(),ACE_TEXT("w+"));
  ACE_OS::fprintf (bfp,"<?xml version=\"1.0\"?>\n");
  persist (bfp, *info, "", this->repo_values_);
  ACE_OS::fflush (bfp);
  ACE_OS::fclose (bfp);
  activator_file.release ();

  const ImplementationRepository::UpdateType type = add ?
    ImplementationRepository::repo_add :
    ImplementationRepository::repo_update;
  replicate<ImplementationRepository::ActivatorUpdate>
    (peer_replica_.in (), name, uid, type, ++seq_num_);
  return 0;
}

const ACE_TCHAR*
Shared_Backing_Store::repo_mode() const
{
  return this->listing_file_.c_str();
}

int
Shared_Backing_Store::connect_replicas (Replica_ptr this_replica)
{
  const ACE_TString& replica_ior_file = replica_ior_filename(true);
  if (this->opts_.debug() > 1)
    {
      ORBSVCS_DEBUG((LM_INFO,
        ACE_TEXT("Resolving ImR replica %s\n"), replica_ior_file.c_str()));
    }

  // Determine if the peer has started previously by checking if the
  // ior file for the replica is there.
  int peer_started_previously = 0;
  if (ACE_OS::access (replica_ior_file.c_str (), F_OK) == 0)
    {
      peer_started_previously = 1;
    }
  else
    this->peer_replica_ =
      ImplementationRepository::UpdatePushNotification::_nil();

  if (peer_started_previously)
    {
      ACE_TString replica_ior = "file://" + replica_ior_file;
      CORBA::Object_var obj =
        this->orb_->string_to_object (replica_ior.c_str());

      if (!CORBA::is_nil (obj.in ()))
        {
          bool non_exist = true;
          try
            {
              this->peer_replica_ = ImplementationRepository::
                UpdatePushNotification::_narrow (obj.in());
              non_exist = (this->peer_replica_->_non_existent() == 1);
            }
          catch (const CORBA::Exception& )
            {
              // let error be handled below
            }

          if (non_exist)
            {
              this->peer_replica_ =
                ImplementationRepository::UpdatePushNotification::_nil();
            }
        }
    }

  // Check if a peer IOR is defined
  if (CORBA::is_nil (this->peer_replica_.in()))
    {
      if (this->imr_type_ == Options::BACKUP_IMR)
        { // We are a backup IMR Locator

          // If the primary has started at some point in the past, but is
          // not available right now, then we will assume that we are in
          // a restart situation where the backup is being started while
          // the primary is still down. This implies that a successful
          // start of the replication pair has been made in the past and
          // we can use the combined ior from the previous run.
          if (peer_started_previously)
            {
              // Verify that we recovered the IOR successfully. If we did not
              // then fail startup of the backup IMR Locator.
              if (this->recover_ior () == -1)
                ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT("Error: Unable to retrieve IOR from combined IOR ")
                                   ACE_TEXT ("file: %C\n"),
                                   replica_ior_file.c_str()),
                                  -1);
            }
          else
            { // There has been a startup error. The backup can only be started
              // after the primary has been successfully started.
              ORBSVCS_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT("Error: Primary has not been started previously.\n ")
                               ACE_TEXT ("file: %C\n"),
                               replica_ior_file.c_str()),
                              -1);
            }
        }

      // For either primary or backup - no connection currently, just wait for peer to start
      return 0;
    }

  if (opts_.debug() > 1)
    {
      ORBSVCS_DEBUG((LM_INFO,
        ACE_TEXT("Registering with previously running ImR replica\n")));
    }

  try
    {
      this->peer_replica_->register_replica(this_replica,
                                            this->imr_ior_.inout(),
                                            this->replica_seq_num_);
    }
  catch (const ImplementationRepository::InvalidPeer& ip)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT("Error: obj key <%s> is an invalid ImR replica because %s\n"),
        replica_ior_file.c_str(), ip.reason.in()), -1);
    }

  if (opts_.debug() > 9)
    {
      ORBSVCS_DEBUG((LM_INFO,
        ACE_TEXT("Initializing repository with ft ior=<%C> ")
        ACE_TEXT("and replica seq number %d\n"),
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
  if (this->opts_.repository_erase() &&
      CORBA::is_nil (this->peer_replica_.in ()))
    {
      Lockable_File listing_lf;
      const XMLHandler_Ptr listings = get_listings(listing_lf, false);
      if (listings.null())
        {
          if (this->opts_.debug() > 9)
            {
              ORBSVCS_DEBUG((LM_INFO,
                ACE_TEXT ("Persisted Repository already empty\n")));
            }
        }
      else
        {
          const ACE_Vector<ACE_TString>& filenames = listings->filenames();
          size_t sz = filenames.size ();
          for (CORBA::ULong i = 0; i < sz; ++i)
            {
              if (this->opts_.debug() > 9)
                {
                  ORBSVCS_DEBUG((LM_INFO, ACE_TEXT ("Removing %s\n"),
                    filenames[i].c_str()));
                }
              ACE_OS::unlink ( filenames[i].c_str () );
            }

          if (this->opts_.debug() > 9)
            {
              ORBSVCS_DEBUG((LM_INFO, ACE_TEXT ("Removing %s\n"),
                this->listing_file_.c_str()));
            }
          ACE_OS::unlink ( this->listing_file_.c_str () );
        }
    }

  // ignore persistent_load return since files don't have to exist
  persistent_load(false);

  if (this->opts_.debug() > 9)
    {
      ORBSVCS_DEBUG((LM_INFO,
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
      listings->remove_unmatched(*this);
    }

  const ACE_Vector<ACE_TString>& filenames = listings->filenames();
  size_t sz = filenames.size ();
  if (this->opts_.debug() > 9)
    {
      ORBSVCS_DEBUG((LM_INFO, ACE_TEXT ("persistent_load %d files\n"), sz));
    }
  for (CORBA::ULong i = 0; i < sz; ++i)
    {
      const ACE_TString& fname = filenames[i];
      Lockable_File file(fname, O_RDONLY);

      if(load(fname, file.get_file()) != 0)
        {
          load(fname + ".bak");
        }
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

      if (load(this->listing_file_ + ".bak",
         *listings_handler,
         this->opts_.debug()) != 0)
         {
           listings_handler.reset();
         }
    }

  return listings_handler;
}

int
Shared_Backing_Store::sync_load ()
{
  int err = 0;
  if (this->opts_.debug() > 5)
    {
      ORBSVCS_DEBUG((
        LM_INFO,
        ACE_TEXT("(%P|%t) sync_load %d, %d\n"),
        this->sync_needed_, this->sync_files_.size()));
    }
  if (this->sync_needed_ == FULL_SYNC)
    {
      err = persistent_load(false);
    }
  else if (this->sync_needed_ == INC_SYNC)
    {
      std::set<ACE_TString>::const_iterator fname = this->sync_files_.begin();
      for ( ; fname != this->sync_files_.end(); ++fname)
        {
          if (this->opts_.debug() > 6)
            {
              ORBSVCS_DEBUG((LM_INFO,
                         ACE_TEXT("(%P|%t) sync_load %s\n"),
                         fname->c_str()));
            }
          Lockable_File file(*fname, O_RDONLY);
          int ind_err = load(*fname, file.get_file());
          if (ind_err != 0)
            {
              err = ind_err;
            }
        }
      this->sync_files_.clear();
    }

  this->sync_needed_ = NO_SYNC;
  return err;
}

void
Shared_Backing_Store::write_listing (FILE* list)
{
  ACE_OS::fprintf (list,"<?xml version=\"1.0\"?>\n");
  ACE_OS::fprintf (list,"<ImRListing>\n");
  UniqueId uid;

  // Save servers
  Locator_Repository::SIMap::ENTRY* sientry = 0;
  Locator_Repository::SIMap::CONST_ITERATOR siit (this->servers ());
  for (; siit.next (sientry); siit.advance() )
    {
      const Server_Info_Ptr& info = sientry->int_id_;

      find_unique_id (sientry->ext_id_, this->server_uids_, uid);
      ACE_CString listing_name = ACEXML_escape_string (info->key_name);
      write_listing_item (list, uid.unique_filename, listing_name,
                          Locator_XMLHandler::SERVER_INFO_TAG);
    }

  // Save Activators
  Locator_Repository::AIMap::ENTRY* aientry = 0;
  Locator_Repository::AIMap::CONST_ITERATOR aiit (this->activators ());
  for (; aiit.next (aientry); aiit.advance ())
    {
      const ACE_CString& aname = aientry->ext_id_;

      find_unique_id (aname, this->activator_uids_, uid);

      write_listing_item (list, uid.unique_filename, aname,
                          Locator_XMLHandler::ACTIVATOR_INFO_TAG);
    }

  ACE_OS::fprintf (list,"</ImRListing>\n");
}

int
Shared_Backing_Store::persist_listings (Lockable_File& listing_lf)
{
  FILE* list = listing_lf.get_file (this->listing_file_, O_WRONLY);
  if (list == 0)
    {
      ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("Couldn't write to file %s\n"),
                      this->listing_file_.c_str()));
      return -1;
    }

   write_listing(list);

  const ACE_TString bfname = this->listing_file_.c_str() + ACE_TString(".bak");

  // Write backup file
  FILE* baklist = ACE_OS::fopen(bfname.c_str(),ACE_TEXT("w"));
  if (baklist == 0)
    {
      ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("Couldn't write to file %s\n"),
                      bfname.c_str()));
      return -1;
    }

  write_listing(baklist);
  ACE_OS::fflush(baklist);
  ACE_OS::fclose(baklist);

  return 0;
}

int
Shared_Backing_Store::report_ior(PortableServer::POA_ptr imr_poa)
{
  if (this->imr_type_ == Options::STANDALONE_IMR)
    {
      return Locator_Repository::report_ior(imr_poa);
    }

  CORBA::Object_var obj = this->orb_->resolve_initial_references ("AsyncIORTable");
  IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in ());
  ACE_ASSERT (! CORBA::is_nil (ior_table.in ()));

  const char* const replica_name(IMR_REPLICA[this->imr_type_]);
  ACE_TString replica_filename = replica_ior_filename(false);
  FILE* fp = ACE_OS::fopen (replica_filename.c_str (), "w");
  if (fp == 0)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT("ImR: Could not open file: %s\n"),
        replica_filename.c_str ()), -1);
    }
  obj = imr_poa->servant_to_reference (this);
  const CORBA::String_var replica_ior = this->orb_->object_to_string (obj.in ());
  ior_table->bind(replica_name, replica_ior.in());
  ACE_OS::fprintf (fp, "%s", replica_ior.in ());
  ACE_OS::fclose (fp);

  int err = 0;
  // only report the imr ior if the fault tolerant ImR is complete
  if (!CORBA::is_nil (this->peer_replica_.in()))
    {
      err = Locator_Repository::report_ior(imr_poa);
    }

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
  return combined_ior;

}

void
Shared_Backing_Store::load_server (Server_Info *info,
                                   bool server_started,
                                   const NameValues& extra_params)
{
  // ensure there is an entry for this server
  this->verify_unique_id (info->key_name,
                          extra_params,
                          this->server_uids_);
  Server_Info_Ptr si;
  if (this->servers ().find (info->key_name, si) != 0)
    {
      // create new or replace the existing entry
      XML_Backing_Store::load_server (info, server_started, extra_params);
      return;
    }

  si->ior = info->ior;
  si->poa_name = info->poa_name;
  si->server_id = info->server_id;
  si->is_jacorb = info->is_jacorb;
  si->activator = info->activator;
  si->cmdline = info->cmdline;
  si->env_vars = info->env_vars;
  si->dir = info->dir;
  si->activation_mode = info->activation_mode;
  si->start_limit_ = info->start_limit_;
  si->partial_ior = info->partial_ior;
  si->peers = info->peers;
  si->key_name = info->key_name;
  si->alt_key = info->alt_key;

  if (!server_started)
    si->server = ImplementationRepository::ServerObject::_nil();
  else
    // will create a new server below if no previous server
    // or the ior has changed
    server_started = CORBA::is_nil(si->server.in ());

  create_server(server_started, si);
}

void
Shared_Backing_Store::load_activator (const ACE_CString& activator_name,
                                      long token,
                                      const ACE_CString& ior,
                                      const NameValues& extra_params)
{
  // use this to make sure an unique id entry is created
  this->verify_unique_id (activator_name,
                          extra_params,
                          this->activator_uids_);
  XML_Backing_Store::load_activator (activator_name, token, ior, extra_params);
}

void
Shared_Backing_Store::notify_updated_server
(const ImplementationRepository::ServerUpdate& server)
{
  if (this->opts_.debug() > 5)
    {
      ORBSVCS_DEBUG ((LM_INFO, ACE_TEXT("(%P|%t) notify_updated_server=%C\n"),
                      server.name.in()));
    }
  if ((this->sync_needed_ == FULL_SYNC) ||
      (++this->replica_seq_num_ != server.seq_num))
    {
      this->replica_seq_num_ = server.seq_num;
      this->sync_needed_ = FULL_SYNC;
      this->sync_files_.clear();
      return;
    }

  const ACE_CString name = server.name.in();
  if (server.type == ImplementationRepository::repo_remove)
    {
      // sync_needed_ doesn't change, since we handle the change
      // imme;diately
      this->servers().unbind (name);
      return;
    }

  this->sync_needed_ = INC_SYNC;
  Options::ImrType repo_type = (Options::ImrType)server.repo_type;
  unsigned int repo_id = server.repo_id;
  UniqueId uid;
  update_unique_id (name, this->server_uids_, repo_type, repo_id, uid);
  const ACE_TString fname = this->filename_ + uid.unique_filename;
  this->sync_files_.insert (fname);
  this->sync_load ();
}

void
Shared_Backing_Store::notify_updated_activator(
  const ImplementationRepository::ActivatorUpdate& activator)
{
  if (this->opts_.debug() > 5)
    {
      ORBSVCS_DEBUG ((LM_INFO,
                      ACE_TEXT("(%P|%t) notify_updated_activator = %C\n"),
                      activator.name.in()));
    }
  if ((this->sync_needed_ == FULL_SYNC) ||
      (++this->replica_seq_num_ != activator.seq_num))
    {
      this->replica_seq_num_ = activator.seq_num;
      this->sync_needed_ = FULL_SYNC;
      this->sync_files_.clear();
      return;
    }

  const ACE_CString name = lcase (activator.name.in());
  if (activator.type == ImplementationRepository::repo_remove)
    {
      // sync_needed_ doesn't change, since we handle the change
      // immediately
      this->activators().unbind (name);
      return;
    }

  this->sync_needed_ = INC_SYNC;
  Options::ImrType repo_type = (Options::ImrType)activator.repo_type;
  unsigned int repo_id = activator.repo_id;
  UniqueId uid;
  update_unique_id (name, this->activator_uids_, repo_type, repo_id, uid);
  const ACE_TString fname = this->filename_ + uid.unique_filename;
  this->sync_files_.insert (fname);
  this->sync_load ();
}

void
Shared_Backing_Store::register_replica
( ImplementationRepository::UpdatePushNotification_ptr replica,
  char*& ft_imr_ior,
  ImplementationRepository::SequenceNum_out seq_num)
{
  ACE_ASSERT (! CORBA::is_nil (replica));
  this->peer_replica_ =
    ImplementationRepository::UpdatePushNotification::_duplicate (replica);

  seq_num = this->seq_num_;
  if (this->imr_type_ == Options::STANDALONE_IMR)
    {
      ORBSVCS_ERROR((LM_ERROR,
        ACE_TEXT("Error: Non-replicated ImR receiving replica ")
        ACE_TEXT("registration <%s>\n"),
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
      if (this->opts_.debug() > 2)
        {
          ORBSVCS_DEBUG ((LM_INFO,
                          ACE_TEXT("(%P|%t) Already registered <%C>\n"),
                          this->imr_ior_.in()));
        }
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
      ORBSVCS_ERROR((LM_ERROR,
        "ERROR: Failed to create Fault Tolerant ImR, reason=%s\n",
        reason.in()));
      throw ImplementationRepository::InvalidPeer(reason.in());
    }

  ft_imr_ior = combined_ior;
  // pass as const char* to make sure string is copied
  this->imr_ior_ = (const char*)ft_imr_ior;

  PortableServer::POA_var null_poa;
  Locator_Repository::report_ior(null_poa.in ());
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
    ORBSVCS_DEBUG((
      LM_INFO,
      ACE_TEXT ("LocatorListings_XMLHandler::startElement ")
      ACE_TEXT ("incorrect number of attrs (%d)\n"),
      attrs->getLength ()));

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
  Locator_Repository& repo)
{
  Locator_Repository::SIMap::ENTRY* sientry = 0;
  Locator_Repository::SIMap::CONST_ITERATOR siit (this->unmatched_servers_);
  for (; siit.next (sientry); siit.advance() )
    {
      int ret = repo.servers().unbind (sientry->ext_id_);
      if (ret != 0)
        {
          ORBSVCS_ERROR((LM_ERROR,
            ACE_TEXT ("ERROR: could not remove server: %s\n"),
            sientry->int_id_->key_name.c_str()));
        }
    }

  Locator_Repository::AIMap::ENTRY* aientry = 0;
  Locator_Repository::AIMap::CONST_ITERATOR aiit (this->unmatched_activators_);
  for (; aiit.next (aientry); aiit.advance ())
    {
      int ret = repo.activators().unbind (aientry->ext_id_);
      if (ret != 0)
        {
          ORBSVCS_ERROR((LM_ERROR,
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
