// $Id$

// ============================================================================
//
// = LIBRARY
//    performance-tests/Misc
//
// = FILENAME
//    basic_perf.cpp
//
// = DESCRIPTION
//    Times various simple operations.
//
//    With Sun C++, use -O2: make CFLAGS="-mt -O2" BIN=basic_perf
//    -fast seems to produce slower times.
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#include "basic_func.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_main.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_sys_utsname.h"

static const char usage [] = "[-? |\n"
                             "            [-i <iterations> [1000000]]";

// These are global.  They appear to bust the CPU cache, on an Ultra 2
// w/Sun C++ 4.2.
static u_int iterations = 1000000;
Foo foo;
Foo_v foo_v;

inline
void
inline_func ()
{
  DO_SOMETHING
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function per_iteration
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Given an elapsed time in nanoseconds, returns the time per iteration
// in microseconds.
static
inline
double
per_iteration (const ACE_hrtime_t elapsed /* nanoseconds */)
{
  double ms_per_iteration = (double) ACE_CU64_TO_CU32 (elapsed) / 1000.0 /
                            (double) iterations;

  // Don't print out "-0.000" or "-0.001" . . .
  return -0.002 < ms_per_iteration  &&  ms_per_iteration < 0.0
           ?  0.0
           :  ms_per_iteration;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Basic_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Basic_Test
{
public:
  Basic_Test (ACE_High_Res_Timer &timer,
              ACE_hrtime_t empty_iteration_time);
  virtual ~Basic_Test (void);

  virtual void run (void) = 0;

  double iteration_time (void);

  void print_iteration_time (const char *message);

protected:
  ACE_hrtime_t elapsed_time_;

  void start_timing (void)
  {
    timer_.reset ();
    timer_.start ();
  }

  void stop_timing (void)
  {
    timer_.stop ();
    timer_.elapsed_time (elapsed_time_);
  }

private:
  ACE_High_Res_Timer &timer_;
  ACE_hrtime_t empty_iteration_time_;

  // Require the timer reference.
  Basic_Test (void);

  // Force construction of independent instances by prohibiting copying.
  Basic_Test (const Basic_Test &);
  Basic_Test &operator= (const Basic_Test &);
};

Basic_Test::Basic_Test (ACE_High_Res_Timer &timer,
                        ACE_hrtime_t empty_iteration_time)
  : elapsed_time_ (0),
    timer_ (timer),
    empty_iteration_time_ (empty_iteration_time)
{
}

Basic_Test::~Basic_Test (void)
{
}

double
Basic_Test::iteration_time (void)
{
  return per_iteration (elapsed_time_ > empty_iteration_time_  ?
                          elapsed_time_ - empty_iteration_time_  :
                          static_cast<ACE_hrtime_t> (0));
}

void
Basic_Test::print_iteration_time (const char *message)
{
  ACE_DEBUG ((LM_INFO, "  %-41s %8.3f\n",
              message,
              this->iteration_time ()));
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Empty_Iteration_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Empty_Iteration_Test : public Basic_Test
{
public:
  Empty_Iteration_Test (ACE_High_Res_Timer &timer) : Basic_Test (timer, 0) {}
  virtual ~Empty_Iteration_Test (void) {};

  virtual void run (void);

  ACE_hrtime_t empty_iteration_time (void) const
    {
      return elapsed_time_;
    }

private:
  // Require the timer reference.
  Empty_Iteration_Test (void);

  // Force construction of independent instances by prohibiting copying.
  Empty_Iteration_Test (const Empty_Iteration_Test &);
  Empty_Iteration_Test &operator= (const Empty_Iteration_Test &);
};

void
Empty_Iteration_Test::run (void)
{
  this->start_timing ();

  for (u_int i = 0; i < iterations; ++i)
    {
      DO_SOMETHING
    }

  this->stop_timing ();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Inline_Call_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Inline_Call_Test : public Basic_Test
{
public:
  Inline_Call_Test (ACE_High_Res_Timer &timer,
                    ACE_hrtime_t empty_iteration_time)
    : Basic_Test (timer, empty_iteration_time) {}
  virtual ~Inline_Call_Test (void) {};

  virtual void run (void);

private:
  // Require the timer reference.
  Inline_Call_Test (void);

  // Force construction of independent instances by prohibiting copying.
  Inline_Call_Test (const Inline_Call_Test &);
  Inline_Call_Test &operator= (const Inline_Call_Test &);
};

void
Inline_Call_Test::run (void)
{
  this->start_timing ();

  for (u_int i = 0; i < iterations; ++i)
    inline_func ();

  this->stop_timing ();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Noninline_Call_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Noninline_Call_Test : public Basic_Test
{
public:
  Noninline_Call_Test (ACE_High_Res_Timer &timer,
                       ACE_hrtime_t empty_iteration_time)
    : Basic_Test (timer, empty_iteration_time) {}
  virtual ~Noninline_Call_Test (void) {};

  virtual void run (void);

private:
  // Require the timer reference.
  Noninline_Call_Test (void);

  // Force construction of independent instances by prohibiting copying.
  Noninline_Call_Test (const Noninline_Call_Test &);
  Noninline_Call_Test &operator= (const Noninline_Call_Test &);
};

void
Noninline_Call_Test::run (void)
{
  this->start_timing ();

  for (u_int i = 0; i < iterations; ++i)
    func ();

  this->stop_timing ();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Inline_Member_Call_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Inline_Member_Call_Test : public Basic_Test
{
public:
  Inline_Member_Call_Test (ACE_High_Res_Timer &timer,
                           ACE_hrtime_t empty_iteration_time)
    : Basic_Test (timer, empty_iteration_time) {}
  virtual ~Inline_Member_Call_Test (void) {};

  virtual void run (void);

private:
  // Require the timer reference.
  Inline_Member_Call_Test (void);

  // Force construction of independent instances by prohibiting copying.
  Inline_Member_Call_Test (const Inline_Member_Call_Test &);
  Inline_Member_Call_Test &operator= (const Inline_Member_Call_Test &);
};

void
Inline_Member_Call_Test::run (void)
{
  this->start_timing ();

  for (u_int i = 0; i < iterations; ++i)
    foo.inline_func ();

  this->stop_timing ();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Noninline_Member_Call_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Noninline_Member_Call_Test : public Basic_Test
{
public:
  Noninline_Member_Call_Test (ACE_High_Res_Timer &timer,
                              ACE_hrtime_t empty_iteration_time)
    : Basic_Test (timer, empty_iteration_time) {}
  virtual ~Noninline_Member_Call_Test (void) {};

  virtual void run (void);

private:
  // Require the timer reference.
  Noninline_Member_Call_Test (void);

  // Force construction of independent instances by prohibiting copying.
  Noninline_Member_Call_Test (const Noninline_Member_Call_Test &);
  Noninline_Member_Call_Test &operator= (const Noninline_Member_Call_Test &);
};

void
Noninline_Member_Call_Test::run (void)
{
  this->start_timing ();

  for (u_int i = 0; i < iterations; ++i)
    foo.func ();

  this->stop_timing ();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Inline_Member_With_Virtual_Call_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Inline_Member_With_Virtual_Call_Test : public Basic_Test
{
public:
  Inline_Member_With_Virtual_Call_Test (ACE_High_Res_Timer &timer,
                                        ACE_hrtime_t empty_iteration_time)
    : Basic_Test (timer, empty_iteration_time) {}
  virtual ~Inline_Member_With_Virtual_Call_Test (void) {};

  virtual void run (void);

private:
  // Require the timer reference.
  Inline_Member_With_Virtual_Call_Test (void);

  // Force construction of independent instances by prohibiting copying.
  Inline_Member_With_Virtual_Call_Test (
    const Inline_Member_With_Virtual_Call_Test &);
  Inline_Member_With_Virtual_Call_Test &operator= (
    const Inline_Member_With_Virtual_Call_Test &);
};

void
Inline_Member_With_Virtual_Call_Test::run (void)
{
  this->start_timing ();

  for (u_int i = 0; i < iterations; ++i)
    foo_v.inline_func ();

  this->stop_timing ();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Noninline_Member_With_Virtual_Call_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Noninline_Member_With_Virtual_Call_Test : public Basic_Test
{
public:
  Noninline_Member_With_Virtual_Call_Test (ACE_High_Res_Timer &timer,
                                           ACE_hrtime_t empty_iteration_time)
    : Basic_Test (timer, empty_iteration_time) {}
  virtual ~Noninline_Member_With_Virtual_Call_Test (void) {};

  virtual void run (void);

private:
  // Require the timer reference.
  Noninline_Member_With_Virtual_Call_Test (void);

  // Force construction of independent instances by prohibiting copying.
  Noninline_Member_With_Virtual_Call_Test
    (const Noninline_Member_With_Virtual_Call_Test &);
  Noninline_Member_With_Virtual_Call_Test &operator=
    (const Noninline_Member_With_Virtual_Call_Test &);
};

void
Noninline_Member_With_Virtual_Call_Test::run (void)
{
  this->start_timing ();

  for (u_int i = 0; i < iterations; ++i)
    foo_v.func ();

  this->stop_timing ();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Virtual_Member_Optimizable_Call_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Virtual_Member_Optimizable_Call_Test : public Basic_Test
{
public:
  Virtual_Member_Optimizable_Call_Test (ACE_High_Res_Timer &timer,
                                        ACE_hrtime_t empty_iteration_time)
    : Basic_Test (timer, empty_iteration_time) {}
  virtual ~Virtual_Member_Optimizable_Call_Test (void) {};

  virtual void run (void);

private:
  // Require the timer reference.
  Virtual_Member_Optimizable_Call_Test (void);

  // Force construction of independent instances by prohibiting copying.
  Virtual_Member_Optimizable_Call_Test (
    const Virtual_Member_Optimizable_Call_Test &);
  Virtual_Member_Optimizable_Call_Test &operator= (
    const Virtual_Member_Optimizable_Call_Test &);
};

void
Virtual_Member_Optimizable_Call_Test::run (void)
{
  Foo_v &fv_o = foo_v;

  this->start_timing ();

  for (u_int i = 0; i < iterations; ++i)
    fv_o.v_func ();

  this->stop_timing ();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// class Virtual_Member_Call_Test
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Virtual_Member_Call_Test : public Basic_Test
{
public:
  Virtual_Member_Call_Test (ACE_High_Res_Timer &timer,
                            ACE_hrtime_t empty_iteration_time)
    : Basic_Test (timer, empty_iteration_time) {}
  virtual ~Virtual_Member_Call_Test (void) {};

  virtual void run (void);

private:
  // Require the timer reference.
  Virtual_Member_Call_Test (void);

  // Force construction of independent instances by prohibiting copying.
  Virtual_Member_Call_Test (const Virtual_Member_Call_Test &);
  Virtual_Member_Call_Test &operator= (const Virtual_Member_Call_Test &);
};

void
Virtual_Member_Call_Test::run (void)
{
  Foo_v *fv;

  if (iterations < 2)
    fv = new Foo_v;
  else
    fv = new Foo_d_v;

  this->start_timing ();

  for (u_int i = 0; i < iterations; ++i)
    fv->v_func ();

  this->stop_timing ();

  delete fv;
  fv = 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function get_options
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static
unsigned int
get_options (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("i:?"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'i':
          {
            int temp = ACE_OS::atoi (get_opt.opt_arg ());
            if (temp > 0)
              iterations = (u_int) temp;
            else
              {
                ACE_DEBUG ((LM_ERROR, "%s: number of iterations must be > 0\n",
                            argv [0]));
                return 1;
              }
            break;
          }
        case '?':
          ACE_DEBUG ((LM_INFO, "usage: %s %s\n", argv [0], usage));
          ACE_OS::exit (0);
          break;
        default:
          ACE_DEBUG ((LM_ERROR, "%s: unknown arg, %c\n", argv [0],
                                (char) opt));
          ACE_DEBUG ((LM_ERROR, "usage: %s %s\n", argv [0], usage));
          return 1;
        }
    }

  switch (argc - get_opt.opt_ind ())
    {
    case 0:
      // OK
      break;
    default:
      ACE_DEBUG ((LM_ERROR, "%s: too many arguments\n", argv [0]));
      ACE_DEBUG ((LM_ERROR, "usage: %s %s\n", argv [0], usage));
      return 1;
    }

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function main
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (get_options (argc, argv))
    ACE_OS::exit (-1);

  ACE_utsname name;

  if (ACE_OS::uname (&name) != -1)
    ACE_DEBUG ((LM_INFO, "%s (%s), %s %s at %T\n",
                name.nodename, name.machine, name.sysname, name.release));

  ACE_DEBUG ((LM_INFO, "%u iterations\n", iterations));


  // Use one timer for all the tests.  No particular reason why.
  ACE_High_Res_Timer timer;

  // Calculate the time for an "empty iteration", and subtract it
  // from each test time.
  ACE_hrtime_t iteration_time;

  Empty_Iteration_Test empty_iteration_test (timer);
  empty_iteration_test.run ();
  ACE_DEBUG ((LM_INFO, "An empty iteration costs %8.3f microseconds.\n\n",
              empty_iteration_test.iteration_time ()));
  iteration_time = empty_iteration_test.empty_iteration_time ();


  // Run the real tests . . .
  Inline_Call_Test inline_call_test (timer, iteration_time);
  inline_call_test.run ();

  Noninline_Call_Test noninline_call_test (timer, iteration_time);
  noninline_call_test.run ();

  Inline_Member_Call_Test inline_member_call_test (timer, iteration_time);
  inline_member_call_test.run ();

  Noninline_Member_Call_Test noninline_member_call_test (
      timer, iteration_time);
  noninline_member_call_test.run ();

  Inline_Member_With_Virtual_Call_Test
    inline_member_with_virtual_call_test (timer, iteration_time);
  inline_member_with_virtual_call_test.run ();

  Noninline_Member_With_Virtual_Call_Test
    noninline_member_with_virtual_call_test (timer, iteration_time);
  noninline_member_with_virtual_call_test.run ();

  Virtual_Member_Optimizable_Call_Test
    virtual_member_optimizable_call_test (timer, iteration_time);
  virtual_member_optimizable_call_test.run ();

  Virtual_Member_Call_Test virtual_member_call_test (timer, iteration_time);
  virtual_member_call_test.run ();


  // Print results . . .
  ACE_DEBUG ((LM_INFO, "operation                              "
                  "time, microseconds\n"));
  ACE_DEBUG ((LM_INFO, "=========                              "
                  "=================="));

  ACE_DEBUG ((LM_INFO, "\nglobal function calls:\n"));
  inline_call_test.print_iteration_time ("inline function call");
  noninline_call_test.print_iteration_time ("non-inline function call");

  ACE_DEBUG ((LM_INFO, "\nmember function calls:\n"));
  inline_member_call_test.print_iteration_time (
    "inline member function call");
  noninline_member_call_test.print_iteration_time (
    "non-inline member function call");

  ACE_DEBUG ((LM_INFO, "\nmember function calls, class has a virtual "
                       "function:\n"));
  inline_member_with_virtual_call_test.print_iteration_time (
    "inline member function with virtual call");
  noninline_member_with_virtual_call_test.print_iteration_time (
    "non-inline member function w/virtual call");

  ACE_DEBUG ((LM_INFO, "\nvirtual member function calls:\n"));
  virtual_member_optimizable_call_test.print_iteration_time (
    "virtual member function call, optimizable");
  virtual_member_call_test.print_iteration_time (
    "virtual member function call");

  return 0;
}


// EOF
