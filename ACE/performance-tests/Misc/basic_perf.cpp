
//=============================================================================
/**
 *  @file    basic_perf.cpp
 *
 *  Times various simple operations.
 *
 *  With Sun C++, use -O2: make CFLAGS="-mt -O2" BIN=basic_perf
 *  -fast seems to produce slower times.
 *
 *  @author David Levine
 */
//=============================================================================


#include "basic_func.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_main.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_sys_utsname.h"

#include <memory>

static constexpr char usage [] = "[-? |\n"
                             "            [-i <iterations> [1000000]]";

// These are global.  They appear to bust the CPU cache, on an Ultra 2
// w/Sun C++ 4.2.
static u_int iterations {1000000};
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
  double const us_per_iteration = static_cast<double> (elapsed) / 1000.0 /
                                  static_cast<double> (iterations);

  // Don't print out "-0.000" or "-0.001" . . .
  return -0.002 < us_per_iteration  &&  us_per_iteration < 0.0
           ?  0.0
           :  us_per_iteration;
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
  virtual ~Basic_Test () = default;

  virtual void run () = 0;

  double iteration_time ();

  void print_iteration_time (const char *message);

protected:
  ACE_hrtime_t elapsed_time_ {};

  void start_timing ()
  {
    timer_.reset ();
    timer_.start ();
  }

  void stop_timing ()
  {
    timer_.stop ();
    timer_.elapsed_time (elapsed_time_);
  }

private:
  ACE_High_Res_Timer &timer_;
  ACE_hrtime_t const empty_iteration_time_;

  Basic_Test () = delete;
  Basic_Test (Basic_Test const &) = delete;
  Basic_Test &operator= (Basic_Test const &) = delete;
  Basic_Test (Basic_Test &&) = delete;
  Basic_Test &operator= (Basic_Test &&) = delete;
};

Basic_Test::Basic_Test (ACE_High_Res_Timer &timer,
                        ACE_hrtime_t empty_iteration_time)
  : timer_ (timer),
    empty_iteration_time_ (empty_iteration_time)
{
}

double
Basic_Test::iteration_time ()
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
  explicit Empty_Iteration_Test (ACE_High_Res_Timer &timer)
    : Basic_Test (timer, ACE_hrtime_t {})
  {
  }
  ~Empty_Iteration_Test () override = default;

  void run () override;

  ACE_hrtime_t empty_iteration_time () const
    {
      return elapsed_time_;
    }

private:
  Empty_Iteration_Test () = delete;
  Empty_Iteration_Test (Empty_Iteration_Test const &) = delete;
  Empty_Iteration_Test &operator= (Empty_Iteration_Test const &) = delete;
  Empty_Iteration_Test (Empty_Iteration_Test &&) = delete;
  Empty_Iteration_Test &operator= (Empty_Iteration_Test &&) = delete;
};

void
Empty_Iteration_Test::run ()
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
  ~Inline_Call_Test () override = default;

  void run () override;

private:
  Inline_Call_Test () = delete;
  Inline_Call_Test (Inline_Call_Test const &) = delete;
  Inline_Call_Test &operator= (Inline_Call_Test const &) = delete;
  Inline_Call_Test (Inline_Call_Test &&) = delete;
  Inline_Call_Test &operator= (Inline_Call_Test &&) = delete;
};

void
Inline_Call_Test::run ()
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
  ~Noninline_Call_Test () override = default;

  void run () override;

private:
  Noninline_Call_Test () = delete;
  Noninline_Call_Test (Noninline_Call_Test const &) = delete;
  Noninline_Call_Test &operator= (Noninline_Call_Test const &) = delete;
  Noninline_Call_Test (Noninline_Call_Test &&) = delete;
  Noninline_Call_Test &operator= (Noninline_Call_Test &&) = delete;
};

void
Noninline_Call_Test::run ()
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
  ~Inline_Member_Call_Test () override = default;

  void run () override;

private:
  Inline_Member_Call_Test () = delete;
  Inline_Member_Call_Test (Inline_Member_Call_Test const &) = delete;
  Inline_Member_Call_Test &operator= (Inline_Member_Call_Test const &) = delete;
  Inline_Member_Call_Test (Inline_Member_Call_Test &&) = delete;
  Inline_Member_Call_Test &operator= (Inline_Member_Call_Test &&) = delete;
};

void
Inline_Member_Call_Test::run ()
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
  ~Noninline_Member_Call_Test () override = default;

  void run () override;

