#ifndef FOO_I_H
#define FOO_I_H

#include <array>
#include "CSD_PT_TestServant_Export.h"
#include "TestServant/FooS.h"
#include "Foo_Statistics.h"

class CSD_PT_TestServant_Export Foo_i : public virtual POA_Foo
{
public:
  Foo_i ();
  ~Foo_i () override;

  Foo_i (Foo_i const &) = delete;
  Foo_i (Foo_i &&) = delete;
  Foo_i &operator= (Foo_i const &) = delete;
  Foo_i &operator= (Foo_i &&) = delete;

  void op1 () override;

  void op2 (CORBA::Long value) override;

  CORBA::Long op3 (CORBA::Long value) override;

  void op4 (CORBA::Long value) override;

  void op5 () override;

  void done () override;

  void gather_stats (Foo_Statistics& stats);

private:
  std::array<unsigned, 5> op_count_ {};
};

#endif
