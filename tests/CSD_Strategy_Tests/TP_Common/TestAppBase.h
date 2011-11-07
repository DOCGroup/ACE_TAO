// $Id$
#ifndef TESTAPPBASE_H
#define TESTAPPBASE_H

#include "CSD_TP_Test_Export.h"
#include "ace/SString.h"
#include "tao/Environment.h"
#include "ace/CORBA_macros.h"

class CSD_TP_Test_Export TestAppBase
{
  public:

    virtual ~TestAppBase();

    // Returns 0 for success, and -1 for failure.
    int run (int argc, ACE_TCHAR *argv[]);

    const char *name() const;

  protected:

    TestAppBase (const char* name);

    // Returns -1 for failure, 0 for success
    virtual int run_i (int argc, ACE_TCHAR *argv[]) = 0;

  private:

    ACE_CString name_;
};

#endif
