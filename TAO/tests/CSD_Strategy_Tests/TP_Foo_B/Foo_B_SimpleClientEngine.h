// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Foo_B_SimpleClientEngine.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef FOO_B_SIMPLE_CLIENT_ENGINE_H
#define FOO_B_SIMPLE_CLIENT_ENGINE_H

#include "CSD_TP_Foo_B_Export.h"
#include "ClientEngine.h"
#include "Foo_BC.h"

class Foo_B_Statistics;
class Callback_i;

class CSD_TP_Foo_B_Export Foo_B_SimpleClientEngine : public ClientEngine
{
  public:

    Foo_B_SimpleClientEngine(Foo_B_ptr obj,
                              Callback_ptr callback,
                              unsigned client_id = 0,
                              bool collocated = false);

    virtual ~Foo_B_SimpleClientEngine();

    virtual bool execute(void);

    static void expected_results(Foo_B_Statistics& stats);
    static unsigned expected_callbacks ();

  private:

    Foo_B_var obj_;
    Callback_var callback_;
    unsigned client_id_;
    bool collocated_;
};

#endif
