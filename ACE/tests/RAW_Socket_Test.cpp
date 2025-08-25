// ============================================================================
// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program tests the basic APIs supported in
//    <ACE_RAW_Socket>, and demonstrates how to use it.
//
// = AUTHOR
//    Smith Achang <changyunlei@126.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/RAW_Socket.h"
#include "ace/SOCK_Dgram.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"

#pragma pack(push)
#pragma pack(1)
typedef struct _IP_HEADER_t
{
  uint8_t  bVersionAndHeaderLen;
  uint8_t  bTypeOfService;
  uint16_t u16TotalLenOfPacket;
  uint16_t u16PacketID;
  uint16_t u16Sliceinfo;
  uint8_t  bTTL;
  uint8_t  bTypeOfProtocol;
  uint16_t u16CheckSum;
  uint32_t u32SourIp;
  uint32_t u32DestIp;
} IPv4_HEADER_t, *IPv4_HEADER_t_Ptr;

typedef struct _IPv6_HEADER_t
{
  union
  {
    uint8_t abVTF[4];
    uint32_t dwVTF;
  };

  uint16_t u16PayloadLen;
  uint8_t  bNextHeader;
  uint8_t  bHopLimit;

  union
  {
    uint8_t  abSrcAddr[16];
    uint32_t au32SrcAddr[4];
    uint64_t au64SrcAddr[2];
  };

  union
  {
    uint8_t  abDstAddr[16];
    uint32_t au32DstAddr[4];
    uint64_t au64DstAddr[2];
  };
} IPv6_HEADER_t, *IPv6_HEADER_t_Ptr;

typedef struct _UDP_HEADER_t
{
  uint16_t u16SrcPort;
  uint16_t u16DstPort;
  uint16_t u16Length;
  uint16_t u16CheckSum;
} UDP_HEADER_t, *UDP_HEADER_t_Ptr;

#pragma pack(pop)

uint32_t Checksum(uint32_t cksum, uint8_t *pBuffer, uint32_t size)
{
  if ((nullptr == pBuffer) || (0 == size))
  {
    // return passed value
    return cksum;
  }

  uint32_t num = 0;
  uint8_t *p   = pBuffer;

  while (size > 1)
  {
    cksum += (static_cast<uint16_t>(p[num]) << 8 & 0xff00) | (static_cast<uint16_t>(p[num + 1]) & 0x00FF);
    size  -= 2;
    num   += 2;
  }

  if (size > 0)
  {
    cksum += (static_cast<uint16_t>(p[num]) << 8) & 0xFFFF;
  }

  while (cksum >> 16)
  {
    cksum = (cksum & 0xFFFF) + (cksum >> 16);
  }

  return cksum;
}

void ipv4_header_checksum(IPv4_HEADER_t_Ptr pIpHeader)
{
  uint8_t ipHeadLen      = (pIpHeader->bVersionAndHeaderLen & 0x0F) << 2;

  pIpHeader->u16CheckSum = 0;

  uint32_t sum = Checksum(0, reinterpret_cast<uint8_t *>(pIpHeader), ipHeadLen);

  pIpHeader->u16CheckSum = htons(static_cast<uint16_t>(~sum));
}

void udp6_header_checksum(IPv6_HEADER_t_Ptr pIpHeader, UDP_HEADER_t_Ptr ptUDPHeader, size_t udpLen)
{
  uint32_t sum = 0;

  //udpLen  = sizeof(UDP_HEADER_T) + contentLen;

  //
  ptUDPHeader->u16Length = htons(udpLen);
  sum += udpLen;

  //sum += (ptUDPHeader->u16Length >> 8 & 0x00FF);
  //sum += (ptUDPHeader->u16Length << 8 & 0xFF00);

  sum  = Checksum(sum, &pIpHeader->abSrcAddr[0], 16);
  sum  = Checksum(sum, &pIpHeader->abDstAddr[0], 16);

  sum += (static_cast<uint16_t>(pIpHeader->bNextHeader) & 0x00FF);
  //finish the pseudo header checksum

  //udp section
  ptUDPHeader->u16CheckSum = 0;
  sum = Checksum(sum,  reinterpret_cast<uint8_t *>(ptUDPHeader), udpLen);

  ptUDPHeader->u16CheckSum = htons(static_cast<uint16_t>(~sum));
}

class SockGuard : private ACE_Copy_Disabled
{
public:
  explicit SockGuard(ACE_SOCK& sock):sock_(sock){};
  ~SockGuard(){ sock_.close(); };

private:
  ACE_SOCK& sock_;
};


#define ACE_TEST_EXCEPTION_RETURN(expression, message)\
do {\
    if(expression)\
    {\
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (message)), 1);\
    }\
} while(0)

static char  sendbuf[4096];
static char  recvbuf[4096];

/*
 * This is the heart of the test. It test the api of RAW socket class
 *
 * It returns 0 for success and 1 for error so we can keep track of the
 * total error count.
 */
