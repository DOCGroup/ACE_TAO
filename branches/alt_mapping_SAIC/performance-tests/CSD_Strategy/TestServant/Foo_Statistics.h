// $Id$
#ifndef FOO_STATISTICS_H
#define FOO_STATISTICS_H

#include "CSD_PT_TestServant_Export.h"


class CSD_PT_TestServant_Export Foo_Statistics
{
  public:

    Foo_Statistics();
    virtual ~Foo_Statistics();

    void init(unsigned num_clients, unsigned num_loops);

    void expected(unsigned op_num, unsigned count);
    void actual(unsigned op_num, unsigned count);

    bool actual_vs_expected();

    unsigned total() const;


  private:

    unsigned num_clients_;
    unsigned num_loops_;
    unsigned expected_[5];
    unsigned actual_[5];
};

#endif
