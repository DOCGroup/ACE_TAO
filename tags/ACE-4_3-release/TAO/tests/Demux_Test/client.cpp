#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tao_demuxC.h"
#include "tao/debug.h"

#include <iostream.h>
#include <fstream.h>

void print_exception (const CORBA_Exception     *x,
                      const char                  *info,
                      FILE                        *stream
                      );

char Usage[] = "\
Usage: client options\n\
options:\n\
-l strategy: operation lookup strategy - a(active), l(linear), d (dyn hash), p (perf)\n\
-i iter: number of iterations \n\
-m num: number of methods \n\
-o num: number of objects \n\
-t type: type can be l(linear), r(random), b (best), w (worst)\n\
";

#if !defined (__cplusplus)
typedef void (*SIG_TYP)(); 
#endif 

#ifdef SVR4
void
sigpipe (int foo)
#else
  void
sigpipe ()
#endif
{
}

static const char ior_prefix [] = "IOR:";
static const char iiop_prefix [] = "iiop:";

enum SendType{
  LINEAR,
  BEST,
  WORST,
  RANDOM
};

void do_test(tao_demux_ptr *demux, int olimit, int mlimit, SendType, int iter);

int
main (int argc, char *argv[])
{
    
  unsigned long addr_tmp;
  unsigned short done = 0;
  int c;
    
  CORBA_ORB_ptr	orb_ptr;
  CORBA_Environment	env;
  CORBA_Object_ptr	objref;
  tao_demux_ptr        *demux;
  unsigned		loop_count = 0;
  int			exit_later = 0;
  int                   i;
  int                   ObjectLimit = 5,
    MethodLimit = 4;
  int iter= 1; // number of iterations
  SendType              st = RANDOM;
  TAO_Operation_Table_Parameters *op_params = TAO_OP_TABLE_PARAMETERS::instance();

  CORBA_String str;
  fstream iorfile;

  while (!done && ((c = getopt (argc, argv, "i:l:t:m:o:")) !=
		   -1)){
    switch(c){
    case 'i':
      iter = atoi (optarg);
      break;
    case 'l': // op table lookup strategy
      break;
    case 'm':
      MethodLimit = atoi(optarg);
      break;
    case 'o':
      ObjectLimit = atoi(optarg);
      break;
    case 't':
      switch(*optarg){
      case 'b':
	st = BEST;
	break;
      case 'w':
	st = WORST;
	break;
      case 'l':
	st = LINEAR;
	break;
      case 'r':
      default:
	st = RANDOM;
	break;
      }
      break;
    default:
      goto usage;
    }
  }

  TAO_debug_level = 0;
  //
  // Transmitter
  //
  orb_ptr = CORBA_ORB_init (argc, argv, "ior", env);
  if (env.exception () != 0) {
    print_exception (env.exception (), "ORB initialization", stdout);
    return 1;
  }

  str = CORBA_string_alloc (500);

  // read the IOR from the file ior.txt
  iorfile.open("./ior.txt", ios::in);

  for (;!iorfile.eof ();){
    memset(str, '\0', 500);
    iorfile >> ((char *)str);
    if (iorfile.good ())
      loop_count++;
  }
  iorfile.close();

  ACE_DEBUG ((LM_DEBUG, "Total of %d object references read\n", loop_count));
  demux = new tao_demux_ptr [loop_count];
  for (i=0; i < loop_count; i++){
    demux[i] = 0;
  }

  // read the IOR from the file ior.txt
  iorfile.open("./ior.txt", ios::in);

  for (i=0; i < loop_count; i++){
    ACE_OS::memset (str, '\0', 500);
    iorfile >> ((char *)str);
    objref = orb_ptr->string_to_object(str, env);
    if (env.exception() != 0){
      print_exception(env.exception(), "string2object", stdout);
      return 1;
    }

    cout << "objref " << i << " has key = " << objref->_get_name(env) << endl;
    if (!CORBA_is_nil(objref)){
      // narrow to the desired interface type
      demux[i] = tao_demux::_narrow (objref);
    }
  }
  iorfile.close();
  CORBA_string_free(str);


  // send operations
  do_test(demux, ObjectLimit, MethodLimit, st, iter);

  for (i=0; i < loop_count; i++){
    CORBA_release(demux[i]);
  }
  delete [] demux;
  return(0);
usage:
  fprintf (stderr, Usage);
  return 1;
}

