// -*- C++ -*-

// ============================================================================
/**
 *  @file Test_Output.cpp
 *
 *  $Id$
 *
 *   This file factors out common macros and other utilities used by the
 *   ACE automated regression tests.
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu>
 *  @author Tim Harrison <harrison@cs.wustl.edu>
 *  @author David Levine <levine@cs.wustl.edu>
 *  @author Don Hinton <dhinton@dresystems.com>
 */
// ============================================================================

#include "tests/test_config.h"
#include "ace/OS.h"
#include "ace/Object_Manager.h"
#include "ace/streams.h"
#include "ace/Framework_Component.h"
#include "ace/Synch.h"
#include "ace/Log_Msg.h"
#include "ace/ACE.h"

ACE_Test_Output *ACE_Test_Output::instance_ = 0;

ACE_Test_Output::ACE_Test_Output (void)
  : output_file_ (0)
{
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  this->output_file_ = new OFSTREAM;
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
}

ACE_Test_Output::~ACE_Test_Output (void)
{
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY) && !defined (ACE_PSOS)
  ACE_LOG_MSG->msg_ostream (&cerr);
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY && ! ACE_PSOS */

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY) && !defined (ACE_HAS_PHARLAP)
  delete this->output_file_;
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */
}

OFSTREAM *
ACE_Test_Output::output_file (void)
{
  return this->output_file_;
}

int
ACE_Test_Output::set_output (const ACE_TCHAR *filename, int append)
{
#if defined (ACE_HAS_PHARLAP)
  // For PharLap, just send it all to the host console for now - redirect
  // to a file there for saving/analysis.
  EtsSelectConsole(ETS_CO_HOST);
  ACE_LOG_MSG->msg_ostream (&cout);

#else
  ACE_TCHAR temp[MAXPATHLEN];
  // Ignore the error value since the directory may already exist.
  const ACE_TCHAR *test_dir;

#if !defined (ACE_HAS_WINCE)
  test_dir = ACE_OS::getenv (ACE_TEXT ("ACE_TEST_DIR"));

  if (test_dir == 0)
#endif /* ACE_HAS_WINCE */
    test_dir = ACE_TEXT ("");

  ACE_OS::sprintf (temp,
                   ACE_TEXT ("%s%s%s%s"),
                   test_dir,
                   ACE_LOG_DIRECTORY,
                   ACE::basename (filename, ACE_DIRECTORY_SEPARATOR_CHAR),
                   ACE_LOG_FILE_EXT_NAME);

#if defined (VXWORKS)
  // This is the only way I could figure out to avoid a console
  // warning about opening an existing file (w/o O_CREAT), or
  // attempting to unlink a non-existant one.
  ACE_HANDLE fd = ACE_OS::open (temp,
                                O_WRONLY|O_CREAT,
                                S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
  if (fd != ERROR)
    {
      ACE_OS::close (fd);
      ACE_OS::unlink (temp);
    }
# else /* ! VXWORKS */
  // This doesn't seem to work on VxWorks if the directory doesn't
  // exist: it creates a plain file instead of a directory.  If the
  // directory does exist, it causes a wierd console error message
  // about "cat: input error on standard input: Is a directory".  So,
  // VxWorks users must create the directory manually.
#   if defined (ACE_HAS_WINCE)
      ACE_OS::mkdir (ACE_LOG_DIRECTORY_FOR_MKDIR);
#   else
      ACE_OS::mkdir (ACE_LOG_DIRECTORY);
#   endif  // ACE_HAS_WINCE
# endif /* ! VXWORKS */

# if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  this->output_file_->open (ACE_TEXT_ALWAYS_CHAR (temp),
                            ios::out | (append ? ios::app : ios::trunc));
  if (this->output_file_->bad ())
    return -1;
#else /* when ACE_LACKS_IOSTREAM_TOTALLY */
  ACE_TCHAR *fmode = 0;
  if (append)
    fmode = ACE_TEXT ("a");
  else
    fmode = ACE_TEXT ("w");
  this->output_file_ = ACE_OS::fopen (temp, fmode);
# endif /* ACE_LACKS_IOSTREAM_TOTALLY */

  ACE_LOG_MSG->msg_ostream (this->output_file ());
#endif /* ACE_HAS_PHARLAP */

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER );
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

  return 0;
}

void
ACE_Test_Output::close (void)
{
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
  this->output_file_->flush ();
  this->output_file_->close ();
#else
  ACE_OS::fflush (this->output_file_);
  ACE_OS::fclose (this->output_file_);
#endif /* !ACE_LACKS_IOSTREAM_TOTALLY */
}

ACE_Test_Output*
ACE_Test_Output::instance ()
{
  if (ACE_Test_Output::instance_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));

      if (ACE_Test_Output::instance_ == 0)
        {
          ACE_NEW_RETURN (ACE_Test_Output::instance_,
                          ACE_Test_Output,
                          0);
          ACE_REGISTER_FRAMEWORK_COMPONENT(ACE_Test_Output, ACE_Test_Output::instance_)
        }
    }
  return ACE_Test_Output::instance_;
}

const ACE_TCHAR *
ACE_Test_Output::dll_name (void)
{
  return ACE_TEXT ("Test_Output");
}

const ACE_TCHAR *
ACE_Test_Output::name (void)
{
  return ACE_TEXT ("ACE_Test_Output");
}

void
ACE_Test_Output::close_singleton (void)
{
  delete ACE_Test_Output::instance_;
  ACE_Test_Output::instance_ = 0;
}

void
randomize (int array[], size_t size)
{
  size_t i;

  for (i = 0; i < size; i++)
    array [i] = ACE_static_cast (int, i);

  // See with a fixed number so that we can produce "repeatable"
  // random numbers.
  ACE_OS::srand (0);

  // Generate an array of random numbers from 0 .. size - 1.

  for (i = 0; i < size; i++)
    {
      size_t index = ACE_OS::rand() % size--;
      int temp = array [index];
      array [index] = array [size];
      array [size] = temp;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
   template class ACE_Framework_Component_T<ACE_Test_Output>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#  pragma instantiate ACE_Framework_Component_T<ACE_Test_Output>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
