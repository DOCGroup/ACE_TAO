// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Bug_3943_Regression_Test.cpp
//
// = DESCRIPTION
//    When a large buffer is sent to ACE_OS::sendv, using winsock2,
//    ENOBUFS occurs, which indicates that the buffer was too large
//    for WSASend to accept. On other platforms ENOBUFS indicates a
//    situation similar to EWOULDBLOCK, only that the buffer on the
//    send side is completely full. ACE_OS::sendv for winsock2 now
//    tries to divide the buffer and try to send, until data is sent
//    or a different error is received. This test allocates a large
//    iovec array buffer and verifies that a partial send occurs.
//
// = AUTHOR
//    Brian Johnson <johnsonb@ociweb.com>,
//
// ============================================================================

#include "test_config.h"
#include "ace/SOCK_Connector.h"
#include "ace/LOCK_SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Handle_Set.h"
#include "ace/Connector.h"
#include "ace/OS_NS_sys_select.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_netdb.h"


// The following works around bugs with some operating systems, which
// don't allow multiple threads/process to call accept() on the same
// listen-mode port/socket.  Also, note that since timed accept is
// implemented using select(), and we use timed accepts with threads,
// we need a real lock when using timed accepts even if the OS has
// thread-safe accept.
//
#if defined (ACE_LACKS_FORK)
#  if defined (ACE_HAS_THREADS)
#    include "ace/Thread_Mutex.h"
     typedef ACE_Thread_Mutex ACCEPTOR_LOCKING;
#  else
#    include "ace/Null_Mutex.h"
     typedef ACE_Null_Mutex ACCEPTOR_LOCKING;
#  endif /* ACE_HAS_THREADS */
#else
#  if defined (ACE_HAS_THREAD_SAFE_ACCEPT)
#    include "ace/Null_Mutex.h"
     typedef ACE_Null_Mutex ACCEPTOR_LOCKING;
#  else
#    include "ace/Process_Mutex.h"
     typedef ACE_Process_Mutex ACCEPTOR_LOCKING;
#  endif /* ACE_HAS_THREAD_SAFE_ACCEPT */
#endif /* ACE_LACKS_FORK */

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
#define LOCK_SOCK_ACCEPTOR ACE_LOCK_SOCK_Acceptor<ACCEPTOR_LOCKING>
#else
#define LOCK_SOCK_ACCEPTOR ACE_LOCK_SOCK_Acceptor<ACCEPTOR_LOCKING>, ACE_INET_Addr
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

#define REFCOUNTED_HASH_RECYCLABLE_ADDR ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>

namespace {

  const char FINISHED_CHAR = '%';
  const char RESTART_CHAR = '&';
  const char START_CHAR = '0';
  bool server_complete = false;
  bool client_complete = false;
  volatile int expected_num_messages = 0;

  char nextChar(const char current)
  {
    if ((current == '9') ||
        (current == RESTART_CHAR) ||
        (current == FINISHED_CHAR))
      return '0';

    return current + 1;
  }

#if defined (ACE_LACKS_IOVEC)
  typedef u_long buffer_len;
#else
  typedef size_t buffer_len;
#endif /* ACE_LACKS_IOVEC */

#if defined (ACE_WIN32)
  int beforeVersion(const DWORD majorVersion,
                    const DWORD minorVersion,
                    const BYTE productType)
  {
#if !defined(ACE_HAS_WINCE)
    OSVERSIONINFOEX versioninfo;
    versioninfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFOEX);
    versioninfo.dwMajorVersion = majorVersion;
    versioninfo.dwMinorVersion = minorVersion;
    versioninfo.wProductType = productType;

