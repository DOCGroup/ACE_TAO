/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//   stl.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#include<map>
#include<vector>
#include<string>
#include<set>
#include<deque>
#include<list>
#include<queue>
#include<stack>

//#if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB) && \
            //(ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB != 0)
using std::map;
using std::vector;
using std::string;
using std::set;
using std::deque;
using std::list;
using std::queue;
using std::stack;
using std::less;
using std::pair;
using std::multimap;

//#endif // ACE_USES



