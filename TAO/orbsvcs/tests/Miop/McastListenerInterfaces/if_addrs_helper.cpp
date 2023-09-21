#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale>
#include <codecvt>

#include "ace/OS_main.h"
#include "ace/ace_wchar.h"

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

bool
is_loopback (const struct in6_addr * a)
  {
    // ::1
    return (a->s6_words[0] == 0 &&
            a->s6_words[1] == 0 &&
            a->s6_words[2] == 0 &&
            a->s6_words[3] == 0 &&
            a->s6_words[4] == 0 &&
            a->s6_words[5] == 0 &&
            a->s6_words[6] == 0 &&
            a->s6_bytes[14] == 0 &&
            a->s6_bytes[15] == 0x01);
  }

bool
is_link_local (const struct in6_addr * a)
  {
    // fe80::/64
    return (a->s6_bytes[0] == 0xfe &&
            a->s6_bytes[1] == 0x80 &&
            a->s6_words[1] == 0 &&
            a->s6_words[2] == 0 &&
            a->s6_words[3] == 0);
  }

bool
is_site_local (const struct in6_addr * a)
  {
    // fec0::/16
    return (a->s6_bytes[0] == 0xfe &&
            a->s6_bytes[1] == 0xc0);
  }

bool
is_v4_mapped (const struct in6_addr * a)
  {
    // ::ffff:0:0/96
    return (a->s6_words[0] == 0 &&
            a->s6_words[1] == 0 &&
            a->s6_words[2] == 0 &&
            a->s6_words[3] == 0 &&
            a->s6_words[4] == 0 &&
            a->s6_words[5] == 0xffff);
  }

bool
is_v4_translated (const struct in6_addr * a)
  {
    // ::ffff:0:0:0/96
    return (a->s6_words[0] == 0 &&
            a->s6_words[1] == 0 &&
            a->s6_words[2] == 0 &&
            a->s6_words[3] == 0 &&
            a->s6_words[4] == 0xffff &&
            a->s6_words[5] == 0);
  }

class WSACleanupHelper
  {
  public:
    ~WSACleanupHelper()
      {
        WSACleanup();
      }
  };

int
ACE_TMAIN (int, ACE_TCHAR * [])
  {
    WORD wVersionRequested = MAKEWORD (2, 2);
    WSADATA wsaData;

    int err = WSAStartup (wVersionRequested, &wsaData);
    if (err != 0)
      {
        ::std::cout << "WSAStartup failed with error: " << err << ::std::endl;
        return 1;
      }

    WSACleanupHelper helper; // use RAII, since we return in numerous places

    DWORD dwRetVal = 0;

    PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = nullptr;

    // Allocate a 15 KB buffer to start with.
    outBufLen = WORKING_BUFFER_SIZE;

    do
      {
        pAddresses = reinterpret_cast <IP_ADAPTER_ADDRESSES *> (MALLOC (outBufLen));
        if (pAddresses == nullptr)
        {
          ::std::cout << "Memory allocation failed for IP_ADAPTER_ADDRESSES struct" << ::std::endl;
          return 1;
        }

        dwRetVal = ::GetAdaptersAddresses (AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW)
          {
              FREE (pAddresses);
              pAddresses = nullptr;
          }
        else
          {
              break;
          }

        Iterations++;
      } while (dwRetVal == ERROR_BUFFER_OVERFLOW && Iterations < MAX_TRIES);

    if (dwRetVal != NO_ERROR)
      {
        ::std::cout << "Call to GetAdaptersAddresses failed with error: " << dwRetVal << ::std::endl;
        if (pAddresses)
          {
            FREE (pAddresses);
          }
        return 1;
      }

    int s;
    static const int hostlen = 100;
    wchar_t host [hostlen];

    bool hasLOv4 (false), hasETHv4 (false), hasLOv6 (false), hasETHv6 (false);

    while (pAddresses)
      {
        IP_ADAPTER_UNICAST_ADDRESS *uni = 0;
        for (uni = pAddresses->FirstUnicastAddress; uni != 0; uni = uni->Next)
          {
            LPSOCKADDR sa = uni->Address.lpSockaddr;
            int minStrLen (7), structLen (sizeof (struct sockaddr_in));
            if (sa->sa_family == AF_INET6)
              {
                minStrLen = 3;
                structLen = sizeof (struct sockaddr_in6);
              }
            int len = hostlen;
            s = ::WSAAddressToStringW (sa, structLen, nullptr, host, (LPDWORD)&len);
            if (s != 0)
              {
                ::std::cout << "WARNING: WSAAddressToString() failed: WSAGetLastError() = " << ::WSAGetLastError () << ::std::endl;
                continue;
              }
            ::std::wstring hostWstr (host);
            // IP addresses never need > 1B/character
            ::std::string hostStr (std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> ().to_bytes (hostWstr));
            if (hostStr.length () < minStrLen)
              {
                ::std::cout << "WARNING: WSAAddressToString() returned a too-short value: " << host << ::std::endl;
              }
            if (sa->sa_family == AF_INET)
              {
                if (hostStr.rfind ("127.", 0) == 0)
                  {
                    if (!hasLOv4)
                      {
                        hasLOv4 = true;
                        ::std::cout << "LOv4: " << pAddresses->AdapterName << " " << hostStr << ::std::endl;
                      }
                  }
                else if (hostStr.rfind ("0.", 0) != 0 &&
                         hostStr.rfind ("169.254.", 0) != 0)
                  {
                    if (!hasETHv4)
                      {
                        hasETHv4 = true;
                        ::std::cout << "ETHv4: " << pAddresses->AdapterName << " " << hostStr << ::std::endl;
                      }
                  }
              }
            else if (sa->sa_family == AF_INET6)
              {
                struct sockaddr_in6 *sin = reinterpret_cast <sockaddr_in6 *> (sa);
                if (is_loopback (&sin->sin6_addr))
                  {
                    if (!hasLOv6)
                      {
                        hasLOv6 = true;
                        ::std::cout << "LOv6: " << pAddresses->AdapterName << " " << hostStr << ::std::endl;
                      }
                  }
                else if (!is_link_local (&sin->sin6_addr) &&
                         !is_site_local (&sin->sin6_addr) &&
                         !is_v4_mapped (&sin->sin6_addr) &&
                         !is_v4_translated (&sin->sin6_addr))
                  {
                    if (!hasETHv6)
                      {
                        hasETHv6 = true;
                        ::std::cout << "ETHv6: " << pAddresses->AdapterName << " " << hostStr << ::std::endl;
                      }
                  }
              }
          }

        pAddresses = pAddresses->Next;
      }

    FREE (pAddresses);

    return 0;
  }

