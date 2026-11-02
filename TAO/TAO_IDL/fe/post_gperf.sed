1i\
#include "fe_private.h"\
#include <cstring>\

/^#include <cstring>$/d
s/,}/,0}/g
s/\*str == \*s && !std::strncmp/len == std::strlen (s) \&\& *str == *s \&\& !std::strncmp/g
s/ \&\& !std::strncmp/ \&\&\
              !std::strncmp/g
s/return 0;/return nullptr;/g
