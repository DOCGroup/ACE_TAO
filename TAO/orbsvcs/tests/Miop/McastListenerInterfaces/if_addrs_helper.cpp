#include <iostream>
#include <string>

#define IPV4_MIN_LENGTH 7
#define IPV6_MIN_LENGTH 3

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

class WinsockHelper
  {
  public:
    WinsockHelper () : status (-1)
      {
        WORD wVersionRequested = MAKEWORD (2, 2);
        WSADATA wsaData;

        int err = WSAStartup (wVersionRequested, &wsaData);
        if (err != 0)
          {
            ::std::cout << "WSAStartup failed with error: " << err << ::std::endl;
            this->status = 1;
            return;
          }

        this->status = 0;
      }

    ~WinsockHelper ()
      {
        WSACleanup ();
      }

    int status;
  };

class AddressesHelper
  {
  public:
    AddressesHelper () : pAddresses (nullptr), status (-2)
      {
        DWORD dwRetVal = 0;
        ULONG iterations = 0;

        // Allocate a 15 KB buffer to start with.
        ULONG outBufLen = WORKING_BUFFER_SIZE;

        do
          {
            this->pAddresses = reinterpret_cast <IP_ADAPTER_ADDRESSES *> (MALLOC (outBufLen));
            if (this->pAddresses == nullptr)
            {
              ::std::cout << "Memory allocation failed for IP_ADAPTER_ADDRESSES struct" << ::std::endl;
              this->status = 2;
              return;
            }

            dwRetVal = ::GetAdaptersAddresses (AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, this->pAddresses, &outBufLen);

            if (dwRetVal == ERROR_BUFFER_OVERFLOW)
              {
                  FREE (this->pAddresses);
                  this->pAddresses = nullptr;
              }
            else
              {
                  break;
              }

            iterations++;
          } while (dwRetVal == ERROR_BUFFER_OVERFLOW && iterations < MAX_TRIES);

        if (dwRetVal != NO_ERROR)
          {
            ::std::cout << "Call to GetAdaptersAddresses failed with error: " << dwRetVal << ::std::endl;
            this->status = 2;
            return;
          }

        this->status = 0;
      }

    ~AddressesHelper ()
      {
        FREE (this->pAddresses);
      }

    PIP_ADAPTER_ADDRESSES pAddresses;
    int status;
  };

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

int
address_to_string (LPSOCKADDR sa, ::std::string & host)
  {
    static const DWORD ipv4_struct_length (sizeof (struct sockaddr_in));
    static const DWORD ipv6_struct_length (sizeof (struct sockaddr_in6));
    static const DWORD host_length = 100;

    ::std::string::size_type min_length (IPV4_MIN_LENGTH);
    DWORD struct_length (ipv4_struct_length);
    if (sa->sa_family == AF_INET6)
      {
        min_length = IPV6_MIN_LENGTH;
        struct_length = ipv6_struct_length;
      }

    DWORD length (host_length);
    wchar_t host_wchars [host_length];

    int err = ::WSAAddressToStringW (sa, struct_length, nullptr, host_wchars, &length);
    if (err != 0)
      {
        ::std::cout << "WARNING: WSAAddressToString() failed: WSAGetLastError() = " <<
                       ::WSAGetLastError () << ::std::endl;
        return 1;
      }
    ::std::wstring whost (host_wchars);
    // IP addresses never need > 1 byte/character
    host = ::std::wstring_convert<::std::codecvt_utf8<wchar_t>, wchar_t> ().to_bytes (whost);
    if (host.length () < min_length)
      {
        ::std::cout << "WARNING: WSAAddressToString() returned a too-short value: " << host << ::std::endl;
        return 1;
      }

    return 0;
  }

