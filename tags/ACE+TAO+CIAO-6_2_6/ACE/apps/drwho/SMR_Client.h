/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SMR_Client.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _SMR_CLIENT_H
#define _SMR_CLIENT_H

#include "SM_Client.h"

class SMR_Client : public SM_Client
{
public:
  SMR_Client (short port_number);
  virtual ~SMR_Client (void);
};

#endif /* _SMR_CLIENT_H */
