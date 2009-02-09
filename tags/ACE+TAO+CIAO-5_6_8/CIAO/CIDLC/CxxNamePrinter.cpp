// file      : CIDLC/CxxNamePrinter.cpp
// author    : Boris Kolpackov <boris@codesynthesis.com>
// cvs-id    : $Id$

#include "CIDLC/CxxNamePrinter.hpp"

#include <ostream>
#include <algorithm>

namespace
{
  // Sorted C++ keywords.
  //
  const char* keywords[] = {
    "and",
    "asm",
    "auto",
    "bitand",
    "bitor",
    "bool",
    "break",
    "case",
    "catch",
    "char",
    "class",
    "compl",
    "const",
    "const_cast",
    "continue",
    "default",
    "delete",
    "do",
    "double",
    "dynamic_cast",
    "else",
    "end_eq",
    "enum",
    "explicit",
    "export",
    "extern",
    "false",
    "float",
    "for",
    "friend",
    "goto",
    "if",
    "inline",
    "int",
    "long",
    "mutable",
    "namespace",
    "new",
    "not",
    "not_eq",
    "operator",
    "or",
    "or_eq",
    "private",
    "protected",
    "public",
    "register",
    "reinterpret_cast",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "static_cast",
    "struct",
    "switch",
    "template",
    "this",
    "throw",
    "true",
    "try",
    "typedef",
    "typeid",
    "typename",
    "union",
    "unsigned",
    "using",
    "virtual",
    "void",
    "volatile",
    "wchar_t",
    "while",
    "xor",
    "xor_eq"
  };
}

void CxxNamePrinter::
print (std::ostream& os, CCF::IDL2::SemanticGraph::SimpleName const& n)
{
  std::size_t const size (sizeof (keywords) / sizeof (char*));
  std::string const& str (n.unescaped_str ());

  if (std::binary_search (keywords, keywords + size, str))
    os << "_cxx_";

  os << str;
}

