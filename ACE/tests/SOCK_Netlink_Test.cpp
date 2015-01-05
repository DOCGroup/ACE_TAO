
//=============================================================================
/**
 *  @file    SOCK_Netlink_Test.cpp
 *
 *  $Id$
 *
 *  Tests adding of a secondary IP-address, using linux netlink
 *   sockets.
 *
 *
 *  @author Robert Iakobashvili
 *  @author coroberti@gmail.com
 */
//=============================================================================


#include "test_config.h"

#ifdef ACE_HAS_NETLINK

#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

#include "ace/Netlink_Addr.h"
#include "ace/SOCK_Netlink.h"

#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_time.h"

#include <linux/rtnetlink.h>


/**
 * NETLINK SOCKET INTERFACE is a socket API communication
 * between linux kernel and userland.
 *
 * Main usage of netlink communication is for communication between
 * kernel/custom modules/drivers and userspace;
 *
 * In order not to force ACE-tests runners to install some driver for testing
 * purposes, we are using here a build-in netlink interface used by linux for
 * "routing" purposes (rtnetlink).
 *
 * This test is hopefully a useful example of how via netlink to add a new
 * secondary IPv4 address to an interface and to delete it further. The
 * example is integrated within reactive framework for IO demultiplexing.
 *
 * A one button test adds a new secondary IP-address to a loopback
 * interface and deletes it. Even if the address added remains, this shall no
 * cause any damage. In any case the lifetime of a secondary ip is limited
 * till the next reboot. You may wish to run the test with -d option (not to
 * make an address cleanup after addtion) and see it by 'ip addr' command.
 * Further re-run of the test without -d option will cleanup the address.
 *
 * Please, note, that it is ok that the message "rtnetlink error message:
 * Cannot assign requested address" will appear on your console, because first
 * the test is trying to remove the secondary-ip and errors, if it was not before.
 *
 * The one-button test fails, of there is no a loopback device named "lo" on a
 * host or the device is disabled.
 *
 * The same rtnetlink interface may be used to get/add/delete ip-addresses,
 * get/add/delete routing rules, control ARP entires, control Qdisk disciplines,
 * traffic classes and traffic filters, manage network interface configuration
 *
 * For more information, please, read man pages:
 * netlink (3), netlink (7), rtnetlink (3), rtnetlink (3), rtnetlink (7) and ip (8).
 *
 * Some ideas for the test were borrowed from the code of iprouted2
 * written by Alexey Kuznetsov.
 *
 * Command line options:
 *
 * -d    do not cleanup the ip-address after addition (so that you can see it by the
 *         'ip addr' command)
 *
 * -i      the name of interface to add the address
 *
 * -a    ipv4 slash netmask bits, like "192.168.1.1/24"
 *

 From Linux headers:

 // Generic structure for encapsulation of optional route information.
 // It is reminiscent of sockaddr, but with sa_family replaced with attribute type.
 struct rtattr
 {
 unsigned short  rta_len;
 unsigned short  rta_type;
 };

 //Interface address.
 struct ifaddrmsg
 {
 unsigned char   ifa_family;
 unsigned char   ifa_prefixlen;  // The prefix length is the length of the address mask
 unsigned char   ifa_flags;        // Flags: IFA_F_SECONDARY for secondary
 // address (old alias interface), IFA_F_PERMANENT
 // for a  permanent address
 unsigned char   ifa_scope;      // locality
 int                      ifa_index;       // Link index is the interface index in the table of interfaces.
 };

 struct nlmsghdr
 {
 __u32           nlmsg_len;       // Length of message including header
 __u16           nlmsg_type;    // Message content
 __u16           nlmsg_flags;   // Additional flags
 __u32           nlmsg_seq;     // Sequence number
 __u32           nlmsg_pid;      // Sending process PID
 };
*/