    ULONGLONG aboveMajorVer6TypeMask = 0;
    aboveMajorVer6TypeMask =
      ::VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER);

    if (::VerifyVersionInfo(&versioninfo, VER_MAJORVERSION,
                            aboveMajorVer6TypeMask) > 0)
      return 1;

    ACE_OS::set_errno_to_last_error ();
    if (errno != ERROR_OLD_WIN_VERSION)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) VerifyVersionInfo errno = %d, ")
                    ACE_TEXT ("major version check must have been ")
                    ACE_TEXT ("defined incorrectly.\n"),
                    errno));
        return -1;
      }

    ULONGLONG majorV6AboveMinorV1TypeMask = 0;
    majorV6AboveMinorV1TypeMask =
      ::VerSetConditionMask(0, VER_MAJORVERSION, VER_EQUAL);
    majorV6AboveMinorV1TypeMask =
      ::VerSetConditionMask(majorV6AboveMinorV1TypeMask,
                            VER_MINORVERSION, VER_GREATER);

    if (::VerifyVersionInfo(&versioninfo,
                            VER_MAJORVERSION | VER_MINORVERSION,
                            majorV6AboveMinorV1TypeMask) > 0)
      return 1;

    ACE_OS::set_errno_to_last_error ();
    if (errno != ERROR_OLD_WIN_VERSION)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) VerifyVersionInfo errno = %d, ")
                    ACE_TEXT ("minor version check must have been ")
                    ACE_TEXT ("defined incorrectly.\n"),
                    errno));
        return -1;
      }

    ULONGLONG majorV6MinorV1NTTypeMask = 0;
    majorV6MinorV1NTTypeMask =
      ::VerSetConditionMask(0, VER_MAJORVERSION, VER_EQUAL);
    majorV6MinorV1NTTypeMask =
      ::VerSetConditionMask(majorV6MinorV1NTTypeMask,
                            VER_MINORVERSION, VER_EQUAL);
    majorV6MinorV1NTTypeMask =
      ::VerSetConditionMask(majorV6MinorV1NTTypeMask,
                            VER_PRODUCT_TYPE, VER_EQUAL);

    if (::VerifyVersionInfo(&versioninfo,
                       VER_MAJORVERSION | VER_MINORVERSION | VER_PRODUCT_TYPE,
                            majorV6MinorV1NTTypeMask) > 0)
      return 1;

    ACE_OS::set_errno_to_last_error ();
    if (errno != ERROR_OLD_WIN_VERSION)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) VerifyVersionInfo errno=%d,")
                    ACE_TEXT (" call must have been defined incorrectly.\n"),
                    errno));
        return -1;
      }

    return 0;
#else // defined(ACE_HAS_WINCE)
    // no version testing of WinCE has been performed
    ACE_UNUSED_ARG (majorVersion);
    ACE_UNUSED_ARG (minorVersion);
    ACE_UNUSED_ARG (productType);
    return -1;
#endif /* ACE_HAS_WINCE */
  }
#endif /* ACE_WIN32 */

  int processENOBUFS()
  {
#if defined (ACE_WIN32) && !defined(ACE_HAS_WINCE)
    // it has been identified that Windows7 does not have the ENOBUFS issue
    // but testing has not been performed on Server 2008 or Vista to identify
    // wether the issue exists or not
    return beforeVersion(6, 1, VER_NT_WORKSTATION);
#else // defined(ACE_HAS_WINCE)
    // currently, no versions of WINCE identified to not have the ENOBUFS error
    return 0;
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */
  }

  struct IovecGuard
  {
    IovecGuard(const int count, const int slot, const buffer_len max);
    ~IovecGuard();
    char* getBufferAtOffset(const ssize_t offset);

    const int iovcnt_;
    buffer_len totalBytes_;
    iovec* iov_;
    static const int ALL_SLOTS = -1;
  };

  class Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  {
    // = TITLE
    //      This class is the product created by both <ACE_Connector>
    //      and <ACE_Acceptor> objects.
    //
    // = DESCRIPTION
    //    This class gets its own header file to work around AIX C++
    //    compiler "features" related to template instantiation...  It is
    //    only used by Conn_Test.cpp.
  public:
    Svc_Handler (ACE_Thread_Manager * = 0);
    // Do-nothing constructor.

    virtual int open (void *);
    // Initialization hook.

    void send_data (void);
    // Send data to server.

    void recv_data (void);
    // Recv data from client.

    int close (u_long = 0);
    // Shutdown the <Svc_Handler>.

  private:
    enum Direction { READ, WRITE };
    bool wait_for_completion(Direction direction);

    ssize_t send (IovecGuard& iovec_array,
                  const ACE_TCHAR * const send_desc,
                  bool use_sendv,
                  bool test_message = false);

    ssize_t send (char send_char, const ACE_TCHAR * const send_desc);

    const ACE_Time_Value DEFAULT_TIME_VALUE;

  };
} // namespace ""

typedef ACE_Oneshot_Acceptor<Svc_Handler,
                             LOCK_SOCK_ACCEPTOR> ACCEPTOR;
typedef ACE_Connector<Svc_Handler,
                      ACE_SOCK_CONNECTOR> CONNECTOR;


IovecGuard::IovecGuard(const int count, const int slot, const buffer_len max)
  : iovcnt_(count),
    totalBytes_(0)
{
  ACE_NEW (iov_,iovec[iovcnt_]);
  int i = 0;
  char expChar = START_CHAR;
  for ( ; i < iovcnt_; ++i)
    {
      iov_[i].iov_len = ((slot == i) || (slot == ALL_SLOTS)) ? max : 10;
      totalBytes_ += iov_[i].iov_len;
    }
  char* totalBuffer;
  // allocate all iov_bases as one big chunk
  ACE_NEW (totalBuffer,
           char[totalBytes_]);
  for (i = 0; i < iovcnt_; ++i)
    {
      iov_[i].iov_base = totalBuffer;
      totalBuffer += iov_[i].iov_len;
      for (u_long j = 0; j < static_cast<u_long>(iov_[i].iov_len); ++j)
        {
          char *charbase = static_cast<char *>(iov_[i].iov_base);
          charbase[j] = expChar;
          expChar = ::nextChar(expChar);
        }
    }
}

IovecGuard::~IovecGuard()
{
  // iov_bases are all just part of one big buffer
  char* totalBuffer = static_cast<char *>(iov_[0].iov_base);
  delete [] totalBuffer;
  delete [] iov_;
}

char*
IovecGuard::getBufferAtOffset(const ssize_t offset)
{
  char * totalBuffer = static_cast<char *>(iov_[0].iov_base);
  return totalBuffer + offset;
}

Svc_Handler::Svc_Handler (ACE_Thread_Manager *)
: DEFAULT_TIME_VALUE (ACE_DEFAULT_TIMEOUT)
{
}

int
Svc_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) opening Svc_Handler %@ with handle %d\n"),
              this,
              this->peer ().get_handle ()));
  // Enable non-blocking I/O.
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) %p\n"),
                       ACE_TEXT ("enable")),
                      -1);
  return 0;
}

void
Svc_Handler::send_data (void)
{
  bool successful = true;
  bool win32_test = false;
  const int testType = processENOBUFS();
  const ACE_TCHAR *send_desc = ACE_TEXT ("");
  ssize_t result = 0;
  if (testType == 0)
    {
      buffer_len tryThreshold = 0x7fff;
      ssize_t thresholdActualSend = -1;
      const ssize_t MAX =
        static_cast<ssize_t>(
          (static_cast<unsigned long long>(1) <<
            (static_cast<unsigned long long>(sizeof(ssize_t) * 8) -
             static_cast<unsigned long long>(1))) - 1);
      const unsigned int startShift = 4;
      unsigned int shift = startShift;
      unsigned int trailingMask = 0xffff;
      while (static_cast<ssize_t>(tryThreshold) < MAX)
        {
          IovecGuard all(1, 0, tryThreshold);
          thresholdActualSend =
            this->send(all, ACE_TEXT ("identifying threshold"), true, true);
          if (thresholdActualSend <= static_cast<ssize_t>(tryThreshold)/2 + 1)
            if (shift <= 1)
              break;
            else
              {
                // try and identify the threshold more closely
                shift /= 2;
                tryThreshold = tryThreshold >> shift;
                continue;
              }
          else if ((shift < startShift) && (shift > 1))
            // already narrowing in on value
            shift /= 2;

          tryThreshold = (tryThreshold << shift) | trailingMask;
          ACE_OS::sleep(0);
        }

#if defined (ACE_WIN32)
      win32_test = true;
      // This test only applies to win32 platforms, on systems with
      // sane sendv impls, this is not a problem.
      if (thresholdActualSend != static_cast<ssize_t>(tryThreshold)/2 + 1)
        {
          if (static_cast<ssize_t>(tryThreshold) == MAX)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) was not able to identify a point ")
                        ACE_TEXT ("where ACE_OS::sendv does not send a ")
                        ACE_TEXT ("complete buffer so the Bug #3943 ENOBUFS ")
                        ACE_TEXT ("condition does not occur on this ")
                        ACE_TEXT ("platform.\n")));
          else
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) was not able to identify a point ")
                        ACE_TEXT ("where ACE_OS::sendv sent a partial buffer ")
                        ACE_TEXT ("that was consistent with Bug #3943 ")
                        ACE_TEXT ("ENOBUFS condition logic, so this test ")
                        ACE_TEXT ("probably running into other socket ")
                        ACE_TEXT ("limitations and needs to be redesigned. ")
                        ACE_TEXT ("Stuck sending %d.\n"),
                        thresholdActualSend));
          close ();
          return;
        }
#endif /* ACE_WIN32 */

      buffer_len overThreshold = tryThreshold;
      if (ACE::debug())
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) identified a buffer with %d bytes ")
                    ACE_TEXT ("hits the ENOBUFS condition.\n"),
                    overThreshold));

#if !defined (ACE_WIN32) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))

      {
        buffer_len underThreshold = (overThreshold + 1) / 2;
        // verify that if the total buffer is too large that partial is sent
        IovecGuard all(2, IovecGuard::ALL_SLOTS, underThreshold);
        send_desc = ACE_TEXT ("2 iovecs combined to be too large");
        result = this->send(all, send_desc, true);
        if (win32_test && static_cast<u_long>(result) != underThreshold)
          {
            successful = false;
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) logic should have sent the ")
                        ACE_TEXT ("complete first iovec, ")
                        ACE_TEXT ("expected %d got %d out of %d\n"),
                        underThreshold, result,all.totalBytes_ ));
          }
      }

      {
        IovecGuard all(2, IovecGuard::ALL_SLOTS, overThreshold);
        send_desc = ACE_TEXT ("2 iovecs each are too large");
        result = this->send(all, send_desc, true);
        if (!(result > 0))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %s: expected > 0, got %d\n"),
                        send_desc, result));
            successful = false;
          }
      }

      {
        IovecGuard small_iov(2, 0, overThreshold);
        send_desc = ACE_TEXT ("large iovec followed by small iovec");
        result = this->send(small_iov, send_desc, true);
        if (!(result > 0))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %s: expected > 0, got %d\n"),
                        send_desc, result));
            successful = false;
          }
      }

      {
        IovecGuard large(4, 2, overThreshold);
        send_desc = ACE_TEXT ("4 iovecs with third large");
        result = this->send(large, send_desc, true);
        if (!(result > 0))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %s: expected > 0, got %d\n"),
                        send_desc, result));
            successful = false;
          }
      }

      {
        // verify that the buffer gets divided till it can send
        IovecGuard large(6, 5, 2 * overThreshold);
        send_desc = ACE_TEXT ("6 iovecs with last very large");
        result = this->send(large, send_desc, true);
        if (!(result > 0))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %s: expected > 0, got %d\n"),
                        send_desc, result));
            successful = false;
          }

      }

      {
        IovecGuard array(1, 0, overThreshold);
        send_desc = ACE_TEXT ("just one large iovec in array");
        result = this->send(array, send_desc, true);
        if (!(result > 0))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %s: expected > 0, got %d\n"),
                        send_desc, result));
            successful = false;
          }
      }

      {
        IovecGuard array(1, 0, 2 * overThreshold);
        send_desc = ACE_TEXT ("just one very large iovec in array");
        result = this->send(array, send_desc, true);
        if (!(result > 0))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %s: expected > 0, got %d\n"),
                        send_desc, result));
            successful = false;
          }
      }

#endif /* ACE_HAS_WINSOCK2 && (ACE_HAS_WINSOCK2 != 0) */
#if !defined (ACE_LACKS_SEND)

      {
        IovecGuard one(1, 0, overThreshold);
        send_desc = ACE_TEXT ("large");
        result = this->send(one, send_desc, false);
        if (!(result > 0))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %s: expected > 0, got %d\n"),
                        send_desc, result));
            successful = false;
          }
      }

      {
        IovecGuard one(1, 0, 2 * overThreshold);
        send_desc = ACE_TEXT ("very large");
        result = send(one, send_desc, false);
        if (!(result > 0))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %s: expected > 0, got %d\n"),
                        send_desc, result));
            successful = false;
          }
      }

