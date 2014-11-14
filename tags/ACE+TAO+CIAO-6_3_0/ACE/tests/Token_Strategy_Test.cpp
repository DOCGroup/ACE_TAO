// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program tests the behavior of ACE_Token under a variety of scenarios
//    in order verify whether or not tokens are returned, and threads run, in
//    a LIFO or FIFO manner.
//
// = AUTHOR
//    Don Hinton <dhinton@ieee.org>
//
// ============================================================================

#include "test_config.h"
#include "ace/Token.h"
#include "ace/Task.h"
#include "ace/Atomic_Op.h"
#include "ace/Auto_IncDec_T.h"
#include "ace/Vector_T.h"
#include "ace/Stats.h"
#include "ace/ACE.h"
#include "ace/Barrier.h"



#if defined (ACE_HAS_THREADS)

class Token_Strategy_Test : public ACE_Task<ACE_MT_SYNCH>
{
public:

  Token_Strategy_Test (ACE_Token::QUEUEING_STRATEGY strategy = ACE_Token::FIFO,
                       int threads = 5, int invocations = 10);
  ~Token_Strategy_Test (void);

  //FUZZ: disable check_for_lack_ACE_OS
  int open (void *a = 0);
  //FUZZ: enable check_for_lack_ACE_OS

  int svc (void);

private:
  // Number of threads for the test, must be 5 or more.
  int threads_;

  // Barrier used to try to synchronize the for loop in the svc() method.
  ACE_Barrier barrier_;

  // Token used to synchonize for loop.
  ACE_Token token_;

  // Token strategy to use, LIFO/FIFO.
  ACE_Token::QUEUEING_STRATEGY strategy_;

  // Number of loops.
  int invocations_;

  // Vector of token counts, one per thread.
  ACE_Vector<ACE_INT32> vec_token_count_;

  // This keeps a count of the number of threads who have the token--should always
  // be 0 or 1;
  ACE_Atomic_Op<ACE_Thread_Mutex, int> counter_;

  // Number of active threads in svc() method.
  ACE_Atomic_Op<ACE_Thread_Mutex, int> active_;

  // Errors count, set in svc() and returned from open().
  ACE_Atomic_Op<ACE_Thread_Mutex, int> errors_;

  ACE_UNIMPLEMENTED_FUNC (Token_Strategy_Test (const Token_Strategy_Test &))
  ACE_UNIMPLEMENTED_FUNC (Token_Strategy_Test &operator= (const Token_Strategy_Test &))
};


Token_Strategy_Test::Token_Strategy_Test (ACE_Token::QUEUEING_STRATEGY strategy, int threads, int invocations)
  : threads_ (threads < 5 ? 5 : threads), // need at least 5 threads to satisfy test conditions.
    barrier_ (threads_),
    strategy_ (strategy),
    invocations_ (invocations < 10 ? 10 : invocations), // insure we loop at least a few times.
    vec_token_count_ (threads_)
{
  this->counter_ = 0;
  this->active_ = 0;
  this->errors_ = 0;

  // Initialize the per thread counters used for generating stats.
  for (int i = 0; i < this->threads_; ++i)
    {
      const ACE_UINT32 sample = 0;
      this->vec_token_count_.push_back (sample);
    }

  this->token_.queueing_strategy (this->strategy_);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (tid = %t) Token_Test::Token_Test (\n")
              ACE_TEXT ("    token_type = %s\n")
              ACE_TEXT ("        thread = %d\n")
              ACE_TEXT ("   invocations = %d\n"),
              this->strategy_ == ACE_Token::FIFO ? ACE_TEXT ("FIFO") : ACE_TEXT ("LIFO"),
              this->threads_,
              this->invocations_));
}

Token_Strategy_Test::~Token_Strategy_Test (void)
{}

int
Token_Strategy_Test::open (void *)
{
  // spawn threads in ace task...
  // Make this Task into an Active Object.
  this->activate (THR_BOUND | THR_DETACHED, this->threads_);

  // Wait for all the threads to exit.
  this->thr_mgr ()->wait ();
  return this->errors_.value ();
}

