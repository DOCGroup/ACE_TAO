// @ (#)clnt.cpp 1.2 05/14/97 Copyright 1994-1995 by Sun Microsystems
// Inc.  All Rights Reserved
//
// TEST:	Simple "cube" client, calling hand-crafted stubs.
//
// Modified version of Cubit Example written by Sun Microsystems Inc.
// Modified by: Brian Mendel

#include "ace/Get_Opt.h"
#include "tao/corba.h"
#include "cubitC.h"

inline int func (unsigned i) { return i - 117; }

extern void
print_exception (const CORBA::Exception *, const char *, FILE *f=stdout);

// Global variables
static const char *TAO_arg_ior = 0;
static u_int loop_count = 1;
static int exit_later = 0;

// Parses the command line arguments and returns an error status.

static int 
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "dn:O:x");
  int c;
   
  while ((c = opts ()) != -1)
    switch (c) 
      {
      case 'd':  // debug flag
	TAO_debug_level++;
	break;
      case 'n':			// loop count
	loop_count = (u_int) ACE_OS::atoi (opts.optarg);
	break;
      case 'O':			// stringified objref
	TAO_arg_ior = ACE_OS::strdup (opts.optarg);
	break;
      case 'x':
	exit_later++;
	break;
      case '?':
      default:
	ACE_ERROR_RETURN ((LM_ERROR,
			   "usage:  %s"
			   " [-d]"
			   " [-n loopcount]"
			   " [-O objref]"
			   " [-x]"
			   "\n", argv [0]
			   ), 1);
      }
   
  return 0;  // Indicates successful parsing of command line
}

static void
cube_union_stub (u_int i,
		 u_int &call_count,
		 u_int &error_count,
		 CORBA::Object_ptr objref,
		 CORBA::Environment &env)
{
  Cubit_ptr cubit = Cubit::_narrow (objref);

  Cubit_oneof u, *r;
   
  call_count++;
   
  u._disc = e_2nd;
  u.l = 3;
   
  // Cube a "union" ...
  r = cubit->Cubit_cube_union (u, env);

  if (env.exception () != 0) 
    {
      print_exception (env.exception (), "from cube_union");
      error_count++;
    } 
  else 
    {
      dmsg ("cube union ...");
      u.l = u.l * u.l * u.l ;
      
      if (u.l != r->l) 
	{
	  ACE_DEBUG ((LM_DEBUG, "** cube_union ERROR\n"));
	  error_count++;
	}
      
      delete r;
    }
   
  call_count++;
   
  u._disc = e_5th;
  u.cm.l = func (i);
  u.cm.s = func (i);
  u.cm.o = func (i);
   
  u.cm.l = 7;
  u.cm.s = 5;
  u.cm.o = 3;
   
  // Cube another "union" which uses the default arm ...
  r = cubit->Cubit_cube_union (u, env);

  if (env.exception () != 0) 
    {
      print_exception (env.exception (), "from cube_union");
      error_count++;
    } 
  else 
    {
      dmsg ("cube union ...");
      u.cm.l = u.cm.l * u.cm.l * u.cm.l;
      u.cm.s = u.cm.s * u.cm.s * u.cm.s;
      u.cm.o = u.cm.o * u.cm.o * u.cm.o;
      
      if (u.cm.l != r->cm.l
	  || u.cm.s != r->cm.s
	  || u.cm.o != r->cm.o) 
	{
	  ACE_DEBUG ((LM_DEBUG, "** cube_union ERROR\n"));
	  error_count++;
	}
      
      delete r;
    }
}

