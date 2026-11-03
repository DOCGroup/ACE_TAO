1i\
#include "fe_private.h"\
#include <cstring>\

/^#include <cstring>$/d
s/,}/, nullptr}/g
s/ -p -K keyword_/ -p\
 * -K keyword_/
s/ + asso_values/\
    + asso_values/g
s/},{/},\
      {/g
s/const struct TAO_IDL_CPP_Keyword_Entry \*/TAO_IDL_CPP_Keyword_Entry const */g
s/const char \*/char const */g
s/\*str == \*s && !std::strncmp/len == std::strlen (s) \&\& *str == *s \&\& !std::strncmp/g
s/ \&\& !std::strncmp/ \&\&\
              !std::strncmp/g
s/return 0;/return nullptr;/g
