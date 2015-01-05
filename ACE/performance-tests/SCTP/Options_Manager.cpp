// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "ace/os_include/netinet/os_in.h"
#include "ace/os_include/sys/os_types.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"

// make sure that the code compiles cleanly even if SCTP is not
// available. If SCTP is not installed, program will exit early in
// main() with an error message.
#ifdef ACE_HAS_SCTP
extern "C" {
#include <netinet/sctp.h>
};
#else
#ifndef IPPROTO_SCTP
#define IPPROTO_SCTP 132 /* always the same value on every platform */
#endif
#endif

#include "Options_Manager.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_arpa_inet.h"

// Set default values
ACE_CDR::ULong Options_Manager::test_iterations=1000000;
ACE_CDR::Boolean Options_Manager::test_enable_nagle=0;
ACE_CDR::Long Options_Manager::test_transport_protocol=IPPROTO_SCTP;
ACE_CDR::Double Options_Manager::histogram_min_bin=0.0;
ACE_CDR::Double Options_Manager::histogram_max_bin=10000.0;
ACE_CDR::ULong Options_Manager::histogram_num_outliers=100;
ACE_CDR::ULong Options_Manager::histogram_bin_count=1000;
ACE_CDR::UShort Options_Manager::client_port = 0;
ACE_CDR::ULong Options_Manager::client_connect_addr=INADDR_ANY;
ACE_CDR::UShort Options_Manager::server_port = 45453;
ACE_TCHAR Options_Manager::server_host[Options_Manager::string_len];
ACE_CDR::ULong Options_Manager::server_accept_addr=INADDR_ANY;
ACE_CDR::UShort Options_Manager::payload_size_power_of_2=31;
ACE_CDR::ULong Options_Manager::secondary_connect_addrs[max_num_secondary_connect_addrs];
ACE_CDR::UShort Options_Manager::num_secondary_connect_addrs = 0;
ACE_CDR::ULong Options_Manager::secondary_accept_addrs[max_num_secondary_accept_addrs];
ACE_CDR::UShort Options_Manager::num_secondary_accept_addrs = 0;
ACE_CDR::UShort Options_Manager::_usage = 0;
ACE_CDR::UShort Options_Manager::_error = 0;
ACE_TCHAR Options_Manager::__program_name[Options_Manager::string_len];
ACE_CDR::Boolean Options_Manager::__initialized=0;
const ACE_TCHAR* Options_Manager::_error_message;


