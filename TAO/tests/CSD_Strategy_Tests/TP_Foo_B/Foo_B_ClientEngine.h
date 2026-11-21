//=============================================================================
/**
 *  @file    Foo_B_ClientEngine.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef FOO_B_CLIENT_ENGINE_H
#define FOO_B_CLIENT_ENGINE_H

#include "CSD_TP_Foo_B_Export.h"
#include "ClientEngine.h"
#include "Foo_BC.h"

class Foo_B_Statistics;
class Callback_i;

class CSD_TP_Foo_B_Export Foo_B_ClientEngine : public ClientEngine
{
public:
  explicit Foo_B_ClientEngine(Foo_B_ptr obj,
                              Callback_ptr callback,
                              unsigned client_id = 0);

  ~Foo_B_ClientEngine() override = default;

  bool execute() override;

  static void expected_results(Foo_B_Statistics& stats);
  static unsigned expected_callbacks ();

  Foo_B_ClientEngine (Foo_B_ClientEngine const &) = delete;
  Foo_B_ClientEngine (Foo_B_ClientEngine &&) = delete;
  Foo_B_ClientEngine & operator= (Foo_B_ClientEngine const &) = delete;
  Foo_B_ClientEngine & operator= (Foo_B_ClientEngine &&) = delete;

private:
  Foo_B_var obj_;
  Callback_var callback_;
  unsigned client_id_;
};

#endif
