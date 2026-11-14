#ifndef SERVERAPP_H
#define SERVERAPP_H

#include "TestAppBase.h"
#include "ace/SString.h"

class ServerApp : public TestAppBase
{
public:
  ServerApp();
  ~ServerApp() override = default;

protected:
  int run_i (int argc, ACE_TCHAR *argv[]) override;

  ServerApp (ServerApp const &) = delete;
  ServerApp (ServerApp &&) = delete;
  ServerApp & operator= (ServerApp const &) = delete;
  ServerApp & operator= (ServerApp &&) = delete;

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);

  ACE_TString ior_filename_;
  unsigned    num_clients_;
};

#endif
