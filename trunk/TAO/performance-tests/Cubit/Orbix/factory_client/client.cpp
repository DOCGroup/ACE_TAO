// $Id$

//**************************************************************************
//
// NAME :   client.C 
// DESCRIPTION:  
//
// Client for the Cubit example
//
//****************************************************************************

#include "ace/OS.h"
#include "ace/Get_Opt.h"
#include "cubit.h"

ACE_RCSID(factory_client, client, "$Id$")

int LOOP_COUNT;
char SERVER_HOST [1024];

inline int func (unsigned i) { return i - 117; }
void run_tests (Cubit_var, int);

// = TITLE
//     Parses the command line arguments and returns an error status
//
// = DESCRIPTION
//     This method parses the command line arguments
int parse_args(int argc, char *argv[])
{
  ACE_OS::strcpy (SERVER_HOST, "localhost");
  ACE_Get_Opt opts (argc, argv, "dh:n:O:x");
  int			c;
  
  while ((c = opts ()) != -1)
    switch (c) {
    case 'h':
      ACE_OS::strcpy (SERVER_HOST, opts.optarg);
      continue;
    case 'd':  // debug flag
      continue;
      
    case 'n':			// loop count
      LOOP_COUNT = (unsigned) ACE_OS::atoi (opts.optarg);
      continue;
      
    case 'O':			// stringified objref
      continue;
      
    case 'x':
      continue;
      
    case '?':
    default:
      ACE_OS::fprintf (stderr, "usage:  %s"
                       " [-d]"
                       " [-n loopcount]"
                       " [-h SERVER_HOST]"
                       " [-x]"
                       "\n", argv [0]
                       );
      return 1;
    }
  
  return 0;  // Indicates successful parsing of command line
}


//
// Mainline
//
int
main (int argc, char *argv[])
{
  if (parse_args (argc, argv) != 0)
    return -1;

  Cubit_var cb;
  Cubit_Factory_var cf;
  
  // cout << "attempting to contact server at host " << SERVER_HOST << '\n' ;
 
  //
  // Initialise client's binding to an
  // arbitrary cubit server (at some host)
  // 
  TRY {
    cf = Cubit_Factory::_bind ("", SERVER_HOST, IT_X);
  }
  CATCHANY {
    cerr << "Binding failed: " << IT_X;
  }
  ENDTRY;
  cb = cf->create_cubit ("My_Cubit");
  
  run_tests (cb, LOOP_COUNT);
  return 0;
}


void
run_tests (Cubit_var cb, int loop_count) 
{
    //
  // Make the calls in a loop.
  //
  unsigned i;
  unsigned call_count, error_count;

  call_count = 0;
  error_count = 0;

  ACE_Time_Value before, after;

  before = ACE_OS::gettimeofday();
  
  //
  // Cube an octet.
  //
  
  for (i = 0; i < loop_count; i++)
    {

      call_count++;

      CORBA::Octet arg_octet = func (i), ret_octet;

      TRY {
      ret_octet = cb->cube_octet (arg_octet);
      } 
      CATCHANY {
        cerr << "Call failed: " << IT_X;
        error_count++;
      } 
      ENDTRY;
      arg_octet = arg_octet * arg_octet * arg_octet;
      if (arg_octet != ret_octet) {
        ACE_OS::printf ("** cube_octet(%d)  (--> %d)\n", arg_octet , ret_octet);
        error_count++;
      }
    
  
  //
  // Cube a short.
  //
      call_count++;

      CORBA::Short arg_short = func (i), ret_short;

      TRY {
      ret_short = cb->cube_short (arg_short);
      } 
      CATCHANY {
        cerr << "Call failed: " << IT_X;
        error_count++;
      } 
      ENDTRY;
      arg_short = arg_short * arg_short * arg_short;
      if (arg_short != ret_short) {
        ACE_OS::printf ("** cube_short(%d)  (--> %d)\n", arg_short , ret_short);
        error_count++;
      }

  //
  // Cube a long.
  //
  
      call_count++;

      CORBA::Long arg_long = func (i), ret_long;

      TRY {
      ret_long = cb->cube_long (arg_long);
      } 
      CATCHANY {
        cerr << "Call failed: " << IT_X;
      } 
      ENDTRY;
      arg_long = arg_long * arg_long * arg_long;
      if (arg_long != ret_long) {
        ACE_OS::printf ("** cube_long(%d)  (--> %d)\n", arg_long , ret_long);
        error_count++;
      }


      //
      // Cube a "struct" ...
      //
      Cubit::Many	arg_struct, ret_struct;
      
       call_count++;
       
       arg_struct.l = func (i);
       arg_struct.s = func (i);
       arg_struct.o = func (i);
       
       TRY {
         ret_struct = cb->cube_struct (arg_struct);
       }
       CATCHANY {
         cerr << "Call failed: " << IT_X;
         error_count++;
       } 
       ENDTRY;
       arg_struct.l = arg_struct.l  * arg_struct.l  * arg_struct.l ;
       arg_struct.s = arg_struct.s  * arg_struct.s  * arg_struct.s ;
       arg_struct.o = arg_struct.o  * arg_struct.o  * arg_struct.o ;
       
       if (arg_struct.l  != ret_struct.l 
           || arg_struct.s  != ret_struct.s 
           || arg_struct.o  != ret_struct.o )
         {
           cerr << "** cube_struct ERROR\n";
           error_count++;
         }
    }
    
  
  after = ACE_OS::gettimeofday();
  
  if (call_count > 0) 
    {
      if (error_count == 0)
        {
          ACE_Time_Value diff = after - before;
          unsigned long	us = diff.sec() * 1000 * 1000 + diff.usec();
          
          us /= call_count;
          
          if (us > 0)
            ACE_OS::printf ("cube average call ACE_OS::time\t= %ld.%.03ldms, \t"
                            "%ld calls/second\n",
                            us / 1000, us % 1000,
                            1000000L / us);
        }
      
      ACE_OS::printf ("%d calls, %d errors\n", call_count, error_count);
    }
}
