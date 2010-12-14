// -*- C++ -*-

//=============================================================================
/**
 *  @file   Update_Manager.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include "orbsvcs/FtRtEvent/EventChannel/Dynamic_Bitset.h"
#include "tao/orbconf.h"
#include "ace/Auto_Event.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef Dynamic_Bitset bit_vector;

class Update_Manager
{
public:
  Update_Manager(ACE_Auto_Event& evt,
    int num_backups,
    int transaction_depth,
    bool& success);
  ~Update_Manager();

  void handle_reply(int id);
  void handle_exception(int id);

private:
  ACE_Auto_Event& evt_;
  bit_vector replied_;
  bit_vector signal_condition_;
  bit_vector suicide_condition_;
  int num_backups_;
  int transaction_level_;
  bool& success_;
  TAO_SYNCH_MUTEX mutex_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
