// file      : CIDLC/UnescapedNamePrinter.hpp
// author    : Boris Kolpackov <boris@codesynthesis.com>
// cvs-id    : $Id: Collectors.hpp 55138 2004-01-05 07:53:05Z parsons $

#ifndef UNESCAPED_NAME_PRINTER_HPP
#define UNESCAPED_NAME_PRINTER_HPP

#include "CCF/IDL2/SemanticGraph/Name.hpp"

#include <ostream>

// Prints names in the unescaped form.
//
struct UnescapedNamePrinter: CCF::IDL2::SemanticGraph::NamePrinter
{
  virtual void
  print (std::ostream& os, CCF::IDL2::SemanticGraph::SimpleName const& n)
  {
    os << n.unescaped_str ();
  }
};

#endif // UNESCAPED_NAME_PRINTER_HPP
