// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Foo_A_ClientEngine.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef FOO_A_CLIENT_ENGINE_H
#define FOO_A_CLIENT_ENGINE_H

#include "CSD_TP_Foo_A_Export.h"
#include "ClientEngine.h"
#include "Foo_AC.h"

class Foo_A_Statistics;

class CSD_TP_Foo_A_Export Foo_A_ClientEngine : public ClientEngine
{
  public:

    Foo_A_ClientEngine(Foo_A_ptr obj, unsigned  client_id = 0, bool collocated = false);
    virtual ~Foo_A_ClientEngine();

    virtual bool execute(void);

    static void expected_results(Foo_A_Statistics& stats);

  private:

    Foo_A_var obj_;
    unsigned  client_id_;
    bool      collocated_;
};

#endif
