// $Id$
#ifndef STATISTICS_C_H
#define STATISTICS_C_H

#include "CSD_TP_Foo_C_Export.h"
#include "StatisticsHelper.h"


class CSD_TP_Foo_C_Export Foo_C_Statistics
{
  public:

    Foo_C_Statistics(unsigned num_remote_clients,
                     unsigned num_collocated_clients);

    virtual ~Foo_C_Statistics();

    void expected(unsigned op_num, unsigned count, bool remote_client);
    void actual(unsigned op_num, unsigned count);
    void actual_in_values(unsigned op_num, LongVector lv);

    bool actual_vs_expected();


  private:

    unsigned num_remote_clients_;
    unsigned num_collocated_clients_;
    unsigned expected_[10];
    unsigned actual_[10];

    LongVector actual_in_values_ [10];
};

#endif
