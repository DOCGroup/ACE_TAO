//=============================================================================
/**
 *  @file    demux_stats.cpp
 *
 *  $Id$
 *
 *  @author Vishal Kachroo
 */
//=============================================================================


// FUZZ: disable check_for_math_include
#include <math.h>
#include "ace/Get_Opt.h"
#include "tao/corba.h"

class Demux_Stats
{

public:

  /// Calculates the average latency and Standard deviation.
  /// Expects the input data in my_results.dat.
  int
  calculate_avg_latency (void);

  /// parses args.
  int
  Demux_Stats::parse_args (int argc_, char * argv_ []);

private :

  /// temporary results file.
  FILE *result_fp_;

  int iterations;
};


// parse command line arguments (if any).
int
Demux_Stats::parse_args (int argc_, char * argv_ [])
{

  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("i:"));
  int c;

  iterations = 0;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      }

  return 0;
}


int
Demux_Stats::calculate_avg_latency ()
{

  CORBA::ULong i, time;
  char str1[50], str2[50], str3[50];

  double temp, mean_poa, mean_servant, mean_dispatch, mean_perfect, mean_dynamic, mean_binary, mean_linear, mean_parse;
  double deviation_poa, deviation_servant, deviation_dispatch;
  double std_deviation_poa, std_deviation_servant, std_deviation_dispatch;
  double sum_poa_temp, sum_servant_temp, sum_dispatch_temp;
  double time_temp, items_temp;

  double items;
  CORBA::ULong sum_poa, sum_servant, sum_dispatch, last_poa_time=0, last_servant_time=0, last_dispatch_time=0;
  CORBA::ULong sum_linear, sum_dynamic, sum_binary, sum_perfect, sum_parse;
  CORBA::ULong last_dynamic_time=0, last_linear_time=0, last_perfect_time=0, last_binary_time=0, last_parse_time = 0;

  int counter;

  if ((this->result_fp_ = ACE_OS::fopen ("my_results.dat", "r")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Demux_Test_Client::print_results - "
                         "Failed to open the results file for reading\n"),
                        -1);
    }

  sum_poa = 0;
  sum_servant = 0;
  sum_dispatch = 0;
  sum_dynamic = 0;
  sum_linear = 0;
  sum_perfect = 0;
  sum_binary = 0;
  sum_parse = 0;

  items = 0;
  counter = 0;

  while (fscanf (this->result_fp_, "%s %s %s %ld %ld", &str1, &str2, &str3, &i, &time) != EOF)
    {

      if(ACE_OS::strcmp (str1,"POA::locate_poa_i")==0 && (ACE_OS::strcmp (str3,"end") == 0))
        {
          if (items < 1) { items = items + 1; counter++; }
          else
            {
              sum_poa += time;
              last_poa_time = time;
              items = items + 1;
              ++counter;
            }
        }

      if(ACE_OS::strcmp (str1,"POA::find_servant")==0 && (ACE_OS::strcmp (str3,"end") == 0))
        {
          if (items < 1) { items = items + 1; counter++; }
          else
            {
              sum_servant += time;
              last_servant_time = time;
              items = items + 1;
              counter++;
            }
        }

      if(ACE_OS::strcmp (str1,"Servant::_dispatch")==0 && (ACE_OS::strcmp (str3,"end") == 0))
        {
          if (items < 1) { ++items; counter++; }
          else
            {
              sum_dispatch += time;
              last_dispatch_time = time;
              ++items;items++;
              counter++;
            }
        }

      if(ACE_OS::strcmp (str1,"TAO_Dynamic_Hash_OpTable::find")==0 && (ACE_OS::strcmp (str3,"end") == 0))
        {
          if (items < 1) { ++items; counter++; }
          else
            {
              sum_dynamic += time;
              last_dynamic_time = time;
              ++items;
              counter++;
            }
        }

      if(ACE_OS::strcmp (str1,"TAO_Linear_Search_OpTable::find")==0 && (ACE_OS::strcmp (str3,"end") == 0))
        {
          if (items < 1) { items++; counter++; }
          else
            {
              sum_linear += time;
              last_linear_time = time;
              items++;
              counter++;
            }
        }

      if(ACE_OS::strcmp (str1,"TAO_Perfect_Hash_OpTable::find")==0 && (ACE_OS::strcmp (str3,"end") == 0))
        {
          if (items < 1) { items++; counter++; }
          else
            {
              sum_perfect += time;
              last_perfect_time = time;
              items++;
              counter++;
            }
        }

      if(ACE_OS::strcmp (str1,"TAO_Binary_Search_OpTable::find")==0 && (ACE_OS::strcmp (str3,"end") == 0))
        {
          if (items < 1) { items++; counter++; }
          else
            {
              sum_binary += time;
              last_binary_time = time;
              items++;
              counter++;
            }
        }

      if(ACE_OS::strcmp (str1,"POA::parse_key")==0 && (ACE_OS::strcmp (str3,"end") == 0))
        {
          if (items < 1) { items++; counter++; }
          else
            {
              sum_parse += time;
              last_parse_time = time;
              items++;
              counter++;
            }
        }

        if (iterations !=0 )
          {
            if (counter == iterations)
              {
                mean_poa = sum_poa/items;
                mean_servant = sum_servant/items;
                mean_dispatch = sum_dispatch/items;
                mean_dynamic = sum_dynamic/items;
                mean_linear = sum_linear/items;
                mean_binary = sum_binary/items;
                mean_perfect = sum_perfect/items;
                ACE_OS::printf("Average response times = POA = %lf SERVANT = %lf DISPATCH = %lf PERFECT = %lf DYNAMIC = %lf BINARY = %lf LINEAR = %lf microsec\n",
                                mean_poa,
                                mean_servant,
                                mean_dispatch,
                                mean_perfect,
                                mean_dynamic,
                                mean_binary,
                                mean_linear);
                sum_poa = 0;
                sum_servant = 0;
                sum_dispatch = 0;
                sum_dynamic = 0;
                sum_linear = 0;
                sum_perfect = 0;
                sum_binary = 0;
                items = 0;
                counter = 0;
              }
          }
    }

  ACE_OS::fclose (this->result_fp_);

  if (iterations == 0)
    {
      sum_poa -= last_poa_time;
      sum_servant -= last_servant_time;
      sum_dispatch -= last_dispatch_time;
      sum_dynamic -= last_dynamic_time;
      sum_linear -= last_linear_time;
      sum_perfect -= last_perfect_time;
      sum_binary -= last_binary_time;
      sum_parse -= last_parse_time;

      //items = items - 4;

      items = items - 2;

      mean_poa = sum_poa/items;
      mean_servant = sum_servant/items;
      mean_dispatch = sum_dispatch/items;
      mean_dynamic = sum_dynamic/items;
      mean_linear = sum_linear/items;
      mean_binary = sum_binary/items;
      mean_perfect = sum_perfect/items;
      mean_parse = sum_parse/items;
    }

  // now compute standard deviation


  if ((this->result_fp_ = ACE_OS::fopen ("my_results.dat", "r")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Demux_Test_Client::print_results - "
                         "Failed to open the results file for reading\n"),
                        -1);
    }


  sum_poa_temp = 0;
  sum_servant_temp = 0;
  sum_dispatch_temp = 0;

  deviation_poa = 0.0;
  deviation_servant = 0.0;
  deviation_dispatch = 0.0;

  while (fscanf (this->result_fp_, "%s %s %s %ld %ld", &str1, &str2, &str3, &i, &time) != EOF)
    {
      time_temp = (double) time;

      if(ACE_OS::strcmp (str1,"POA::locate_poa_i")==0 && (ACE_OS::strcmp (str3,"end") == 0))
        {
          deviation_poa = time_temp - mean_poa;
          sum_poa_temp += deviation_poa * deviation_poa;
        }

      if(ACE_OS::strcmp (str1,"POA::find_servant")==0 && (ACE_OS::strcmp (str3,"end") == 0))
        {
          deviation_servant = time_temp - mean_servant;
          sum_servant_temp += deviation_servant * deviation_servant;
        }

      if(ACE_OS::strcmp (str1,"Servant::_dispatch")==0 && (ACE_OS::strcmp (str3,"end") == 0))
        {
          deviation_dispatch = time_temp - mean_dispatch;
          sum_dispatch_temp += deviation_dispatch * deviation_dispatch;
        }
    }

  items_temp = (double) items;

  if (items == 1)
    {
      std_deviation_poa = 0;
      std_deviation_servant = 0;
      std_deviation_dispatch = 0;
    }
  else
    {
      std_deviation_poa = sqrt (sum_poa_temp/(items_temp - 1));
      std_deviation_servant = sqrt (sum_servant_temp/(items_temp - 1));
      std_deviation_dispatch = sqrt (sum_dispatch_temp/(items_temp - 1));
    }

  // ACE_OS::unlink ("my_results.dat");
  ACE_OS::fclose (this->result_fp_);

  if (iterations == 0)
    {

      ACE_OS::printf("Average response times = POA = %lf SERVANT = %lf DISPATCH = %lf PERFECT = %lf DYNAMIC = %lf BINARY = %lf LINEAR = %lf PARSE = %lf microsec\n", mean_poa, mean_servant,
                     mean_dispatch,
                     mean_perfect,
                     mean_dynamic,
                     mean_binary,
                     mean_linear,
                     mean_parse);
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Demux_Stats demux_stats;

  demux_stats.parse_args (argc, argv);
  demux_stats.calculate_avg_latency ();
}
