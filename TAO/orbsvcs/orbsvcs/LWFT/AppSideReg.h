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

/**
 *  @class  AppSideReg
 *
 *  @brief Encapsulates AppSideReg
 */

class LWFT_Server_Export AppSideReg
{
public:
  AppSideReg (CORBA::ORB_ptr);

  ~AppSideReg (void);

  int register_process (void);
  
  // This needs to be moved somewhere else.
//  void unregister_process (void);

private:
  std::string HM_ior_;
  CORBA::ORB_var orb_;
};



#endif /// __APPSIDEREG_H_