// The global config params
//
static int one_button_test = 0;
static char ip_slash_mask[32];
static char net_dev_name[16];
static int dont_cleanup_added_ip = 0;

// The function returns index of an interface by its name
//
int
get_if_index (const char*const interface,
              int &if_index)
{
  if_index = -1;

  struct ifreq if_req;
  ACE_OS::memset (&if_req, 0, sizeof (struct ifreq));

  ACE_OS::strncpy (if_req.ifr_name,
                   static_cast<const char*> (interface),
                   sizeof (if_req.ifr_name));

  ACE_HANDLE s = ACE_OS::socket (AF_INET,SOCK_DGRAM,0);

  if (s == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("get_if_index - failed on\n")
                       ACE_TEXT ("ACE_OS::socket")),
                      -1);

  int result = ACE_OS::ioctl (s,
                              SIOCGIFINDEX,
                              reinterpret_cast<char*> (&if_req));

  if (result == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("get_if_index:")
                  ACE_TEXT ("ioctl (get interface index)")));
    }
  else
    {
      if_index = if_req.ifr_ifindex;
    }
  return result;
}

/**
   An assisting structure for passing data via routing netlink socket.
   The idea borrowed from a great iprouted2 utility,
   written by Alexey Kuznetsov.
*/
struct Inet_Prefix
{
  u_char family;
  u_char bytelen;
  ACE_INT16 bitlen;
  ACE_UINT32 data[4];
};


/**
 * Contains header netlink message header of a type nlmsghdr with a
 * following request type dependent controlling structure, which is for
 * adding/deleting IP-addresses is of type ifaddrmsg with a following
 * buffer, containing the data: ip-address, its length, number of netmask
 * bits, etc.
 */
struct Netlink_Request
{
  struct nlmsghdr     nhdr_; // message header
  struct ifaddrmsg ifa_; // interface
  char            buf_[256];
};



/**
 * The handler first is trying to delete an ip-address, further
 * to add the ip and, if successful to cleanup the address.
 */
class Secondary_Ipaddr_Handler : public ACE_Event_Handler
{
public:

  // Default constructor
  Secondary_Ipaddr_Handler (void);

  // Destructor
  virtual ~Secondary_Ipaddr_Handler (void);

  //FUZZ: disable check_for_lack_ACE_OS
  // Initialization. Schedules a timer to run start the business.
  //
  ///FUZZ: enable check_for_lack_ACE_OS
  int open (ACE_Reactor *const reactor,
            char* const ip_slash_mask,
            const char *const if_name);

  // Returns reference to netlink socket. Necessary for reactor.
  virtual ACE_HANDLE get_handle (void) const;

  /**
   * Takes care of the input. Reads the incoming messages,
   * makes their processing.
   */
  virtual int handle_input (ACE_HANDLE handle);

  // Makes clean-up
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

  // Runs a state machine. Controls adding/deleting of ip-address.
  int handle_timeout (ACE_Time_Value const & tv,
                      void const * arg = 0);

  // Sends to kernel a request to add secondary ip/mask to an
  // interface.
  int add_ip (char* const ip_slash_mask,
              const char *const if_name);

  // Sends to kernel a request to delete secondary ip/mask
  // from an interface.
  int delete_ip (char* const ip_slash_mask,
                 const char *const if_name);

  /*
   * 1. We are trying to delete the ip-address, if exists.
   *      Shall be either successful, or fail, when no-ip yet.
   * 2. Adding the ip-address, shall be successful;
   * 3. Cleaning up the ip-address. Shall be successful as well.
   */
  enum
    {
      START = 0,
      IP_DELETED,
      IP_ADDED,
      IP_CLEANUPED,
      SUCCESS,
      FAILED
    };

  // Returns the currect state
  int get_state () const { return this->state_;}

protected:

  //FUZZ: disable check_for_lack_ACE_OS
  // De-registers the handler from the reactor,
  // other cleanup jobs
  virtual int close ();

