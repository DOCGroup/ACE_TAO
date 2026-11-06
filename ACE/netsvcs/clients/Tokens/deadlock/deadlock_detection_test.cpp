
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

#include <array>
#include <cstddef>
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
  int iterations {100};
};

struct Four_Tokens
{
  Four_Tokens () = default;
  Four_Tokens (Four_Tokens const &) = delete;
  Four_Tokens (Four_Tokens &&) = delete;
  Four_Tokens &operator= (Four_Tokens const &) = delete;
  Four_Tokens &operator= (Four_Tokens &&) = delete;

  std::array<std::unique_ptr<ACE_Token_Proxy>, 3> first_owners_ {};
  std::unique_ptr<ACE_Token_Proxy> second_owner_;
  std::array<ACE_Token_Proxy *, 3> first_ {}; // Borrowed from first_owners_.
  ACE_Token_Proxy *second_ {}; // Borrowed from second_owner_.
  int iterations {100};
};

enum class Token_Kind
{
  MUTEX,
  READER,
  WRITER
};

struct Options
{
  ACE_TCHAR const *server_host {ACE_DEFAULT_SERVER_HOST};
  int server_port {ACE_DEFAULT_SERVER_PORT};
  int iterations {100};
  bool ignore_deadlock {};
  bool remote_mutexes {};
  bool rwlocks {};
};

static std::unique_ptr<ACE_Token_Proxy>
create_token (ACE_TCHAR const *name, Token_Kind kind, Options const &options)
{
  if (options.remote_mutexes)
    {
      switch (kind)
        {
        case Token_Kind::MUTEX:
          return std::make_unique<ACE_Remote_Mutex> (name, options.ignore_deadlock, 1);
        case Token_Kind::READER:
          return std::make_unique<ACE_Remote_RLock> (name, options.ignore_deadlock, 1);
        case Token_Kind::WRITER:
          return std::make_unique<ACE_Remote_WLock> (name, options.ignore_deadlock, 1);
        }
    }
  else
    {
      switch (kind)
        {
        case Token_Kind::MUTEX:
          return std::make_unique<ACE_Local_Mutex> (name, options.ignore_deadlock, 1);
        case Token_Kind::READER:
          return std::make_unique<ACE_Local_RLock> (name, options.ignore_deadlock, 1);
        case Token_Kind::WRITER:
          return std::make_unique<ACE_Local_WLock> (name, options.ignore_deadlock, 1);
        }
    }

  return nullptr;
}

static ACE_THR_FUNC_RETURN
two_token_thread (void *argument)
{
  Two_Tokens const * const tokens = static_cast<Two_Tokens const *> (argument);

  for (int iteration {}; iteration < tokens->iterations; ++iteration)
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
run_writer (void *argument)
{
  Four_Tokens const * const ft = static_cast<Four_Tokens const *> (argument);
  std::size_t acquire_number {};

  for (int iteration {}; iteration < ft->iterations; ++iteration)
    {
      // Cycle through each of the first three tokens.
      ACE_Token_Proxy * const token = ft->first_[acquire_number];
      acquire_number = (acquire_number + 1) % ft->first_.size ();

      if (token->acquire () == -1)
        {
          ACE_ASSERT (errno == EDEADLK);
          ACE_DEBUG ((LM_DEBUG, "Deadlock detected.\n"));
          continue;
        }

      if (ACE_TOKEN_INVARIANTS::instance ()->acquired (token) == 0)
        {
          token->dump ();
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
        ACE_TOKEN_INVARIANTS::instance ()->releasing (token);

        token->release ();
    }

  ACE_DEBUG ((LM_DEBUG, "thread %t exiting\n"));
  return ACE_THR_FUNC_RETURN_NULL;
}

static int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_LOG_MSG->open (argv[0]);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("iuh:rp:n:"), 1);

  for (int option {}; (option = get_opt ()) != -1; )
    {
      switch (option)
        {
        case 'r':
          options.rwlocks = true;
          break;
        case 'i':
          options.ignore_deadlock = true;
          break;
        case 'h':
          options.server_host = get_opt.opt_arg ();
          options.remote_mutexes = true;
          break;
        case 'p':
          options.server_port = ACE_OS::atoi (get_opt.opt_arg ());
          options.remote_mutexes = true;
          break;
        case 'n':
          options.iterations = ACE_OS::atoi (get_opt.opt_arg ());
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

  if (options.iterations < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "iterations cannot be negative\n"), -1);

  if (options.remote_mutexes)
    ACE_Remote_Mutex::set_server_address (
      ACE_INET_Addr (options.server_port, options.server_host));

  return 0;
}

int
mutex_test (Options const &options)
{
  ACE_Thread_Manager thr_mgr;

  Two_Tokens one;
  Two_Tokens two;
  one.iterations = options.iterations;
  two.iterations = options.iterations;
  std::unique_ptr<ACE_Token_Proxy> global_mutex = create_token (
    ACE_TEXT ("global proxy"), Token_Kind::MUTEX, options);
  one.first_owner_ = create_token (ACE_TEXT ("local proxy"), Token_Kind::MUTEX, options);
  two.second_owner_ = create_token (ACE_TEXT ("local proxy"), Token_Kind::MUTEX, options);
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
rwlock_test (Options const &options)
{
  ACE_Thread_Manager thr_mgr;

  std::array<Two_Tokens, 3> readers;
  Four_Tokens writer;

  writer.iterations = options.iterations;

  std::array<ACE_TCHAR const *, 3> const token_names {{
    ACE_TEXT ("writer first 1"),
    ACE_TEXT ("writer first 2"),
    ACE_TEXT ("writer first 3")
  }};

  for (std::size_t index {}; index < readers.size (); ++index)
    {
      Two_Tokens &reader = readers[index];
      reader.iterations = options.iterations;
      reader.first_owner_ = create_token (
        ACE_TEXT ("reader first"), Token_Kind::READER, options);
      ACE_TCHAR const * const second_name = options.remote_mutexes
        ? ACE_TEXT ("reader first") : token_names[index];
      reader.second_owner_ = create_token (second_name, Token_Kind::READER, options);
      reader.first_ = reader.first_owner_.get ();
      reader.second_ = reader.second_owner_.get ();

      writer.first_owners_[index] = create_token (
        token_names[index], Token_Kind::WRITER, options);
      writer.first_[index] = writer.first_owners_[index].get ();
    }

  writer.second_owner_ = create_token (
    ACE_TEXT ("reader first"), Token_Kind::WRITER, options);
  writer.second_ = writer.second_owner_.get ();

  // Tell the token manager to be verbose when reporting deadlock.
  ACE_Token_Manager::instance ()->debug (1);

  for (Two_Tokens &reader : readers)
    if (thr_mgr.spawn (two_token_thread, &reader, THR_BOUND) == -1)
      ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "reader spawn"), -1);

  if (thr_mgr.spawn (run_writer, &writer, THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "second spawn"), -1);

  // Wait for all threads to exit.
  thr_mgr.wait ();

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  if (parse_args (argc, argv, options) == -1)
    return -1;

  if (options.rwlocks)
    rwlock_test (options);
  else
    mutex_test (options);

  ACE_DEBUG ((LM_DEBUG, "test exiting.\n"));
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "threads not supported on this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS */
