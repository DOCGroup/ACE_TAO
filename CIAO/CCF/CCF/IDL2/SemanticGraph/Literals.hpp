// file      : CCF/IDL2/SemanticGraph/Literals.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_LITERALS_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_LITERALS_HPP

#include <string>
#include <iosfwd>

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      //
      //
      //
      class StringLiteral
      {
      public:
        StringLiteral (std::string const& literal)
            : literal_ (literal)
        {
        }

        std::string
        literal () const
        {
          return literal_;
        }

      private:
        std::string literal_;
      };

      inline bool
      operator== (StringLiteral const& a, StringLiteral const& b)
      {
        return a.literal () == b.literal ();
      }

      inline bool
      operator!= (StringLiteral const& a, StringLiteral const& b)
      {
        return a.literal () != b.literal ();
      }
    }
  }
}

std::ostream&
operator<< (std::ostream&, CCF::IDL2::SemanticGraph::StringLiteral const&);

#endif  // CCF_IDL2_SEMANTIC_GRAPH_LITERALS_HPP
