// file      : CCF/IDL2/Traversal/Fundamental.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_FUNDAMENTAL_HPP
#define CCF_IDL2_TRAVERSAL_FUNDAMENTAL_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/Fundamental.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      typedef
      Node<SemanticGraph::FundamentalType>
      FundamentalType;

      typedef
      Node<SemanticGraph::Object>
      Object;

      typedef
      Node<SemanticGraph::ValueBase>
      ValueBase;

      typedef
      Node<SemanticGraph::Any>
      Any;

      typedef
      Node<SemanticGraph::Boolean>
      Boolean;

      typedef
      Node<SemanticGraph::Char>
      Char;

      typedef
      Node<SemanticGraph::Double>
      Double;

      typedef
      Node<SemanticGraph::Float>
      Float;

      typedef
      Node<SemanticGraph::Long>
      Long;

      typedef
      Node<SemanticGraph::LongDouble>
      LongDouble;

      typedef
      Node<SemanticGraph::LongLong>
      LongLong;

      typedef
      Node<SemanticGraph::Octet>
      Octet;

      typedef
      Node<SemanticGraph::Short>
      Short;

      typedef
      Node<SemanticGraph::String>
      String;

      typedef
      Node<SemanticGraph::UnsignedLong>
      UnsignedLong;

      typedef
      Node<SemanticGraph::UnsignedLongLong>
      UnsignedLongLong;

      typedef
      Node<SemanticGraph::UnsignedShort>
      UnsignedShort;

      typedef
      Node<SemanticGraph::Void>
      Void;

      typedef
      Node<SemanticGraph::Wchar>
      Wchar;

      typedef
      Node<SemanticGraph::Wstring>
      Wstring;
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_FUNDAMENTAL_HPP
