//=============================================================================
/**
 *  @file    Foo_ClientEngine.h
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
  explicit Foo_ClientEngine (Foo_ptr obj, unsigned client_id = 0);
  ~Foo_ClientEngine () override;

  bool execute (unsigned num_loops) override;

  static void expected_results (Foo_Statistics& stats);

  Foo_ClientEngine (Foo_ClientEngine const &) = delete;
  Foo_ClientEngine (Foo_ClientEngine &&) = delete;
  Foo_ClientEngine &operator= (Foo_ClientEngine const &) = delete;
  Foo_ClientEngine &operator= (Foo_ClientEngine &&) = delete;

private:
  Foo_var obj_;
  unsigned client_id_ {};
};

#endif
