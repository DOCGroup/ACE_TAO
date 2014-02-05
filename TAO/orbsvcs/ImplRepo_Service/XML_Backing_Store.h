/* -*- C++ -*- */

//=============================================================================
/**
*  @file XML_Backing_Store.h
*
*  $Id$
*
*  This class defines an implementation of the backing store as a single XML file.
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*  @author Priyanka Gontla <gontla_p@ociweb.com>
*/
//=============================================================================

#ifndef XML_BACKING_STORE_H
#define XML_BACKING_STORE_H

#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Locator_Repository.h"

#include <vector>

class ACE_Configuration;
class ACEXML_FileCharStream;
class ACEXML_DefaultHandler;

/**
* @class XML_Backing_Store
*
* @brief XML backing store interface containing all ImR persistent information
* in a single file
*
*/
class XML_Backing_Store : public Locator_Repository
{
public:
  typedef std::pair<ACE_CString, ACE_CString> NameValue;
  typedef std::vector<NameValue> NameValues;
  XML_Backing_Store(const Options& opts,
                    CORBA::ORB_ptr orb,
                    bool suppress_erase = false);

  virtual ~XML_Backing_Store();

  /// indicate the XML filename as the persistence mode for the repository
  virtual const ACE_TCHAR* repo_mode() const;

  /// create the Server_Info server object
  /// @param server_id the Server_Info server_id
  /// @param server_name the Server_Info server_name
  /// @param activator_name the Server_Info activator
  /// @param cmdline the Server_Info cmdline
  /// @param env_vars the Server_Info env_vars
  /// @param workin_dir the Server_Info dir
  /// @param actmode the Server_Info activation_mode
  /// @param start_limit the Server_Info start_limit
  /// @param partial_ior the Server_Info partial_ior
  /// @param ior the Server_Info ior
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
  /// perform XML backing store specific initialization
  /// (loads servers and activators from the backing store)
  virtual int init_repo(PortableServer::POA_ptr imr_poa);

  /// perform server persistent update
  virtual int persistent_update(const Server_Info_Ptr& info, bool add);

  /// perform activator persistent update
  virtual int persistent_update(const Activator_Info_Ptr& info, bool add);

  /// perform persistent remove
  virtual int persistent_remove(const ACE_CString& name, bool activator);

  /// load the contents of a file into the repo using a Locator_XMLHandler
  /// @param filename the filename to read the contents from
  /// @param open_file the already open FILE stream for the
  ///        filename
  int load(const ACE_TString& filename, FILE* open_file = 0);

  /// load the contents of a file into the repo using the provided
  /// ACEXML_DefaultHandler
  /// @param filename the filename to read the contents from
  /// @param xml_handler the ACEXML_DefaultHandler to use to parse
  ///        the file
  /// @param open_file the already open FILE stream for the
  ///        filename
  static int load(const ACE_TString& filename,
                  ACEXML_DefaultHandler& xml_handler,
                  unsigned int debug,
                  FILE* open_file = 0);

  /// persist the server
  /// @param fp the FILE stream to persist the server contents to
  /// @param info the Server_Info to persist
  /// @param tag_prepend a character string to prepend at the start
  ///        of every xml line to maintain proper indentation
  /// @param name_values extra name value pairs to write as attributes
  void persist(FILE* fp,
               const Server_Info& info,
               const char* tag_prepend,
               const NameValues& name_values = NameValues());

  /// persist the activator
  /// @param fp the FILE stream to persist the activator contents to
  /// @param info the Server_Info to persist
  /// @param tag_prepend a character string to prepend at the start
  ///        of every xml line to maintain proper indentation
  /// @param name_values extra name value pairs to write as attributes
  void persist(FILE* fp,
               const Activator_Info& info,
               const char* tag_prepend,
               const NameValues& name_values = NameValues());

  /// create the Server_Info server object
  /// @param server_started indication from persistence indicating if the
  ///        server object was present
  /// @param si the server info in question
  void create_server(bool server_started, const Server_Info_Ptr& si);

protected:
  /// the filename indicated in the Options for the backing store
  const ACE_TString filename_;

private:
  /// persist all servers and activators
  int persist();
};

#endif /* XML_BACKING_STORE_H */