static void
cube_union_dii (u_int &call_count,
                u_int &error_count,
                CORBA::Object_ptr objref,
                CORBA::Environment &env)
{
  // Create the request ...
  CORBA::Request_ptr req;
   
  call_count++;

  req = objref->_request ((const CORBA::String) "cube_union", env);

  if (env.exception () != 0) 
    {
      error_count++;
      
      print_exception (env.exception (), "cube_union_dii request create");
      return;
    }

  // ... initialise the argument list and result ...
  Cubit_oneof	u, *r;
   
  u._disc = e_3rd;
  u.cm.l = 5;
  u.cm.s = -7;
  u.cm.o = 3;

  CORBA::Any tmp_arg (TC_Cubit_oneof, &u, CORBA::B_FALSE);
   
  req->arguments ()->add_value (0, tmp_arg, CORBA::ARG_IN, env);

  if (env.exception () != 0) 
    {
      error_count++;
      print_exception (env.exception (), "cube_union_dii request arg add");
      CORBA::release (req);
      return;
    }
	
  req->result ()->value ()->replace (TC_Cubit_oneof, 0, CORBA::B_TRUE, env);

  if (env.exception () != 0) 
    {
      error_count++;
      print_exception (env.exception (), "cube_union_dii result type");
      CORBA::release (req);
      return;
    }

  // Make the invocation, verify the result.

  req->invoke ();

  if (req->env ()->exception () != 0) 
    {
      error_count++;
      print_exception (req->env ()->exception (),"cube_union_dii invoke");
      CORBA::release (req);
      return;
    }

  r = (Cubit_oneof *) req->result ()->value ()->value ();
   
  if (r->cm.o != 27 || r->cm.l != 125 || r->cm.s != -343) 
    {
      error_count++;
      ACE_ERROR ((LM_ERROR, "cube_union_dii -- bad results\n"));
    }
  else
    dmsg ("cube_union_dii ... success!!");
   
  CORBA::release (req);
}