static int
run_option_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run ...\n", __func__));

  ACE_INET_Addr addr(static_cast<u_short>(0), "127.0.0.1");
  ACE_RAW_SOCKET  rawSocket(addr);
  SockGuard guard(rawSocket);

  ACE_TEST_EXCEPTION_RETURN(rawSocket.get_handle() == ACE_INVALID_HANDLE, "  can not bind the addr\n");

  int rc = rawSocket.enable(ACE_NONBLOCK);

  ACE_TEST_EXCEPTION_RETURN(rc < 0, "  can not bind the addr\n");

  int optval = 0, optlen = sizeof(optval);
  rc = rawSocket.get_option(SOL_SOCKET, SO_RCVBUF, &optval, &optlen);

  ACE_TEST_EXCEPTION_RETURN(rc < 0, "  get SO_RCVBUF in failure\n");

  optlen  = sizeof(optval);
  int new_optval = optval << 1;
  rc = rawSocket.set_option(SOL_SOCKET, SO_RCVBUF, &new_optval, sizeof(new_optval));

  ACE_TEST_EXCEPTION_RETURN(rc < 0, "  set SO_RCVBUF new value in failure\n");
  ACE_DEBUG ((LM_INFO, "%C set_option got optlen: %d ...\n", __func__, optlen));

  new_optval = 0;
  optlen  = sizeof(new_optval);

  rawSocket.get_option(SOL_SOCKET, SO_RCVBUF, &new_optval, &optlen);

  ACE_TEST_EXCEPTION_RETURN(new_optval < optval, "  set SO_RCVBUF with no effect\n");

  ACE_DEBUG ((LM_INFO, "old optval: %d, new optval ...\n", optval, new_optval));

  return 0;
}

static int
run_reopen_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run ...\n", __func__));

  ACE_INET_Addr addr(static_cast<u_short>(0), "127.0.0.1");
  ACE_RAW_SOCKET  rawSocket(addr);
  SockGuard guard(rawSocket);

  ACE_TEST_EXCEPTION_RETURN(rawSocket.get_handle() == ACE_INVALID_HANDLE, "  can not bind the addr\n");

  rawSocket.close();

  ACE_TEST_EXCEPTION_RETURN(rawSocket.get_handle() != ACE_INVALID_HANDLE, "  close in failure\n");

  ACE_INET_Addr addr2(static_cast<u_short>(0), "127.0.0.8");
  int rc = rawSocket.open(addr2);

  ACE_TEST_EXCEPTION_RETURN(rc < 0, "  reopen in failue\n");
  ACE_TEST_EXCEPTION_RETURN(rawSocket.get_handle() == ACE_INVALID_HANDLE, "  handle is invalid after re-open\n");

  return 0;
}

static void
readUdpSocektToEmpty(ACE_SOCK_Dgram& udpSock)
{
  ACE_INET_Addr remote;
  while(1)
  {
    int rc = udpSock.recv(recvbuf, sizeof(recvbuf),remote);
    if(rc == -1)
      break;
  }
}

static int
raw_recv_data_until_meet_condition(ACE_RAW_SOCKET& raw, u_short port, size_t n, ACE_INET_Addr& remote, bool bUseIOVec = false, ACE_INET_Addr* to_addr = nullptr)
{
  ACE_INET_Addr local;
  raw.get_local_addr(local);

  ssize_t len;
  ssize_t expectedLen;

  do
  {
    ACE_OS::memset(recvbuf, 0, sizeof(recvbuf));

    if(bUseIOVec)
    {
      iovec  vec[5];
      unsigned int i = 0;
      unsigned int const oneByteRecvVecNum = (sizeof(vec)/sizeof(vec[0])) - 1;
      for(; i< oneByteRecvVecNum; ++i)
      {
        vec[i].iov_base = &recvbuf[i];
        vec[i].iov_len  = 1;
      }

      vec[i].iov_base = &recvbuf[i];
      vec[i].iov_len  = sizeof(recvbuf) - oneByteRecvVecNum;

      len = raw.recv(vec, static_cast<int>(sizeof(vec)/sizeof(vec[0])), remote, 0/*flags*/, nullptr, to_addr);
    }
    else
    {
      len = raw.recv(recvbuf, sizeof(recvbuf), remote, 0/*flags*/, nullptr, to_addr);
    }

    if(len < 0)
    {
      ACE_DEBUG ((LM_INFO, "%C receive prcess reach the end ...\n", __func__));
      return -1;
    }

    u_short nDstPort;
    char const* szInetType;
    UDP_HEADER_t_Ptr  ptUDPHeader;
    if(local.get_type() == AF_INET)
    {
      ptUDPHeader  = reinterpret_cast<UDP_HEADER_t_Ptr>(recvbuf + sizeof(IPv4_HEADER_t));
      expectedLen  = (n + sizeof(IPv4_HEADER_t) + sizeof(UDP_HEADER_t));
      szInetType   = "IPv4";
    }
    else
    {
      ptUDPHeader  = reinterpret_cast<UDP_HEADER_t_Ptr>(recvbuf);
      expectedLen  = (n + sizeof(UDP_HEADER_t));
      szInetType   = "IPv6";
    }

    nDstPort = ntohs (ptUDPHeader->u16DstPort);
    if (port == nDstPort && len == expectedLen)
    {
      ACE_DEBUG ((LM_INFO, "%C %C recv expected pkgs ...\n", __func__, szInetType));
      break;
    }

    ACE_DEBUG ((LM_DEBUG, "%C recv unexpected pkgs len: %d, srcPort: %u, dstPort:%u; expectedLen: %d, expectedPort: %u ...\n", __func__, len, ntohs(ptUDPHeader->u16SrcPort), ntohs(ptUDPHeader->u16DstPort), expectedLen, port));
    ACE_OS::sleep(1);
  } while (1);

  return 0;
}

