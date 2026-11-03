
//=============================================================================
/**
 *  @file    deadlock_detection_test.cpp
 *
 *  @author Tim Harrison
 */
//=============================================================================

#include "ace/Token_Manager.h"
#include "ace/Remote_Tokens.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Token_Invariants.h"
#include <memory>

#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREADS_LIBRARY)

using ACE_TOKEN_INVARIANTS = ACE_Token_Invariant_Manager;

struct Two_Tokens
{
  Two_Tokens () = default;
  Two_Tokens (Two_Tokens const &) = delete;
  Two_Tokens (Two_Tokens &&) = delete;
  Two_Tokens &operator= (Two_Tokens const &) = delete;
  Two_Tokens &operator= (Two_Tokens &&) = delete;

  std::unique_ptr<ACE_Token_Proxy> first_owner_;
  std::unique_ptr<ACE_Token_Proxy> second_owner_;
  ACE_Token_Proxy *first_ {};  // Borrowed from the corresponding owner or the shared global token.
  ACE_Token_Proxy *second_ {}; // Borrowed from the corresponding owner or the shared global token.
};

struct Four_Tokens
{
  Four_Tokens () = default;
  Four_Tokens (Four_Tokens const &) = delete;
  Four_Tokens (Four_Tokens &&) = delete;
  Four_Tokens &operator= (Four_Tokens const &) = delete;
  Four_Tokens &operator= (Four_Tokens &&) = delete;

  std::unique_ptr<ACE_Token_Proxy> first1_owner_;
  std::unique_ptr<ACE_Token_Proxy> first2_owner_;
  std::unique_ptr<ACE_Token_Proxy> first3_owner_;
  std::unique_ptr<ACE_Token_Proxy> second_owner_;
  ACE_Token_Proxy *first1_ {}; // Borrowed from first1_owner_.
  ACE_Token_Proxy *first2_ {}; // Borrowed from first2_owner_.
  ACE_Token_Proxy *first3_ {}; // Borrowed from first3_owner_.
  ACE_Token_Proxy *second_ {}; // Borrowed from second_owner_.
};

enum class Token_Kind
{
  MUTEX,
  READER,
  WRITER
};

static bool ignore_deadlock = false;
static bool remote_mutexes = false;
static ACE_TCHAR const *server_host = ACE_DEFAULT_SERVER_HOST;
static int server_port = ACE_DEFAULT_SERVER_PORT;
static int iterations = 100;
static bool rwlocks = false;

static std::unique_ptr<ACE_Token_Proxy>
create_token (ACE_TCHAR const *name, Token_Kind kind)
{
  if (remote_mutexes)
    {
      switch (kind)
        {
        case Token_Kind::MUTEX:
          return std::make_unique<ACE_Remote_Mutex> (name, ignore_deadlock, 1);
        case Token_Kind::READER:
          return std::make_unique<ACE_Remote_RLock> (name, ignore_deadlock, 1);
        case Token_Kind::WRITER:
          return std::make_unique<ACE_Remote_WLock> (name, ignore_deadlock, 1);
        }
    }
  else
    {
      switch (kind)
        {
        case Token_Kind::MUTEX:
          return std::make_unique<ACE_Local_Mutex> (name, ignore_deadlock, 1);
        case Token_Kind::READER:
          return std::make_unique<ACE_Local_RLock> (name, ignore_deadlock, 1);
        case Token_Kind::WRITER:
          return std::make_unique<ACE_Local_WLock> (name, ignore_deadlock, 1);
        }
    }

  return nullptr;
}

