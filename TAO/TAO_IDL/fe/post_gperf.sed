s/,}/,0}/g
s/\*str == \*s && !ACE_OS::strncmp/len == static_cast<unsigned int> (ACE_OS::strlen (s)) \&\& *str == *s \&\& !ACE_OS::strncmp/g