static int
run_raw_udp_test_child_flow_sendby_self (ACE_RAW_SOCKET& raw, ACE_INET_Addr& client_addr, ACE_INET_Addr& server_addr, size_t n)
{
  ACE_DEBUG ((LM_INFO, "%C begin to run when sending data by self ...\n", __func__));

  UDP_HEADER_t_Ptr   ptUDPHeader  = reinterpret_cast<UDP_HEADER_t_Ptr>(sendbuf + sizeof(IPv4_HEADER_t_Ptr));

  ptUDPHeader->u16SrcPort  = htons(client_addr.get_port_number());
  ptUDPHeader->u16DstPort  = htons(server_addr.get_port_number());
  ptUDPHeader->u16Length   = htons(n + sizeof(UDP_HEADER_t));
  ptUDPHeader->u16CheckSum = 0;

  int rc = raw.send(ptUDPHeader, n + sizeof(UDP_HEADER_t), server_addr);
  ssize_t expectedLen = n + sizeof(UDP_HEADER_t);
  ACE_TEST_EXCEPTION_RETURN(rc != expectedLen, "  raw socket can not send test pkg to server\n");

  u_short server_port = server_addr.get_port_number();
  ACE_INET_Addr remote;
  rc = raw_recv_data_until_meet_condition(raw, server_port, n, remote);
  ACE_TEST_EXCEPTION_RETURN(rc != 0, "  can recv test pkg from raw socket\n");

  return 0;
}

static int
run_raw_udp_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run using the port auto assigned by OS to avoid port conflict ...\n", __func__));

  ACE_INET_Addr addr(static_cast<u_short>(0), "127.0.0.1");

  ACE_SOCK_Dgram  dgram(addr);
  SockGuard dgram_guard(dgram);

  ACE_SOCK_Dgram  dgram_server(addr);
  SockGuard dgram_server_guard(dgram_server);

  ACE_RAW_SOCKET  rawSocket(addr);
  SockGuard raw_guard(rawSocket);

  ACE_INET_Addr client_addr ,server_addr,remote;
  int rc = dgram.get_local_addr (client_addr);

  ACE_TEST_EXCEPTION_RETURN(rc < 0, "  can not get client bound address\n");

  rc     = dgram_server.get_local_addr (server_addr);

  ACE_TEST_EXCEPTION_RETURN(rc < 0, "  can not get server address\n");

  ssize_t n = 512;
  rc = dgram.send(sendbuf, n, server_addr);
  ACE_TEST_EXCEPTION_RETURN(rc != n, "  can send test pkg to server\n");

  rc = dgram_server.recv(recvbuf, sizeof(recvbuf), remote);
  ACE_TEST_EXCEPTION_RETURN(rc != n, "  server socket can not recv the pkg client has send. It will be recv lately by raw socket\n");

  u_short server_port = server_addr.get_port_number();
  rc = raw_recv_data_until_meet_condition(rawSocket, server_port, n, remote);

  ACE_TEST_EXCEPTION_RETURN(rc != 0, "  can recv test pkg from raw socket\n");

  rc = run_raw_udp_test_child_flow_sendby_self (rawSocket, client_addr, server_addr, n + 1);
  ACE_TEST_EXCEPTION_RETURN(rc != 0, "  can recv test pkg from raw socket when sending by self\n");

#if !defined (ACE_WIN32)
  if(ACE_OS::getuid() == 0)
  {
    ACE_DEBUG ((LM_INFO, "%C test send & recv big pkt ...\n", __func__));
    rc = run_raw_udp_test_child_flow_sendby_self (rawSocket, client_addr, server_addr, n + 2048);
    ACE_TEST_EXCEPTION_RETURN(rc != 0, "  can recv test pkg from raw socket when sending big pkg by self\n");
  }
#endif

  return 0;
}

