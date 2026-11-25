// Compile the traversal templates with the min/max macros used by Windows.
#include <cstddef>
#include <cstdlib>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "ace/XML_Utils/XSCRT/ExtendedTypeInfo.hpp"

#undef min
#undef max
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#include "ace/XML_Utils/XSCRT/Traversal.hpp"
#undef max
#undef min
