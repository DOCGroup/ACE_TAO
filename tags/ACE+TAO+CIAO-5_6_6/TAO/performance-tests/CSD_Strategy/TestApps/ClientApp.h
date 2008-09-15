// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    ClientApp.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================
#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include "TestInf/TestAppBase.h"
#include "TestInf/ClientEngine.h"
#include "tao/ORB.h"
#include "ace/SString.h"


class ClientApp : public TestAppBase
{
  public:

    ClientApp();
    virtual ~ClientApp();


  protected:

    virtual int run_i(int argc, char* argv[]);


  private:

    // These are all called, in order, by the run_i() method.
    int init(int argc, char* argv[]);
    void client_setup(void);
    int  run_engine(void);
    void cleanup();

    // Helper methods used by the methods above.
    int parse_args(int argc, char* argv[]);

    int set_arg(unsigned&   value,
                 const char* arg,
                 char        opt,
                 const char* name,
                 int         min = 0);

    void usage_statement();
    int arg_dependency_checks();


    CORBA::ORB_var      orb_;
    ACE_CString         exe_name_;
    ACE_CString         ior_;
    ClientEngine_Handle engine_;
    unsigned            client_id_;
    unsigned            num_loops_;
};

#endif