int
ACE_TMAIN (int, ACE_TCHAR * [])
  {
    WinsockHelper winsock;
    if (winsock.status != 0)
      {
        return winsock.status;
      }

    AddressesHelper addresses;
    if (addresses.status != 0)
      {
        return addresses.status;
      }

    bool hasLOv4 (false), hasETHv4 (false), hasLOv6 (false), hasETHv6 (false);

    while (addresses.pAddresses)
      {
        if (addresses.pAddresses->OperStatus != IfOperStatusUp)
          {
            continue;
          }

        IP_ADAPTER_UNICAST_ADDRESS *uni = 0;
        for (uni = addresses.pAddresses->FirstUnicastAddress; uni != 0; uni = uni->Next)
          {
            LPSOCKADDR sa = uni->Address.lpSockaddr;
            if (sa->sa_family != AF_INET && sa->sa_family != AF_INET6)
              {
                continue;
              }

            ::std::string host;
            if (address_to_string (sa, host) != 0)
              {
                continue;
              }

            if (sa->sa_family == AF_INET)
              {
                if (host.rfind ("127.", 0) == 0)
                  {
                    if (!hasLOv4)
                      {
                        hasLOv4 = true;
                        ::std::cout << "LOv4: " << addresses.pAddresses->AdapterName << " " << host << ::std::endl;
                      }
                  }
                else if (host.rfind ("0.", 0) != 0 &&
                         host.rfind ("169.254.", 0) != 0)
                  {
                    if (!hasETHv4)
                      {
                        hasETHv4 = true;
                        ::std::cout << "ETHv4: " << addresses.pAddresses->AdapterName << " " << host << ::std::endl;
                      }
                  }
              }
            else
              {
                struct sockaddr_in6 *sin = reinterpret_cast <sockaddr_in6 *> (sa);
                if (is_loopback (&sin->sin6_addr))
                  {
                    if (!hasLOv6)
                      {
                        hasLOv6 = true;
                        ::std::cout << "LOv6: " << addresses.pAddresses->AdapterName << " " << host << ::std::endl;
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
                        ::std::cout << "ETHv6: " << addresses.pAddresses->AdapterName << " " << host << ::std::endl;
                      }
                  }
              }

            if (hasLOv4 && hasLOv6 && hasETHv4 && hasETHv6)
              {
                break;
              }
          }

        addresses.pAddresses = addresses.pAddresses->Next;
      }

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
address_to_string (struct sockaddr *ifa_addr, ::std::string & host)
  {
    static const size_t ipv4_struct_length (sizeof (struct sockaddr_in));
    static const size_t ipv6_struct_length (sizeof (struct sockaddr_in6));
    static const size_t host_length = 100;

    ::std::string::size_type min_length (IPV4_MIN_LENGTH);
    size_t struct_length (ipv4_struct_length);
    if (ifa_addr->sa_family == AF_INET6)
      {
        min_length = IPV6_MIN_LENGTH;
        struct_length = ipv6_struct_length;
      }

    char host_chars [host_length];

    int err = ::getnameinfo (ifa_addr, struct_length, host_chars, host_length, 0, 0, NI_NUMERICHOST);
    if (err != 0)
      {
        ::std::cout << "WARNING: getnameinfo() failed: " << ::gai_strerror (err) << ::std::endl;
        return 1;
      }

    host = ::std::string (host_chars);
    if (host.length () < min_length)
      {
        ::std::cout << "WARNING: getnameinfo() returned a too-short value: " << host << ::std::endl;
        return 1;
      }

    return 0;
  }

int
ACE_TMAIN (int, ACE_TCHAR * [])
  {
    struct ifaddrs *ifaddr;
    if (::getifaddrs (&ifaddr) == -1)
      {
        ::perror ("getifaddrs");
        return 1;
      }

    bool hasLOv4 (false), hasETHv4 (false), hasLOv6 (false), hasETHv6 (false);

    for (struct ifaddrs *ifa = ifaddr; ifa != 0; ifa = ifa->ifa_next)
      {
        if (ifa->ifa_addr == 0 || (ifa->ifa_flags & IFF_UP) != IFF_UP || (
              ifa->ifa_addr->sa_family != AF_INET && ifa->ifa_addr->sa_family != AF_INET6))
          {
            continue;
          }

        ::std::string host;
        if (address_to_string (ifa->ifa_addr, host) != 0)
          {
            continue;
          }

        if (ifa->ifa_addr->sa_family == AF_INET)
          {
            if (host.rfind ("127.", 0) == 0)
              {
                if (!hasLOv4)
                  {
                    hasLOv4 = true;
                    ::std::cout << "LOv4: " << ifa->ifa_name << " " << host << ::std::endl;
                  }
              }
            else if (host.rfind ("0.", 0) != 0 &&
                     host.rfind ("169.254.", 0) != 0)
              {
                if (!hasETHv4)
                  {
                    hasETHv4 = true;
                    ::std::cout << "ETHv4: " << ifa->ifa_name << " " << host << ::std::endl;
                  }
              }
          }

        else
          {
            struct sockaddr_in6 *addr = reinterpret_cast <sockaddr_in6 *> (ifa->ifa_addr);
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
