// $Id$

#ifndef ACE_ADAPTER_STATS_H
#define ACE_ADAPTER_STATS_H
#include "ace/pre.h"

#include "../../Stats.h"

class ACE_Throughput_Stats_W : public ACE_Throughput_Stats
{
public:
  ACE_Throughput_Stats_W (void)
  {}

  void dump_results (const wchar_t *msg, ACE_UINT32 scale_factor)
  { ACE_Throughput_Stats::dump_results (ACE_TEXT_WCHAR_TO_CHAR (msg), scale_factor); }
};

#include "ace/post.h"
#endif /* ! ACE_ADAPTER_STATS_H */
