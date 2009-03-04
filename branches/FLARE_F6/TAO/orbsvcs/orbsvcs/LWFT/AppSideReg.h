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

#include "lwft_server_export.h"

/**
 *  @class  AppSideReg
 *
 *  @brief Encapsulates AppSideReg
 */

class LWFT_Server_Export AppSideReg
{
public:
  AppSideReg (void);
  ~AppSideReg (void);

  int register_process (void);
  
  // This needs to be moved somewhere else.
//  void unregister_process (void);
};



#endif /// __APPSIDEREG_H_