static int
run_raw_generic_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run generic raw socket i.e. send only RAW socket  ...\n", __func__));

  ACE_INET_Addr bindAddr(static_cast<u_short>(0), "127.0.0.1"), remote;
  ACE_INET_Addr client_addr(static_cast<u_short>(0), "127.0.0.7") ,server_addr(static_cast<u_short>(0), "127.0.0.8");
  ACE_SOCK_Dgram  client_dgram(client_addr);
  SockGuard client_dgram_guard(client_dgram);

  ACE_SOCK_Dgram  server_dgram(server_addr);
  SockGuard server_dgram_guard(server_dgram);

  ACE_RAW_SOCKET  rawSocket(bindAddr, IPPROTO_RAW);
  SockGuard raw_guard(rawSocket);

  ACE_TEST_EXCEPTION_RETURN(rawSocket.is_send_only() == false, "  raw socket is not send only\n");

  ssize_t len = rawSocket.recv(recvbuf, sizeof(recvbuf),  remote);
  ACE_TEST_EXCEPTION_RETURN(len  != -1, "  raw generic socket is send only , must not can recv data\n");

  client_dgram.get_local_addr (client_addr);
  server_dgram.get_local_addr (server_addr);

  IPv4_HEADER_t_Ptr  ptIPv4Header    = reinterpret_cast<IPv4_HEADER_t_Ptr>(sendbuf);
  UDP_HEADER_t_Ptr   ptUDPHeader     = reinterpret_cast<UDP_HEADER_t_Ptr>(sendbuf + sizeof(IPv4_HEADER_t));
  u_short n = 2048;

  *ptIPv4Header                      = {};
  ptIPv4Header->bVersionAndHeaderLen = 0x45;
  ptIPv4Header->u16TotalLenOfPacket  = htons(sizeof(IPv4_HEADER_t) + sizeof(UDP_HEADER_t) + n);     // 数据包长度
  ptIPv4Header->u16PacketID          = ACE_OS::rand();
  ptIPv4Header->bTTL                 = 64;
  ptIPv4Header->bTypeOfProtocol      = IPPROTO_UDP;
  ptIPv4Header->u16CheckSum          = 0;
  ptIPv4Header->u32SourIp            = (static_cast<sockaddr_in*>(client_addr.get_addr()))->sin_addr.s_addr;
  ptIPv4Header->u32DestIp            = (static_cast<sockaddr_in*>(server_addr.get_addr()))->sin_addr.s_addr;

  u_short client_port_number = client_addr.get_port_number();
  u_short server_port_number = server_addr.get_port_number();

  ptUDPHeader->u16SrcPort  = htons(client_port_number);
  ptUDPHeader->u16DstPort  = htons(server_port_number);
  ptUDPHeader->u16Length   = htons(sizeof(UDP_HEADER_t) + n);
  ptUDPHeader->u16CheckSum = 0;

#if !defined (ACE_WIN32)
  if(ACE_OS::getuid() == 0)
  {
    ACE_DEBUG ((LM_INFO, "%C raw generic socket will send bytes exceeding the MTU  ...\n", __func__));
    n = 2048;
    len = rawSocket.send(sendbuf, sizeof(IPv4_HEADER_t) + sizeof(UDP_HEADER_t) + n,  server_addr);
    ACE_TEST_EXCEPTION_RETURN(len  != -1, "  raw generic socket can not send pkg more than MTU\n");
  }
#endif

  n = 468;
  ptUDPHeader->u16Length   = htons(sizeof(UDP_HEADER_t) + n);
  len = rawSocket.send(sendbuf, sizeof(IPv4_HEADER_t) + sizeof(UDP_HEADER_t) + n,  server_addr);
  size_t expectedLen = (sizeof(IPv4_HEADER_t) + sizeof(UDP_HEADER_t) + n);
  ACE_DEBUG ((LM_INFO, "%C raw generic socket send %d bytes, expected %u bytes  ...\n", __func__, len, expectedLen));
  ACE_TEST_EXCEPTION_RETURN(static_cast<size_t>(len)  != expectedLen, "  raw generic socket send pkg in failure\n");

  ACE_OS::sleep(1);
  ACE_DEBUG ((LM_INFO, "%C enable nonblock status ...\n", __func__));
  server_dgram.enable(ACE_NONBLOCK);
  len = server_dgram.recv(recvbuf, sizeof(recvbuf), remote);
  expectedLen =  n;
  ACE_DEBUG ((LM_INFO, "%C udp server socket recv %d bytes, expected %u bytes  ...\n", __func__, len, expectedLen));
  ACE_TEST_EXCEPTION_RETURN(static_cast<size_t>(len)  !=  n, "  server socket receives pkg in failure length is not the same\n");
  ACE_TEST_EXCEPTION_RETURN(static_cast<sockaddr_in*>(remote.get_addr())->sin_addr.s_addr  !=  static_cast<sockaddr_in*>(client_addr.get_addr())->sin_addr.s_addr, "  server socket receives pkg in failure: the source IP is not the same\n");

  return 0;
}