int
Token_Strategy_Test::svc (void)
{
  int current = this->active_.value ();
  ACE_Auto_IncDec<ACE_Atomic_Op<ACE_Thread_Mutex, int> > active_counter (this->active_);
  this->barrier_.wait ();


  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (tid = %t) starting loop\n")));
  for (int i = 0; i < this->invocations_; i++)
    {
      ACE_GUARD_RETURN (ACE_Token, lock, this->token_, -1);
      this->vec_token_count_[current]++;
      ACE_Auto_IncDec<ACE_Atomic_Op<ACE_Thread_Mutex, int> > token_count_counter (this->counter_);

      // Turn this on to watch each thread grab the token.  LIFO has the interesting
      // behavior that two thread seem to take turns while all the other threads wait.
      if (0)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (tid = %t) token count = %d, ")
                              ACE_TEXT ("waiters = %d, loop: %d/%d\n"),
                  this->counter_.value (),
                  this->token_.waiters (), i + 1,
                  this->invocations_));

      // Yield, then simulate some work in order to give the other threads a chance to queue up.
      ACE_Thread::yield ();
      for (int k = 0; k != 100; ++k)
        {
          ACE::is_prime (k, 2, k/2);
        }

      // If we are the first thread to finish, compute the stats.
      if (i + 1 == this->invocations_)
        {
          if (this->active_ == this->threads_)
            {
              ACE_Stats stats;
              ACE_Stats_Value std_dev (2);
              ACE_Stats_Value mean (2);
              for (int i = 0; i < this->threads_; ++i)
                {
                  stats.sample (this->vec_token_count_[i]);
                }

              //stats.print_summary (2);
              stats.std_dev (std_dev);
              stats.mean (mean);
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT (" (tid = %t) mean = %d.%d, std_dev = %d.%d, max = %d, min = %d\n"),
                          mean.whole (), mean.fractional (), std_dev.whole (), std_dev.fractional (),
                          stats.max_value (), stats.min_value ()));

              // These are pretty simplistic tests, so let me know if you have a better idea.
              // The assumption is that the standard deviation will be small when using the
              // FIFO strategy since all threads will share the token more or less evenly.
              // In contrast, the LIFO strategy will allow the two threads to alternate, thus
              // several threads will have a low, or zero, token count and create a low mean and
              // high standard deviation.  If the the thread count is over say 4 or 5, the
              // standard deviation will actually excide the mean, hence the test.
              if (this->strategy_ == ACE_Token::LIFO &&
                  (mean.whole () > std_dev.whole () &&
                   mean.fractional () > std_dev.fractional ()))
                {
                  ACE_DEBUG ((LM_ERROR,
                              ACE_TEXT (" (tid = %t) LIFO: mean greater than std_dev.\n")));
                  this->errors_++;
                }
              if (this->strategy_ == ACE_Token::FIFO &&
                  (mean.whole () < std_dev.whole () &&
                   mean.fractional () < std_dev.fractional ()))
                {
                  ACE_DEBUG ((LM_ERROR,
                              ACE_TEXT (" (tid = %t) FIFO: mean less than std_dev.\n")));
                  this->errors_++;
                }
            }
        }
    }
  return 0;
}


int run_test (ACE_Token::QUEUEING_STRATEGY strategy, int threads = 5,
              int invocations = 10)
{
  Token_Strategy_Test test (strategy, threads, invocations);
  return test.open () == 0 ? 0 : 1;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Token_Strategy_Test"));
  int retval = 0;

  if (argc > 3)
    {
      // print usage
      retval = 1;
    }
  else
    {
      int threads = 5;
      int invocations = 100;

      if (argc > 1) threads = ACE_OS::atoi (argv[1]);
      if (argc > 2) invocations = ACE_OS::atoi (argv[2]);

      // New test using ACE_Token::queueing_strategy ()
      retval += run_test (ACE_Token::FIFO, threads, invocations);
      retval += run_test (ACE_Token::LIFO, threads, invocations);
    }

  ACE_END_TEST;
  return retval;
}

#else /* ACE_HAS_THREADS */
int
run_main (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Token_Strategy_Test: your platform doesn't support threads\n")), 1);
}
#endif /* ACE_HAS_THREADS */
