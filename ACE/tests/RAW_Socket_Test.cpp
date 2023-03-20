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

typedef struct _IP_HEADER_t
{
   uint8_t  bVersionAndHeaderLen;      // 版本信息(前4位)，头长度(后4位)
   uint8_t  bTypeOfService;           // 服务类型8位
   uint16_t u16TotalLenOfPacket;     // 数据包长度
   uint16_t u16PacketID;            // 数据包标识
   uint16_t u16Sliceinfo;          // 分片使用
   uint8_t  bTTL;                 // 存活时间
   uint8_t  bTypeOfProtocol;     // 协议类型
   uint16_t u16CheckSum;        // 校验和
   uint32_t u32SourIp;         // 源ip
   uint32_t u32DestIp;        // 目的ip
} __attribute__((packed)) IPv4_HEADER_t, *IPv4_HEADER_t_Ptr;

typedef struct _IPv6_HEADER_t
{
   union
   {
      uint8_t abVTF[4];  // 版本信息 most top 4 bits , traffic 8 bits
      uint32_t dwVTF;   // flow label lefted 20 bits;
   };

   uint16_t u16PayloadLen; // 数据包长度
   uint8_t  bNextHeader;  // 数据包标识
   uint8_t  bHopLimit;   // max hop limit

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

} __attribute__((packed)) IPv6_HEADER_t, *IPv6_HEADER_t_Ptr;

typedef struct _UDP_HEADER_t
{
   uint16_t u16SrcPort;
   uint16_t u16DstPort;
   uint16_t u16Length;
   uint16_t u16CheckSum;
} __attribute__((packed)) UDP_HEADER_t, *UDP_HEADER_t_Ptr;


class SockGuard : private ACE_Copy_Disabled 
{ 
public:
SockGuard(ACE_SOCK& sock):sock_(sock){};
~SockGuard(){ sock_.close(); };

private:
 ACE_SOCK& sock_;
};


#define EXCEPTION_RETURN(expression, message)\
do{\
    if(expression)\
    {\
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (message)), 1);\
}\
}while(0)

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
 
  ACE_DEBUG ((LM_INFO, "%s begin to run ...\n", __func__));

  ACE_INET_Addr addr(u_short(0), "127.0.0.1");
  ACE_RAW_SOCKET  rawSocket(addr);
  SockGuard guard(rawSocket);

  EXCEPTION_RETURN(rawSocket.get_handle() == ACE_INVALID_HANDLE, "  can not bind the addr\n");

  int rc = rawSocket.enable(ACE_NONBLOCK);

  EXCEPTION_RETURN(rc < 0, "  can not bind the addr\n");


  int optval = 0, optlen = sizeof(optval);
  rc = rawSocket.get_option(SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
  
  EXCEPTION_RETURN(rc < 0, "  get SO_RCVBUF in failure\n");

  optlen  = sizeof(optval);
  int new_optval = optval << 1;
  rc = rawSocket.set_option(SOL_SOCKET, SO_RCVBUF, &new_optval, sizeof(new_optval));

  EXCEPTION_RETURN(rc < 0, "  set SO_RCVBUF new value in failure\n");

  new_optval = 0;
  optlen  = sizeof(new_optval);

  rawSocket.get_option(SOL_SOCKET, SO_RCVBUF, &new_optval, &optlen);

  EXCEPTION_RETURN(new_optval < optval, "  set SO_RCVBUF with no effect\n");

  ACE_DEBUG ((LM_INFO, "old optval: %d, new optval ...\n", optval, new_optval));

  return 0;
}


static int
run_reopen_test ()
{
 
  ACE_DEBUG ((LM_INFO, "%s begin to run ...\n", __func__));
  
  ACE_INET_Addr addr((u_short)0, "127.0.0.1");
  ACE_RAW_SOCKET  rawSocket(addr);
  SockGuard guard(rawSocket);

  EXCEPTION_RETURN(rawSocket.get_handle() == ACE_INVALID_HANDLE, "  can not bind the addr\n");


  rawSocket.close();

  EXCEPTION_RETURN(rawSocket.get_handle() != ACE_INVALID_HANDLE, "  close in failure\n");

  ACE_INET_Addr addr2((u_short)0, "127.0.0.8");
  int rc = rawSocket.open(addr2);

  EXCEPTION_RETURN(rc < 0, "  reopen in failue\n");
  EXCEPTION_RETURN(rawSocket.get_handle() == ACE_INVALID_HANDLE, "  handle is invalid after re-open\n");
 

  return 0;
}

