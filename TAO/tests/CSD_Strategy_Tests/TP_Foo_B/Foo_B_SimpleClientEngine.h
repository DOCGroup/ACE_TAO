//=============================================================================
/**
 *  @file    Foo_B_SimpleClientEngine.h
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
  explicit Foo_B_SimpleClientEngine(Foo_B_ptr obj,
                                    Callback_ptr callback,
                                    unsigned client_id = 0);

  ~Foo_B_SimpleClientEngine() override = default;

  bool execute() override;

  static void expected_results(Foo_B_Statistics& stats);
  static unsigned expected_callbacks ();

  Foo_B_SimpleClientEngine (Foo_B_SimpleClientEngine const &) = delete;
  Foo_B_SimpleClientEngine (Foo_B_SimpleClientEngine &&) = delete;
  Foo_B_SimpleClientEngine & operator= (Foo_B_SimpleClientEngine const &) = delete;
  Foo_B_SimpleClientEngine & operator= (Foo_B_SimpleClientEngine &&) = delete;

private:
  Foo_B_var obj_;
  Callback_var callback_;
  unsigned client_id_;
};

#endif