#else

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "ace/OS_main.h"
#include "ace/ace_wchar.h"

int
ACE_TMAIN (int, ACE_TCHAR * [])
  {
    struct ifaddrs *ifaddr;
    if (::getifaddrs (&ifaddr) == -1)
      {
        ::perror ("getifaddrs");
        return 1;
      }

    int s;
    static const size_t hostlen = 100;
    char host [hostlen];

    bool hasLOv4 (false), hasETHv4 (false), hasLOv6 (false), hasETHv6 (false);

    for (struct ifaddrs *ifa = ifaddr; ifa != 0; ifa = ifa->ifa_next)
      {
        if (ifa->ifa_addr == 0)
          {
            continue;
          }

        // Check to see if it's up.
        if ((ifa->ifa_flags & IFF_UP) != IFF_UP)
          {
            continue;
          }

        if (ifa->ifa_addr->sa_family == AF_INET)
          {
            s = ::getnameinfo (ifa->ifa_addr,
                               sizeof(struct sockaddr_in),
                               host, hostlen,
                               0, 0, NI_NUMERICHOST);
            if (s != 0)
              {
                ::std::cout << "WARNING: getnameinfo() failed: " << ::gai_strerror (s) << ::std::endl;
                continue;
              }
            ::std::string hostStr (host);
            if (hostStr.rfind ("127.", 0) == 0)
              {
                if (!hasLOv4)
                  {
                    hasLOv4 = true;
                    ::std::cout << "LOv4: " << ifa->ifa_name << " " << host << ::std::endl;
                  }
              }
            else if (hostStr.rfind ("0.", 0) != 0 &&
                     hostStr.rfind ("169.254.", 0) != 0)
              {
                if (!hasETHv4)
                  {
                    hasETHv4 = true;
                    ::std::cout << "ETHv4: " << ifa->ifa_name << " " << host << ::std::endl;
                  }
              }
          }

        else if (ifa->ifa_addr->sa_family == AF_INET6)
          {
            struct sockaddr_in6 *addr = reinterpret_cast <sockaddr_in6 *> (ifa->ifa_addr);
            s = ::getnameinfo (ifa->ifa_addr,
                               sizeof(struct sockaddr_in6),
                               host, hostlen,
                               0, 0, NI_NUMERICHOST);
            if (s != 0)
              {
                ::std::cout << "WARNING: getnameinfo() failed: " << ::gai_strerror (s) << ::std::endl;
                continue;
              }
            if (IN6_IS_ADDR_LOOPBACK (&addr->sin6_addr) && (ifa->ifa_flags & IFF_LOOPBACK) == IFF_LOOPBACK)
              {
                if (!hasLOv6)
                  {
                    hasLOv6 = true;
                    ::std::cout << "LOv6: " << ifa->ifa_name << " " << host << ::std::endl;
                  }
              }
            else if (!IN6_IS_ADDR_UNSPECIFIED (&addr->sin6_addr) &&
                     !IN6_IS_ADDR_LINKLOCAL (&addr->sin6_addr) &&
                     !IN6_IS_ADDR_SITELOCAL (&addr->sin6_addr) &&
                     !IN6_IS_ADDR_V4MAPPED (&addr->sin6_addr) &&
                     !IN6_IS_ADDR_V4COMPAT (&addr->sin6_addr) &&
                     !IN6_IS_ADDR_MC_NODELOCAL (&addr->sin6_addr))
              {
                if (!hasETHv6)
                  {
                    hasETHv6 = true;
                    ::std::cout << "ETHv6: " << ifa->ifa_name << " " << host << ::std::endl;
                  }
              }
          }

        if (hasLOv4 && hasLOv6 && hasETHv4 && hasETHv6)
          {
            break;
          }
      }

    ::freeifaddrs (ifaddr);

    return hasETHv4 || hasETHv6 ? 0 : 1;
  }

#endif