  ///FUZZ: enable check_for_lack_ACE_OS
  ACE_SOCK_Netlink& socket ();

private:

  // Schedule two sec timer.
  int schedule_one_sec_timer ();

  // A workhorse for add_ip () and delete_ip ()
  int dispatch_ip_operation (char* const ip_slash_mask,
                             const char *const if_name,
                             bool action);

  /**
   *  Initializes netlink request for adding (action = true) or
   *  deleting (action = false) of a secondary ip-address/mask.
   */
  int init_netlink_request (char* const ip_slash_mask,
                            const char *const if_name,
                            Netlink_Request& net_req,
                            bool action);

  // Fills data part of Netlink_Request
  int fill_inet_prefix (Inet_Prefix &inet_prefix,
                        const char*const ip_slash_netmask);

  /**
   * Fills routing request (operations with ip-addresses are
   * a part of netlink routing interface).
   */
  void fill_rtnetlink_request (nlmsghdr &hdr,
                               int type,
                               void *data,
                               int data_length);

  enum
    {
      COMMAND_TIMEOUT = -3,
      COMMAND_RECV_ERROR = -2,
      COMMAND_ERROR = -1,
      COMMAND_SUCCESS = 0
    };

  // Mark command status in handle_input ().
  void on_recv_error () { this->command_status_ = COMMAND_RECV_ERROR; }
  void on_command_success () { this->command_status_ = COMMAND_SUCCESS; }
  void on_command_error () { this->command_status_ = COMMAND_ERROR; }

  // The socket.
  ACE_SOCK_Netlink socket_ ;

  // The address of the socket.
  ACE_Netlink_Addr  address_ ;

  // Message sequence number.
  ACE_UINT32 seq_ ;

  // The request structure passed to kernel.
  Netlink_Request netlink_request_;

  // ip-addr-slash-mask
  char ip_buff_[32];

  // interface
  char if_buff_[16];

  // Buffer to receive messages. Not too large?
  char recv_buff_[1024];

  // The state of the handler.
  int state_;

  // The status of the command.
  int command_status_;
};

Secondary_Ipaddr_Handler::Secondary_Ipaddr_Handler ()
  :
  socket_ (),
  address_ (),
  seq_ (0),
  state_ (START),
  command_status_ (COMMAND_SUCCESS)
{
}

Secondary_Ipaddr_Handler::~Secondary_Ipaddr_Handler ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Secondary_Ipaddr_Handler::~Secondary_Ipaddr_Handler\n")));
  this->close ();
}
int
Secondary_Ipaddr_Handler::open (ACE_Reactor *const reactor,
                                char* const ip_slash_mask,
                                const char *const if_name)
{
  if (!reactor ||
      !ip_slash_mask || !ACE_OS::strlen (ip_slash_mask) ||
      !if_name || !ACE_OS::strlen (if_name))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P) Secondary_Ipaddr_Handler::open: error ")
                       ACE_TEXT("zero pointers or zero length strings used as input.\n")),
                      -1);

  this->reactor (reactor);

  // Another option is to pass a zero pid to the address, to call open () on socket
  // performing binding and after bind () to call getsockbyname to fill the address

  this->address_.set (ACE_OS::getpid (), 0);

  if (this->socket ().open (this->address_,
                            ACE_PROTOCOL_FAMILY_NETLINK,
                            NETLINK_ROUTE) == -1)
    //FUZZ: disable check_for_lack_ACE_OS
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P|%t) Secondary_Ipaddr_Handler::open: - failed\n")
                       ACE_TEXT("to initialize netlink socket bu open ().\n")),
                      -1);
    //FUZZ: enable check_for_lack_ACE_OS

  // register with the reactor for input
  if (this->reactor ()->register_handler (this,
                                          ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P|%t) Secondary_Ipaddr_Handler::open - ")
                       ACE_TEXT("can't register with reactor for handling input.\n")),
                      -1);

  if (this->reactor ()->schedule_timer (this,
                                        0,
                                        ACE_Time_Value::zero) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P) Secondary_Ipaddr_Handler::open - ")
                       ACE_TEXT("can't schedule timer with reactor.\n")),
                      -1);

  this->seq_ = ACE_OS::time (0);

  ACE_OS::strncpy (this->ip_buff_,
                   ip_slash_mask,
                   sizeof this->ip_buff_);

  ACE_OS::strncpy (this->if_buff_,
                   if_name,
                   sizeof this->if_buff_);

  return 0;
}

ACE_HANDLE
Secondary_Ipaddr_Handler::get_handle (void) const
{
  return this->socket_.get_handle ();
}

int
Secondary_Ipaddr_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_input -  entered\n")));

  nlmsghdr *hdr = 0;
  iovec iov;

  iov.iov_base = this->recv_buff_;
  iov.iov_len = sizeof (this->recv_buff_);

  int rval_bytes = -1;
  ACE_Netlink_Addr raddr;
  raddr.set (0, 0);

  rval_bytes = this->socket ().recv (&iov, 1, raddr);
  switch (rval_bytes)
    {
    case -1: // Complain and leave
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_input - ")
                         ACE_TEXT("%p bad read\n"), ACE_TEXT("client")),
                        -1);

    case 0: // Complain and leave
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_input - "),
                         ACE_TEXT("eof, closing daemon (fd = %d)\n"),
                         this->get_handle ()),
                        -1);

    default:
      if (raddr.get_size () != sizeof (sockaddr_nl))
        {
          this->on_recv_error ();
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("(%n %P) Secondary_Ipaddr_Handler::handle_input - ")
                             ACE_TEXT("address length not equal sockaddr_nl\n")),
                            -1);
        }

      hdr =  reinterpret_cast <nlmsghdr*> (this->recv_buff_);

      if (static_cast <int> (hdr->nlmsg_len) != rval_bytes)
        {
          this->on_recv_error ();
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_input - ")
                             ACE_TEXT("size of nlmsg_len not equal received bytes\n")),
                            -1);
        }

      if (static_cast <int> (hdr->nlmsg_pid) != this->address_.get_pid () || hdr->nlmsg_seq != this->seq_)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_input - ")
                           ACE_TEXT("process id or message sequence is different\n")),
                          -1);

      if (hdr->nlmsg_type == NLMSG_ERROR)
        {
          struct nlmsgerr *err = static_cast <struct nlmsgerr*> (NLMSG_DATA(hdr));

          errno = -err->error;
          if (errno == 0)
            {
              this->on_command_success ();
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_input -  command success\n")));
              return 0;
            }

          this->on_command_error ();
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_input -  command informs about error\n")));

          // some error message
          ACE_OS::perror("rtnetlink error message: ");

          return 0;
        }
    }
  return -1;
}

int
Secondary_Ipaddr_Handler::handle_timeout (ACE_Time_Value const &,
                                          void const *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_timeout - entered\n")));

  if (this->command_status_ != COMMAND_SUCCESS &&
      (this->command_status_ != COMMAND_ERROR &&
       this->state_ == IP_DELETED))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_timeout - ")
                         ACE_TEXT("previous command failed\n")),
                        -1);
    }
  else
    this->command_status_ = COMMAND_TIMEOUT; //invalidate command status

  switch (this->state_)
    {
    case START: //delete the ip, if it presents
      if (this->delete_ip (this->ip_buff_, this->if_buff_) == -1)
        {
          this->state_ = FAILED;
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_timeout - ")
                             ACE_TEXT("delete_ip failed\n")),
                            -1);
        }
      break;

    case IP_DELETED: // add the ip
      if (this->add_ip (this->ip_buff_, this->if_buff_) == -1)
        {
          this->state_ = FAILED;
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_timeout - ")
                             ACE_TEXT("add_ip failed\n")),
                            -1);
        }
      break;

    case IP_ADDED: //delete added ip to make cleanup
      if (dont_cleanup_added_ip)
        {
          this->state_ = SUCCESS;
          return 0;
        }
      else
        {
          if (this->delete_ip (this->ip_buff_, this->if_buff_) == -1)
            {
              this->state_ = FAILED;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT("(%P) Secondary_Ipaddr_Handler::handle_timeout - ")
                                 ACE_TEXT("delete_ip failed\n")),
                                -1);
            }
        }
      break;

    case IP_CLEANUPED:
      this->state_ = SUCCESS;
      return 0;

    default:
      return -1;
    }

  this->schedule_one_sec_timer ();
  this->state_++;
  return 0;
}

