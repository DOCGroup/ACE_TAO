//=============================================================================
/**
 *  @file    Foo_C_ClientEngine.h
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
  ~Foo_C_ClientEngine() override;

  bool execute() override;

  static void expected_results(Foo_C_Statistics& stats);

  Foo_C_ClientEngine (Foo_C_ClientEngine const &) = delete;
  Foo_C_ClientEngine (Foo_C_ClientEngine &&) = delete;
  Foo_C_ClientEngine & operator= (Foo_C_ClientEngine const &) = delete;
  Foo_C_ClientEngine & operator= (Foo_C_ClientEngine &&) = delete;

private:
  Foo_C_var obj_;
  unsigned  client_id_;
};

#endif