static int raw_recv_data_until_meet_condition(ACE_RAW_SOCKET& raw, u_short port, size_t n)
{
   ACE_INET_Addr local;
   raw.get_local_addr(local);
   do
   {
     ACE_INET_Addr remote;
     ssize_t len = raw.recv(recvbuf, sizeof(recvbuf), remote);

     if(len < 0)
     {
      return 1;
     }

     if(local.get_type() == AF_INET)
     {
       IPv4_HEADER_t_Ptr ptIPv4Header = (IPv4_HEADER_t_Ptr)recvbuf;
       UDP_HEADER_t_Ptr  ptUDPHeader  = (UDP_HEADER_t_Ptr)(recvbuf + sizeof(IPv4_HEADER_t));
       u_short nDstPort = ntohs(ptUDPHeader->u16DstPort) ;
       if(port == nDstPort && len == (n + sizeof(IPv4_HEADER_t) + sizeof(UDP_HEADER_t)))
       {
        ACE_DEBUG ((LM_INFO, "%s recv expected pkgs ...\n", __func__));
        break;
       }
     }

     
     ACE_DEBUG ((LM_DEBUG, "%s recv unexpected pkgs ...\n", __func__));
     ACE_OS::sleep(1);
   } while (1);
   
  return 0;
}

static int
run_data_transfer_test_common_child_flow_sendby_self (ACE_RAW_SOCKET& raw, ACE_INET_Addr& client_addr, ACE_INET_Addr& server_addr, size_t n)
{
   ACE_DEBUG ((LM_INFO, "%s begin to run when sending data by self ...\n", __func__));
   IPv4_HEADER_t_Ptr  ptIPv4Header = (IPv4_HEADER_t_Ptr)sendbuf;
   UDP_HEADER_t_Ptr   ptUDPHeader  = (UDP_HEADER_t_Ptr)(sendbuf + sizeof(IPv4_HEADER_t_Ptr));

   ptUDPHeader->u16SrcPort  = htons(client_addr.get_port_number());
   ptUDPHeader->u16DstPort  = htons(server_addr.get_port_number());
   ptUDPHeader->u16Length   = htons(n + sizeof(UDP_HEADER_t));
   ptUDPHeader->u16CheckSum = 0;

   int rc = raw.send(ptUDPHeader, n + sizeof(UDP_HEADER_t), server_addr);
   EXCEPTION_RETURN(rc != (n + sizeof(UDP_HEADER_t)), "  raw socket can not send test pkg to server\n");

   u_short server_port = server_addr.get_port_number();
   rc = raw_recv_data_until_meet_condition(raw, server_port, n);
   EXCEPTION_RETURN(rc != 0, "  can recv test pkg from raw socket\n");
   
   return 0;
}

static int
run_data_transfer_test_common ()
{
  ACE_DEBUG ((LM_INFO, "%s begin to run using the port assigned by OS to avoid conflict ...\n", __func__));

  ACE_INET_Addr addr((u_short)0, "127.0.0.1");
  
  ACE_SOCK_Dgram  dgram(addr);
  SockGuard dgram_guard(dgram);

  ACE_SOCK_Dgram  dgram_server(addr);
  SockGuard dgram_server_guard(dgram_server);

  ACE_RAW_SOCKET  rawSocket(addr);
  SockGuard raw_guard(rawSocket);

  ACE_INET_Addr client_addr ,server_addr;
  int rc = dgram.get_local_addr (client_addr);

  EXCEPTION_RETURN(rc < 0, "  can not get client bound address\n");

  rc     = dgram_server.get_local_addr (server_addr);

  EXCEPTION_RETURN(rc < 0, "  can not get server address\n");

  size_t n = 512;
  rc = dgram.send(sendbuf, n, server_addr);
  EXCEPTION_RETURN(rc != n, "  can send test pkg to server\n");

  u_short server_port = server_addr.get_port_number();
  rc = raw_recv_data_until_meet_condition(rawSocket, server_port, n);

  EXCEPTION_RETURN(rc != 0, "  can recv test pkg from raw socket\n");

  rc = run_data_transfer_test_common_child_flow_sendby_self (rawSocket, client_addr, server_addr, n + 1);
  EXCEPTION_RETURN(rc != 0, "  can recv test pkg from raw socket when sending by self\n");

  ACE_DEBUG ((LM_INFO, "%s test send & recv big pkt ...\n", __func__));
  rc = run_data_transfer_test_common_child_flow_sendby_self (rawSocket, client_addr, server_addr, n + 2048);
  EXCEPTION_RETURN(rc != 0, "  can recv test pkg from raw socket when sending big pkg by self\n");
  
  
  return 0;
}

int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("RAW_Socket_Test"));
  ACE_UNUSED_ARG (argv);
  int retval = 0;
 

  // Run the tests for each type of ordering.
  retval = run_option_test ();
  retval += run_reopen_test();
  retval += run_data_transfer_test_common();
  

  ACE_END_TEST;

  return retval;
}
