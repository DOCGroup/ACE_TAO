// $Id$
#ifndef STATISTICS_B_H
#define STATISTICS_B_H

#include "CSD_TP_Foo_B_Export.h"
#include "StatisticsHelper.h"


class CSD_TP_Foo_B_Export Foo_B_Statistics
{
  public:

    enum In_Value_Type
    {
      FOO_B_STAT_NONE,
      FOO_B_STAT_STRING,
      FOO_B_STAT_LONG
    };

    Foo_B_Statistics(unsigned num_remote_clients,
                     unsigned num_collcated_clients);

    virtual ~Foo_B_Statistics();

    void expected(unsigned op_num, unsigned count, In_Value_Type type);
    void actual(unsigned op_num, unsigned count);
    void actual(unsigned op_num, LongVector lv);
    void actual(unsigned op_num, StringVector sv);
    void servant_error_count(unsigned error_count);

    void expected_callbacks (unsigned num_cbs);
    void actual_callbacks (unsigned num_cbs);

    bool actual_vs_expected();

private:

    unsigned num_clients_;
    unsigned num_collcated_clients_;

    LongVector actual_in_long_ [16];
    StringVector actual_in_string_ [16];

    unsigned expected_op_count_[16];
    unsigned actual_op_count_[16];
    unsigned expected_in_value_type_[16];

    unsigned expected_callbacks_;
    unsigned actual_callbacks_;

    unsigned servant_error_count_;
};

#endif