static ACE_THR_FUNC_RETURN
two_token_thread (void *vp)
{
  Two_Tokens * const tokens = static_cast<Two_Tokens *> (vp);

  for (int x = 0; x < iterations; x++)
    {
      if (tokens->first_->acquire () == -1)
        {
          ACE_DEBUG ((LM_DEBUG, "Deadlock detected\n"));
          continue;
        }

      if (ACE_TOKEN_INVARIANTS::instance ()->acquired (tokens->first_) == 0)
        {
          tokens->first_->dump ();
          ACE_ERROR_RETURN ((LM_ERROR, "violated invariant.\n"), 0);
        }

      if (tokens->second_->acquire () == -1)
        {
          ACE_DEBUG ((LM_DEBUG, "Deadlock Detected\n"));
          goto G1;
        }

      if (ACE_TOKEN_INVARIANTS::instance ()->acquired (tokens->second_) == 0)
        {
          tokens->second_->dump ();
          ACE_ERROR_RETURN ((LM_ERROR, "violated invariant.\n"), 0);
        }

      ACE_TOKEN_INVARIANTS::instance ()->releasing (tokens->second_);

      tokens->second_->release ();
    G1:
      ACE_TOKEN_INVARIANTS::instance ()->releasing (tokens->first_);

      tokens->first_->release ();
    }

  ACE_DEBUG ((LM_DEBUG, "thread %t exiting\n"));
  return ACE_THR_FUNC_RETURN_NULL;
}

static ACE_THR_FUNC_RETURN
run_writer (void *vp)
{
  Four_Tokens * const ft = static_cast<Four_Tokens *> (vp);
  int acquire_number = 0;

  for (int x = 0; x < iterations; x++)
    {
      // Cycle through each of the first three tokens.
      ACE_Token_Proxy *t = nullptr;
      switch (acquire_number)
        {
        case 0:
          t = ft->first1_;
          break;
        case 1:
          t = ft->first2_;
          break;
        case 2:
          t = ft->first3_;
          break;
        }

      acquire_number = (acquire_number + 1) % 3;

      if (t->acquire () == -1)
        {
          ACE_ASSERT (errno == EDEADLK);
          ACE_DEBUG ((LM_DEBUG, "Deadlock detected.\n"));
          continue;
        }

      if (ACE_TOKEN_INVARIANTS::instance ()->acquired (t) == 0)
        {
          t->dump ();
          ACE_ERROR_RETURN ((LM_ERROR, "violated invariant.\n"), 0);
        }

      if (ft->second_->acquire () == -1)
        {
          ACE_ASSERT (errno == EDEADLK);
          ACE_DEBUG ((LM_DEBUG, "Deadlock Detected..\n"));
          goto G1;
        }

      if (ACE_TOKEN_INVARIANTS::instance ()->acquired (ft->second_) == 0)
        {
          ft->second_->dump ();
          ACE_ERROR_RETURN ((LM_ERROR, "violated invariant.\n"), 0);
        }

      ACE_TOKEN_INVARIANTS::instance ()->releasing (ft->second_);

      ft->second_->release ();

      G1:
        ACE_TOKEN_INVARIANTS::instance ()->releasing (t);

        t->release ();
    }

  ACE_DEBUG ((LM_DEBUG, "thread %t exiting\n"));
  return ACE_THR_FUNC_RETURN_NULL;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("iuh:rp:n:"), 1);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
        {
        case 'r':
          rwlocks = true;
          break;
        case 'i':
          ignore_deadlock = true;
          break;
        case 'h':
          server_host = get_opt.opt_arg ();
          remote_mutexes = true;
          break;
        case 'p':
          server_port = ACE_OS::atoi (get_opt.opt_arg ());
          remote_mutexes = true;
          break;
        case 'n':
          iterations = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'u':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%n:\n"
                             "[-r test readers/writer locks]\n"
                             "[-n <iterations>]\n"
                             "[-h <remote host>]\n"
                             "[-p <remote port>]\n"
                             "[-i ignore deadlock]\n%a", 1), -1);
        }
    }

  if (remote_mutexes)
    ACE_Remote_Mutex::set_server_address (ACE_INET_Addr (server_port, server_host));

  return 0;
}

