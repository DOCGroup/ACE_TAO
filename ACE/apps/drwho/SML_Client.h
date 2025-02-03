/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SML_Client.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _SML_CLIENT_H
#define _SML_CLIENT_H

#include "SM_Client.h"
#include "SML_Server.h"

class SML_Client : public SM_Client
{
public:
  SML_Client ();
  virtual ~SML_Client ();
  virtual int receive (int timeout = 0);
  virtual int send ();

private:
  SML_Server sml_server;
  int packet_length;
};

#endif /* _SML_CLIENT_H */
