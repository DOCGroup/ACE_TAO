// $Id$

#include "Shared_Backing_Store.h"
#include "Server_Info.h"
#include "Activator_Info.h"
#include "utils.h"
#include "Locator_XMLHandler.h"
#include "ImR_LocatorC.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_unistd.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/XML_Util.h"

Shared_Backing_Store::Shared_Backing_Store(const ACE_CString& filename,
                                           bool start_clean)
: XML_Backing_Store(filename, false),
  listing_file_(filename + "imr_listing.xml"),
  seq_num_(0),
  replica_seq_num_(0)
{
  if (start_clean)
    {
      const XMLHandler_Ptr listings = get_listings(false);
      if (listings.null())
        {
          if (this->debug_ > 9)
            {
              ACE_DEBUG((LM_INFO,
                ACE_TEXT ("Persisted Repository already empty\n")));
            }
          return;
        }
      const ACE_Vector<ACE_CString>& filenames = listings->filenames();
      CORBA::ULong sz = filenames.size ();
      for (CORBA::ULong i = 0; i < sz; ++i)
        {
          if (this->debug_ > 9)
            {
              ACE_DEBUG((LM_INFO, ACE_TEXT ("Removing %s\n"),
                filenames[i].c_str()));
            }
          ACE_OS::unlink ( filenames[i].c_str () );
        }

      if (this->debug_ > 9)
        {
          ACE_DEBUG((LM_INFO, ACE_TEXT ("Removing %s\n"),
            this->listing_file_.c_str()));
        }
      ACE_OS::unlink ( this->listing_file_.c_str () );
    }
}

Shared_Backing_Store::~Shared_Backing_Store()
{
}


static void replicate(
  ImplementationRepository::UpdatePushNotification_var& replica,
  const ImplementationRepository::ServerUpdate& update)
{
  // replicate the ServerUpdate to our replicated locator
  replica->notify_updated_server(update);
}

static void replicate(
  ImplementationRepository::UpdatePushNotification_var& replica,
  const ImplementationRepository::ActivatorUpdate& update)
{
  // replicate the ActivatorUpdate to our replicated locator
  replica->notify_updated_activator(update);
}

template< typename Update>
static void replicate(
  ImplementationRepository::UpdatePushNotification_var& replica,
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
      update.type = ImplementationRepository::repo_remove;
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
  const ACE_CString fname = make_filename(name, activator);
  ACE_OS::unlink ( fname.c_str () );

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

ACE_CString
Shared_Backing_Store::make_filename(const ACE_CString& name,
                                    bool activator) const
{
  const char* const prefix = (activator ? "a_" : "s_");
  return this->filename_ + prefix + name + ".xml";
}

int
Shared_Backing_Store::persistent_update(const Server_Info_Ptr& info, bool add)
{
  if (add)
    {
      const int err = persist_listings();
      if (err != 0)
        {
          return err;
        }
    }

  ACE_CString name = ACEXML_escape_string (info->name);

  const ACE_CString fname = make_filename(name, false);
  if (this->debug_ > 9)
    {
      ACE_DEBUG((LM_INFO, ACE_TEXT ("Persisting to %s(%s)\n"),
        fname.c_str(), info->name.c_str()));
    }
  FILE* fp = ACE_OS::fopen (fname.c_str (), "w");
  if (fp == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Couldn't write to file %C\n"),
        fname.c_str()));
      return -1;
    }
  ACE_OS::fprintf (fp,"<?xml version=\"1.0\"?>\n");
  persist(fp, *info, "");
  ACE_OS::fclose (fp);

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
  if (add)
    {
      const int err = persist_listings();
      if (err != 0)
        {
          return err;
        }
    }

  ACE_CString name = lcase (info->name);

  const ACE_CString fname = make_filename(name, true);
  if (this->debug_ > 9)
    {
      ACE_DEBUG((LM_INFO, ACE_TEXT ("Persisting to %s(%s)\n"),
        fname.c_str(), info->name.c_str()));
    }
  FILE* fp = ACE_OS::fopen (fname.c_str (), "w");
  if (fp == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Couldn't write to file %C\n"),
                  fname.c_str()));
      return -1;
    }
  ACE_OS::fprintf (fp,"<?xml version=\"1.0\"?>\n");
  persist(fp, *info, "");
  ACE_OS::fclose (fp);

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
Shared_Backing_Store::persistent_load ()
{
  return persistent_load(false);
}

