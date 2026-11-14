/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SML_Server.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================

#ifndef _SML_SERVER_H
#define _SML_SERVER_H

#include "SM_Server.h"

class SML_Server : public SM_Server
{
public:
  SML_Server () = default;
  ~SML_Server () override = default;
};

#endif /* _SML_SERVER_H */