#if defined (ACE_HAS_IPV6)
static int
run_ipv6_pkginfo_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run IPv6 pkginfo test ...\n", __func__));

  ACE_INET_Addr bindAddr(static_cast<u_short>(0), "::1");
  ACE_INET_Addr anyAddr(static_cast<u_short>(0), "::");

  ACE_INET_Addr client_addr(static_cast<u_short>(0), "::1") ,server_addr(static_cast<u_short>(0), "::1");

  ACE_SOCK_Dgram  client_dgram(client_addr);
  SockGuard client_dgram_guard(client_dgram);
  client_dgram.get_local_addr(client_addr);

  ACE_SOCK_Dgram  server_dgram(server_addr);
  SockGuard server_dgram_guard(server_dgram);
  server_dgram.get_local_addr(server_addr);

  ACE_DEBUG ((LM_INFO, "%C get the real bound addr and port client_port: %u, server_port: %u...\n", __func__, client_addr.get_port_number(), server_addr.get_port_number()));

  ACE_RAW_SOCKET  rawSocket(bindAddr, IPPROTO_UDP);
  rawSocket.enable(ACE_NONBLOCK);
  SockGuard raw_guard(rawSocket);

  ACE_RAW_SOCKET  rawWildcardSocket(anyAddr, IPPROTO_UDP);
  rawWildcardSocket.enable(ACE_NONBLOCK);
  SockGuard raw_wildcard_guard(rawWildcardSocket);

  client_dgram.send("hello world", sizeof("hello world"), server_addr);

  ACE_DEBUG ((LM_INFO, "%C the send pkg will be received by two raw sockets ...\n", __func__));
  ACE_OS::sleep(1);

  ACE_INET_Addr remote;
  int rc = raw_recv_data_until_meet_condition(rawSocket, server_addr.get_port_number(), sizeof("hello world"), remote);

  ACE_TEST_EXCEPTION_RETURN(rc != 0, "  non wildcard raw socket can not recv expectedRecvLen\n");

  ACE_INET_Addr to_addr;

  ACE_DEBUG ((LM_INFO, "%C send pkg again to test common raw socket with to_adr parameter ...\n", __func__));
  client_dgram.send("hello world", sizeof("hello world"), server_addr);
  ACE_OS::sleep(1);

  rc = raw_recv_data_until_meet_condition(rawSocket, server_addr.get_port_number(), sizeof("hello world"), remote, false, &to_addr);
  ACE_TEST_EXCEPTION_RETURN(rc != 0, "  non wildcard raw socket can not recv expectedRecvLen with to_addr parameter\n");

  in6_addr* remote_sin6_addr = &(static_cast<sockaddr_in6 *>(remote.get_addr())->sin6_addr);
  in6_addr* to_sin6_addr     = &(static_cast<sockaddr_in6 *>(to_addr.get_addr())->sin6_addr);
  int cmp = ACE_OS::memcmp(remote_sin6_addr, to_sin6_addr, sizeof(*to_sin6_addr));
  ACE_TEST_EXCEPTION_RETURN(cmp != 0, "  non wildcard raw socket got to_addr with invalid value\n");

  rc = raw_recv_data_until_meet_condition(rawWildcardSocket, server_addr.get_port_number(), sizeof("hello world"), remote, false, &to_addr);
  ACE_TEST_EXCEPTION_RETURN(rc != 0, "  can not recv expectedRecvLen with to_addr when provided to wildcard RAW socket\n");
  remote_sin6_addr = &(static_cast<sockaddr_in6 *>(remote.get_addr())->sin6_addr);
  to_sin6_addr     = &(static_cast<sockaddr_in6 *>(to_addr.get_addr())->sin6_addr);
  cmp = ACE_OS::memcmp(remote_sin6_addr, to_sin6_addr, sizeof(*to_sin6_addr));
  ACE_TEST_EXCEPTION_RETURN(cmp != 0, "  to_addr with invalid value when provided to wildcard RAW socket\n");

  return 0;
}
#endif