int
Shared_Backing_Store::persistent_load (bool only_changes)
{
  const XMLHandler_Ptr listings = get_listings(only_changes);
  if (listings.null())
    {
      // failed to retrieve listings
      return -1;
    }

  if (only_changes)
    {
      listings->remove_unmatched(this->servers(), this->activators());
    }

  const ACE_Vector<ACE_CString>& filenames = listings->filenames();
  CORBA::ULong sz = filenames.size ();
  if (this->debug_ > 9)
    {
      ACE_DEBUG((LM_INFO, ACE_TEXT ("persistent_load %d files\n"), sz));
    }
  for (CORBA::ULong i = 0; i < sz; ++i)
    {
      load(filenames[i]);
    }

  return 0;
}

Shared_Backing_Store::XMLHandler_Ptr
Shared_Backing_Store::get_listings(bool only_changes) const
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

  if (load(this->listing_file_, *listings_handler, this->debug_) != 0)
    {
      listings_handler.reset();
    }

  return listings_handler;
}

int
Shared_Backing_Store::sync_load (const ACE_CString& name, SyncOp sync_op,
                                 bool activator)
{
  return 0;
}

static void write_listing(FILE* list, const ACE_CString& fname,
                          const ACE_CString& name, const ACE_TCHAR* tag)
{
  ACE_OS::fprintf (list,"\t<%s", tag);
  ACE_OS::fprintf (list," fname=\"%s\"", fname.c_str ());
  ACE_OS::fprintf (list," name=\"%s\"/>\n", name.c_str ());
}

int
Shared_Backing_Store::persist_listings (void) const
{
  FILE* list = ACE_OS::fopen (this->listing_file_.c_str (), "w");
  if (list == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Couldn't write to file %C\n"),
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

      const ACE_CString fname = make_filename(info->name, false);
      ACE_CString listing_name = ACEXML_escape_string (info->name);
      write_listing(list, fname, listing_name,
        Locator_XMLHandler::SERVER_INFO_TAG);
    }

  // Save Activators
  Locator_Repository::AIMap::ENTRY* aientry = 0;
  Locator_Repository::AIMap::CONST_ITERATOR aiit (this->activators ());
  for (; aiit.next (aientry); aiit.advance ())
    {
      Activator_Info_Ptr& info = aientry->int_id_;

      const ACE_CString& aname = aientry->ext_id_;
      const ACE_CString fname = make_filename(aname, true);
      write_listing(list, fname, aname,
        Locator_XMLHandler::ACTIVATOR_INFO_TAG);
    }

  ACE_OS::fprintf (list,"</ImRListing>\n");
  ACE_OS::fclose (list);
  return 0;
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
      const ACE_CString fname = make_filename(server.name.in(), false);
      load(fname);
    }
}

void
Shared_Backing_Store::notify_updated_activator(
  const ImplementationRepository::ActivatorUpdate& activator)
{
  ACE_DEBUG((LM_INFO, "(%P|%t) notify_updated_activator=%s\n", activator.name.in()));
  if (sync_repo(activator.seq_num, activator.type))
    {
      const ACE_CString fname = make_filename(activator.name.in(), false);
      load(fname);
    }
}

void
Shared_Backing_Store::register_replica(
  ImplementationRepository::UpdatePushNotification_ptr replica,
  ImplementationRepository::SequenceNum_out seq_num,
  ImplementationRepository::SequenceNum replica_seq_num)
{
  ACE_ASSERT (! CORBA::is_nil (replica));
  this->replica_ =
    ImplementationRepository::UpdatePushNotification::_duplicate (replica);

  seq_num = seq_num_;

  replica_seq_num_ = replica_seq_num;
}

Shared_Backing_Store::LocatorListings_XMLHandler::LocatorListings_XMLHandler(
  const ACE_CString& dir)
: dir_(dir),
  only_changes_(false)
{
}

Shared_Backing_Store::LocatorListings_XMLHandler::LocatorListings_XMLHandler(
  const ACE_CString& dir,
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
  const ACEXML_Char* namespaceURI,
  const ACEXML_Char* localName,
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
      ACE_CString fname = ACE_TEXT_ALWAYS_CHAR(attrs->getValue ((size_t)0));
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

const ACE_Vector<ACE_CString>&
Shared_Backing_Store::LocatorListings_XMLHandler::filenames() const
{
  return this->filenames_;
}
