/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    ACE.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_ACE_H)
#define ACE_ACE_H

#include "ace/OS.h"

// Forward declarations.
class ACE_Time_Value;
class ACE_Thread_Manager;
class ACE_Reactor;
class ACE_Event_Handler;

class ACE_Export ACE
{
  // = TITLE
  //     Contains value added ACE methods that extend the behavior
  //     of the UNIX and Win32 OS calls.
  //
  // = DESCRIPTION
  //     This class consolidates all these ACE static methods in a
  //     single place in order to manage the namespace better.  These
  //     methods are put here rather than in ACE_OS in order to
  //     separate concerns.
public:
  // = Network I/O functions that factor out differences between Win32 and UNIX.
  static ssize_t recv (ACE_HANDLE handle, 
		       void *buf, 
		       size_t len, 
		       int flags);
  // Receive up to <len> bytes into <buf> from <handle> (uses the
  // <recv> call).

  static ssize_t recv (ACE_HANDLE handle, 
		       void *buf, 
		       size_t len);
  // Receive up to <len> bytes into <buf> from <handle> (uses the
  // <read> system call on UNIX and the <recv> call on Win32).

  static ssize_t recv (ACE_HANDLE handle, 
		       void *buf, 
		       size_t len, 
		       int flags,
		       const ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to receive up to <len> bytes
  // into <buf> from <handle> (uses the <recv> call).  If <recv> times
  // out a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes received is returned.

  // = Network I/O functions that recv and send exactly n bytes.
  static ssize_t recv_n (ACE_HANDLE handle, 
			 void *buf, 
			 size_t len, 
			 int flags);
  // Receive <len> bytes into <buf> from <handle> (uses the <recv>
  // call).  If <handle> is set to non-blocking mode this call will
  // poll until all <len> bytes are received.

  static ssize_t recv_n (ACE_HANDLE handle, 
			 void *buf, 
			 size_t len);
  // Receive <len> bytes into <buf> from <handle> (uses the <read>
  // system call on UNIX and the <recv> call on Win32).  If <handle>
  // is set to non-blocking mode this call will poll until all <len>
  // bytes are received.

  static ssize_t recv_n (ACE_HANDLE handle, 
			 void *buf, 
			 size_t len, 
			 int flags,
			 const ACE_Time_Value *timeout);
  // Try to recv exactly <len> bytes into <buf> from <handle> (uses
  // the <recv> call).  If <recv> blocks for longer than <timeout> the
  // number of bytes actually read is returned with <errno == ETIME>.
  // If a timeout does not occur, <recv_n> return <len> (i.e., the
  // number of bytes requested to be read).

  static ssize_t send (ACE_HANDLE handle, 
		       const void *buf, 
		       size_t len, 
		       int flags);
  // Send up to <len> bytes into <buf> from <handle> (uses the <send>
  // call).

  static ssize_t send (ACE_HANDLE handle, 
		       const void *buf, 
		       size_t len);
  // Send up to <len> bytes into <buf> from <handle> (uses the <write>
  // system call on UNIX and the <send> call on Win32).

  static ssize_t send_n (ACE_HANDLE handle, 
			 const void *buf, 
			 size_t len, 
			 int flags);
  // Send <len> bytes from <buf> to <handle> (uses the <send> system
  // call).  If <handle> is set to non-blocking mode this call will
  // poll until all <len> bytes are sent.

  static ssize_t send_n (ACE_HANDLE handle, 
			 const void *buf, 
			 size_t len);
  // Send <len> bytes from <buf> to <handle> (uses the <write> system
  // call on UNIX and the <recv> call on Win32).  If <handle> is set
  // to non-blocking mode this call will poll until all <len> bytes
  // are sent.

  static ssize_t send (ACE_HANDLE handle, 
		       const void *buf, 
		       size_t len, 
		       int flags,
		       const ACE_Time_Value *timeout);
  // Wait to to <timeout> amount of time to send up to <len> bytes
  // into <buf> from <handle> (uses the <send> call).  If <send> times
  // out a -1 is returned with <errno == ETIME>.  If it succeeds the
  // number of bytes sent is returned.  

  static ssize_t send_n (ACE_HANDLE handle, 
			 const void *buf, 
			 size_t len, 
			 int flags,
			 const ACE_Time_Value *timeout);
  // Try to send exactly <len> bytes into <buf> from <handle> (uses
  // the <send> call).  If <send> blocks for longer than <timeout> the
  // number of bytes actually sent is returned with <errno == ETIME>.
  // If a timeout does not occur, <send_n> return <len> (i.e., the
  // number of bytes requested to be sent).

  // = File system I/O functions that encapsulate differences between UNIX and Win32 and also send and recv exactly n bytes.
  static ssize_t read_n (ACE_HANDLE handle, 
			 void *buf, 
			 size_t len);
  // Receive <len> bytes into <buf> from <handle> (uses the <read>
  // system call on UNIX and the <ReadFile> call on Win32).  If
  // <handle> is set to non-blocking mode this call will poll until
  // all <len> bytes are received.

  static ssize_t write_n (ACE_HANDLE handle, 
			  const void *buf, 
			  size_t len);
  // Send <len> bytes from <buf> to <handle> (uses the <write> system
  // call on UNIX and the <WriteFile> call on Win32).  If <handle> is
  // set to non-blocking mode this call will poll until all <len>
  // bytes are sent.

  // = Functions that perform useful behavior related to establishing
  // socket connections active and passively.
  static int bind_port (ACE_HANDLE handle);
  // Bind a new unused port to <handle>.

  static int handle_timed_accept (ACE_HANDLE listener,
				  ACE_Time_Value *timeout, 
				  int restart);
  // Wait up to <timeout> amount of time to passively establish a
  // connection.  This method doesn't perform the <accept>, it just
  // does the timed wait...

  static ACE_HANDLE handle_timed_complete (ACE_HANDLE listener, 
					   ACE_Time_Value *timeout);
  // Wait up to <timeout> amount of time to complete an actively
  // established non-blocking connection.

  // = Operations on HANDLEs.

  static ACE_HANDLE handle_timed_open (ACE_Time_Value *timeout,
				       LPCTSTR name,
				       int flags, 
				       int perms);
  // Wait up to <timeout> amount of time to actively open a device.
  // This method doesn't perform the <connect>, it just does the timed
  // wait...

  // = Set/get/clear various flags related to I/O HANDLE.
  static int set_flags (ACE_HANDLE handle, 
			int flags);
  // Set flags associated with <handle>.

  static int clr_flags (ACE_HANDLE handle, 
			int flags);
  // Clear flags associated with <handle>.

  static int get_flags (ACE_HANDLE handle);
  // Return the current setting of flags associated with <handle>.

  static int set_handle_limit (int new_limit = -1);
  // Reset the limit on the number of open handles.  If <new_limit> ==
  // -1 set the limit to the maximum allowable.  Otherwise, set it to
  // be the value of <new_limit>.

  static int max_handles (void);
  // Returns the maximum number of open handles currently permitted in
  // this process.  This maximum may be extended using
  // <ACE::set_handle_limit>.

  // = Miscelleous functions.
  static size_t	round_to_pagesize (off_t length);
  // Rounds the request to a multiple of the page size.

  static int format_hexdump (char *buffer, int size, char *obuf, int obuf_sz);
  // Format buffer into printable format.  This is useful for
  // debugging.

  static char *strenvdup (const char *str);
  // Return a dynamically allocated duplicate of <str>, substituting
  // the environment variable if <str[0] == '$'>.  Note that the
  // pointer is allocated with <ACE_OS::malloc> and must be freed by
  // <ACE_OS::free>

  static char *strecpy (char *s, const char *t);
  // Copies <t> to <s>, returning a pointer to the end of the copied
  // region (rather than the beginning, a la <strcpy>.

  static const char *execname (const char *pathname);
  // On Win32 returns <pathname> if it already ends in ".exe,"
  // otherwise returns a dynamically allocated buffer containing
  // "<pathname>.exe".  Always returns <pathname> on UNIX.

  static const char *basename (const char *pathname, char delim);
  // Returns the "basename" of a <pathname>.

#if defined (ACE_HAS_UNICODE)
  static const wchar_t *basename (const wchar_t *pathname, wchar_t delim);
  // Returns the "basename" of a <pathname>.
#endif /* ACE_HAS_UNICODE */

  static char *timestamp (char date_and_time[], int time_len);
  // Returns the current timestamp in the form
  // "hour:minute:second:microsecond."  The month, day, and year are
  // also stored in the beginning of the date_and_time array.  Returns
  // 0 if unsuccessful, else returns pointer to beginning of the
  // "time" portion of <day_and_time>.

  static int daemonize (const char pathname[] = "/");
  // Become a daemon process.

  // = Methods for searching and opening shared libraries using relative naming. 
  static int ldfind (const char *filename, 
		     char *pathname, 
		     size_t maxlen);
  // Finds the file <filename> either using an absolute path or using
  // a relative path in conjunction with ACE_LD_SEARCH_PATH (e.g.,
  // $LD_LIBRARY_PATH on UNIX or $PATH on Win32).  This function will
  // add appropriate suffix (e.g., .dll on Win32 or .so on UNIX)
  // according to the OS platform.  In addition, if a relative path is
  // used, this function will prefix the appropriate prefix (e.g.,
  // "lib" on UNIX and "" on Win32).

  static FILE *ldopen (const char *filename, const char *type);
  // Uses <ldopen> to locate and open the appropriate <filename> and
  // returns a pointer to the file, else it returns a NULL
  // pointer. <type> specifies how the file should be open.

  static u_long hash_pjw (const char *str);
  // Computes the hash value of <str> using the ``Hash PJW'' routine...

  static int map_errno (int error);
  // Map troublesome win32 errno values to values that standard C
  // strerr function understands.  Thank you Microsoft.

  static void *read_adapter (void *event_handler);
  // Used to read from non-socket ACE_HANDLEs in our own thread to
  // work around Win32 limitations that don't allow us to select() on
  // non-sockets (such as ACE_STDIN).  This is commonly used in
  // situations where the Reactor is used to demultiplex read events
  // on ACE_STDIN on UNIX.  Note that <event_handler> must be a
  // subclass of <ACE_Event_Handler>.  If the <get_handle> method of
  // this event handler returns <ACE_INVALID_HANDLE> we default to
  // reading from ACE_STDIN.

  static int register_stdin_handler (ACE_Event_Handler *eh,
				     ACE_Reactor *reactor,
				     ACE_Thread_Manager *thr_mgr,
				     int flags = THR_DETACHED);
  // Abstracts away from the differences between Win32 and ACE with
  // respect to reading from ACE_STDIN (which is non-select()'able on
  // Win32.

private:
  ACE (void);
  // Ensure we can't define an instance of this class...
};

#include "ace/ACE.i"

#endif  /* ACE_ACE_H */
