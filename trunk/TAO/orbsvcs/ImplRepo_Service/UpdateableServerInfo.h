/* -*- C++ -*- */

//=============================================================================
/**
*  @file UpdateableServerInfo.h
*
*  $Id$
*
*/
//=============================================================================

#ifndef UPDATEABLE_SERVER_INFO_H
#define UPDATEABLE_SERVER_INFO_H

#include "Server_Info.h"

class Locator_Repository;

/**
* @class UpdateableServerInfo
*
* @brief Class for managing changes to ServerInfo memory to ensure
* it is persisted
*
*/
class UpdateableServerInfo
{
public:
  /// constructor
  /// @param repo the repo to report updates to
  /// @param name the name of the server to retrieve
  UpdateableServerInfo(Locator_Repository* repo,
                       const ACE_CString& name,
                       int pid = 0);

  /// constructor
  /// @param repo the repo to report updates to
  /// @param si an already retrieved Server_Info_Ptr
  UpdateableServerInfo(Locator_Repository* repo,
                       const Server_Info_Ptr& si,
                       bool reset_start_count = false);

  /// constructor (no repo updates will be performed)
  /// @param si a Server_Info to create a non-stored Server_Info_Ptr from
  UpdateableServerInfo(const Server_Info& si);

  /// destructor (updates repo if needed)
  ~UpdateableServerInfo(void);

  /// explicitly update repo if needed
  void update_repo(void);

  /// const Server_Info access
  const Server_Info* operator->() const;

  /// const Server_Info& access
  const Server_Info& operator*() const;

  /// retrieve smart pointer to non-const Server_Info
  /// and indicate repo update required
  const Server_Info_Ptr& edit(void);

  /// force indication of update needed
  void needs_update(void);

  /// indicate it Server_Info_Ptr is null
  bool null(void) const;

private:
  UpdateableServerInfo(const UpdateableServerInfo& );
  const UpdateableServerInfo& operator=(const UpdateableServerInfo& );

  /// the repo
  Locator_Repository* const repo_;

  /// the retrieved, passed, or non-stored server info
  const Server_Info_Ptr si_;

  /// the server info has changes and needs to be updated to the repo
  bool needs_update_;
};

#endif /* UPDATEABLE_SERVER_INFO_H */