int
Secondary_Ipaddr_Handler::handle_close (ACE_HANDLE,
                                        ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Secondary_Ipaddr_Handler::handle_close\n")));
  this->close ();
  return 0;
}

int
Secondary_Ipaddr_Handler::close ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P) Secondary_Ipaddr_Handler::close\n")));

  if (this->reactor ())
    {
      this->reactor ()->remove_handler (this,
                                        ACE_Event_Handler::ALL_EVENTS_MASK |
                                        ACE_Event_Handler::DONT_CALL);

      this->reactor ()->cancel_timer (this);

      this->reactor (0);
    }
  return 0;
}

int
Secondary_Ipaddr_Handler::schedule_one_sec_timer ()
{
  const ACE_Time_Value one_sec (1, 0);

  if (this->reactor ()->schedule_timer (this,
                                        0,
                                        one_sec) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P) Secondary_Ipaddr_Handler::schedule_one_sec_timer - ")
                       ACE_TEXT("can't schedule timer with reactor.\n")),
                      -1);
  return 0;
}

ACE_SOCK_Netlink&
Secondary_Ipaddr_Handler::socket ()
{
  return this->socket_;
}

int
Secondary_Ipaddr_Handler::fill_inet_prefix (
                                            Inet_Prefix &inet_prefix,
                                            const char*const ip_slash_netmask)
{
  ACE_OS::memset (&inet_prefix, 0, sizeof inet_prefix);
  char* slash =  const_cast <char *> (ACE_OS::strchr (ip_slash_netmask,  '/'));
  if (slash)
    *slash = '\0'; // The idea from Igor Potulnitsky

  inet_prefix.family = AF_INET; // another option is AF_UNSPEC
  inet_prefix.bytelen = 4;
  inet_prefix.bitlen = -1;

  char ip_buff[32];
  ACE_OS::strncpy (ip_buff, ip_slash_netmask, sizeof (ip_buff));

  char* to_search = ip_buff, *dot = 0;

  for (int i = 0; i < 4; i++)
    {
      if (i < 3)
        {
          dot = ACE_OS::strchr (to_search,  '.');
          if (!dot || !ACE_OS::strlen (to_search))
            return -1;
          else
            *dot = '\0';
        }
      int num = ACE_OS::atoi (to_search);
      if (num < 0 || num > 255)
        return -1;
      else
        ((u_char *) &inet_prefix.data)[i] = (u_char)num;

      if (i < 3)
        to_search = dot + 1;
    }

  inet_prefix.bitlen = 32; // AF_INET: 32

  if (slash)
    {
      int mask_len = 0;
      mask_len = ACE_OS::atoi (slash + 1);
      if (mask_len >= 0)
        inet_prefix.bitlen = mask_len;
      *slash = '/';
    }
  return 0;
}

void
Secondary_Ipaddr_Handler::fill_rtnetlink_request (
                                                  nlmsghdr &hdr,
                                                  int type,
                                                  void *data,
                                                  int data_length)
{
  // points to the end of the aligned header
  struct rtattr *rta = reinterpret_cast <struct rtattr*> (((reinterpret_cast <char*>(&hdr)) + NLMSG_ALIGN (hdr.nlmsg_len)));

  rta->rta_type = type;
  rta->rta_len = RTA_LENGTH (data_length);
  ACE_OS::memcpy (RTA_DATA(rta), data, data_length);
  hdr.nlmsg_len = NLMSG_ALIGN (hdr.nlmsg_len) + RTA_LENGTH (data_length);
}

