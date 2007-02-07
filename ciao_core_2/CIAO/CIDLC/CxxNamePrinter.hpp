// file      : CIDLC/CxxNamePrinter.hpp
// author    : Boris Kolpackov <boris@codesynthesis.com>
// cvs-id    : $Id: Collectors.hpp 55138 2004-01-05 07:53:05Z parsons $

#ifndef CXX_NAME_PRINTER_HPP
#define CXX_NAME_PRINTER_HPP

#include "CCF/IDL2/SemanticGraph/Name.hpp"

// Prints names while escaping C++ keywords with the _cxx_ prefix.
//
struct CxxNamePrinter: CCF::IDL2::SemanticGraph::NamePrinter
{
  virtual void
  print (std::ostream&, CCF::IDL2::SemanticGraph::SimpleName const&);
};

#endif // CXX_NAME_PRINTER_HPP
