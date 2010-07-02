// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Foo_ClientEngine.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef FOO_CLIENT_ENGINE_H
#define FOO_CLIENT_ENGINE_H

#include "CSD_PT_TestServant_Export.h"
#include "TestInf/ClientEngine.h"
#include "TestServant/FooC.h"

class Foo_Statistics;


class CSD_PT_TestServant_Export Foo_ClientEngine : public ClientEngine
{
  public:

    Foo_ClientEngine(Foo_ptr obj, unsigned  client_id = 0);
    virtual ~Foo_ClientEngine();

    virtual bool execute(unsigned num_loops);

    static void expected_results(Foo_Statistics& stats);


  private:

    Foo_var  obj_;
    unsigned client_id_;
};

#endif
