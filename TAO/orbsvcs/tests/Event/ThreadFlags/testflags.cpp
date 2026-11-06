/*
 * Test TAO_EC_Thread_Flags encapsulation.
 */

#include "orbsvcs/Event/EC_Thread_Flags.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_Thread.h"
#include "ace/OS_NS_strings.h"

#include <array>

/*
  Here are the scenarios we need to test:

  1. Initialize/assign empty instance and validate defaults, priority, etc.
  2. Initialize/assign with each flag and validate value of flags.
  3. Initialize/assign with each scope and validate scope and flags.
  4. Initialize/assign with each sched and validate sched and flags.
  5. Initialize/assign with single numeric value and validate flags, sched, and scope values.
  6. Initialize/assign with bogus symbols and validate scope/flags + error.
  7. Initialize/assign with multiple flags and validate flags, scope, sched.

 */
struct ExpectedValues
{
  long flags {};
  long scope {};
  long sched {};

  explicit constexpr ExpectedValues (long f = 0, long sco = 0, long sch = 0)
    : flags (f),
      scope (sco),
      sched (sch)
  {
  }
};

struct ThreadFlagsTest
{
  enum How {
    INIT = 0x1, // use constructor initialization
    ASSIGN = 0x2, // use assignment operator
    BOTH = 0x3
  };

  ThreadFlagsTest (char const* sym, How assign, ExpectedValues ev);

  // Execute the test and pass back an ExpectedValues struct where the
  // values are zero if the values were not as expected or non-zero if
  // they were as expected.  Return value is zero for success, or
  // the bitmask of the "How" version of the test that failed.
  int runtest (ExpectedValues& results) const;

  char const* sym_;
  How const assign_;
  ExpectedValues const expected_values_;
};

ThreadFlagsTest::ThreadFlagsTest (char const* sym,
                                  How assign,
                                  ExpectedValues ev)
  : sym_ (sym),
    assign_ (assign),
    expected_values_ (ev)
{
}

int
ThreadFlagsTest::runtest (ExpectedValues& results) const
{
  int r {};
  if (assign_ & INIT)
    {
      TAO_EC_Thread_Flags f(sym_);
      if ( ((results.flags = f.flags()) == expected_values_.flags) &&
           ((results.scope = f.scope()) == expected_values_.scope) &&
           ((results.sched = f.sched()) == expected_values_.sched) )
        ; // we succeeded, do nothing
      else
        r |= INIT;
    }
  if (assign_ & ASSIGN)
    {
      TAO_EC_Thread_Flags f;
      f = sym_;
      if ( ((results.flags = f.flags()) == expected_values_.flags) &&
           ((results.scope = f.scope()) == expected_values_.scope) &&
           ((results.sched = f.sched()) == expected_values_.sched) )
        ; // we succeeded, do nothing
      else
        r |= ASSIGN;
    }

  return r;
}

// ExpectedValues set indicating the default values
static constexpr ExpectedValues DEFAULTS {};