typedef void (*method_ptr)(tao_demux_ptr);
struct method_db {
  method_ptr method;
};

#include "client.i"

void do_test(tao_demux_ptr *demux, int olimit, int mlimit, SendType st, int iter)
{
  int i, j, k;
  int loop = 0;
  hrtime_t  start, end, sum, diff;
  fstream result;
  CORBA_Environment env;

  result.open("result.dat",ios::out|ios::app);
#ifdef __TAO_ACTIVE__
  result << "# ACTIVE ";
#endif
#ifdef __TAO_GPERF__
  result << "# GPERF ";
#endif
#ifdef __TAO_LINEAR__
  result << "# LINEAR ";
#endif

  //  result << "@type xy" << endl;
  sum = 0;
  
  if (st == LINEAR){
    result << " LINEAR" << endl;
#ifdef __DEBUG__
    cout << "Linear strategy for sending" << endl;
#endif
    for (k=0; k < iter; k++) {
      // invoke the jth method on the ith object
      for (i=0; i < olimit; i++){
	for (j=0; j < mlimit; j++){
	  loop++;
	  start = gethrtime();
	  mtbl[j].method(demux[i]);
	  end = gethrtime();
	  diff = (end - start);
#ifdef __DEBUG__
	  cout << "Latency for this request = " << diff/1.0e+06 << endl;
#endif
	  sum += diff;
	  //	result << loop << "\t" << diff/1.e+06 << endl;
	}
      }
    }
  } else if (st == BEST){
    result << " BEST" << endl;
#ifdef __DEBUG__
    cout << "best strategy for sending" << endl;
#endif
    for (k=0; k < iter; k++) {
      // invoke the jth method on the ith object
      for (i=0; i < olimit; i++){
	for (j=0; j < mlimit; j++){
	  loop++;
	  start = gethrtime();
	  mtbl[0].method(demux[0]);
	  end = gethrtime();
	  diff = (end - start);
#ifdef __DEBUG__
	  cout << "Latency for this request = " << diff/1.0e+06 << endl;
#endif
	  sum += diff;
	  //	result << loop << "\t" << diff/1.e+06 << endl;
	}
      }
    }
  } else if (st == RANDOM) {
    result << " RANDOM" << endl;
#ifdef __DEBUG__
    cout << "Random strategy for sending" << endl;
#endif
    // choose the object and method name randomly
    srand48(time(NULL));
    long p, q;
    for (k=0; k < iter; k++) {
      for (i=0; i < olimit; i++){
	for (j=0; j < mlimit; j++){
	  loop++;
	  p = lrand48() % olimit;
	  q = lrand48() % mlimit;
	  start = gethrtime();
#ifdef DEBUG
	  ACE_DEBUG ((LM_DEBUG, "Invoking op %s on object with key %s\n",
		      mtbl[q], demux[p]->_get_name(env)));
#endif
	  mtbl[q].method(demux[p]);
	  end = gethrtime();
	  diff = (end - start);
#ifdef __DEBUG__
	  cout << "Latency for this request = " << diff/1.0e+06 << endl;
#endif
	  sum += diff;
	  //	result << loop << "\t" << diff/1.e+06 << endl;
	}
      }
    }
  } else if (st == WORST){
    result << " WORST" << endl;
#ifdef __DEBUG__
    cout << "worst strategy for sending" << endl;
#endif
    // invoke the jth method on the ith object
    for (k=0; k < iter; k++) {
      for (i=0; i < olimit; i++){
	for (j=0; j < mlimit; j++){
	  loop++;
	  start = gethrtime();
	  mtbl[mlimit-1].method(demux[olimit-1]);
	  end = gethrtime();
	  diff = (end - start);
#ifdef __DEBUG__
	  cout << "Latency for this request = " << diff/1.0e+06  << endl;
#endif
	  sum += diff;
	  //	result << loop << "\t" << diff/1.e+06 << endl;
	}
      }
    }
  }
  result << olimit << "\t" << mlimit << "\t" << sum/(loop*1000000.0) << endl;
  cout << olimit << "\t" << mlimit << "\t" << sum/(loop*1000000.0) << endl;
  result.close();
}