int
mutex_test ()
{
  ACE_Thread_Manager thr_mgr;

  Two_Tokens one;
  Two_Tokens two;
  std::unique_ptr<ACE_Token_Proxy> global_mutex = create_token (ACE_TEXT ("global proxy"),
                                                               Token_Kind::MUTEX);
  one.first_owner_ = create_token (ACE_TEXT ("local proxy"), Token_Kind::MUTEX);
  two.second_owner_ = create_token (ACE_TEXT ("local proxy"), Token_Kind::MUTEX);
  one.first_ = one.first_owner_.get ();
  one.second_ = global_mutex.get ();
  two.first_ = global_mutex.get ();
  two.second_ = two.second_owner_.get ();

  // Tell the token manager to be verbose when reporting deadlock.
  ACE_Token_Manager::instance ()->debug (1);

  if (thr_mgr.spawn (two_token_thread, &one, THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "first spawn"), -1);

  if (thr_mgr.spawn (two_token_thread, &two, THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "second spawn"), -1);

  // Wait for all threads to exit.
  thr_mgr.wait ();

  return 0;
}

static int
rwlock_test ()
{
  ACE_Thread_Manager thr_mgr;

  Two_Tokens reader1;
  Two_Tokens reader2;
  Two_Tokens reader3;
  Four_Tokens writer;

  reader1.first_owner_ = create_token (ACE_TEXT ("reader first"), Token_Kind::READER);
  ACE_TCHAR const * const reader1_second_name = remote_mutexes
    ? ACE_TEXT ("reader first") : ACE_TEXT ("writer first 1");
  reader1.second_owner_ = create_token (reader1_second_name, Token_Kind::READER);
  reader2.first_owner_ = create_token (ACE_TEXT ("reader first"), Token_Kind::READER);
  ACE_TCHAR const * const reader2_second_name = remote_mutexes
    ? ACE_TEXT ("reader first") : ACE_TEXT ("writer first 2");
  reader2.second_owner_ = create_token (reader2_second_name, Token_Kind::READER);
  reader3.first_owner_ = create_token (ACE_TEXT ("reader first"), Token_Kind::READER);
  ACE_TCHAR const * const reader3_second_name = remote_mutexes
    ? ACE_TEXT ("reader first") : ACE_TEXT ("writer first 3");
  reader3.second_owner_ = create_token (reader3_second_name, Token_Kind::READER);

  writer.first1_owner_ = create_token (ACE_TEXT ("writer first 1"), Token_Kind::WRITER);
  writer.first2_owner_ = create_token (ACE_TEXT ("writer first 2"), Token_Kind::WRITER);
  writer.first3_owner_ = create_token (ACE_TEXT ("writer first 3"), Token_Kind::WRITER);
  writer.second_owner_ = create_token (ACE_TEXT ("reader first"), Token_Kind::WRITER);

  reader1.first_ = reader1.first_owner_.get ();
  reader1.second_ = reader1.second_owner_.get ();
  reader2.first_ = reader2.first_owner_.get ();
  reader2.second_ = reader2.second_owner_.get ();
  reader3.first_ = reader3.first_owner_.get ();
  reader3.second_ = reader3.second_owner_.get ();
  writer.first1_ = writer.first1_owner_.get ();
  writer.first2_ = writer.first2_owner_.get ();
  writer.first3_ = writer.first3_owner_.get ();
  writer.second_ = writer.second_owner_.get ();

  // Tell the token manager to be verbose when reporting deadlock.
  ACE_Token_Manager::instance ()->debug (1);

  if (thr_mgr.spawn (two_token_thread, &reader1, THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "first spawn"), -1);

  if (thr_mgr.spawn (two_token_thread, &reader2, THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "first spawn"), -1);

  if (thr_mgr.spawn (two_token_thread, &reader3, THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "first spawn"), -1);

  if (thr_mgr.spawn (run_writer, &writer, THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "second spawn"), -1);

  // Wait for all threads to exit.
  thr_mgr.wait ();

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) == -1)
    return -1;

  if (rwlocks)
    rwlock_test ();
  else
    mutex_test ();

  ACE_DEBUG ((LM_DEBUG, "test exiting.\n"));
  return 0;
}
#else
int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "threads not supported on this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS */
