// -*- C++ -*-

// $Id$

extern "C" {
#include <sys/types.h>
};

#include "ace/Get_Opt.h"

// make sure that the code compiles cleanly even if SCTP is not
// available. If SCTP is not installed, program will exit early in
// main() with an error message.
#ifdef ACE_HAS_SCTP
extern "C" {
#include <netinet/in.h>
#include <netinet/sctp.h>
};
#else
#ifndef IPPROTO_SCTP
#define IPPROTO_SCTP 132 /* always the same value on every platform */ 
#endif
#endif

#include "Options_Manager.h"
#include "ace/streams.h"

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
    ACE_OS::strcpy(server_host, "localhost");

    // Remember argv[0]
    if (strlen(argv[0]) < Options_Manager::string_len)
      ACE_OS::strcpy(__program_name, argv[0]);
    else{
      ACE_OS::strncpy(__program_name, argv[0], (Options_Manager::string_len-1));
      __program_name[Options_Manager::string_len] = '\0';
    }

    // Declare options with ACE_Get_Opt
    int c;
    ACE_Get_Opt * get_opt=NULL;
    if (!ACE_OS::strcmp("client-opts", opts_set)){
      get_opt = new ACE_Get_Opt(argc, argv, ACE_TEXT("c:nt:m:M:x:b:C:i:p:H:s:h"));

      get_opt->long_option("test_iterations",         'c', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("test_enable_nagle",       'n');
      get_opt->long_option("test_transport_protocol", 't', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("histogram_min_bin",       'm', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("histogram_max_bin",       'M', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("histogram_num_outliers",  'x', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("histogram_bin_count",     'b', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("client_port",             'C', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("client_accept_addr",      'i', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("server_port",             'p', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("server_host",             'H', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("payload_size_power_of_2", 's', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("help",                    'h');
    } else if (!ACE_OS::strcmp("server-opts", opts_set)){
      get_opt = new ACE_Get_Opt(argc, argv, ACE_TEXT("nt:p:a:u"));
      get_opt->long_option("test_enable_nagle",       'n');
      get_opt->long_option("test_transport_protocol", 't', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("server_port",             'p', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("server_accept_addr",      'a', ACE_Get_Opt::ARG_REQUIRED);
      get_opt->long_option("help",                    'h');
    } else {
      _error = 1;
      _error_message = "invalid options set specified";
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
            char const * const str=get_opt->opt_arg();
            if (!ACE_OS::strcmp(str, "tcp"))
              test_transport_protocol = IPPROTO_TCP;
            else if (!ACE_OS::strcmp(str, "sctp"))
              test_transport_protocol = IPPROTO_SCTP;
            else
              test_transport_protocol = -1;
            break;
          }
          case 'm':
            histogram_min_bin      = ACE_OS_String::strtod(get_opt->opt_arg (), 0);
            break;
          case 'M':
            histogram_max_bin      = ACE_OS_String::strtod(get_opt->opt_arg (), 0);
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
            char str[Options_Manager::string_len];
            ACE_OS::strncpy(str, get_opt->opt_arg(), Options_Manager::string_len);

            // Get a pointer to the first comma in the list
            char *next_secondary_addr = ACE_OS::strchr(str, ',');

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
            int aton_retval = ACE_OS::inet_aton(str, &foo);

            // If this representation was not obtained, terminate with
            // an error.
            if (!aton_retval) {

              char error_message[Options_Manager::string_len + 100];
              ACE_OS::sprintf(error_message,
                              "Could not make sense of primary "
                              "address: %s",
                              str);

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
              char *next_next_secondary_addr = ACE_OS::strchr(next_secondary_addr, ',');

              // If found, the comma is replaced with \0 and pointer
              // updated to point to the string that begins immediately
              // after the comma.
              if (next_next_secondary_addr) {
                *next_next_secondary_addr = '\0';
                ++next_next_secondary_addr;
              }

              // Obtain the 32-bit, host-byte-order representation of
              // a secondary address.
              aton_retval = ACE_OS::inet_aton(next_secondary_addr, &foo);

              // If the representation was obtained without error,
              // store it in the next available slot of the
              // secondary_connect_addrs array.  Otherwise, terminate
              // with an error.
              if (aton_retval) {
                secondary_connect_addrs[num_secondary_connect_addrs++] =
                  ntohl(foo.s_addr);
              } else {

                char error_message[Options_Manager::string_len + 100];
                ACE_OS::sprintf(error_message,
                                "Could not make sense of secondary "
                                "address: %s",
                                next_secondary_addr);

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
            server_port            = ACE_OS::atoi(get_opt->opt_arg ());
            break;
          case 'a':{

            // The argument to this option is a comma-separated list
            // of dotted-decimal ipv4 addresses.

            // Create a writable copy of the options argument
            char str[Options_Manager::string_len];
            ACE_OS::strncpy(str, get_opt->opt_arg(), Options_Manager::string_len);

            // Get a pointer to the first comma in the list
            char *next_secondary_addr = ACE_OS::strchr(str, ',');

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
            int aton_retval = ACE_OS::inet_aton(str, &foo);

            // If this representation was not obtained, terminate with
            // an error.
            if (!aton_retval) {

              char error_message[Options_Manager::string_len + 100];
              ACE_OS::sprintf(error_message,
                              "Could not make sense of primary "
                              "address: %s",
                              str);

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
              char *next_next_secondary_addr = ACE_OS::strchr(next_secondary_addr, ',');

              // If found, the comma is replaced with \0 and pointer
              // updated to point to the string that begins immediately
              // after the comma.
              if (next_next_secondary_addr) {
                *next_next_secondary_addr = '\0';
                ++next_next_secondary_addr;
              }

              // Obtain the 32-bit, host-byte-order representation of
              // a secondary address.
              aton_retval = ACE_OS::inet_aton(next_secondary_addr, &foo);

              // If the representation was obtained without error,
              // store it in the next available slot of the
              // secondary_accept_addrs array.  Otherwise, terminate
              // with an error.
              if (aton_retval) {
                secondary_accept_addrs[num_secondary_accept_addrs++] =
                  ntohl(foo.s_addr);
              } else {

                char error_message[Options_Manager::string_len + 100];
                ACE_OS::sprintf(error_message,
                                "Could not make sense of secondary "
                                "address: %s",
                                next_secondary_addr);

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
            char const * const str=get_opt->opt_arg();
            if (strlen(str) < Options_Manager::string_len)
              ACE_OS::strcpy(server_host, str);
            else{
              ACE_OS::strncpy(server_host, str, (Options_Manager::string_len-1));
              server_host[Options_Manager::string_len] = '\0';
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
        _error_message = "test_iterations must be no less than than 1";
      }

    if (histogram_min_bin < 0.0)
      {
        _error = 1;
        _error_message = "histogram_min_bin must be no less than 0.0";
      }

    if (histogram_max_bin < histogram_min_bin)
      {
        _error = 1;
        _error_message = "histogram_max_bin must be no less than histogram_min_bin";
      }

    if (histogram_num_outliers < 1)
      {
        _error = 1;
        _error_message = "histogram_num_outliers must be no less than 1";
      }

    if (histogram_bin_count < 1)
      {
        _error = 1;
        _error_message = "histogram_bin_count must be no less than 1";
      }

    if (server_port < 1010 ||
        server_port > 65000)
      {
        _error = 1;
        _error_message = "server_port must be between 1010 and 65000 inclusive";
      }

    if ((!ACE_OS::strcmp("client-opts", opts_set)) && payload_size_power_of_2 > 17)
      {
        _error = 1;
        _error_message = "payload_size_power_of_2 must be specified between 0 and 16 inclusive";
      }

    if (test_transport_protocol == -1) {
      _error = 1;
      _error_message = "test_transport_protocol may only take 'sctp' and 'tcp' as values";
    }

    __initialized = 1;

    delete get_opt;
  }
}

void Options_Manager::_show_usage(ostream& out, ACE_TCHAR const * const opts_set)
{
  // Show usage message.  KEEP THE DEFAULTS DISPLAYED HERE IN SYNC
  // WITH THE DEFAULTS SET AT THE BEGINNING OF THE CONSTRUCTOR.

  out << __program_name << " - Measures round trip latency statistics of ACE synchronous" << endl;
  // indent past program name
  for (unsigned int i=0;i<ACE_OS::strlen(__program_name);++i)
    out << " ";

  if (ACE_OS::strstr(__program_name, "SOCK_STREAM_clt") || ACE_OS::strstr(__program_name, "SOCK_STREAM_srv")) {
    out << "   messaging (SOCK_Stream) using unmarshalled ACE_CDR::Octet." << endl;
  } else {
    out << "   messaging (SOCK_SEQPACK) using unmarshalled ACE_CDR::Octet." << endl;
  }

  out << "USAGE: " << __program_name << " [ -<flag> [<val>] | --<name> [<val>] ]..." << endl;

  out << endl;

  if (!ACE_OS::strcmp("client-opts", opts_set)){
    out << "  Flag  Args           Option-Name                 Default"    << endl;
    out << "   -c   int            test-iterations             1000000"    << endl;
    out << "   -n   none           test-enable-nagle           NO NAGLING" << endl;
    if (ACE_OS::strstr(__program_name, "SOCK_STREAM_clt")) {
      out << "   -t   str (sctp|tcp) test-transport-protocol     sctp"       << endl;
    }

    out << endl;

    out << "   -m   dbl            histogram-min-bin           0"     << endl;
    out << "   -M   dbl            histogram-max-bin           10000" << endl;
    out << "   -x   int            histogram-num-outliers      100"   << endl;
    out << "   -b   int            histogram-bin-count         1000"  << endl;

    out << endl;

    out << "   -C   int            client-port                 assigned by kernel"     << endl;
    out << "   -i   str            client-connect-addr         INADDR_ANY" << endl;

    out << endl;

    out << "   -p   int            server-port                 45453"     << endl;
    out << "   -H   str            server-host                 localhost" << endl;

    out << endl;

    out << "   -s   int (0-16)     payload-size-power-of-2     <MUST SET VALUE>" << endl;

    out << endl;

    out << "   -h   none           help" << endl;

    out << endl;

  } else if (!ACE_OS::strcmp("server-opts", opts_set)){
    out << "  Flag  Args           Option-Name                 Default"    << endl;
    out << "   -n   none           test-enable-nagle           NO NAGLING" << endl;
    if (ACE_OS::strstr(__program_name, "SOCK_STREAM_srv")) {
      out << "   -t   str (sctp|tcp) test-transport-protocol     sctp"       << endl;
    }

    out << endl;

    out << "   -p   int            server-port                 45453"     << endl;

    if (ACE_OS::strstr(__program_name, "SOCK_SEQPACK_srv")) {
      out << "   -a   w.x.y.z,a.b.c.d,...  server-accept-addr    INADDR_ANY" << endl;
      out << "        (comma-separated                                     " << endl;
      out << "         list of one or more                                 " << endl;
      out << "         addresses)                                          " << endl;
    } else {
      out << "   -a   w.x.y.z        server-accept-addr          INADDR_ANY" << endl;
    }

    out << "   -h   none           help" << endl;

    out << endl;

  } else {
    out << "Invalid options set specified." << endl;
  }

  return;
}