int
Secondary_Ipaddr_Handler::dispatch_ip_operation (
                                                 char* const ip_slash_mask,
                                                 const char *const if_name,
                                                 bool action)
{
  if (this->init_netlink_request (ip_slash_mask,
                                  if_name,
                                  this->netlink_request_,
                                  action) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Secondary_Ipaddr_Handler::ip_operation - "
                       "init_netlink_request () failed.\n"),
                      -1);

  this->netlink_request_.nhdr_.nlmsg_seq = ++this->seq_;
  this->netlink_request_.nhdr_.nlmsg_flags |= NLM_F_ACK;

  iovec iov_send =
    {
      static_cast <void*> (&this->netlink_request_.nhdr_),
      this->netlink_request_.nhdr_.nlmsg_len
    };

  ACE_Netlink_Addr  addr_send;
  addr_send.set (0, 0);

  if (this->socket ().send (&iov_send,
                            1,
                            addr_send) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("Secondary_Ipaddr_Handler::ip_operation - ")
                       ACE_TEXT("send of request failed with errno %d.\n"),
                       errno),
                      -1);
  return 0;
}

int
Secondary_Ipaddr_Handler::delete_ip (
                                     char* const ip_slash_mask,
                                     const char *const if_name)
{
  if (this->dispatch_ip_operation (
                                   ip_slash_mask,
                                   if_name,
                                   false) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P) Secondary_Ipaddr_Handler::delete_ip - ")
                       ACE_TEXT("dispatch_ip_operation() failed.\n")),
                      -1);
  return 0;
}

int
Secondary_Ipaddr_Handler::add_ip (
                                  char* const ip_slash_mask,
                                  const char *const if_name)
{
  if (this->dispatch_ip_operation (ip_slash_mask,
                                   if_name,
                                   true) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P) Secondary_Ipaddr_Handler::add_ip - ")
                       ACE_TEXT("dispatch_ip_operation() failed.\n")),
                      -1);
  return 0;
}

int
Secondary_Ipaddr_Handler::init_netlink_request (
                                                char* const ip_slash_netmask,
                                                const char *const if_name,
                                                Netlink_Request& net_req,
                                                bool action)
{
  ACE_OS::memset (&net_req, 0, sizeof(net_req));

  // fill the request header
  net_req.nhdr_.nlmsg_len =
    NLMSG_LENGTH (sizeof(struct ifaddrmsg));
  net_req.nhdr_.nlmsg_flags = NLM_F_REQUEST;
  net_req.nhdr_.nlmsg_type = action ? RTM_NEWADDR : RTM_DELADDR;
  net_req.ifa_.ifa_family = AF_INET;

  int interface_index = -1;
  if (get_if_index (if_name,
                    interface_index) == -1 || interface_index < 0)
    {
      ACE_OS::fprintf (stderr, "get_if_index () - failed\n");
      return -1;
    }
  net_req.ifa_.ifa_index = interface_index;

  Inet_Prefix local_prefix;

  if (fill_inet_prefix (local_prefix,
                        ip_slash_netmask) == -1)
    {
      ACE_OS::fprintf (stderr, "fill_inet_prefix () - failed\n");
      return -1;
    }

  fill_rtnetlink_request (net_req.nhdr_,
                          IFA_LOCAL,
                          &local_prefix.data,
                          local_prefix.bytelen);

  net_req.ifa_.ifa_prefixlen = local_prefix.bitlen; // number of bits in netmask
  net_req.ifa_.ifa_scope = 0;

  return 0;
}