#endif /* !ACE_LACKS_SEND */
    }
  else if (testType > 0)
    {
      // since ENOBUFS condition is expected to not occur on this platform,
      // send a very large message and verify that ACE_OS::sendv and send
      // are able to send the whole buffer in one call
#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)

      {
        IovecGuard small_iov(2, 0, 0x0fffffff);
        send_desc =  ACE_TEXT ("large iovec followed by small iovec");
        result = this->send(small_iov, send_desc, true, true);
        if (result < 0 || static_cast<u_long>(result) != small_iov.totalBytes_)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %s: expected %d, got %d\n"),
                        send_desc, small_iov.totalBytes_, result));
            successful = false;
          }
      }

      {
        IovecGuard array(1, 0, 0x0fffffff);
        send_desc = ACE_TEXT ("just one large iovec in array");
        result = this->send(array, send_desc, true, true);
        if (result < 0 || (static_cast<u_long>(result) != array.totalBytes_))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %s: expected %d, got %d\n"),
                        send_desc, array.totalBytes_, result));
            successful = false;
          }
      }

#endif /* ACE_HAS_WINSOCK2 && (ACE_HAS_WINSOCK2 != 0) */
#if !defined (ACE_LACKS_SEND)

      {
        IovecGuard one(1, 0, 0x0fffffff);
        send_desc = ACE_TEXT ("large");
        result = this->send(one, send_desc, false, true);
        if (result < 0 || static_cast<u_long>(result) != one.totalBytes_)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %s: expected %d, got %d\n"),
                        send_desc, one.totalBytes_, result));
            successful = false;
          }
      }

#endif /* !ACE_LACKS_SEND */
    }
  else // testType < 0
    // the determination of testType failed, ERROR is already reported
    successful = false;

  // need to indicate that the message is restarting
  // this may fail if the server reads the char and closes before
  // it is done, so let the server report the error if there was one
  send_desc = ACE_TEXT ("indicating no more messages");
  this->send(FINISHED_CHAR, send_desc);

  this->wait_for_completion(READ);
  if (close () == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("close")));
      return;
    }
  client_complete = successful;
}

ssize_t
Svc_Handler::send (IovecGuard& iovec_array,
                   const ACE_TCHAR * const send_desc,
                   const bool use_sendv,
                   const bool test_message)
{
  ++expected_num_messages;
  const ACE_TCHAR* const send_func_name =
    (use_sendv) ? ACE_TEXT ("ACE_OS::sendv") : ACE_TEXT ("ACE_OS::send");
  if (ACE::debug())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) send, using %s for %s (%d bytes)\n"),
                send_func_name, send_desc, iovec_array.totalBytes_));

  if (!use_sendv && (iovec_array.iovcnt_ != 1))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) send, this function is not designed to ")
                  ACE_TEXT ("send an array of iovecs as individuals, %s\n"),
                  send_desc));
      return -1;
    }

  if (expected_num_messages > 1)
    // need to indicate that the message is restarting
    if (this->send(RESTART_CHAR, send_desc) < 1)
      return -1;

  ssize_t actual_send_status;

  if (use_sendv)
    while (((actual_send_status =
             this->peer ().sendv (iovec_array.iov_,
                                  iovec_array.iovcnt_,
                                  &DEFAULT_TIME_VALUE)) == -1) &&
           (errno == EWOULDBLOCK))
      {
      }
  else
    while (((actual_send_status =
             this->peer ().send (iovec_array.iov_->iov_base,
                                 iovec_array.iov_->iov_len,
                                 &DEFAULT_TIME_VALUE)) == -1) &&
           (errno == EWOULDBLOCK))
      {
      }

  if (actual_send_status == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p, %s no data sent\n"),
                  send_func_name, send_desc));
      return -1;
    }
  if (actual_send_status == -1)
    {
      if (errno == ENOBUFS)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p, failed regression test for %s\n"),
                    send_func_name, send_desc));
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p, %s send returned errno=%d\n"),
                    send_func_name, send_desc, errno));
      return -1;
    }
  buffer_len sent_bytes = static_cast<buffer_len>(actual_send_status);
  if (sent_bytes >= iovec_array.totalBytes_)
    {
#if defined (ACE_WIN32)
      if (!test_message)
        {
          // the particular call to send was designed poorly and is not
          // hitting the ENOBUFS condition
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) expected %s to hit an ENOBUFS ")
                      ACE_TEXT ("condition and divide the buffer in half, ")
                      ACE_TEXT ("till a partial buffer is finally sent, ")
                      ACE_TEXT ("but the whole buffer was sent, so either ")
                      ACE_TEXT ("the call to Svc_Handler::send was designed ")
                      ACE_TEXT ("poorly, or the ENOBUFS condition doesn't ")
                      ACE_TEXT ("occur on this platform.")
                      ACE_TEXT (" See call to beforeVersion.\n"),
                      send_func_name));
          return -1;
        }
      else
        return sent_bytes;
