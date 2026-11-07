//=============================================================================
/**
 *  @file    ClientEngine.h
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
using ClientEngine_Handle = TAO_Intrusive_Ref_Count_Handle<ClientEngine>;

class CSD_TP_Test_Export ClientEngine : public TAO_Intrusive_Ref_Count_Base<TAO_SYNCH_MUTEX>
{
public:
  ClientEngine () = default;
  ~ClientEngine () override;

  virtual bool execute() = 0;

  ClientEngine (ClientEngine const &) = delete;
  ClientEngine (ClientEngine &&) = delete;
  ClientEngine & operator= (ClientEngine const &) = delete;
  ClientEngine & operator= (ClientEngine &&) = delete;
};

#endif
