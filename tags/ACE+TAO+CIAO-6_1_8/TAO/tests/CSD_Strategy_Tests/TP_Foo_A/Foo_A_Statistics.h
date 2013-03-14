// $Id$
#ifndef STATISTICS_A_H
#define STATISTICS_A_H

#include "CSD_TP_Foo_A_Export.h"
#include "StatisticsHelper.h"


class CSD_TP_Foo_A_Export Foo_A_Statistics
{
  public:

    Foo_A_Statistics(unsigned num_clients);

    virtual ~Foo_A_Statistics();

    void expected(unsigned op_num, unsigned count);
    void actual(unsigned op_num, unsigned count);
    void actual_in_values(unsigned op_num, LongVector lv);

    bool actual_vs_expected();


  private:

    unsigned num_clients_;

    unsigned expected_[5];
    unsigned actual_[5];

    LongVector actual_in_values_ [5];
};

#endif