#else
      ACE_UNUSED_ARG (test_message);
      return sent_bytes;
#endif /* ACE_WIN32 */
    }

#if defined (ACE_win32)
  // the test here only matters for windows, on other platforms there is
  // no issue, so we skip this test

  // the algorithm subtracts half of the whole, so we round up
  u_long expectedBytes =
    (iovec_array.totalBytes_ % 2) + (iovec_array.totalBytes_ / 2);
  for ( ; sent_bytes < expectedBytes;
        expectedBytes = (expectedBytes % 2) + (expectedBytes / 2))
    {
    }
  if (sent_bytes != expectedBytes)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p, bytes sent are not consistent ")
                  ACE_TEXT ("with the sendv logic, expected %d, got %d\n"),
                  send_func_name, expectedBytes, sent_bytes));
      return -1;
    }
#endif /* ACE_WIN32 */

  buffer_len send_remainder =
    iovec_array.totalBytes_ - static_cast<buffer_len>(actual_send_status);
  char* offset = iovec_array.getBufferAtOffset(actual_send_status);
  ssize_t send_status;
  while (send_remainder > 0)
    {
      const ssize_t sendSize =
        (send_remainder < 10000) ? send_remainder : 10000;
      ACE_OS::sleep(0);
      send_status = this->peer ().send (offset, sendSize, &DEFAULT_TIME_VALUE);
      if (send_status == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %p, %s remainder send no data sent\n"),
                      send_func_name));
          return -1;
        }
      if (send_status == -1)
        {
          if (errno == EWOULDBLOCK)
            continue;

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %p, %s remainder send returned ")
                      ACE_TEXT ("errno = %d\n"),
                      send_func_name, send_desc, errno));
          return -1;
        }
      send_remainder -= send_status;
      offset += send_status;
    }

  return actual_send_status;
}

ssize_t
Svc_Handler::send (char send_char, const ACE_TCHAR * const send_desc)
{
  ssize_t send_status;
  while ((send_status =
          this->peer ().send (&send_char, 1, &DEFAULT_TIME_VALUE)) < 1)
    {
      if (send_status == -1)
        {
          if (errno == EWOULDBLOCK)
            continue;

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %p, %s sending character ")
                      ACE_TEXT ("%c returned errno=%d\n"),
                      ACE_TEXT ("send"), send_desc, send_char, errno));
          return -1;
        }
      if (send_status == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) %p, socket closed prematurely while ")
                      ACE_TEXT ("%s sending character %c\n"),
                      ACE_TEXT ("send"), send_desc, send_char));
          return -1;
        }
    }
  return send_status;
}

