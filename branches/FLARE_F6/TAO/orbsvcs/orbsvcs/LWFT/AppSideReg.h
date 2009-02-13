// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Interface: AppSideReg
 *
 *  @brief Declares interface for AppSideReg.
 *
 */

#ifndef __APPSIDEREG_H_
#define __APPSIDEREG_H_

#include <string>

#include "HostMonitorC.h"
#include "AppSideMonitor_Thread.h"

/**
 *  @class  AppSideReg
 *
 *  @brief Encapsulates AppSideReg
 */

class ACE_Barrier;

class LWFT_Server_Export AppSideReg
{
public:
  AppSideReg (CORBA::ORB_ptr);

  ~AppSideReg (void);

  int register_process (void);
  
  void unregister_process (void);

private:
  u_short port_;
  std::string HM_ior_;
  std::auto_ptr <AppSideMonitor_Thread> monitor_;
  HostMonitor_var hmvar_;
  CORBA::ORB_var orb_;
};



#endif /// __APPSIDEREG_H_