private:
  Noninline_Member_Call_Test () = delete;
  Noninline_Member_Call_Test (Noninline_Member_Call_Test const &) = delete;
  Noninline_Member_Call_Test &operator= (Noninline_Member_Call_Test const &) = delete;
  Noninline_Member_Call_Test (Noninline_Member_Call_Test &&) = delete;
  Noninline_Member_Call_Test &operator= (Noninline_Member_Call_Test &&) = delete;
};

void
Noninline_Member_Call_Test::run ()
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
  ~Inline_Member_With_Virtual_Call_Test () override = default;

  void run () override;

private:
  Inline_Member_With_Virtual_Call_Test () = delete;
  Inline_Member_With_Virtual_Call_Test (
    Inline_Member_With_Virtual_Call_Test const &) = delete;
  Inline_Member_With_Virtual_Call_Test &operator= (
    Inline_Member_With_Virtual_Call_Test const &) = delete;
  Inline_Member_With_Virtual_Call_Test (
    Inline_Member_With_Virtual_Call_Test &&) = delete;
  Inline_Member_With_Virtual_Call_Test &operator= (
    Inline_Member_With_Virtual_Call_Test &&) = delete;
};

void
Inline_Member_With_Virtual_Call_Test::run ()
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
  ~Noninline_Member_With_Virtual_Call_Test () override = default;

  void run () override;

private:
  Noninline_Member_With_Virtual_Call_Test () = delete;
  Noninline_Member_With_Virtual_Call_Test (
    Noninline_Member_With_Virtual_Call_Test const &) = delete;
  Noninline_Member_With_Virtual_Call_Test &operator= (
    Noninline_Member_With_Virtual_Call_Test const &) = delete;
  Noninline_Member_With_Virtual_Call_Test (
    Noninline_Member_With_Virtual_Call_Test &&) = delete;
  Noninline_Member_With_Virtual_Call_Test &operator= (
    Noninline_Member_With_Virtual_Call_Test &&) = delete;
};

void
Noninline_Member_With_Virtual_Call_Test::run ()
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
  ~Virtual_Member_Optimizable_Call_Test () override = default;

  void run () override;

private:
  Virtual_Member_Optimizable_Call_Test () = delete;
  Virtual_Member_Optimizable_Call_Test (
    Virtual_Member_Optimizable_Call_Test const &) = delete;
  Virtual_Member_Optimizable_Call_Test &operator= (
    Virtual_Member_Optimizable_Call_Test const &) = delete;
  Virtual_Member_Optimizable_Call_Test (
    Virtual_Member_Optimizable_Call_Test &&) = delete;
  Virtual_Member_Optimizable_Call_Test &operator= (
    Virtual_Member_Optimizable_Call_Test &&) = delete;
};

void
Virtual_Member_Optimizable_Call_Test::run ()
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
  ~Virtual_Member_Call_Test () override = default;

  void run () override;

private:
  Virtual_Member_Call_Test () = delete;
  Virtual_Member_Call_Test (Virtual_Member_Call_Test const &) = delete;
  Virtual_Member_Call_Test &operator= (Virtual_Member_Call_Test const &) = delete;
  Virtual_Member_Call_Test (Virtual_Member_Call_Test &&) = delete;
  Virtual_Member_Call_Test &operator= (Virtual_Member_Call_Test &&) = delete;
};

void
Virtual_Member_Call_Test::run ()
{
  std::unique_ptr<Foo_v> fv;

  if (iterations < 2)
    fv = std::make_unique<Foo_v> ();
  else
    fv = std::make_unique<Foo_d_v> ();

  this->start_timing ();

  for (u_int i = 0; i < iterations; ++i)
    fv->v_func ();

  this->stop_timing ();

}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function get_options
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static
int
get_options (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("i:?"));
  int opt {};

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'i':
          {
            int const temp = ACE_OS::atoi (get_opt.opt_arg ());
            if (temp > 0)
              iterations = static_cast<u_int> (temp);
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
          return 2;
        default:
          ACE_DEBUG ((LM_ERROR, "%s: unknown arg, %c\n", argv [0],
                                static_cast<char> (opt)));
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
  int const option_status = get_options (argc, argv);
  if (option_status != 0)
    return option_status == 2 ? 0 : 1;

  ACE_utsname name {};

  if (ACE_OS::uname (std::addressof (name)) != -1)
    ACE_DEBUG ((LM_INFO, "%s (%s), %s %s at %T\n",
                name.nodename, name.machine, name.sysname, name.release));

  ACE_DEBUG ((LM_INFO, "%u iterations\n", iterations));


  // Use one timer for all the tests.  No particular reason why.
  ACE_High_Res_Timer timer;

  // Calculate the time for an "empty iteration", and subtract it
  // from each test time.
  ACE_hrtime_t iteration_time {};

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
