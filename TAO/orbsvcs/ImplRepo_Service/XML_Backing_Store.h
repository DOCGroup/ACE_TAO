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
  XML_Backing_Store(const ACE_CString& filename, bool start_clean);

  virtual ~XML_Backing_Store();

  virtual const char* repo_mode() const;

  virtual int persistent_load();

protected:
  virtual int persistent_update(const Server_Info_Ptr& info, bool add);

  virtual int persistent_update(const Activator_Info_Ptr& info, bool add);

  virtual int persistent_remove(const ACE_CString& name, bool activator);

  int load(const ACE_CString& filename);
  static int load(const ACE_CString& filename,
                  ACEXML_DefaultHandler& xml_handler, unsigned int debug);

  void persist(FILE* fp, const Server_Info& info, const char* tag_prepend);

  void persist(FILE* fp, const Activator_Info& info, const char* tag_prepend);

protected:
  const ACE_CString filename_;

private:
  int persist();
};

#endif /* XML_BACKING_STORE_H */
