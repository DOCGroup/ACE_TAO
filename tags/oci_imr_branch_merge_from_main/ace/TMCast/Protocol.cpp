// file      : TMCast/Protocol.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "Protocol.hpp"

namespace TMCast
{
  namespace Protocol
  {
    namespace
    {
      char const* labels[] = {
        "NONE", "BEGIN", "COMMIT", "ABORT", "COMMITED", "ABORTED"};
    }

    /*
      std::string
      tslabel (Protocol::TransactionStatus s)
      {
      return labels[s];
      }

      std::ostream&
      operator << (std::ostream& o, Transaction const& t)
      {
      return o << "{" << t.id << "; " << tslabel (t.status) << "}";
      }
    */
  }
}
