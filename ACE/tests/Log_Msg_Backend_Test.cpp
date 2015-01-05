
//=============================================================================
/**
 *  @file    Log_Msg_Backend_Test.cpp
 *
 *  $Id$
 *
 *   This program tests the ACE_Log_Msg class's use of a custom backend,
 *   including its initialization and reset as well as logging information
 *   through the backend.
 *
 *
 *  @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#include "test_config.h"

#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Backend.h"
#include "ace/Log_Record.h"



class Backend : public ACE_Log_Msg_Backend
{
public:
  Backend ()
    : reset_ (false), open_ (false), close_ (false), log_count_ (0) {}

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  virtual int open (const ACE_TCHAR *logger_key);

  virtual int reset (void);

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  virtual int close (void);

  virtual ssize_t log (ACE_Log_Record &log_record);

  // Test probes to see if things worked as specified.
  bool hooks_ok (void) const;
  unsigned int log_count (void) const { return this->log_count_; }

private:
  bool reset_;
  bool open_;
  bool close_;
  unsigned int log_count_;
};

int
Backend::open (const ACE_TCHAR *)
{
  this->open_ = true;
  return 0;
}

int
Backend::reset (void)
{
  this->reset_ = true;
  return 0;
}

int
Backend::close (void)
{
  return 0;
}

ssize_t
Backend::log (ACE_Log_Record &)
{
  ++this->log_count_;
  return 1;
}

// Test probes to see if things worked as specified.
bool
Backend::hooks_ok (void) const
{
  // Close should not have been called yet.
  const ACE_TCHAR *yes = ACE_TEXT ("yes");
  const ACE_TCHAR *no = ACE_TEXT ("no");
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Open: %s, Reset: %s, Close: %s\n"),
              this->open_ ? yes : no,
              this->reset_ ? yes : no,
              this->close_ ? yes : no));
  return this->open_ && this->reset_ && !this->close_;
}

// Main function.

int
run_main (int, ACE_TCHAR *[])
{
  // Set up the backend prior to ACE_START_TEST so the initialization can
  // call the back end's reset().
  Backend b;
  ACE_Log_Msg_Backend *old_b = ACE_Log_Msg::msg_backend (&b);

  ACE_START_TEST (ACE_TEXT ("Log_Msg_Backend_Test"));

  int status = 0;

  if (old_b != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Old backend (%@) not 0 at start\n"),
                  old_b));
      ++status;
    }

  // Reopen to get the backend established.
  u_long flags = ACE_LOG_MSG->flags ();
  flags |= ACE_Log_Msg::CUSTOM;
  if (-1 == ACE_LOG_MSG->open (ACE_TEXT ("Log_Msg_Backend_Test"), flags))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("Reopening log")));
      ++status;
    }

  // Make sure messages are getting routed correctly.
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Message 1\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Message 2\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Message 3\n")));
  unsigned int count = b.log_count ();
  if (count != 3)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Backend counted %u; expected 3\n"),
                  count));
      ++status;
    }

  // Should have seen the hooks other than close() so far. Note that
  // hooks_ok() logs a message so check counts before checking hooks.
  if (!b.hooks_ok ())
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Hooks not called correctly\n")));
      ++status;
    }

  ACE_END_TEST;

  // Reset the backend to avoid references to our soon-to-be-destroyed one.
  ACE_Log_Msg::msg_backend (old_b);

  return status;
}

