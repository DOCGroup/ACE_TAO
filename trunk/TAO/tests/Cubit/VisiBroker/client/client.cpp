// $Id$

//**************************************************************************
//
// NAME :   client.C 
// DESCRIPTION:  
//
// Client for the Cubit example
//
//****************************************************************************

#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "cubitC.h"
#include "Profile_Timer.h"

ACE_RCSID(client, client, "$Id$")

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
  if (argc != 3) {
    cerr << "Format: client <machine name> <loop count>" << endl;
    return -1;
  }

  strcpy(SERVER_HOST, argv[1]);

  LOOP_COUNT = atoi(argv[2]);
  
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
  // cout << "attempting to contact server at host " << SERVER_HOST << '\n' ;
 
  //
  // Initialise client's binding to an
  // arbitrary cubit server (at some host)
  // 
  try {
    cb = Cubit::_bind ("Cubit", SERVER_HOST);

  } catch (const CORBA::Exception & sysEx) {
    cerr << "Binding failed: " << endl;
    cerr << sysEx;
  } catch (...) {
    cerr << "Unexpected exception" << endl;
  }       
  
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

  Profile_Timer pt;
  Elapsed_Time et;

  //
  // Cube an octet.
  //
  
  call_count = 0;
  error_count = 0;
  pt.start();

  for (i = 0; i < loop_count; i++)
    {

      call_count++;

      CORBA::Octet arg_octet = func (i), ret_octet;

      try {
	ret_octet = cb->cube_octet (arg_octet);
      
      } catch (const CORBA::Exception &sysEx) {
	cerr << "Call failed: " << endl;
	cerr << sysEx;
	error_count++;
      } catch (...) {
	cerr << "Unexpected exception" << endl;
	error_count++;
      }       

      arg_octet = arg_octet * arg_octet * arg_octet;
      if (arg_octet != ret_octet) {
        printf ("** cube_octet(%d)  (--> %d)\n", arg_octet , ret_octet);
        error_count++;
      }
    }
    
  pt.stop();
  pt.elapsed_time(et);
  
  if (call_count > 0) 
    {
      if (error_count == 0)
        {
          unsigned long	us = et.real_time * 1000 * 1000;
          
          us /= call_count;
          
          if (us > 0)
            printf ("cube octet average call ACE_OS::time\t= %ld.%.03ldms, \t"
                    "%ld calls/second\n",
                    us / 1000, us % 1000,
                    1000000L / us);
        }
      
      printf ("%d calls, %d errors\n", call_count, error_count);
    }
    
  
  //
  // Cube a short.
  //
  call_count = 0;
  error_count = 0;
  pt.start();

  for (i = 0; i < loop_count; i++)
    {

      call_count++;

      CORBA::Short arg_short = func (i), ret_short;

      try {
	ret_short = cb->cube_short (arg_short);
       
      } catch (const CORBA::Exception &sysEx) {
	cerr << "Call failed: " << endl;
	cerr << sysEx;
	error_count++;
      } catch (...) {
	cerr << "Unexpected exception" << endl;
	error_count++;
      }       

      arg_short = arg_short * arg_short * arg_short;
      if (arg_short != ret_short) {
        printf ("** cube_short(%d)  (--> %d)\n", arg_short , ret_short);
        error_count++;
      }
    }
    
  pt.stop();
  pt.elapsed_time(et);
  
  if (call_count > 0) 
    {
      if (error_count == 0)
        {
          unsigned long	us = et.real_time * 1000 * 1000;
          
          us /= call_count;
          
          if (us > 0)
            printf ("cube short average call ACE_OS::time\t= %ld.%.03ldms, \t"
                    "%ld calls/second\n",
                    us / 1000, us % 1000,
                    1000000L / us);
        }
      
      printf ("%d calls, %d errors\n", call_count, error_count);
    }

  //
  // Cube a long.
  //
  
  call_count = 0;
  error_count = 0;
  pt.start();

  for (i = 0; i < loop_count; i++)
    {

      call_count++;

      CORBA::Long arg_long = func (i), ret_long;

      try {
	ret_long = cb->cube_long (arg_long);
      } catch (const CORBA::Exception &sysEx) {
	cerr << "Call failed: " << endl;
	cerr << sysEx;
	error_count++;
      } catch (...) {
	cerr << "Unexpected exception" << endl;
	error_count++;
      }       

      arg_long = arg_long * arg_long * arg_long;
      if (arg_long != ret_long) {
        printf ("** cube_long(%d)  (--> %d)\n", arg_long , ret_long);
        error_count++;
      }
    }
    
  pt.stop();
  pt.elapsed_time(et);
  
  if (call_count > 0) 
    {
      if (error_count == 0)
        {
          unsigned long	us = et.real_time * 1000 * 1000;
          
          us /= call_count;
          
          if (us > 0)
            printf ("cube long average call ACE_OS::time\t= %ld.%.03ldms, \t"
                    "%ld calls/second\n",
                    us / 1000, us % 1000,
                    1000000L / us);
        }
      
      printf ("%d calls, %d errors\n", call_count, error_count);
    }


  //
  // Cube a "struct" ...
  //
  Cubit::Many	arg_struct, ret_struct;
      
  call_count = 0;
  error_count = 0;
  pt.start();

  for (i = 0; i < loop_count; i++)
    {

      call_count++;
       
      arg_struct.l = func (i);
      arg_struct.s = func (i);
      arg_struct.o = func (i);
       
      try {
        ret_struct = cb->cube_struct (arg_struct);

      } catch (const CORBA::Exception &sysEx) {
        cerr << "Call failed: " << endl;
        cerr << sysEx;
        error_count++;
      } catch (...) {
        cerr << "Unexpected exception" << endl;
        error_count++;
      }       


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
    
  pt.stop();
  pt.elapsed_time(et);
  
  if (call_count > 0) 
    {
      if (error_count == 0)
        {
          unsigned long	us = et.real_time * 1000 * 1000;
          
          us /= call_count;
          
          if (us > 0)
            printf ("cube struuct average call ACE_OS::time\t= %ld.%.03ldms, \t"
                    "%ld calls/second\n",
                    us / 1000, us % 1000,
                    1000000L / us);
        }
      
      printf ("%d calls, %d errors\n", call_count, error_count);
    }
}