int
main (int argc, char *argv[])
{
  CORBA::Object_ptr objref = CORBA::Object::_nil ();
  CORBA::Environment env;

  CORBA::ORB_ptr orb_ptr = CORBA::ORB_init (argc, argv, "internet", env);

  if (env.exception () != 0)
    {
      print_exception (env.exception (), "ORB initialization");
      return 1;
    }

  // Parse command line and verify parameters.
  parse_args (argc, argv);

  if (TAO_arg_ior == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%s: must specify an object reference using -O <ior>\n",
		       argv[0]), 
		      1);

  objref = orb_ptr->string_to_object ((CORBA::String) TAO_arg_ior, env);

  ACE_OS::free ((void *) TAO_arg_ior);
  TAO_arg_ior = 0;

  if (env.exception () != 0)
    {
      print_exception (env.exception (), "string2object");
      return 1;
    }

  if (CORBA::is_nil (objref) == CORBA::B_TRUE)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "%s:  must identify non-null target objref\n",
		       argv [0]),
		      1);

  // Narrow the CORBA::Object reference to the stub object, checking
  // the type along the way using _is_a.
  Cubit_ptr cubit = Cubit::_narrow (objref);

  if (cubit == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to narrow object reference to a Cubit_ptr.\n"),
		      1);

  // Make the calls in a loop.

  u_int i;
  u_int call_count, error_count;

  call_count = 0;
  error_count = 0;

  ACE_Time_Value before;

  // @@ We should use an ACE_Profile_Timer here...
  before = ACE_OS::gettimeofday ();

  for (i = 0; i < loop_count; i++)
    {
      CORBA::Octet arg_octet;
      CORBA::Octet ret_octet;
       
      call_count++;

      // Cube an octet.
      ret_octet = cubit->Cubit_cube_octet (arg_octet = func (i), env);

      if (env.exception () != 0)
        {
          print_exception (env.exception (), "from cube_octet");
          error_count++;
        }
      else
        {
          dmsg2 ("cube octet:  %d --> %d\n", arg_octet, ret_octet);
          arg_octet = arg_octet * arg_octet * arg_octet;
          if (arg_octet != ret_octet) {
            ACE_DEBUG ((LM_DEBUG,
			"** cube_octet (%d) ERROR (--> %d)\n",
			(CORBA::Octet) func (i),
			ret_octet));
            error_count++;
          }
        }

      CORBA::Short arg_short, ret_short;
       
      call_count++;

      // Cube a short.
      ret_short = cubit->Cubit_cube_short (arg_short = func (i), env);

      if (env.exception () != 0)
        {
          print_exception (env.exception (),
			   "from cube_short");
          error_count++;
        }
      else
        {
          dmsg2 ("cube short:  %d --> %d\n",
		 arg_short,
		 ret_short);
          arg_short = arg_short * arg_short * arg_short;

          if (arg_short != ret_short)
            {
              ACE_ERROR ((LM_ERROR, "** cube_short (%d) ERROR (--> %d)\n",
			  (CORBA::Short) func (i),
			  ret_short));
              error_count++;
            }
        }

      CORBA::Long arg_long, ret_long;
       
      call_count++;

      // Cube a long.
      ret_long = cubit->Cubit_cube_long (arg_long = func (i), env);

      if (env.exception () != 0)
        {
          print_exception (env.exception (), "from cube_long");
          error_count++;
        }
      else
        {
          dmsg2 ("cube long:  %d --> %d\n", arg_long, ret_long);
          arg_long = arg_long * arg_long * arg_long;
          if (arg_long != ret_long) {
            ACE_ERROR ((LM_ERROR,
			"** cube_long (%ld) ERROR (--> %ld)\n",
			(CORBA::Long) func (i),
			ret_long));
            error_count++;
          }
        }

      Cubit_Many arg_struct, *ret_struct;
       
      call_count++;
       
      arg_struct.l = func (i);
      arg_struct.s = func (i);
      arg_struct.o = func (i);
       
      // Cube a "struct" ...
      ret_struct = cubit->Cubit_cube_struct (arg_struct, env);

      if (env.exception () != 0)
        {
          print_exception (env.exception (), "from cube_struct");
          error_count++;
        }
      else
        {
          dmsg ("cube struct ...");
          arg_struct.l = arg_struct.l * arg_struct.l * arg_struct.l;
          arg_struct.s = arg_struct.s * arg_struct.s * arg_struct.s;
          arg_struct.o = arg_struct.o * arg_struct.o * arg_struct.o;
          
          if (arg_struct.l != ret_struct->l
              || arg_struct.s != ret_struct->s
              || arg_struct.o != ret_struct->o)
            {
              ACE_ERROR ((LM_ERROR, "** cube_struct ERROR\n"));
              error_count++;
            }

          delete ret_struct;
        }
       
    }
    
  ACE_Time_Value after = ACE_OS::gettimeofday ();
    
  if (call_count > 0) 
    {
      if (error_count == 0)
        {
          ACE_Time_Value diff = after - before;
          unsigned long us = diff.sec () * 1000 * 1000 + diff.usec ();
          
          us /= call_count;
          
          if (us > 0)
            ACE_DEBUG ((LM_DEBUG,
			"cube average call ACE_OS::time\t= %d.%.03dms, \t"
			"%d calls/second\n",
			us / 1000, us % 1000,
			1000000L / us));
        }

      ACE_DEBUG ((LM_DEBUG, "%d calls, %d errors\n", call_count, error_count));
    }

  // Simple test for DII: call "cube_struct". (It's not timed since
  // the copious mallocation of DII would bias numbers against typical
  // stub-based calls).
  do 
    {
      // Create the request ...

      CORBA::Request_ptr	req = 
	objref->_request ((const CORBA::String) "cube_struct", env);

      if (env.exception () != 0) 
	{
	  print_exception (env.exception (), "DII request create");
	  break;
	}

      // ... initialise the argument list and result ...
      Cubit_Many arg, *result;
       
      arg.o = 3;
      arg.l = 5;
      arg.s = -7;
       
      CORBA::Any	tmp_arg (TC_Cubit_Many, &arg, CORBA::B_FALSE);
       
      req->arguments ()->add_value (0, tmp_arg, CORBA::ARG_IN, env);

      if (env.exception () != 0) 
	{
	  print_exception (env.exception (), "DII request arg add");
	  CORBA::release (req);
	  break;
	}
       
      req->result ()->value ()->replace (TC_Cubit_Many, 0, CORBA::B_TRUE, env);

      if (env.exception () != 0) 
	{
	  print_exception (env.exception (), "DII request result type");
	  CORBA::release (req);
	  break;
	}

      // Make the invocation, verify the result

      req->invoke ();

      if (req->env ()->exception () != 0) 
	{
	  print_exception (req->env ()->exception (), "DII invoke");
	  CORBA::release (req);
	  break;
	}
       
      result = (Cubit_Many *) req->result ()->value ()->value ();
       
      if (result->o != 27 || result->l != 125 || result->s != -343)
	ACE_ERROR ((LM_ERROR, "DII cube_struct -- bad results\n"));
      else
	dmsg ("DII cube_struct ... success!!");
       
      CORBA::release (req);
       
    } while (0);

  // Two more tests, using the "cube_union" function

  cube_union_dii (call_count, error_count, objref, env);

  if (env.exception () != 0)
    error_count++;
    
  cube_union_stub (i, call_count, error_count, objref, env);

  if (env.exception () != 0)
    error_count++;
    
  if (exit_later) 
    {
      cubit->Cubit_please_exit (env);
      dexc (env, "server, please ACE_OS::exit");
    }
    
  CORBA::release (objref);
  CORBA::release (orb_ptr);
    
  return (error_count == 0) ? 0 : 1;
}


