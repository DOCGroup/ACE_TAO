#ifndef TESTAPPBASE_H
#define TESTAPPBASE_H

#include "CSD_PT_TestInf_Export.h"
#include "ace/SString.h"
#include "tao/Environment.h"
#include "ace/CORBA_macros.h"

class CSD_PT_TestInf_Export TestAppBase
{
  public:
    TestAppBase (TestAppBase const &) = delete;
    TestAppBase (TestAppBase &&) = delete;
    TestAppBase &operator= (TestAppBase const &) = delete;
    TestAppBase &operator= (TestAppBase &&) = delete;
    virtual ~TestAppBase ();

    // Returns 0 for success, and -1 for failure.
    int run (int argc, ACE_TCHAR *argv[]);

    char const *name () const;

  protected:
    explicit TestAppBase (char const *name);

    // Returns -1 for failure, 0 for success
    virtual int run_i (int argc, ACE_TCHAR *argv[]) = 0;

  private:
    ACE_CString name_;
};

#endif