static int
run_iovec_IPv6_api_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run IPv6 iovec api test ...\n", __func__));

  ACE_INET_Addr bindAddr(static_cast<u_short>(0), "::1");
  ACE_INET_Addr anyAddr(static_cast<u_short>(0), "::");

  ACE_INET_Addr client_addr(static_cast<u_short>(0), "::1") ,server_addr(static_cast<u_short>(0), "::1");

  ACE_SOCK_Dgram  client_dgram(client_addr);
  SockGuard client_dgram_guard(client_dgram);
  client_dgram.get_local_addr(client_addr);

  ACE_SOCK_Dgram  server_dgram(server_addr);
  server_dgram.enable(ACE_NONBLOCK);
  SockGuard server_dgram_guard(server_dgram);
  server_dgram.get_local_addr(server_addr);

  ACE_DEBUG ((LM_INFO, "%C get the real bound addr and port client_port: %u, server_port: %u...\n", __func__, client_addr.get_port_number(), server_addr.get_port_number()));

  ACE_RAW_SOCKET  rawSocket(bindAddr, IPPROTO_UDP);
  rawSocket.enable(ACE_NONBLOCK);
  SockGuard raw_guard(rawSocket);

  client_dgram.send("hello world", sizeof("hello world"), server_addr);

  ACE_DEBUG ((LM_INFO, "%C the send pkg will be received by raw sockets ...\n", __func__));
  ACE_OS::sleep(1);

  ACE_INET_Addr remote;
  int rc = raw_recv_data_until_meet_condition(rawSocket, server_addr.get_port_number(), sizeof("hello world"), remote);

  ACE_TEST_EXCEPTION_RETURN(rc != 0, "  raw socket can not recv expectedRecvLen\n");
  ACE_TEST_EXCEPTION_RETURN(ACE_OS::strncmp(recvbuf + sizeof(UDP_HEADER_t), "hello world", sizeof("hello world")) != 0, "  non wildcard raw socket can not recv expected content\n");

  ACE_INET_Addr to_addr;

  ACE_DEBUG ((LM_INFO, "%C send pkg again to test common raw socket with to_adr parameter ...\n", __func__));
  client_dgram.send("hello world", sizeof("hello world"), server_addr);
  ACE_OS::sleep(1);

  rc = raw_recv_data_until_meet_condition(rawSocket, server_addr.get_port_number(), sizeof("hello world"), remote, true, &to_addr);
  ACE_TEST_EXCEPTION_RETURN(rc != 0, "  raw socket can not recv expectedRecvLen with to_addr parameter\n");
  ACE_TEST_EXCEPTION_RETURN(ACE_OS::strncmp(recvbuf + sizeof(UDP_HEADER_t), "hello world", sizeof("hello world")) != 0, "  raw socket can not recv expected content with to_addr parameter\n");

  in6_addr* remote_sin6_addr = &(static_cast<sockaddr_in6 *>(remote.get_addr())->sin6_addr);
  in6_addr* to_sin6_addr     = &(static_cast<sockaddr_in6 *>(to_addr.get_addr())->sin6_addr);
  int cmp = ACE_OS::memcmp(remote_sin6_addr, to_sin6_addr, sizeof(*to_sin6_addr));
  ACE_TEST_EXCEPTION_RETURN(cmp != 0, "  raw socket got to_addr with invalid value\n");

  ACE_DEBUG ((LM_INFO, "%C test iovec send ...\n", __func__));

  IPv6_HEADER_t_Ptr  ptIPv6Header    = reinterpret_cast<IPv6_HEADER_t_Ptr>(sendbuf);
  UDP_HEADER_t_Ptr   ptUDPHeader     = reinterpret_cast<UDP_HEADER_t_Ptr>(sendbuf + sizeof(IPv6_HEADER_t));
  u_short n = sizeof("hello world");

  *ptIPv6Header = {};
  ptIPv6Header->bNextHeader          = IPPROTO_UDP;
  memcpy(ptIPv6Header->abSrcAddr, static_cast<sockaddr_in6*>(client_addr.get_addr())->sin6_addr.s6_addr, 16);
  memcpy(ptIPv6Header->abDstAddr, static_cast<sockaddr_in6*>(server_addr.get_addr())->sin6_addr.s6_addr, 16);

  u_short client_port_number = client_addr.get_port_number();
  u_short server_port_number = server_addr.get_port_number();

  ptUDPHeader->u16SrcPort  = htons(client_port_number);
  ptUDPHeader->u16DstPort  = htons(server_port_number);
  //fill content
  ACE_OS::strcpy(sendbuf + sizeof(IPv6_HEADER_t) + sizeof(UDP_HEADER_t), "hello world");
  udp6_header_checksum(ptIPv6Header, ptUDPHeader, sizeof(UDP_HEADER_t) + n);

  iovec   iov_udp[2];
  iov_udp[0].iov_base = reinterpret_cast<char*>(ptUDPHeader);
  iov_udp[0].iov_len  = sizeof(UDP_HEADER_t);
  iov_udp[1].iov_base = const_cast<char*>("hello world");
  iov_udp[1].iov_len  = sizeof("hello world");

  ACE_DEBUG ((LM_INFO, "%C test iovec using common udp6 socket ...\n", __func__));

  rc = client_dgram.send(iov_udp, static_cast<int>(sizeof(iov_udp)/sizeof(iov_udp[0])), server_addr);
  ACE_TEST_EXCEPTION_RETURN(rc  == -1, "  udp6 socket can not send using iov \n");

  readUdpSocektToEmpty(server_dgram);
  ACE_INET_Addr  iov_server_addr(server_addr);

  ACE_DEBUG ((LM_INFO, "%C must set port to zero ??? ...\n", __func__));
  iov_server_addr.set_port_number(0);
  rc = rawSocket.send(iov_udp, static_cast<int>(sizeof(iov_udp)/sizeof(iov_udp[0])), iov_server_addr);
  ACE_TEST_EXCEPTION_RETURN(rc  == -1, "  raw6 socket can not send using iov \n");
  ACE_OS::sleep(1);

  rc = server_dgram.recv(recvbuf, sizeof(recvbuf), remote);
  ACE_TEST_EXCEPTION_RETURN(rc  == -1, "  server socket6 can not recv pkg by iov send\n");
  ACE_TEST_EXCEPTION_RETURN(ACE_OS::strcmp(recvbuf, "hello world")  != 0, "  the content of server socket6 receive pkg is not expected by iov send\n");

  return 0;
}

