
//=============================================================================
/**
 *  @file    manual.cpp
 *
 *  Allows manual operations on local and remote tokens.
 *
 *  @author Tim Harrison
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Local_Tokens.h"
#include "ace/Remote_Tokens.h"
#include "ace/Singleton.h"
#include "ace/Thread_Manager.h"
#include "ace/Token_Invariants.h"
#include "ace/Token_Collection.h"
#include "ace/Map_Manager.h"
#include "ace/Service_Config.h"
#include <memory>
#include <utility>
#include <vector>

#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREADS_LIBRARY)


using ACE_TOKEN_INVARIANTS = ACE_Token_Invariant_Manager;

/**
 * @class STDIN_Token
 *
 * @brief STDIN Token
 *
 * Translates STDIN commands to ACE Token commands.
 */
class STDIN_Token : public ACE_Event_Handler
{
public:
  /// Construction.
  STDIN_Token ();

  /// Parse command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  //FUZZ: disable check_for_lack_ACE_OS
  /// Register with whatever event dispatcher is needed and run.
  ///FUZZ: enable check_for_lack_ACE_OS
  int open (int argc, char *argv[]);

  // = Event_Handler methods.
  int handle_input (ACE_HANDLE) override;
  int handle_exception (ACE_HANDLE) override;

  STDIN_Token (STDIN_Token const &) = delete;
  STDIN_Token (STDIN_Token &&) = delete;
  STDIN_Token &operator= (STDIN_Token const &) = delete;
  STDIN_Token &operator= (STDIN_Token &&) = delete;

  using TID = ACE_CString;

private:
  /// Display options.
  void display_menu ();

  /// Get or make a proxy to <token> with a <tid> client id.
  ACE_Token_Proxy *get_proxy (const char *tid, const char *token, char type);

  /// Create a proxy to <token> with a <tid> client id.
  std::unique_ptr<ACE_Token_Proxy> create_proxy (const char *token, char type);

  // = Mapping from tid to Token_Collection.
  /// COLLECTION maintains a mapping from tid to a collection.
  using COLLECTIONS = ACE_Map_Manager<TID, ACE_Token_Collection *, ACE_Null_Mutex>;

  std::vector<std::unique_ptr<ACE_Token_Collection>> owned_collections_;
  /// A collection for each <tid>.
  COLLECTIONS collections_;

  char const *server_host_; // Borrowed from argv or ACE_DEFAULT_SERVER_HOST.
  int server_port_;
  bool ignore_deadlock_;
  bool debug_;
  bool remote_;
};

STDIN_Token::STDIN_Token ()
  : server_host_ (ACE_DEFAULT_SERVER_HOST),
    server_port_ (ACE_DEFAULT_SERVER_PORT),
    ignore_deadlock_ (false),
    debug_ (false),
    remote_ (false)
{
}

int
STDIN_Token::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_LOG_MSG->open (argv[0], ACE_Log_Msg::STDERR);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("h:p:diu"), 1);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
      {
      case 'h':  // specify the host machine on which the server is running
        server_host_ = get_opt.opt_arg ();
        remote_ = true;
        break;
      case 'p':  // specify the port on which the server is running
        server_port_ = ACE_OS::atoi (get_opt.opt_arg ());
        remote_ = true;
        break;
      case 'd':
        debug_ = true;
        break;
      case 'i':
        ignore_deadlock_ = true;
        break;
      case 'u':
        // usage same as unknown
        ACE_FALLTHROUGH;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
        "%n:\n"
        "[-h <remote host>]\n"
        "[-p <remote port>]\n"
        "[-i ignore deadlock]\n"
        "[-d debug]\n", 1), -1);
      }
    }

  if (remote_)
    ACE_Remote_Mutex::set_server_address (ACE_INET_Addr (server_port_,
                                                         server_host_));

  return 0;
}

int
STDIN_Token::open (int argc, char *argv[])
{
  if (this->parse_args (argc, argv) == -1)
    return -1;

  // Register for signals.
  if (ACE_Reactor::instance ()->register_handler
      (SIGINT, this) == -1)
    ACE_DEBUG ((LM_DEBUG, "Can't register signal handler\n"));

#if defined (ACE_WIN32)

#else
  // Register for STDIN events with Reactor.
  if (ACE_Reactor::instance ()->register_handler
      (ACE_STDIN, this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "Can't register signal handler\n"), 0);


#endif /* ACE_WIN32 */


  this->display_menu ();

#if defined (ACE_WIN32)

#else
  ACE_Reactor::run_event_loop ();
#endif /* ACE_WIN32 */

  ACE_OS::printf ("Exiting...\n");
  return 0;
}

