/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SMR_Server.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _SMR_SERVER_H
#define _SMR_SERVER_H

#include "SM_Server.h"

class SMR_Server : public SM_Server
{
public:
  SMR_Server (short port_number);
  ~SMR_Server ();
};

#endif /* _SMR_SERVER_H */
