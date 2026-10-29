#ifndef ANNOTEST_IDL_BE_EXTERN_HEADER
#define ANNOTEST_IDL_BE_EXTERN_HEADER

#include <string>
#include <vector>

#include "ace/ACE.h"
#include "be_global.h"

extern BE_GlobalData *be_global;

extern int BE_init(int&, ACE_TCHAR*[]);
extern void BE_post_init (std::vector<std::string> const &);
extern void BE_version();
extern void BE_produce();
extern void BE_abort();
extern void BE_cleanup();

#endif

