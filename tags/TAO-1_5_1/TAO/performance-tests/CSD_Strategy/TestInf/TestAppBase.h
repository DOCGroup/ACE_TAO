// $Id$
#ifndef TESTAPPBASE_H
#define TESTAPPBASE_H

#include "CSD_PT_TestInf_Export.h"
#include "ace/SString.h"
#include "tao/Environment.h"
#include "ace/CORBA_macros.h"


class CSD_PT_TestInf_Export TestAppBase
{
  public:

    virtual ~TestAppBase();

    // Returns 0 for success, and -1 for failure.
    int run(int argc, char* argv[] ACE_ENV_ARG_DECL);

    const char* name() const;


  protected:

    TestAppBase(const char* name);

    // Returns -1 for failure, 0 for success
    virtual int run_i(int argc, char* argv[] ACE_ENV_ARG_DECL) = 0;


  private:

    ACE_CString name_;
};

#endif
