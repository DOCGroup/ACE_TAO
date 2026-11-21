/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SM_Server.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================

#ifndef _SM_SERVER_H
#define _SM_SERVER_H

#include "PM_Server.h"
#include "CM_Server.h"
#include "Select_Manager.h"
#include <memory>

class SM_Server : public Select_Manager, public CM_Server
{
public:
  SM_Server () = default;
  ~SM_Server () override = default;
  SM_Server (SM_Server const &) = delete;
  SM_Server (SM_Server &&) = delete;
  SM_Server &operator= (SM_Server const &) = delete;
  SM_Server &operator= (SM_Server &&) = delete;
  int mux (char *packet, int &packet_length) override;
  int demux (char *packet, int &packet_length) override;

private:
  std::unique_ptr<PM_Server> pm_server_;
};

#endif /* _SM_SERVER_H */
