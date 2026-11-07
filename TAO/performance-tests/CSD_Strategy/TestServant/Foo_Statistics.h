#ifndef FOO_STATISTICS_H
#define FOO_STATISTICS_H

#include <array>
#include "CSD_PT_TestServant_Export.h"

class CSD_PT_TestServant_Export Foo_Statistics
{
public:
  Foo_Statistics ();
  virtual ~Foo_Statistics ();

  Foo_Statistics (Foo_Statistics const &) = delete;
  Foo_Statistics (Foo_Statistics &&) = delete;
  Foo_Statistics &operator= (Foo_Statistics const &) = delete;
  Foo_Statistics &operator= (Foo_Statistics &&) = delete;

  void init (unsigned num_clients, unsigned num_loops);

  void expected (unsigned op_num, unsigned count);
  void actual (unsigned op_num, unsigned count);

  bool actual_vs_expected () const;

  unsigned total () const;

private:
  unsigned num_clients_ {};
  unsigned num_loops_ {};
  std::array<unsigned, 5> expected_ {};
  std::array<unsigned, 5> actual_ {};
};

#endif
