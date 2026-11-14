#ifndef SERVERAPP_H
#define SERVERAPP_H

#include "ace/SString.h"
#include "ace/CORBA_macros.h"
#include "tao/Environment.h"

class ServerApp
{
  public:
    ServerApp () = default;
    ~ServerApp () = default;

    ServerApp (ServerApp const &) = delete;
    ServerApp (ServerApp &&) = delete;
    ServerApp &operator= (ServerApp const &) = delete;
    ServerApp &operator= (ServerApp &&) = delete;

    int run (int argc, ACE_TCHAR *argv[]);

  private:
    struct Options
    {
      ACE_TString ior_filename {ACE_TEXT ("server.ior")};
      unsigned num_clients {1};
    };

    int parse_args (int argc, ACE_TCHAR *argv[]);
    void usage_statement () const;

    ACE_TString exe_name_ {};
    Options options_ {};
};

#endif
