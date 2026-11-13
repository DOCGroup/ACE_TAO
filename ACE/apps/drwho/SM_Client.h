/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SM_Client.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _SM_CLIENT_H
#define _SM_CLIENT_H

#include "PM_Client.h"
#include "CM_Client.h"
#include "Select_Manager.h"
#include <memory>

class SM_Client : public Select_Manager, public CM_Client
{
public:
  SM_Client () = default;
  ~SM_Client () override = default;
  SM_Client (SM_Client const &) = delete;
  SM_Client (SM_Client &&) = delete;
  SM_Client &operator= (SM_Client const &) = delete;
  SM_Client &operator= (SM_Client &&) = delete;

  int mux (char *packet, int &packet_length) override;
  int demux (char *packet, int &packet_length) override;
  virtual void process ();

private:
  std::unique_ptr<PM_Client> pm_client_;
};

#endif /* _SM_CLIENT_H */
