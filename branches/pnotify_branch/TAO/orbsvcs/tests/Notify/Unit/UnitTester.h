// $Id$
// No real framework is provided per se, just a bunch of macros that
// throw an exception class that must then be handled by a main program.
// Each project, such as the Notification service, should define a
// single main() which calls functions that may throw UnitTestException.

#ifndef TAO_NOTIFY_UNITTESTER_H
#define TAO_NOTIFY_UNITTESTER_H
#include <tao/corba.h>
#include <ace/SString.h>

#ifdef ACE_HAS_EXCEPTIONS

class UnitTesterException {
public:
  static ACE_CString create_message(const ACE_CString& s, const ACE_CString& file, int line) {
    char tmp[100];
    ACE_OS::snprintf(tmp, 100, "%d", line);
    return s + ACE_CString("\n  ") + file + ACE_CString(":") + tmp;
  }

  UnitTesterException(const ACE_CString& s, const ACE_CString& file, int line)
    : msg_(create_message(s, file, line))
  {
  }
  const char* what() const {
    return msg_.c_str();
  }
private:
  ACE_CString msg_;
};

#define assertTrue(CONDITION) \
    if ((CONDITION) == false) { \
      throw UnitTesterException(#CONDITION, __FILE__, __LINE__); \
    }
#define failIt(MESSAGE)\
  throw UnitTesterException(#MESSAGE, __FILE__, __LINE__);

#define assertEquals(COND1, COND2) \
    if ((COND1) != (COND2)) { \
      throw UnitTesterException(#COND1" != "#COND2, __FILE__, __LINE__); \
    }

// Feel free to add other JUnit style assert tests here
#else /*ACE_HAS_EXCEPTIONS*/
#define assertTrue(CONDITION) \
    if ((CONDITION) == false) { \
      ACE_ERROR((LM_ERROR, "Test Failed : %s\n  %s:%d\n", #CONDITION, __FILE__, __LINE__));\
      ACE_OS::abort();\
    }
#define failIt(MESSAGE)\
    ACE_ERROR((LM_ERROR, "Test Failed : %s\n  %s:%d\n", #MESSAGE, __FILE__, __LINE__));\
    ACE_OS::abort();

#define assertEquals(COND1, COND2) \
    if ((COND1) != (COND2)) { \
      ACE_ERROR((LM_ERROR, "Test Failed : %s != %s\n%s:%d\n", #COND1, #COND2, __FILE__, __LINE__));\
      ACE_OS::abort();

// Feel free to add other JUnit style assert tests here
#endif /*ACE_HAS_EXCEPTIONS*/

#endif
