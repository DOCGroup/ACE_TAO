#ifndef Bug_3647_Regression_Throw_Spec_hpp
#define Bug_3647_Regression_Throw_Spec_hpp

#include "ace/Version.h"

#if ACE_MINOR_VERSION >= 6
# define THROW_SPEC(X) /**/
#else
# define THROW_SPEC(X) throw(X)
#endif // ACE_MINOR_VERSION

#endif // Bug_3647_Regression_Throw_Spec_hpp