Options_Manager::Options_Manager(int argc, ACE_TCHAR **argv, ACE_TCHAR const * const opts_set)
{
  if (!__initialized) {
    // Set default values that were not set during static initialization
    ACE_OS::strcpy(server_host, ACE_LOCALHOST);

    // Remember argv[0]
    if (ACE_OS::strlen(argv[0]) < Options_Manager::string_len)
      ACE_OS::strcpy(__program_name, argv[0]);
    else{
      ACE_OS::strncpy(__program_name, argv[0], (Options_Manager::string_len-1));
      __program_name[Options_Manager::string_len - 1] = '\0';
    }

    // Declare options with ACE_Get_Opt
    int c;
    ACE_Get_Opt * get_opt = 0;
    if (!ACE_OS::strcmp(ACE_TEXT ("client-opts"), opts_set)){
      get_opt = new ACE_Get_Opt(argc, argv, ACE_TEXT("c:nt:m:M:x:b:C:i:p:H:s:h"));

      get_opt->long_option (ACE_TEXT ("test_iterations"),         'c',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("test_enable_nagle"),       'n');
      get_opt->long_option (ACE_TEXT ("test_transport_protocol"), 't',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("histogram_min_bin"),       'm',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("histogram_max_bin"),       'M',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("histogram_num_outliers"),  'x',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("histogram_bin_count"),     'b',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("client_port"),             'C',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("client_accept_addr"),      'i',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("server_port"),             'p',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("server_host"),             'H',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("payload_size_power_of_2"), 's',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("help"),                    'h');
    } else if (!ACE_OS::strcmp (ACE_TEXT ("server-opts"), opts_set)){
      get_opt = new ACE_Get_Opt(argc, argv, ACE_TEXT("nt:p:a:u"));
      get_opt->long_option (ACE_TEXT ("test_enable_nagle"),       'n');
      get_opt->long_option (ACE_TEXT ("test_transport_protocol"), 't',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("server_port"),             'p',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("server_accept_addr"),      'a',
                            ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option (ACE_TEXT ("help"),                    'h');
    } else {
      _error = 1;
      _error_message = ACE_TEXT ("invalid options set specified");
      delete get_opt;
      return;
    }

    // Parse options with ACE_Get_Opt
    while ((c = (*get_opt)()) != -1)
      {
        switch ((char) c)
          {
          case 'c':
            test_iterations        = ACE_OS::atoi(get_opt->opt_arg ());
            break;
          case 'n':
            test_enable_nagle     = 1;
            break;
          case 't':{
            ACE_TCHAR const * const str = get_opt->opt_arg ();
            if (!ACE_OS::strcmp (str, ACE_TEXT ("tcp")))
              test_transport_protocol = IPPROTO_TCP;
            else if (!ACE_OS::strcmp (str, ACE_TEXT ("sctp")))
              test_transport_protocol = IPPROTO_SCTP;
            else
              test_transport_protocol = -1;
            break;
          }
          case 'm':
            histogram_min_bin      = ACE_OS::strtod(get_opt->opt_arg (), 0);
            break;
          case 'M':
            histogram_max_bin      = ACE_OS::strtod(get_opt->opt_arg (), 0);
            break;
          case 'x':
            histogram_num_outliers = ACE_OS::atoi(get_opt->opt_arg ());
            break;
          case 'b':
            histogram_bin_count    = ACE_OS::atoi(get_opt->opt_arg ());
            break;
          case 'C':
            client_port            = ACE_OS::atoi(get_opt->opt_arg ());
            break;
          case 'i':{

            // The argument to this option is a comma-separated list
            // of dotted-decimal ipv4 addresses.

            // Create a writable copy of the options argument
            ACE_TCHAR str[Options_Manager::string_len];
            ACE_OS::strncpy(str, get_opt->opt_arg(), Options_Manager::string_len);

            // Get a pointer to the first comma in the list
            ACE_TCHAR *next_secondary_addr = ACE_OS::strchr(str, ',');

            // If found, the comma is replaced with \0 and pointer
            // updated to point to the string that begins immediately
            // after the comma.
            if (next_secondary_addr) {
              *next_secondary_addr = '\0';
              ++next_secondary_addr;
            }

            // Obtain the 32-bit, host-byte-order representation of
            // the primary address.
            struct in_addr foo;
            int aton_retval = ACE_OS::inet_aton(ACE_TEXT_ALWAYS_CHAR (str),
                                                &foo);

            // If this representation was not obtained, terminate with
            // an error.
            if (!aton_retval) {

              ACE_TCHAR error_message[Options_Manager::string_len + 100];
              ACE_OS::strcpy
                (error_message,
                 ACE_TEXT ("Could not make sense of primary address: "));
              ACE_OS::strcat (error_message, str);

              _error = 1;
              _error_message = ACE_OS::strdup(error_message);
              break;
            }

            // Otherwise, store the representation in the
            // client_connect_addr member variable.
            client_connect_addr = ntohl(foo.s_addr);

//              ACE_DEBUG ((LM_DEBUG,
//                          "Primary connect addr: %s     retval = %d\n",
//                          str,
//                      aton_retval));

//              if (next_secondary_addr) {
//                ACE_DEBUG ((LM_DEBUG,
//                            "Secondary addr(s) remaining to be parsed: %s\n",
//                            next_secondary_addr));
//              } else {
//                ACE_DEBUG ((LM_DEBUG,
//                            "No secondary addr remaining to be parsed.\n"));
//              }

            // The following loop parses secondary addresses from the
            // list.  The loop repeats as long as ACE_OS::strchr
            // returns non-null (i.e., as long as yet another comma is
            // found.
            while (next_secondary_addr &&
                   num_secondary_connect_addrs <
                   max_num_secondary_connect_addrs) {

              // Get a pointer to the next comma in the list.
              ACE_TCHAR *next_next_secondary_addr = ACE_OS::strchr(next_secondary_addr, ',');

              // If found, the comma is replaced with \0 and pointer
              // updated to point to the string that begins immediately
              // after the comma.
              if (next_next_secondary_addr) {
                *next_next_secondary_addr = '\0';
                ++next_next_secondary_addr;
              }

              // Obtain the 32-bit, host-byte-order representation of
              // a secondary address.
              aton_retval =
                ACE_OS::inet_aton(ACE_TEXT_ALWAYS_CHAR (next_secondary_addr),
                                  &foo);

              // If the representation was obtained without error,
              // store it in the next available slot of the
              // secondary_connect_addrs array.  Otherwise, terminate
              // with an error.
              if (aton_retval) {
                secondary_connect_addrs[num_secondary_connect_addrs++] =
                  ntohl(foo.s_addr);
              } else {

                ACE_TCHAR error_message[Options_Manager::string_len + 100];
                ACE_OS::strcpy
                  (error_message,
                   ACE_TEXT ("Could not make sense of secondary address: "));
                ACE_OS::strcat (error_message, next_secondary_addr);
                _error = 1;
                _error_message = ACE_OS::strdup(error_message);
                break;
              }

//                ACE_DEBUG ((LM_DEBUG,
//                            "secondary_addr[%d] = %s      retval = %d\n",
//                            num_secondary_connect_addrs - 1,
//                            next_secondary_addr,
//                        aton_retval));

              next_secondary_addr = next_next_secondary_addr;
            }

            break;
          }
          case 'p':
            server_port = ACE_OS::atoi(get_opt->opt_arg ());
            break;
          case 'a':{

            // The argument to this option is a comma-separated list
            // of dotted-decimal ipv4 addresses.

            // Create a writable copy of the options argument
            ACE_TCHAR str[Options_Manager::string_len];
            ACE_OS::strncpy(str, get_opt->opt_arg(), Options_Manager::string_len);

            // Get a pointer to the first comma in the list
            ACE_TCHAR *next_secondary_addr = ACE_OS::strchr(str, ',');

            // If found, the comma is replaced with \0 and pointer
            // updated to point to the string that begins immediately
            // after the comma.
            if (next_secondary_addr) {
              *next_secondary_addr = '\0';
              ++next_secondary_addr;
            }

            // Obtain the 32-bit, host-byte-order representation of
            // the primary address.
            struct in_addr foo;
            int aton_retval = ACE_OS::inet_aton (ACE_TEXT_ALWAYS_CHAR (str),
                                                 &foo);

            // If this representation was not obtained, terminate with
            // an error.
            if (!aton_retval) {

              ACE_TCHAR error_message[Options_Manager::string_len + 100];
              ACE_OS::strcpy
                (error_message,
                 ACE_TEXT ("Could not make sense of primary address: "));
              ACE_OS::strcat (error_message, str);

              _error = 1;
              _error_message = ACE_OS::strdup(error_message);
              break;
            }

            // Otherwise, store the representation in the
            // server_accept_addr member variable.
            server_accept_addr = ntohl(foo.s_addr);

//              ACE_DEBUG ((LM_DEBUG,
//                          "Primary accept addr: %s     retval = %d\n",
//                          str,
//                      aton_retval));

//              if (next_secondary_addr) {
//                ACE_DEBUG ((LM_DEBUG,
//                            "Secondary addr(s) remaining to be parsed: %s\n",
//                            next_secondary_addr));
//              } else {
//                ACE_DEBUG ((LM_DEBUG,
//                            "No secondary addr remaining to be parsed.\n"));
//              }

            // The following loop parses secondary addresses from the
            // list.  The loop repeats as long as ACE_OS::strchr
            // returns non-null (i.e., as long as yet another comma is
            // found.
            while (next_secondary_addr &&
                   num_secondary_accept_addrs <
                   max_num_secondary_accept_addrs) {

              // Get a pointer to the next comma in the list.
              ACE_TCHAR *next_next_secondary_addr = ACE_OS::strchr(next_secondary_addr, ',');

              // If found, the comma is replaced with \0 and pointer
              // updated to point to the string that begins immediately
              // after the comma.
              if (next_next_secondary_addr) {
                *next_next_secondary_addr = '\0';
                ++next_next_secondary_addr;
              }

              // Obtain the 32-bit, host-byte-order representation of
              // a secondary address.
              aton_retval =
                ACE_OS::inet_aton (ACE_TEXT_ALWAYS_CHAR (next_secondary_addr),
                                   &foo);

              // If the representation was obtained without error,
              // store it in the next available slot of the
              // secondary_accept_addrs array.  Otherwise, terminate
              // with an error.
              if (aton_retval) {
                secondary_accept_addrs[num_secondary_accept_addrs++] =
                  ntohl(foo.s_addr);
              } else {

                ACE_TCHAR error_message[Options_Manager::string_len + 100];
                ACE_OS::strcpy
                  (error_message,
                   ACE_TEXT ("Could not make sense of secondary address: "));
                ACE_OS::strcat (error_message, next_secondary_addr);

                _error = 1;
                _error_message = ACE_OS::strdup(error_message);
                break;
              }

//                ACE_DEBUG ((LM_DEBUG,
//                            "secondary_addr[%d] = %s      retval = %d\n",
//                            num_secondary_accept_addrs - 1,
//                            next_secondary_addr,
//                        aton_retval));

              next_secondary_addr = next_next_secondary_addr;
            }

            break;
          }
          case 'H':{
            ACE_TCHAR const * const str=get_opt->opt_arg();
            if (ACE_OS::strlen (str) < Options_Manager::string_len)
              ACE_OS::strcpy(server_host, str);
            else{
              ACE_OS::strncpy(server_host, str, (Options_Manager::string_len-1));
              server_host[Options_Manager::string_len - 1] = '\0';
            }
            break;
          }
          case 's':
            payload_size_power_of_2 = ACE_OS::atoi(get_opt->opt_arg ());
            break;
          case 'h':
            _usage = 1;
            break;
          }
      }

    // Check option values
    if (test_iterations < 1)
      {
        _error = 1;
        _error_message = ACE_TEXT ("test_iterations must be no less than than 1");
      }

    if (histogram_min_bin < 0.0)
      {
        _error = 1;
        _error_message = ACE_TEXT ("histogram_min_bin must be no less than 0.0");
      }

    if (histogram_max_bin < histogram_min_bin)
      {
        _error = 1;
        _error_message = ACE_TEXT ("histogram_max_bin must be no less than histogram_min_bin");
      }

    if (histogram_num_outliers < 1)
      {
        _error = 1;
        _error_message = ACE_TEXT ("histogram_num_outliers must be no less than 1");
      }

    if (histogram_bin_count < 1)
      {
        _error = 1;
        _error_message = ACE_TEXT ("histogram_bin_count must be no less than 1");
      }

    if ((server_port < 1010 ||
        server_port > 65000) && server_port != 0)
      {
        _error = 1;
        _error_message = ACE_TEXT ("server_port must be between 1010 and 65000 inclusive, or zero.");
      }

    if ((!ACE_OS::strcmp(ACE_TEXT ("client-opts"), opts_set)) && payload_size_power_of_2 > 17)
      {
        _error = 1;
        _error_message = ACE_TEXT("payload_size_power_of_2 must be specified between 0 and 16 inclusive");
      }

    if (test_transport_protocol == -1) {
      _error = 1;
      _error_message = ACE_TEXT ("test_transport_protocol may only take 'sctp' and 'tcp' as values");
    }

    __initialized = 1;

    delete get_opt;
  }
}

void Options_Manager::_show_usage(FILE* out, ACE_TCHAR const * const opts_set)
{
  // Show usage message.  KEEP THE DEFAULTS DISPLAYED HERE IN SYNC
  // WITH THE DEFAULTS SET AT THE BEGINNING OF THE CONSTRUCTOR.

  ACE_OS::fprintf (out, "%s - Measures round trip latency statistics of ACE synchronous\n",
                   ACE_TEXT_ALWAYS_CHAR (__program_name));
  // indent past program name
  for (unsigned int i=0;i<ACE_OS::strlen(__program_name);++i)
    ACE_OS::fprintf (out, " ");

  if (ACE_OS::strstr(__program_name, ACE_TEXT ("SOCK_STREAM_clt")) ||
      ACE_OS::strstr(__program_name, ACE_TEXT ("SOCK_STREAM_srv"))    ) {
    ACE_OS::fprintf (out, "   messaging (SOCK_Stream) using unmarshalled ACE_CDR::Octet.\n");
  } else {
    ACE_OS::fprintf (out, "   messaging (SOCK_SEQPACK) using unmarshalled ACE_CDR::Octet.\n");
  }

  ACE_OS::fprintf (out, "USAGE: %s [ -<flag> [<val>] | --<name> [<val>] ]...\n\n",
                   ACE_TEXT_ALWAYS_CHAR (__program_name));

  if (!ACE_OS::strcmp (ACE_TEXT ("client-opts"), opts_set)){
    ACE_OS::fprintf (out,   "  Flag  Args           Option-Name                 Default\n"
                            "   -c   int            test-iterations             1000000\n"
                            "   -n   none           test-enable-nagle           NO NAGLING\n");
    if (ACE_OS::strstr(__program_name, ACE_TEXT ("SOCK_STREAM_clt"))) {
      ACE_OS::fprintf (out, "   -t   str (sctp|tcp) test-transport-protocol     sctp\n");
    }

    ACE_OS::fprintf (out, "\n");

    ACE_OS::fprintf (out, "   -m   dbl            histogram-min-bin           0\n"
                          "   -M   dbl            histogram-max-bin           10000\n"
                          "   -x   int            histogram-num-outliers      100\n"
                          "   -b   int            histogram-bin-count         1000\n");

    ACE_OS::fprintf (out, "\n");

    ACE_OS::fprintf (out, "   -C   int            client-port                 assigned by kernel\n"
                          "   -i   str            client-connect-addr         INADDR_ANY\n");

    ACE_OS::fprintf (out, "\n");

    ACE_OS::fprintf (out, "   -p   int            server-port                 45453\n"
                          "   -H   str            server-host                 localhost\n");

    ACE_OS::fprintf (out, "\n");

    ACE_OS::fprintf (out, "   -s   int (0-16)     payload-size-power-of-2     <MUST SET VALUE>\n");

    ACE_OS::fprintf (out, "\n");

    ACE_OS::fprintf (out, "   -h   none           help\n");

    ACE_OS::fprintf (out, "\n");

  } else if (!ACE_OS::strcmp(ACE_TEXT ("server-opts"), opts_set)){
    ACE_OS::fprintf (out, "  Flag  Args           Option-Name                 Default\n"
                          "   -n   none           test-enable-nagle           NO NAGLING\n");
    if (ACE_OS::strstr(__program_name, ACE_TEXT ("SOCK_STREAM_srv"))) {
      ACE_OS::fprintf (out, "   -t   str (sctp|tcp) test-transport-protocol     sctp\n");
    }

    ACE_OS::fprintf (out, "\n");

    ACE_OS::fprintf (out, "   -p   int            server-port                 45453\n");

    if (ACE_OS::strstr(__program_name, ACE_TEXT ("SOCK_SEQPACK_srv"))) {
      ACE_OS::fprintf (out, "   -a   w.x.y.z,a.b.c.d,...  server-accept-addr    INADDR_ANY\n"
                            "        (comma-separated                                    \n"
                            "         list of one or more                                \n"
                            "         addresses)                                         \n");
    } else {
      ACE_OS::fprintf (out, "   -a   w.x.y.z        server-accept-addr          INADDR_ANY\n");
    }

    ACE_OS::fprintf (out, "   -h   none           help\n");

    ACE_OS::fprintf (out, "\n");

  } else {
    ACE_OS::fprintf (out, "Invalid options set specified.\n");
  }

  return;
}
