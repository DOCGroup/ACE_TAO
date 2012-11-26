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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "XML_Backing_Store.h"

#include "ace/Vector_T.h"

class ACE_Configuration;
class ACEXML_FileCharStream;

/**
* @class Shared_Backing_Store
*
* @brief XML backing store containing all ImR persistent information in
* multiple files shared between multiple Locators
*
*/
class Shared_Backing_Store : public XML_Backing_Store
{
public:
  Shared_Backing_Store(const ACE_CString& directory, bool start_clean);

  virtual ~Shared_Backing_Store();

  virtual int persistent_load();

protected:
  virtual int persistent_update(const Server_Info_Ptr& info);

  virtual int persistent_update(const Activator_Info_Ptr& info);

  virtual int persistent_remove(const ACE_CString& name, bool activator);

  virtual int sync_load (const ACE_CString& name, SyncOp sync_op, bool activator);

private:
  ACE_CString make_filename(const ACE_CString& name, const bool activator);
  int get_listings(ACE_Vector<ACE_CString>& filenames) const;

  const ACE_CString listing_file_;
};

#endif /* SHARED_BACKING_STORE_H */