void
Svc_Handler::recv_data (void)
{
  ACE_SOCK_Stream &new_stream = this->peer ();

  // Read data from client (terminate on error).
  ACE_UINT64 total_bytes = 0;
  ACE_UINT64 total_bytes_since_last_message = 0;
  const int BUFFER_SIZE = 10000;
  char buffer[BUFFER_SIZE+1];
  ssize_t r_bytes = 0;
  bool badData = false;
  const int EXPECTED_BUFFER_SIZE = BUFFER_SIZE + 9;
  char expectedBuffer[EXPECTED_BUFFER_SIZE];
  char expChar = START_CHAR;
  int i = 0;
  for ( ; i < EXPECTED_BUFFER_SIZE; ++i)
    {
      expectedBuffer[i] = expChar;
      expChar = ::nextChar(expChar);
    }
  expChar = START_CHAR;
  int messages = 0;

  for (;;)
    {
      if (!this->wait_for_completion(READ))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n"),
                    ACE_TEXT ("select")));
      else
        {
          for ( ;
                ((r_bytes = new_stream.recv(&buffer[0], BUFFER_SIZE)) > 0);
                total_bytes += r_bytes)
            {
              bool finished = false;
              const char* const actualBufferEnd = buffer + r_bytes;
              *(buffer + r_bytes) = 0;
              const char* partOfBufferEnd = actualBufferEnd;
              const char* partOfBufferStart = buffer;
              if (*(actualBufferEnd - 1) == FINISHED_CHAR)
                {
                  finished = true;
                  ++messages;
                  if (ACE::debug())
                    ACE_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("(%P|%t) identified %d messages ")
                                ACE_TEXT ("and it is finished.\n"),
                                messages));
                }
              // loop through in case there is more than one message represented
              while (partOfBufferStart < partOfBufferEnd)
                {
                  const char* restartLoc =
                    ACE_OS::strchr(partOfBufferStart, RESTART_CHAR);
                  if ((restartLoc > 0) && (restartLoc < partOfBufferEnd))
                    {
                      ++messages;
                      total_bytes_since_last_message = 0;
                      partOfBufferEnd = restartLoc;
                      if (ACE::debug())
                        ACE_DEBUG ((LM_DEBUG,
                                    ACE_TEXT ("(%P|%t) identified %d ")
                                    ACE_TEXT ("messages.\n"),
                                    messages));
                    }
                  else if (finished)
                    --partOfBufferEnd;
                  else
                    total_bytes_since_last_message +=
                      partOfBufferEnd - partOfBufferStart;

                  if (ACE_OS::memcmp(partOfBufferStart,
                                     &(expectedBuffer[expChar - START_CHAR]),
                                     partOfBufferEnd - partOfBufferStart) != 0)
                    {
                      badData = true;
                    }
                  const char lastCharOfBuffer =
                    *((partOfBufferEnd < actualBufferEnd) ?
                      partOfBufferEnd : partOfBufferEnd - 1);
                  expChar = ::nextChar(lastCharOfBuffer);
                  // see if there is more data in the buffer
                  partOfBufferStart = partOfBufferEnd + 1;
                  partOfBufferEnd = actualBufferEnd;
                }
              // if FINISHED_CHAR was found
              if (finished)
                {
                  if (close (1) == -1)
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("(%P|%t) %p\n"),
                                ACE_TEXT ("close")));
                  else if (badData)
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("(%P|%t) received final char, ")
                                ACE_TEXT ("but did not receive all data\n")));
                  else if (messages != expected_num_messages)
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("(%P|%t) received final char, ")
                                ACE_TEXT ("but expected %d messages ")
                                ACE_TEXT ("and got %d\n"),
                                expected_num_messages,
                                messages));
                  else
                    server_complete = true;

                  return;
                }
            }

          if (r_bytes == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) %p, socket closed prematurely\n"),
                          ACE_TEXT ("recv")));
              break;
            }
          else if (r_bytes < 0)
            {
              if (errno != EWOULDBLOCK)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("(%P|%t) %p, received %d messages and ")
                              ACE_TEXT ("%Q bytes and %Q bytes since the ")
                              ACE_TEXT ("last message\n"),
                              ACE_TEXT ("recv"), messages,
                              total_bytes, total_bytes_since_last_message));
                  break;
                }
            }
        }
    }
}

bool
Svc_Handler::wait_for_completion(Direction direction)
{
  ACE_SOCK_Stream &new_stream = this->peer ();

  ACE_Handle_Set handle_set;
  handle_set.set_bit (new_stream.get_handle ());

  // Since we're in non-blocking mode we need to use <select> to
  // avoid busy waiting.
#if defined (ACE_WIN64)
  int select_width = 0;
#else
  int select_width = int (new_stream.get_handle ()) + 1;
#endif /* ACE_WIN64 */

  int result =
   (direction == READ) ?
    ACE_OS::select (select_width, handle_set, 0, 0, &DEFAULT_TIME_VALUE) :
    ACE_OS::select (select_width, 0, handle_set, 0, &DEFAULT_TIME_VALUE);
  return result != -1;
}

int
Svc_Handler::close (u_long side)
{
  // Only run this protocol if we're the write-side (i.e., "1").
  if (side == 1 && this->peer ().close () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("close_writer")));
  // Trigger the shutdown.
  return this->handle_close ();
}

#if defined (ACE_HAS_THREADS)

static void *
client (void *arg)
{
  ACE_INET_Addr *remote_addr = reinterpret_cast<ACE_INET_Addr *> (arg);
  ACE_INET_Addr server_addr (remote_addr->get_port_number (),
                             ACE_DEFAULT_SERVER_HOST);
  CONNECTOR connector;

  Svc_Handler *svc_handler = 0;
  // Run the blocking test.
  ACE_NEW_RETURN (svc_handler,
                  Svc_Handler,
                  0);

  // Perform a blocking connect to the server.
  if (connector.connect (svc_handler,
                         server_addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n"),
                ACE_TEXT ("connection failed")));
  else
    {
      // Send the data to the server.
      svc_handler->send_data ();

    }
  return 0;
}

