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
  /// @param pid an optional process id to further discriminate the server
  UpdateableServerInfo(Locator_Repository* repo,
                       const ACE_CString& name,
                       int pid = 0);

  /// constructor
  /// @param repo the repo to report updates to
  /// @param si an already retrieved Server_Info_Ptr
  /// @param reset_start_count controls the reset of the start count value
  UpdateableServerInfo(Locator_Repository* repo,
                       const Server_Info_Ptr& si,
                       bool reset_start_count = false);

  UpdateableServerInfo(UpdateableServerInfo& other );

  /// destructor (updates repo if needed)
  ~UpdateableServerInfo(void);

  /// explicitly update repo if needed
  void update_repo(void);

  /// assign a server info
  void server_info (const Server_Info_Ptr& si);

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
  const UpdateableServerInfo& operator=(const UpdateableServerInfo& );

  /// the repo
  Locator_Repository* repo_;

  /// the retrieved, passed, or non-stored server info
  Server_Info_Ptr si_;

  /// the server info has changes and needs to be updated to the repo
  bool needs_update_;
};

#endif /* UPDATEABLE_SERVER_INFO_H */
