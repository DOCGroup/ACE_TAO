// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Foo_C_ClientEngine.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef FOO_C_CLIENT_ENGINE_H
#define FOO_C_CLIENT_ENGINE_H

#include "CSD_TP_Foo_C_Export.h"
#include "ClientEngine.h"
#include "Foo_CC.h"
#include "Foo_C_Statistics.h"


class CSD_TP_Foo_C_Export Foo_C_ClientEngine : public ClientEngine
{
  public:

    Foo_C_ClientEngine(Foo_C_ptr obj,
                       unsigned  client_id);
    virtual ~Foo_C_ClientEngine();

    virtual bool execute(void);

    static void expected_results(Foo_C_Statistics& stats);

  private:

    Foo_C_var obj_;
    unsigned  client_id_;
};

#endif
