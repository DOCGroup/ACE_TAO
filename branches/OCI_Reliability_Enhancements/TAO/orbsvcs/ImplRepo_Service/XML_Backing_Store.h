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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Locator_Repository.h"

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
  XML_Backing_Store(const Options& opts,
                    CORBA::ORB_ptr orb,
                    bool suppress_erase = false);

  virtual ~XML_Backing_Store();

  virtual const char* repo_mode() const;

protected:
  virtual int init_repo(PortableServer::POA_ptr imr_poa);

  virtual int persistent_update(const Server_Info_Ptr& info, bool add);

  virtual int persistent_update(const Activator_Info_Ptr& info, bool add);

  virtual int persistent_remove(const ACE_CString& name, bool activator);

  int load(const ACE_TString& filename, FILE* open_file = 0);
  static int load(const ACE_TString& filename,
                  ACEXML_DefaultHandler& xml_handler,
                  unsigned int debug,
                  FILE* open_file = 0);

  void persist(FILE* fp, const Server_Info& info, const char* tag_prepend);

  void persist(FILE* fp, const Activator_Info& info, const char* tag_prepend);

protected:
  const ACE_TString filename_;

private:
  int persist();
};

#endif /* XML_BACKING_STORE_H */
