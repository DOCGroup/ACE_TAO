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

class SM_Client : public Select_Manager, public CM_Client
{
public:
  SM_Client ();
  virtual ~SM_Client ();

  virtual int mux (char *packet, int &packet_length);
  virtual int demux (char *packet, int &packet_length);
  virtual void process ();

private:
  PM_Client *pm_client;
};

#endif /* _SM_CLIENT_H */