static int run_test (char*const ip_slash_netmask,
                     const char*const if_name)
{
  Secondary_Ipaddr_Handler sec_ip_handler;

  if (sec_ip_handler.open (ACE_Reactor::instance (),
                           ip_slash_netmask,
                           if_name)
      == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P) SOCK_Netlink_Test - run_test () failed ")
                       ACE_TEXT("due to sec_ip_handler.open () error.\n")),
                      -1);

  ACE_Time_Value wait_time (4, 0);

  ACE_Reactor::instance()->run_reactor_event_loop (wait_time);

  if (sec_ip_handler.get_state () != Secondary_Ipaddr_Handler::SUCCESS)
    return -1;

  return 0;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc == 1) // one button test
    {
      one_button_test = 1;
      return 0;
    }

  ACE_OS::memset (ip_slash_mask, 0, sizeof ip_slash_mask);
  ACE_OS::memset (net_dev_name, 0, sizeof net_dev_name);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("a:di:"));
  int c = 0, ip_len = 0, if_len = 0;
  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 'a':     // ip_slash_netmask_bits
          ACE_OS::strcpy (ip_slash_mask,
                          reinterpret_cast <const char*> (get_opt.opt_arg ()));

          if (! (ip_len = ACE_OS::strlen (ip_slash_mask)))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT("%s, -a  should be followed by a dotted ipv4 addr slash netmask bits.\n")
                          ACE_TEXT("Example: \"-a 192.168.1.1/24\" .\n"),
                          ACE_TEXT("SOCK_Netlink_Test")));
              return -1;
            }
          break;

        case 'd':
          dont_cleanup_added_ip = 1;
          break;

        case 'i':     // name of the interface
          ACE_OS::strcpy (net_dev_name,
                          reinterpret_cast <const char*> (get_opt.opt_arg ()));
          if (! (if_len = ACE_OS::strlen (net_dev_name)))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT("%s, -i  should be followed by a valid name of network interface.\n"),
                          ACE_TEXT("SOCK_Netlink_Test")));
              return -1;
            }
          break;

        default:
          break;
        }
    }

  if ((ip_len == 0 && if_len) || (ip_len && if_len == 0))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("%s - error: both options -a and -i should be provided.\n"),
                         ACE_TEXT("SOCK_Netlink_Test")),
                        -1);
    }
  else if (ip_len == 0 && if_len == 0)
    {
      one_button_test = 1;
    }

  return 0;
}

#define DEFAULT_IP_SLASH_MASK  "192.168.1.100/24"
#define DEFAULT_NET_DEVICE_NAME "lo"

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("SOCK_Netlink_Test"));

  if (ACE_OS::geteuid ())
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Process has no superuser priveleges. ")
                  ACE_TEXT ("Unable to run this test.\n")));
    }
  else
    {
      if (::parse_args (argc, argv) == -1)
        {
          return -1;
        }

      if (one_button_test)
        {
          ACE_OS::strcpy (ip_slash_mask,
                          DEFAULT_IP_SLASH_MASK);
          ACE_OS::strcpy (net_dev_name,
                          DEFAULT_NET_DEVICE_NAME);
        }

      int rval = -1;
      if ((rval = run_test (ip_slash_mask,
                            net_dev_name)) < 0)
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("run_test() failed with rval returned %d. "),
                      rval));
          return -1;
        }
    }

  ACE_END_TEST;

  return 0;
}

#else /* ACE_HAS_NETLINK*/

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SOCK_Netlink_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT("(%P|%t|%T) \"SOCK_Netlink_Test\" main() - ")
              ACE_TEXT("Linux netlink socket support not configured.\n")
              ACE_TEXT("#define ACE_HAS_NETLINK = 1 in your config.h ")
              ACE_TEXT("file to enable and run the process as a superuser.\n")));

  ACE_END_TEST;

  return 0;
}

#endif /* ACE_HAS_NETLINK*/

