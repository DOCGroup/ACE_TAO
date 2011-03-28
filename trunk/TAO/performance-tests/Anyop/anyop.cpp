
//=============================================================================
/**
 *  @file   anyop.cpp
 *
 *  $Id$
 *
 * Modified from anyop.cpp in Param_Test to benchmark Any insertion and
 * extraction operators for various IDL types.
 *
 *
 *  @author Carlos O'RyanJeff Parsons
 */
//=============================================================================


#include "testC.h"
#include "tao/debug.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Stub.h"
#include "tao/Object_T.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sample_History.h"
#include "ace/Sched_Params.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;

  ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                          priority,
                                          ACE_SCOPE_PROCESS));

  int n = 50000;
  int insertion = 1;

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("dien:"));
      int opt;

      while ((opt = get_opt ()) != EOF)
        {
          switch (opt)
            {
            case 'd':
              TAO_debug_level++;
              break;
            case 'i':
              insertion = 1;
              break;
            case 'e':
              insertion = 0;
              break;
            case 'n':
              n = ACE_OS::atoi (get_opt.opt_arg ());
              break;
            case '?':
            default:
              ACE_DEBUG ((LM_DEBUG,
                          "Usage: %s "
                          "-d debug"
                          "-n <num> "
                          "\n",
                          argv[0]));
              return -1;
            }
        }

      CORBA::Boolean result = 0;
      int j;

      {
        CORBA::Object_var obj =
          orb->string_to_object ("corbaloc:iiop:localhost:1234/Foo/Bar");

        Param_Test_var param_test =
          TAO::Narrow_Utils<Param_Test>::unchecked_narrow (obj.in (),
                                                           0);
        TAO_Stub *stub = param_test->_stubobj ();
        stub->type_id = CORBA::string_dup ("IDL:Param_Test:1.0");

        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();

        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;

            if (insertion == 1)
              {
                ACE_hrtime_t start = ACE_OS::gethrtime ();

                any <<= param_test.in ();

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                Param_Test_ptr o;

                result = any >>= o;
              }
            else
              {
                any <<= param_test.in ();

                Param_Test_ptr o;

                ACE_hrtime_t start = ACE_OS::gethrtime ();

                result = any >>= o;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);
              }
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        if (insertion == 1)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Objref insertion test finished\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Objref extraction test finished\n"));
          }

        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      {
        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();

        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;
            CORBA::Short i = 123;

            if (insertion == 1)
              {
                ACE_hrtime_t start = ACE_OS::gethrtime ();

                any <<= i;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                CORBA::Short o;

                result = any >>= o;
                  }
                else
              {
                any <<= i;

                CORBA::Short o;

                ACE_hrtime_t start = ACE_OS::gethrtime ();

                result = any >>= o;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);
              }
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        if (insertion == 1)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Short insertion test finished\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Short extraction test finished\n"));
          }
        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      {
        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();

        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;
            CORBA::Long i = 123;

            if (insertion == 1)
              {
                ACE_hrtime_t start = ACE_OS::gethrtime ();

                any <<= i;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                CORBA::Long o;

                result = any >>= o;
              }
            else
              {
                any <<= i;

                CORBA::Long o;

                ACE_hrtime_t start = ACE_OS::gethrtime ();

                result = any >>= o;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);
              }
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        if (insertion == 1)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Long insertion test finished\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Long extraction test finished\n"));
          }

        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      {
        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();

        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;
            CORBA::Double i = 123;

            if (insertion == 1)
              {
                ACE_hrtime_t start = ACE_OS::gethrtime ();

                any <<= i;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                CORBA::Double o;

                result = any >>= o;
              }
            else
              {
                any <<= i;

                CORBA::Double o;

                ACE_hrtime_t start = ACE_OS::gethrtime ();

                result = any >>= o;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);
              }
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        if (insertion == 1)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Double insertion test finished\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Double extraction test finished\n"));
          }

        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      {
        Param_Test::Fixed_Struct i;
        i.l = -7;
        i.c = 'c';
        i.s = 5;
        i.o = 255;
        i.f = 2.3f;
        i.b = 0;
        i.d = 3.1416;

        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();

        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;

            if (insertion == 1)
              {
                ACE_hrtime_t start = ACE_OS::gethrtime ();

                any <<= i;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                Param_Test::Fixed_Struct *o;

                result = any >>= o;
              }
            else
              {
                any <<= i;

                Param_Test::Fixed_Struct *o;

                ACE_hrtime_t start = ACE_OS::gethrtime ();

                result = any >>= o;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);
              }
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        if (insertion == 1)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Struct copying insertion test finished\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Struct extraction test finished\n"));
          }

        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      {
        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();
        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;
            Param_Test::Fixed_Struct *i = 0;
            ACE_NEW_RETURN (i,
                            Param_Test::Fixed_Struct,
                            -1);
            i->l = -7;
            i->c = 'c';
            i->s = 5;
            i->o = 255;
            i->f = 2.3f;
            i->b = 0;
            i->d = 3.1416;

            if (insertion == 1)
              {
                ACE_hrtime_t start = ACE_OS::gethrtime ();

                any <<= i;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                Param_Test::Fixed_Struct *o;

                result = any >>= o;
              }
            else
              {
                any <<= i;

                Param_Test::Fixed_Struct *o;

                ACE_hrtime_t start = ACE_OS::gethrtime ();

                result = any >>= o;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);
              }
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        if (insertion == 1)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Struct non-copying insertion test finished\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Struct extraction test finished\n"));
          }

        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      {
        CORBA::ULong len = 1024;
        Param_Test::Long_Seq i (len);
        i.length (len);

        for (CORBA::ULong k = 0; k < len; ++k)
          {
            i[k] = 11;
          }

        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();

        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;

            if (insertion == 1)
              {
                ACE_hrtime_t start = ACE_OS::gethrtime ();

                any <<= i;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                Param_Test::Long_Seq *o;

                result = any >>= o;
              }
            else
              {
                any <<= i;

                Param_Test::Long_Seq *o;

                ACE_hrtime_t start = ACE_OS::gethrtime ();

                result = any >>= o;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);
              }
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        if (insertion == 1)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Sequence copying insertion test finished\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Sequence extraction test finished\n"));
          }

        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      {
        CORBA::ULong len = 1024;
        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();
        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;
            Param_Test::Long_Seq *i = 0;
            ACE_NEW_RETURN (i,
                            Param_Test::Long_Seq (len),
                            -1);
            i->length (len);

            for (CORBA::ULong k = 0; k < len; ++k)
              {
                (*i)[k] = 11;
              }

            if (insertion == 1)
              {
                ACE_hrtime_t start = ACE_OS::gethrtime ();

                any <<= i;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                Param_Test::Long_Seq *o;

                result = any >>= o;
              }
            else
              {
                any <<= i;

                Param_Test::Long_Seq *o;

                ACE_hrtime_t start = ACE_OS::gethrtime ();

                result = any >>= o;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);
              }
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        if (insertion == 1)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Sequence non-copying insertion test finished\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Sequence extraction test finished\n"));
          }

        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      {
        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();

        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;
            CORBA::Any i;
            i <<= CORBA::Short (123);

            if (insertion == 1)
              {
                ACE_hrtime_t start = ACE_OS::gethrtime ();

                any <<= i;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                const CORBA::Any *o;
                CORBA::Short oo;

                result = any >>= o;
                result = *o >>= oo;
              }
            else
              {
                any <<= i;

                const CORBA::Any *o;
                CORBA::Short oo;

                ACE_hrtime_t start = ACE_OS::gethrtime ();

                result = any >>= o;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                result = *o >>= oo;
              }
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        if (insertion == 1)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Any copying insertion test finished\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Any extraction test finished\n"));
          }

        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      {
        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();

        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;
            CORBA::Any *i = 0;
            ACE_NEW_RETURN (i,
                            CORBA::Any,
                            -1);
            *i <<= CORBA::Short (123);

            if (insertion == 1)
              {
                ACE_hrtime_t start = ACE_OS::gethrtime ();

                any <<= i;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                const CORBA::Any *o;
                CORBA::Short oo;

                result = any >>= o;
                result = *o >>= oo;
              }
            else
              {
                any <<= i;

                const CORBA::Any *o;
                CORBA::Short oo;

                ACE_hrtime_t start = ACE_OS::gethrtime ();

                result = any >>= o;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                result = *o >>= oo;
              }
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        if (insertion == 1)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Any non-copying insertion test finished\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Any extraction test finished\n"));
          }

        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      {
        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();

        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;
            const char i[] = "1234567890";

            if (insertion == 1)
              {
                ACE_hrtime_t start = ACE_OS::gethrtime ();

                any <<= i;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);

                const char *o;

                result = any >>= o;
                  }
            else
              {
                any <<= i;

                const char *o;

                ACE_hrtime_t start = ACE_OS::gethrtime ();

                result = any >>= o;

                ACE_hrtime_t now = ACE_OS::gethrtime ();
                history.sample (now - start);
              }
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        if (insertion == 1)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "String insertion test finished\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "String extraction test finished\n"));
          }

        ACE_ASSERT (result);

        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }

      ACE_DEBUG ((LM_DEBUG, "\n"));

      {
        ACE_Sample_History history (n);
        ACE_hrtime_t test_start = ACE_OS::gethrtime ();

        for (j = 0; j != n; ++j)
          {
            CORBA::Any any;
            const char i[] = "1234567890";
            any <<= i;
            CORBA::Any copy;

            ACE_hrtime_t start = ACE_OS::gethrtime ();

            copy = any;

            ACE_hrtime_t now = ACE_OS::gethrtime ();
            history.sample (now - start);
          }

        ACE_hrtime_t test_end = ACE_OS::gethrtime ();

        ACE_DEBUG ((LM_DEBUG,
                    "Copy test finished\n"));
        ACE_DEBUG ((LM_DEBUG,
                    "High resolution timer calibration...."));
        ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
        ACE_DEBUG ((LM_DEBUG,
                    "done\n"));

        ACE_Basic_Stats stats;
        history.collect_basic_stats (stats);
        stats.dump_results (ACE_TEXT("Total"), gsf);

        ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"),
                                               gsf,
                                               test_end - test_start,
                                               stats.samples_count ());
      }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Any performance test");
      return 1;
    }

  return 0;
}