static void
print_results (int const how, ExpectedValues const& expected, ExpectedValues const& actuals)
{
  char const* const op =
    (how == ThreadFlagsTest::INIT) ? "INIT" :
    (how == ThreadFlagsTest::ASSIGN) ? "ASSIGN" :
    (how == ThreadFlagsTest::BOTH) ? "BOTH" :
    "unk?";

  ACE_DEBUG ((LM_DEBUG, "  %s: expected {%x,%x,%x} got {%x,%x,%x}",
              op,
              expected.flags, expected.scope, expected.sched,
              actuals.flags, actuals.scope, actuals.sched));
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  std::array<ThreadFlagsTest, 22> const static_tests {{
    // Test 1
    ThreadFlagsTest (nullptr, ThreadFlagsTest::BOTH, DEFAULTS),
    // Test 2
    ThreadFlagsTest ("THR_CANCEL_DISABLE", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_CANCEL_DISABLE)),
    ThreadFlagsTest ("THR_CANCEL_ENABLE", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_CANCEL_ENABLE)),
    ThreadFlagsTest ("THR_CANCEL_DEFERRED", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_CANCEL_DEFERRED)),
    ThreadFlagsTest ("THR_CANCEL_ASYNCHRONOUS", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_CANCEL_ASYNCHRONOUS)),
    ThreadFlagsTest ("THR_BOUND", ThreadFlagsTest::BOTH, ExpectedValues (THR_BOUND)),
    ThreadFlagsTest ("THR_NEW_LWP", ThreadFlagsTest::BOTH, ExpectedValues (THR_NEW_LWP)),
    ThreadFlagsTest ("THR_DETACHED", ThreadFlagsTest::BOTH, ExpectedValues (THR_DETACHED)),
    ThreadFlagsTest ("THR_SUSPENDED", ThreadFlagsTest::BOTH, ExpectedValues (THR_SUSPENDED)),
    ThreadFlagsTest ("THR_DAEMON", ThreadFlagsTest::BOTH, ExpectedValues (THR_DAEMON)),
    ThreadFlagsTest ("THR_JOINABLE", ThreadFlagsTest::BOTH, ExpectedValues (THR_JOINABLE)),
    // Test 3
    ThreadFlagsTest ("THR_SCHED_FIFO", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_SCHED_FIFO, 0, THR_SCHED_FIFO)),
    ThreadFlagsTest ("THR_SCHED_RR", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_SCHED_RR, 0, THR_SCHED_RR)),
    ThreadFlagsTest ("THR_SCHED_DEFAULT", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_SCHED_DEFAULT, 0, THR_SCHED_DEFAULT)),
    // Test 4
    ThreadFlagsTest ("THR_SCOPE_SYSTEM", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_SCOPE_SYSTEM, THR_SCOPE_SYSTEM, 0)),
    ThreadFlagsTest ("THR_SCOPE_PROCESS", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_SCOPE_PROCESS, THR_SCOPE_PROCESS, 0)),
    // Test 5
    ThreadFlagsTest ("0x0001", ThreadFlagsTest::BOTH, ExpectedValues (1, 0, 0)),
    // Test 6
    ThreadFlagsTest ("Zaphod|Beeblebrox|THR_BOGUS", ThreadFlagsTest::BOTH, DEFAULTS),
    // Test 7
    ThreadFlagsTest ("THR_BOUND|THR_JOINABLE", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_BOUND|THR_JOINABLE)),
    ThreadFlagsTest ("THR_SCHED_FIFO|THR_DAEMON|THR_SCOPE_PROCESS", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_SCHED_FIFO|THR_DAEMON|THR_SCOPE_PROCESS,
                                     THR_SCOPE_PROCESS, THR_SCHED_FIFO)),
    ThreadFlagsTest ("THR_SCHED_DEFAULT|THR_SCOPE_SYSTEM|THR_NEW_LWP", ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_SCHED_DEFAULT|THR_SCOPE_SYSTEM|THR_NEW_LWP,
                                     THR_SCOPE_SYSTEM, THR_SCHED_DEFAULT)),
    ThreadFlagsTest ("THR_NEW_LWP|THR_SCOPE_SYSTEM|THR_JOINABLE|THR_SCHED_FIFO",
                     ThreadFlagsTest::BOTH,
                     ExpectedValues (THR_NEW_LWP|THR_SCHED_FIFO|THR_SCOPE_SYSTEM|THR_JOINABLE,
                                     THR_SCOPE_SYSTEM, THR_SCHED_FIFO))
  }};

  int returnval {};
  size_t const numtests = static_tests.size ();
  if (argc > 1)
    {
      ACE_TCHAR* which_test {};
      while (nullptr != (which_test = *++argv))
        {
          size_t const testnum = ACE_OS::strtol (which_test, nullptr, 0);
          if (testnum < numtests)
            {
              ExpectedValues results;
              ThreadFlagsTest const& t = static_tests[testnum];
              ACE_DEBUG ((LM_DEBUG, "Test #%d for %s...", testnum, t.sym_));
              int const r = t.runtest (results);
              if (r != 0)
                {
                  returnval = 1;
                  ACE_DEBUG ((LM_DEBUG, "FAILED -- details: "));
                  if (r & ThreadFlagsTest::INIT)
                    print_results (ThreadFlagsTest::INIT, t.expected_values_, results);

                  if (r & ThreadFlagsTest::ASSIGN)
                    print_results (ThreadFlagsTest::ASSIGN, t.expected_values_, results);

                  ACE_DEBUG ((LM_DEBUG, "\n"));
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG, "ok.\n"));
                }
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%d is not a valid test number\n", testnum));
            }
        }
    }
  else
    {
      for (size_t i {}; i < numtests; ++i)
        {
          ExpectedValues results;
          ThreadFlagsTest const& t = static_tests[i];
          ACE_DEBUG ((LM_DEBUG, "Test #%d for %s...", i, t.sym_));
          int const r = t.runtest (results);
          if (r != 0)
            {
              returnval = 1;
              ACE_DEBUG ((LM_DEBUG, "FAILED -- details: "));
              if (r & ThreadFlagsTest::INIT)
                print_results (ThreadFlagsTest::INIT, t.expected_values_, results);

              if (r & ThreadFlagsTest::ASSIGN)
                print_results (ThreadFlagsTest::ASSIGN, t.expected_values_, results);

              ACE_DEBUG ((LM_DEBUG, "\n"));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, "ok.\n"));
            }
        }

    }

  return returnval;
}