static int
run_iovec_IPv4_api_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run IPv4 iovec api test ...\n", __func__));

  ACE_INET_Addr bindAddr(static_cast<u_short>(0), "127.0.0.1");

  ACE_INET_Addr client_addr(static_cast<u_short>(0), "127.0.0.1") ,server_addr(static_cast<u_short>(0), "127.0.0.1");

  ACE_SOCK_Dgram  client_dgram(client_addr);
  SockGuard client_dgram_guard(client_dgram);
  client_dgram.get_local_addr(client_addr);

  ACE_SOCK_Dgram  server_dgram(server_addr);
  server_dgram.enable(ACE_NONBLOCK);
  SockGuard server_dgram_guard(server_dgram);
  server_dgram.get_local_addr(server_addr);

  ACE_DEBUG ((LM_INFO, "%C get the real bound addr and port client_port: %u, server_port: %u...\n", __func__, client_addr.get_port_number(), server_addr.get_port_number()));

  ACE_RAW_SOCKET  rawSocket(bindAddr, IPPROTO_UDP);
  rawSocket.enable(ACE_NONBLOCK);
  SockGuard raw_guard(rawSocket);

  client_dgram.send("hello world", sizeof("hello world"), server_addr);

  ACE_DEBUG ((LM_INFO, "%C the send pkg will be received by raw sockets ...\n", __func__));
  ACE_OS::sleep(1);

  ACE_INET_Addr remote;
  int rc = raw_recv_data_until_meet_condition(rawSocket, server_addr.get_port_number(), sizeof("hello world"), remote);

  ACE_TEST_EXCEPTION_RETURN(rc != 0, "  raw socket can not recv expectedRecvLen\n");
  ACE_TEST_EXCEPTION_RETURN(ACE_OS::strncmp(recvbuf + sizeof(IPv4_HEADER_t) + sizeof(UDP_HEADER_t), "hello world", sizeof("hello world")) != 0, "  non wildcard raw socket can not recv expected content\n");

  ACE_INET_Addr to_addr;

  ACE_DEBUG ((LM_INFO, "%C send pkg again to test common raw socket with to_adr parameter ...\n", __func__));
  client_dgram.send("hello world", sizeof("hello world"), server_addr);
  ACE_OS::sleep(1);

  rc = raw_recv_data_until_meet_condition(rawSocket, server_addr.get_port_number(), sizeof("hello world"), remote, true, &to_addr);
  ACE_TEST_EXCEPTION_RETURN(rc != 0, "  raw socket can not recv expectedRecvLen with to_addr parameter\n");
  ACE_TEST_EXCEPTION_RETURN(ACE_OS::strncmp(recvbuf + sizeof(IPv4_HEADER_t) + sizeof(UDP_HEADER_t), "hello world", sizeof("hello world")) != 0, "  raw socket can not recv expected content with to_addr parameter\n");

  in_addr* remote_sin_addr = &(static_cast<sockaddr_in *>(remote.get_addr())->sin_addr);
  in_addr* to_sin_addr     = &(static_cast<sockaddr_in *>(to_addr.get_addr())->sin_addr);
  int cmp = ACE_OS::memcmp(remote_sin_addr, to_sin_addr, sizeof(*to_sin_addr));
  ACE_TEST_EXCEPTION_RETURN(cmp != 0, "  raw socket got to_addr with invalid value\n");

  ACE_DEBUG ((LM_INFO, "%C test iovec send ...\n", __func__));
  readUdpSocektToEmpty(server_dgram);

  IPv4_HEADER_t_Ptr  ptIPv4Header    = reinterpret_cast<IPv4_HEADER_t_Ptr>(sendbuf);
  UDP_HEADER_t_Ptr   ptUDPHeader     = reinterpret_cast<UDP_HEADER_t_Ptr>(sendbuf + sizeof(IPv4_HEADER_t));
  u_short n = sizeof("hello world");

  *ptIPv4Header = {};

  ptIPv4Header->bVersionAndHeaderLen = 0x45;
  ptIPv4Header->u16TotalLenOfPacket  = htons(sizeof(IPv4_HEADER_t) + sizeof(UDP_HEADER_t) + n);     // 数据包长度
  ptIPv4Header->u16PacketID          = ACE_OS::rand();
  ptIPv4Header->bTTL                 = 64;
  ptIPv4Header->bTypeOfProtocol      = IPPROTO_UDP;
  ptIPv4Header->u16CheckSum          = 0;
  ptIPv4Header->u32SourIp            = (static_cast<sockaddr_in*>(client_addr.get_addr()))->sin_addr.s_addr;
  ptIPv4Header->u32DestIp            = (static_cast<sockaddr_in*>(server_addr.get_addr()))->sin_addr.s_addr;

  u_short client_port_number = client_addr.get_port_number();
  u_short server_port_number = server_addr.get_port_number();

  ptUDPHeader->u16SrcPort  = htons(client_port_number);
  ptUDPHeader->u16DstPort  = htons(server_port_number);
  ptUDPHeader->u16Length   = htons(sizeof(UDP_HEADER_t) + n);
  ptUDPHeader->u16CheckSum = 0;
  ACE_OS::strcpy(sendbuf + sizeof(IPv4_HEADER_t) + sizeof(UDP_HEADER_t), "hello world");

  iovec   iov_udp[2];
  iov_udp[0].iov_base = reinterpret_cast<char*>(ptUDPHeader);
  iov_udp[0].iov_len  = sizeof(UDP_HEADER_t);
  iov_udp[1].iov_base = const_cast<char*>("hello world");
  iov_udp[1].iov_len  = sizeof("hello world");

  rc = rawSocket.send(iov_udp, static_cast<int>(sizeof(iov_udp)/sizeof(iov_udp[0])), server_addr);
  ACE_TEST_EXCEPTION_RETURN(rc  == -1, "  raw4 socket can send using iov\n");
  ACE_OS::sleep(1);

  rc = server_dgram.recv(recvbuf, sizeof(recvbuf), remote);
  ACE_TEST_EXCEPTION_RETURN(rc  == -1, "  server socket4 can not recv pkg by iov send\n");
  ACE_TEST_EXCEPTION_RETURN(ACE_OS::strcmp(recvbuf, "hello world")  != 0, "  the content of server socket4 receive pkg is not expected by iov send\n");

  return 0;
}