// Performs the iterative server activities.

static void *
server (void *arg)
{
  ACCEPTOR *acceptor = (ACCEPTOR *) arg;
  ACE_INET_Addr cli_addr;
  ACE_TCHAR peer_host[MAXHOSTNAMELEN];
  const ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);
  ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT, tv);

  Svc_Handler *svc_handler = 0;
  ACE_NEW_RETURN (svc_handler,
                  Svc_Handler,
                  0);

  // Keep looping until we timeout on <accept> or fail.

  for (;;)
    {
      // Create a new <Svc_Handler> to consume the data.

      int result = acceptor->accept (svc_handler,
                                     &cli_addr,
                                     options);

      // Timing out is the only way for threads to stop accepting
      // since we don't have signals.

      if (result == -1)
        {
          // svc_handler->close (); The ACE_Onsehot_Acceptor closed it.

          if (errno == ETIMEDOUT)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("accept timed out\n")));
              return 0;
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%P|%t) %p\n"),
                               ACE_TEXT ("accept failed, shutting down")),
                              0);
        }
      // Use this rather than get_host_name() to properly adjust to the
      // charset width in use.
      cli_addr.get_host_name (peer_host, MAXHOSTNAMELEN);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) client %s connected from %d\n"),
                  peer_host,
                  cli_addr.get_port_number ()));

      svc_handler->recv_data ();
      break;
    }

  return 0;
}

// Spawn threads and run the client and server.

static
int
spawn_threads (ACCEPTOR *acceptor,
               ACE_INET_Addr *server_addr)
{
  int status = 0;

  if (ACE_Thread_Manager::instance ()->spawn_n
      (
       1,
       (ACE_THR_FUNC) server,
       (void *) acceptor,
       THR_NEW_LWP
       , ACE_DEFAULT_THREAD_PRIORITY
       , -1
       , 0
       ) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n%a"),
                ACE_TEXT ("server thread create failed"),
                1));

  if (ACE_Thread_Manager::instance ()->spawn
      ((ACE_THR_FUNC) client,
       (void *) server_addr,
       THR_NEW_LWP,
       0
       ) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) %p\n%a"),
                ACE_TEXT ("client thread create failed"),
                1));

  // Wait for the threads to exit.
  // But, wait for a limited time because sometimes the test hangs on Irix.
  ACE_Time_Value const max_wait (400 /* seconds */);
  ACE_Time_Value const wait_time (ACE_OS::gettimeofday () + max_wait);
  if (ACE_Thread_Manager::instance ()->wait (&wait_time) == -1)
    {
      if (errno == ETIME)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("maximum wait time of %d msec exceeded\n"),
                               max_wait.msec ()));
      else
        ACE_OS::perror (ACE_TEXT ("ACE_Thread_Manager::wait"));

      status = -1;
    }

  return status;
}
#endif /* ACE_HAS_THREADS */
//#endif /* ACE_WIN32 */

int
run_main (int , ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3943_Regression_Test"));
  int status = 0;
#if !defined (ACE_WIN32) || ((defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)) || !defined (ACE_LACKS_SEND))
  // Acceptor
  ACCEPTOR acceptor;
  ACE_INET_Addr server_addr;

  // Bind acceptor to any port and then find out what the port was.
  if (acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &)) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("open")));
      ACE_ASSERT (0);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) starting server at port %d\n"),
                  server_addr.get_port_number ()));

#if defined (ACE_HAS_THREADS)
      if (spawn_threads (&acceptor, &server_addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) %p\n"),
                           ACE_TEXT ("spawn_threads")),
                          1);
#else  /* !ACE_HAS_THREADS */
      ACE_ERROR ((LM_INFO,
                  ACE_TEXT ("(%P|%t) ")
                  ACE_TEXT ("only one thread may be run")
                  ACE_TEXT (" in a process on this platform")));
#endif /* ACE_HAS_THREADS */
    }

  if (!client_complete || !server_complete)
    status = 1;

#endif /* ACE_HAS_WINSOCK2 && (ACE_HAS_WINSOCK2 != 0)) || !ACE_LACKS_SEND */

  ACE_END_TEST;
  return status;
}
