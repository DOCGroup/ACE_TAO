// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    ClientEngine.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef CLIENT_ENGINE_H
#define CLIENT_ENGINE_H

#include "CSD_TP_Test_Export.h"
#include "tao/Intrusive_Ref_Count_Base_T.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
#include "tao/Environment.h"
#include "ace/Synch.h"
#include "ace/CORBA_macros.h"


class ClientEngine;
typedef TAO_Intrusive_Ref_Count_Handle<ClientEngine> ClientEngine_Handle;


class CSD_TP_Test_Export ClientEngine : public TAO_Intrusive_Ref_Count_Base<TAO_SYNCH_MUTEX>
{
  public:

    ClientEngine();
    virtual ~ClientEngine();

    virtual bool execute(void) = 0;
};

#endif