int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("RAW_Socket_Test"));
  ACE_UNUSED_ARG (argv);
  int retval = 0;

#if !defined (ACE_WIN32)
  // set the lo interface MTU
  int oldMTU;
  if(ACE_OS::getuid() == 0)
  {
    ACE_INET_Addr anyAddr(static_cast<u_short>(0));
    ACE_SOCK_Dgram  netdevice(anyAddr);
    SockGuard dgram_guard(netdevice);

    struct ifreq tReq = {};
    ACE_OS::snprintf(tReq.ifr_name, sizeof(tReq.ifr_name), "%s", "lo");

    tReq.ifr_mtu = 0;
    ACE_OS::ioctl(netdevice.get_handle(), SIOCGIFMTU, &tReq);
    oldMTU = tReq.ifr_mtu;

    tReq.ifr_mtu = 1400;
    ACE_OS::ioctl(netdevice.get_handle(), SIOCSIFMTU, &tReq);

    tReq.ifr_mtu = 0;
    ACE_OS::ioctl(netdevice.get_handle(), SIOCGIFMTU, &tReq);
    ACE_TEST_EXCEPTION_RETURN(tReq.ifr_mtu != 1400, "  can set MTU for lo interface\n");
  }
#endif

  // Run the tests for each type of ordering.
  retval = run_option_test ();
  retval += run_reopen_test();
  retval += run_raw_udp_test();
  retval += run_raw_generic_test();
  retval += run_iovec_IPv4_api_test();
  retval += run_iovec_IPv6_api_test();

#if defined (ACE_HAS_IPV6)
  retval += run_ipv6_pkginfo_test();
#else
  ACE_DEBUG ((LM_INFO, "%C without IPv6 macro ...\n", __func__));
#endif

  ACE_END_TEST;

#if !defined (ACE_WIN32)
 if(ACE_OS::getuid() == 0)
  {
    ACE_INET_Addr anyAddr(static_cast<u_short>(0));
    ACE_SOCK_Dgram  netdevice(anyAddr);
    SockGuard dgram_guard(netdevice);

    struct ifreq tReq = {};
    ACE_OS::snprintf(tReq.ifr_name, sizeof(tReq.ifr_name), "%s", "lo");
    tReq.ifr_mtu = oldMTU;
    ACE_OS::ioctl(netdevice.get_handle(), SIOCSIFMTU, &tReq);
  }
#endif

  return retval;
}
