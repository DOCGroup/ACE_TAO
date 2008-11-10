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

#include "ace/Task.h"

#include "HostMonitorC.h"
#include "AppSideMonitor_Thread.h"

/**
 *  @class  AppSideReg
 *
 *  @brief Encapsulates AppSideReg
 */

class ACE_Barrier;

class LWFT_Server_Export AppSideReg : public ACE_Task_Base
{
public:
    AppSideReg (ACE_Barrier *ext_b, CORBA::ORB_ptr);

    ~AppSideReg (void);

    virtual int svc (void);
    void unregister_process (void);

private:
    std::string HM_ior_;
    std::auto_ptr <AppSideMonitor_Thread> monitor_;
    HostMonitor_var hmvar_;
    CORBA::ORB_var orb_;
    ACE_Barrier *external_barrier_;
};



#endif /// __APPSIDEREG_H_