#if 0
static void
cube_union_stub(unsigned          i,
                unsigned          &call_count,
                unsigned          &error_count,
                CORBA::Object_ptr  objref,
                CORBA::Environment &env)
{
   Cubit_ptr aCubit = Cubit::_narrow(objref);
   //
   // Cube a "union" ...
   //
   Cubit_oneof    u, *r;
   
   call_count++;
   
   u._disc = e_2nd;
   u.l = 3;
   
   r = aCubit->Cubit_cube_union (u, env);
   if (env.exception () != 0) {
      print_exception (env.exception (), "from cube_union");
      error_count++;
   } else {
      dmsg ("cube union ...");
      u.l = u.l  * u.l * u.l ;
      
      if (u.l != r->l) {
         ACE_OS::printf ("** cube_union_stub ERROR with discrim = e_2nd\n");
         error_count++;
      }
      
      delete r;
   }
   
   //
   // Cube another "union" which uses the default arm ...
   //
   call_count++;
   
   u._disc = e_5th;
   u.cm.l = func (i);
   u.cm.s = func (i);
   u.cm.o = func (i);
   
   u.cm.l = 7;
   u.cm.s = 5;
   u.cm.o = 3;
   
   r = aCubit->Cubit_cube_union (u, env);
   if (env.exception () != 0) {
      print_exception (env.exception (), "from cube_union");
      error_count++;
   } else {
      dmsg ("cube union ...");
      u.cm.l = u.cm.l * u.cm.l * u.cm.l;
      u.cm.s = u.cm.s * u.cm.s * u.cm.s;
      u.cm.o = u.cm.o * u.cm.o * u.cm.o;
      
      if (u.cm.l != r->cm.l
         || u.cm.s != r->cm.s
         || u.cm.o != r->cm.o) {
         ACE_OS::printf ("** cube_union_stub ERROR with discrim = e_5th\n");
         error_count++;
      }
      
      delete r;
   }

  return error_count == 0 ? 0 : 1;
}



static void
cube_union_dii (unsigned          &call_count,
                unsigned          &error_count,
                CORBA::Object_ptr  objref,
                CORBA::Environment &env)
{
   //
   // Create the request ...
   //
   CORBA::Request_ptr	req;
   
   call_count++;

   req = objref->_request ((const CORBA::String) "cube_union", env);
   if (env.exception () != 0) {
      error_count++;
      
      print_exception (env.exception (), "cube_union_dii request create");
      return;
   }

   //
   // ... initialise the argument list and result ...
   //
   Cubit_oneof	u, *r;
   
   u._disc = e_3rd;
   u.cm.l = 5;
   u.cm.s = -7;
   u.cm.o = 3;

   CORBA::Any	tmp_arg (TC_Cubit_oneof, &u, CORBA::B_FALSE);
   
   req->arguments ()->add_value (0, tmp_arg, CORBA::ARG_IN, env);
   if (env.exception () != 0) {
      error_count++;
      print_exception (env.exception (), "cube_union_dii request arg add");
      CORBA::release (req);
      return;
   }
	
   req->result ()->value ()->replace (TC_Cubit_oneof, 0, CORBA::B_TRUE, env);
   if (env.exception () != 0) {
      error_count++;
      print_exception (env.exception (), "cube_union_dii result type");
      CORBA::release (req);
      return;
   }

   //
   // Make the invocation, verify the result
   //
   req->invoke ();
   if (req->env ()->exception () != 0) {
      error_count++;
      print_exception (req->env ()->exception (),"cube_union_dii invoke");
      CORBA::release (req);
      return;
   }

   r = (Cubit_oneof *) req->result ()->value ()->value ();
   
   if (r->cm.o != 27 || r->cm.l != 125 || r->cm.s != -343) {
      error_count++;
      ACE_OS::fprintf (stderr, "cube_union_dii -- bad results\n");
   }
   else
      dmsg ("cube_union_dii ... success!!");
   
   CORBA::release (req);
}
#endif