int
STDIN_Token::handle_input (ACE_HANDLE fd)
{
  ACE_UNUSED_ARG (fd);

  char tid[BUFSIZ];
  char token[BUFSIZ];
  char type[16];
  char operation[16];

  if (::scanf ("%s %s %s %s", tid, token, type, operation) <= 0)
    {
      ACE_OS::printf ("Try again.\n");
      return 0;
    }

  ACE_Token_Proxy *proxy =
    this->get_proxy (tid, token, type[0]);

  if (proxy == nullptr)
    return -1;

  switch (operation[0])
    {
    case 'a':
    case 'A':
      if (proxy->acquire () == 0)
        {
          ACE_OS::printf ("Succeeded.\n");
          if (ACE_TOKEN_INVARIANTS::instance ()->acquired (proxy) == 0)
            ACE_OS::printf ("Violated invariant.\n");
        }
      else
        ACE_ERROR ((LM_ERROR, "%p.\n", "Acquire failed"));
      break;
    case 'n':
    case 'N':
      ACE_TOKEN_INVARIANTS::instance ()->releasing (proxy);
      if (proxy->renew () == 0)
        {
          ACE_OS::printf ("Succeeded.\n");
          if (ACE_TOKEN_INVARIANTS::instance ()->acquired (proxy) == 0)
            ACE_OS::printf ("Violated invariant.\n");
        }
      else
        ACE_ERROR ((LM_ERROR, "%p.\n", "Renew failed"));
      break;

    case 'r':
    case 'R':
      ACE_TOKEN_INVARIANTS::instance ()->releasing (proxy);
      if (proxy->release () == 0)
        ACE_OS::printf ("Succeeded.\n");
      else
        ACE_ERROR ((LM_ERROR, "%p.\n", "Release failed"));
      break;

    case 't':
    case 'T':
      if (proxy->tryacquire () == 0)
        {
          ACE_OS::printf ("Succeeded.\n");
          if (ACE_TOKEN_INVARIANTS::instance ()->acquired (proxy) == 0)
            ACE_OS::printf ("Violated invariant.\n");
        }
      else
        ACE_ERROR ((LM_ERROR, "%p.\n", "Tryacquire failed"));
      break;
    }

  this->display_menu ();
  return 0;
}

void
STDIN_Token::display_menu ()
{
  ACE_OS::printf ("<tid> <token> <type> <operation>\n");
}

int
STDIN_Token::handle_exception (ACE_HANDLE fd)
{
  ACE_UNUSED_ARG (fd);

  ACE_Reactor::run_event_loop ();
  return -1;
}

ACE_Token_Proxy *
STDIN_Token::get_proxy (const char *_tid, const char *token, char type)
{
  ACE_Token_Collection *proxy_collection; // Borrowed from owned_collections_.

  TID const tid (_tid);

  if (collections_.find (tid, proxy_collection) == -1)
    // We did not find a proxy_collection.
    {
      // Make one.
      std::unique_ptr<ACE_Token_Collection> collection =
        std::make_unique<ACE_Token_Collection> (debug_, "no name collection");
      proxy_collection = collection.get ();

      this->owned_collections_.push_back (std::move (collection));

      // Put it in the collections.
      if (collections_.bind (tid, proxy_collection) == -1)
        {
          this->owned_collections_.pop_back ();
          return nullptr;
        }
    }

  // Either way, we have a proxy_collection now.

  // See if the proxy already exists in the collection.
  ACE_Token_Proxy *proxy = proxy_collection->is_member (token); // Borrowed from proxy_collection.

  // If not, create one.
  if (proxy == nullptr)
    {
      std::unique_ptr<ACE_Token_Proxy> new_proxy = this->create_proxy (token, type);
      if (!new_proxy)
        return nullptr;

      // Put the new_proxy in this tid's collection.
      if (proxy_collection->insert (*new_proxy) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "insert failed\n"), nullptr);

      proxy = proxy_collection->is_member (token);

      if (proxy == nullptr)
        ACE_ERROR_RETURN ((LM_ERROR, "is_member failed\n"), nullptr);

      // Set the client_id (it was set to 1 since we're
      // single-threaded.
      proxy->client_id (_tid);
    }

  return proxy;
}

std::unique_ptr<ACE_Token_Proxy>
STDIN_Token::create_proxy (const char *token, char type)
{
  switch (type)
    {
    case 'm':
    case 'M':
      if (remote_)
        return std::make_unique<ACE_Remote_Mutex> (token, ignore_deadlock_, debug_);
      else
        return std::make_unique<ACE_Local_Mutex> (token, ignore_deadlock_, debug_);

    case 'r':
    case 'R':
      if (remote_)
        return std::make_unique<ACE_Remote_RLock> (token, ignore_deadlock_, debug_);
      else
        return std::make_unique<ACE_Local_RLock> (token, ignore_deadlock_, debug_);

    case 'w':
    case 'W':
      if (remote_)
        return std::make_unique<ACE_Remote_WLock> (token, ignore_deadlock_, debug_);
      else
        return std::make_unique<ACE_Local_WLock> (token, ignore_deadlock_, debug_);
    }

  // should never get here, but this avoids a compiler warning . . .
  return nullptr;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  STDIN_Token st;
  return st.open (argc, argv);
}

#else
int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "threads or ACE_HAS_TOKENS_LIBRARY not supported on this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS && ACE_HAS_TOKENS_LIBRARY */
