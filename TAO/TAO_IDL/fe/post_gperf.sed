1i\
#include "fe_private.h"\
#include <cstring>\

s/,}/,0}/g
s/\*str == \*s && !ACE_OS::strncmp/len == std::strlen (s) \&\& *str == *s \&\& !std::strncmp/g
s/ \&\& !std::strncmp/ \&\&\
              !std::strncmp/g
s/return 0;/return nullptr;/g
