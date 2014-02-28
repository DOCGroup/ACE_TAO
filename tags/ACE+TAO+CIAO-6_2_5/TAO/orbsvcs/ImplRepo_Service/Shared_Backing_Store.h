/* -*- C++ -*- */

//=============================================================================
/**
*  @file Shared_Backing_Store.h
*
*  $Id$
*
*  This class defines an implementation of the backing store as multiple XML files
*  shared between multiple Locators.
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*  @author Priyanka Gontla <gontla_p@ociweb.com>
*/
//=============================================================================

#ifndef SHARED_BACKING_STORE_H
#define SHARED_BACKING_STORE_H

#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "XML_Backing_Store.h"

#include "ImR_LocatorS.h"
#include "ace/Bound_Ptr.h"
#include "ace/Vector_T.h"
#include "ACEXML/common/DefaultHandler.h"

#include <set>

class ACE_Configuration;
class ACEXML_FileCharStream;
class LocatorListings_XMLHandler;

namespace {
  class Lockable_File;
}

/**
* @class Shared_Backing_Store
*
* @brief XML backing store containing all ImR persistent information in
* multiple files shared between multiple Locators
*
*/
class Shared_Backing_Store
  : public XML_Backing_Store,
    public virtual POA_ImplementationRepository::UpdatePushNotification
{
public:
  typedef ImplementationRepository::UpdatePushNotification_var Replica_var;
  typedef ImplementationRepository::UpdatePushNotification_ptr Replica_ptr;
  struct UniqueId
  {
    Options::ImrType repo_type;
    unsigned int repo_id;
    ACE_TString repo_type_str;
    ACE_TString repo_id_str;
    ACE_TString unique_filename;
  };
  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
    UniqueId,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> UniqueIdMap;

  enum ExtraParams { REPO_TYPE = 0, REPO_ID = 1 };

  Shared_Backing_Store(const Options& opts,
                       CORBA::ORB_ptr orb);

  virtual ~Shared_Backing_Store();

  /// indicate the persistence mode for the repository
  virtual const ACE_TCHAR* repo_mode() const;

  /// provide the implementation for being notified of a
  /// server update
  virtual void notify_updated_server(
    const ImplementationRepository::ServerUpdate& server);

  /// provide the implementation for being notified of a
  /// activator update
  virtual void notify_updated_activator(
    const ImplementationRepository::ActivatorUpdate& activator);

  /// provide the implementation for registering a peer replica
  /// @param replica the peer replica
  /// @param ft_imr_ior the fault tolerant ImR IOR (passed in
  ///        as the replica's ImR IOR, passed back as fault
  ///        tolerant ImR IOR)
  /// @param seq_num current sequence number to return to replica
  virtual void register_replica(
    ImplementationRepository::UpdatePushNotification_ptr replica,
    char*& ft_imr_ior,
    ImplementationRepository::SequenceNum_out seq_num);

  /// enum to indicate whether the repo is in-sync, individual
  /// server and/or activator files need to be sync-ed or if
  /// a full sync of servers and activators is needed
  enum SyncType { NO_SYNC, INC_SYNC, FULL_SYNC };

  /// recover a persisted Server_Info object
  /// @param info points to a newly created Server_Info object
  /// @param server_started indicates if the server object
  ///        existed when data was persisted
  /// @param extra_params extra name value pairs that
  ///        were reported for the server
  virtual void load_server (Server_Info *info,
                            bool server_started,
                            const NameValues& extra_params);

  /// create the Activator_Info activator object
  /// @param activator_name the Activator_Info name
  /// @param token the Activator_Info token
  /// @param ior the Activator_Info ior
  /// @param extra_params extra name value pairs that
  ///        were reported for the activator
  virtual void load_activator (const ACE_CString& activator_name,
                               long token,
                               const ACE_CString& ior,
                               const NameValues& extra_params);
protected:
  /// perform shared backing store specific initialization
  /// (activates this Shared_Backing_Store as the "ImR_Replica",
  /// trys to connect to the peer replica, unless it is a
  /// STANDALONE_IMR, clears out the persisted store if needed,
  /// and loads the repo from the persisted store)
  virtual int init_repo(PortableServer::POA_ptr imr_poa);

  /// perform server persistent update
  virtual int persistent_update(const Server_Info_Ptr& info, bool add);

  /// perform activator persistent update
  virtual int persistent_update(const Activator_Info_Ptr& info, bool add);

  /// perform persistent remove
  virtual int persistent_remove(const ACE_CString& name, bool activator);

  /// perform sync of repo with backing store
  /// uses sync_needed_ and sync_files_ to determine what to update
  virtual int sync_load ();

  /// calls Locator_Repository::report_ior with the Fault Tolerant ImR
  /// Locator's IOR if available, otherwise reporting the ior is delayed
  /// until the peer replica registers with this replica
  virtual int report_ior(PortableServer::POA_ptr imr_poa);

  /// create the Fault Tolerant ImR Locator IOR, using the peer_ior and
  /// this ImR Locator's IOR
  char* locator_service_ior(const char* peer_ior) const;

private:
  /// map management helper functions
  void bind_unique_id (const ACE_CString& key,
                       UniqueIdMap& unique_ids,
                       const UniqueId& id);

  void find_unique_id (const ACE_CString &key,
                       UniqueIdMap &unique_ids,
                       UniqueId &uid);

  void update_unique_id (const ACE_CString &key,
                         UniqueIdMap& unique_ids,
                         Options::ImrType& entry_repo_type,
                         unsigned int& entry_repo_id,
                         UniqueId& uid);

  void verify_unique_id (const ACE_CString& key,
                         const XML_Backing_Store::NameValues& extra_params,
                         UniqueIdMap& unique_ids);

private:
  /**
   * Callback SAX XML Handler for parsing the imr_listings.xml file.
   */
  class LocatorListings_XMLHandler : public ACEXML_DefaultHandler
  {
  public:
    /// constructor
    /// @param dir the directory path to add to the server or activator
    ///        filename
    LocatorListings_XMLHandler(const ACE_TString& dir);

    /// constructor for tracking only changes to what servers and activators
    /// are currently in the repo
    /// @param dir the directory path to add to the server or activator
    ///        filename
    /// @param servers a map of servers currently in the repo
    /// @param activators a map of activators currently in the repo
    LocatorListings_XMLHandler(const ACE_TString& dir,
                               const Locator_Repository::SIMap& servers,
                               const Locator_Repository::AIMap& activators);

    /// provide implementation for handling a new XML element
    virtual void startElement (const ACEXML_Char* namespaceURI,
                               const ACEXML_Char* localName,
                               const ACEXML_Char* qName,
                               ACEXML_Attributes* attrs);

    /// provide implementation for handling terminating an XML element
    virtual void endElement (const ACEXML_Char* namespaceURI,
                             const ACEXML_Char* localName,
                             const ACEXML_Char* qName);

    /// remove the servers and activators that were in the repo but not
    /// in the listings file (this does nothing if existing servers and
    /// activators were not provided in constructor)
    void remove_unmatched(Locator_Repository& repo);

    /// the filenames identified in the listings file, if tracking changes
    /// only then only the filenames of the new servers and activators
    const ACE_Vector<ACE_TString>& filenames() const;

  private:
    /// the directory to add to the listings file relative filenames
    const ACE_TString& dir_;
    /// the filenames identified in the listings file (or the new filenames)
    ACE_Vector<ACE_TString> filenames_;
    /// the existing servers in the repo that were not present in the
    /// listings file
    Locator_Repository::SIMap unmatched_servers_;
    /// the existing activators in the repo that were not present in the
    /// listings file
    Locator_Repository::AIMap unmatched_activators_;
    /// indicates if only changes should be tracked
    const bool only_changes_;
  };
  typedef ACE_Strong_Bound_Ptr
    <LocatorListings_XMLHandler, ACE_Null_Mutex> XMLHandler_Ptr;

  /// determine the filename where the replica ior is reported
  /// @param peer_ior_file if this is the filename for the peer
  ///        replica or this replica
  ACE_CString replica_ior_filename(bool peer_ior_file) const;

  /// get the listings file contents
  /// @param listing_lf a Lockable_File for the listings file
  ///        that will be locked when the function returns
  /// @param only_changes if only changes to the repo should be
  ///        reported
  /// @return a pointer to the LocatorListings_XMLHandler
  XMLHandler_Ptr get_listings(Lockable_File& listing_lf,
                              bool only_changes) const;

  /// perform a persistent load from the backing store
  /// @param only_changes if only changes to the repo should be
  ///        loaded
  int persistent_load(bool only_changes);

  /// persistent the listings file
  /// @param listing_lf a Lockable_File for the listings file
  ///        that will be locked when the function returns
  int persist_listings(Lockable_File& listing_lf);

  /// write content to the listings file and optionally its backup
  /// @param list File pointer to write to. Can be primary or backup.
  void write_listing(FILE *list);

  /// connect this replica to its peer replica
  /// @param this_replica a pointer to this Replica object
  int connect_replicas(Replica_ptr this_replica);

  /// the path and filename for the listings file
  const ACE_TString listing_file_;
  /// the path and filename for the listings file
  Replica_var peer_replica_;
  /// the current sequence number for reporting changes made directly to
  /// this repo
  ImplementationRepository::SequenceNum seq_num_;
  /// the current sequence number last reported by the peer replica
  ImplementationRepository::SequenceNum replica_seq_num_;
  /// the imr type of this Shared_Backing_Store
  const Options::ImrType imr_type_;
  /// the current type of sync needed by the repo
  SyncType sync_needed_;
  /// the server and activator files that need to be updated for
  /// sync_needed_ == INC_SYNC
  std::set<ACE_TString> sync_files_;
  /// the ior for just this ImR Locator
  CORBA::String_var non_ft_imr_ior_;
  /// an array associating ImrType with the appropriate name
  const char* IMR_REPLICA[3];
  /// map  for server unique ids
  UniqueIdMap server_uids_;
  /// map  for activator unique ids
  UniqueIdMap activator_uids_;
  /// next unique repo id
  unsigned int repo_id_;
  /// extra parameters for XML
  XML_Backing_Store::NameValues repo_values_;
};

#endif /* SHARED_BACKING_STORE_H */
