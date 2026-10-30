//=============================================================================
/**
 *  @file    Foo_A_ClientEngine.h
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
  explicit Foo_A_ClientEngine(Foo_A_ptr obj, unsigned  client_id = 0);
  ~Foo_A_ClientEngine() override = default;

  bool execute() override;

  static void expected_results(Foo_A_Statistics& stats);

  Foo_A_ClientEngine (Foo_A_ClientEngine const &) = delete;
  Foo_A_ClientEngine (Foo_A_ClientEngine &&) = delete;
  Foo_A_ClientEngine & operator= (Foo_A_ClientEngine const &) = delete;
  Foo_A_ClientEngine & operator= (Foo_A_ClientEngine &&) = delete;

private:
  Foo_A_var obj_;
  unsigned  client_id_;
};

#endif
