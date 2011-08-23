// Test of raw copy performance when using different sequence allocations
// $Id$

#include "sequenceC.h"
#include "ace/Time_Value.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_strings.h"
#include "ace/Log_Msg.h"


const char * short_str = "abcdefghijklmnopqrstuvwxyz";
const char * long_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ";

bool use_csv = false;

void stringseq_time_test (CORBA::ULong num_loops, bool use_long_str)
{
  ACE_High_Res_Timer timer;
  ACE_hrtime_t time;

  CORBA::ULong str_len;

  Str_Sequences seqs;
  seqs.first_str.length(0);
  seqs.second_str.length(0);

  // start timing
  timer.start();

  for (CORBA::ULong idx = 0; idx < num_loops; ++idx)
  {
    str_len = seqs.first_str.length();
    seqs.first_str.length(str_len + 1);
    seqs.first_str[str_len] = use_long_str ? long_str : short_str;
  }
  // end timing
  timer.stop();
  timer.elapsed_time(time);

  if (use_csv)
    {
      ACE_DEBUG((LM_INFO,
                 ACE_TEXT("1, 0, 0, %u, %s, %Q\n"),
                 num_loops,
                 use_long_str ? ACE_TEXT("long"): ACE_TEXT("short"),
                 time ));
    }
  else
    {
      ACE_DEBUG((LM_INFO,
                 ACE_TEXT("String sequence (%u, %s) = %Q ns\n"),
                 num_loops,
                 use_long_str ? ACE_TEXT("long"): ACE_TEXT("short"),
                 time ));
    }
}


void seqstrseq_time_test (CORBA::ULong num_seq_loops,
                          CORBA::ULong num_string_loops,
                          bool use_long_str)
{
  ACE_High_Res_Timer timer;
  ACE_hrtime_t time;

  CORBA::ULong sss_len;
  CORBA::ULong str_len;

  Sequence_Str_Sequences seqs;
  seqs.seq_str_seq.length(0);

  // start timing
  timer.start();

  for (CORBA::ULong seq_idx = 0; seq_idx < num_seq_loops; ++seq_idx)
  {
    sss_len = seqs.seq_str_seq.length();
    seqs.seq_str_seq.length(sss_len + 1);
    Str_Sequences & strs = seqs.seq_str_seq[sss_len];
    //strs.first_str.length(0);
    for (CORBA::ULong str_idx = 0; str_idx < num_string_loops; ++str_idx)
    {
      str_len = strs.second_str.length();
      strs.second_str.length(str_len + 1);
      strs.second_str[str_len] = use_long_str ? long_str : short_str;
    }
  }
  // end timing
  timer.stop();
  timer.elapsed_time(time);

  if (use_csv)
    {
      ACE_DEBUG((LM_INFO,
                 ACE_TEXT("2, 0, %u, %u, %s, %Q\n"),
                 num_string_loops,
                 num_seq_loops,
                 use_long_str ? ACE_TEXT("long"): ACE_TEXT("short"),
                 time ));
    }
  else
    {
      ACE_DEBUG((LM_INFO,
                 ACE_TEXT("Sequence of string seq (%u, %u, %s) = %Q ns\n"),
                 num_string_loops,
                 num_seq_loops,
                 use_long_str ? ACE_TEXT("long"): ACE_TEXT("short"),
                 time ));
    }
}


void big_time_test (CORBA::ULong num_list_loops,
                    CORBA::ULong num_seq_loops,
                    CORBA::ULong num_string_loops,
                    bool use_long_str)
{
  ACE_High_Res_Timer timer;
  ACE_hrtime_t time;

  Seq_Structs big_list;
  big_list.seq.length(0);

  CORBA::ULong big_len;
  CORBA::ULong red_len;
  CORBA::ULong sss_len;
  CORBA::ULong str_len;

  // start timing
  timer.start();

  for (CORBA::ULong list_idx = 0; list_idx < num_list_loops; ++list_idx)
  {
    big_len = big_list.seq.length();
    big_list.seq.length(big_len + 1);
    Biglist_Struct & list = big_list.seq[big_len];

    for (CORBA::ULong seq_idx = 0; seq_idx < num_seq_loops; ++seq_idx)
    {
      red_len = list.red_seqs.length();
      list.red_seqs.length(red_len + 1);

      Sequence_Str_Sequences & sss = list.red_seqs[red_len];

      sss_len = sss.seq_str_seq.length();
      sss.seq_str_seq.length(sss_len + 1);

      Str_Sequences & strs = sss.seq_str_seq[sss_len];
      for (CORBA::ULong str_idx = 0; str_idx < num_string_loops; ++str_idx)
      {
        str_len = strs.second_str.length();
        strs.second_str.length(str_len + 1);
        strs.second_str[str_len] = use_long_str ? long_str : short_str;

        str_len = list.strs.first_str.length();
        list.strs.first_str.length(str_len + 1);
        list.strs.first_str[str_len] = use_long_str ? long_str : short_str;

      } // end of str loop
    } // end of seq loop

  } // end of list loop
  // end timing
  timer.stop();
  timer.elapsed_time(time);

  if (use_csv)
    {
      ACE_DEBUG((LM_INFO,
                 ACE_TEXT("3, %u, %u, %u, %s, %Q\n"),
                 num_list_loops,
                 num_string_loops,
                 num_seq_loops,
                 use_long_str ? ACE_TEXT("long"): ACE_TEXT("short"),
                 time ));
    }
  else
    {
      ACE_DEBUG((LM_INFO,
                 ACE_TEXT("Big list test(%u, %u, %u, %s) = %Q ns\n"),
                 num_list_loops,
                 num_string_loops,
                 num_seq_loops,
                 use_long_str ? ACE_TEXT("long"): ACE_TEXT("short"),
                 time ));
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  if (argc > 1 && ACE_OS::strcasecmp (argv[1],ACE_TEXT("-csv")) == 0)
    use_csv = true;

  stringseq_time_test(100, false);
  stringseq_time_test(1000, false);

  stringseq_time_test(100, true);
  stringseq_time_test(1000, true);


  seqstrseq_time_test(100, 1, false);
  seqstrseq_time_test(100, 10, false);
  seqstrseq_time_test(1000, 10, false);

  seqstrseq_time_test(100, 1, true);
  seqstrseq_time_test(100, 10, true);
  seqstrseq_time_test(1000, 10, true);


  big_time_test(10, 1, 10, false);
  big_time_test(10, 1, 10, false);
  big_time_test(10, 10, 10, false);
  big_time_test(100, 1, 10, false);

  big_time_test(10, 1, 10, true);
  big_time_test(10, 1, 10, true);
  big_time_test(10, 10, 10, true);
  big_time_test(100, 1, 10, true);

  return 0;
}
