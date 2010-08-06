// file      : CIDLC/ServantHeaderGenerator.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef SERVANT_HEADER_GENERATOR_HPP
#define SERVANT_HEADER_GENERATOR_HPP

#include <string>
#include <iosfwd>

#include "EmitterContext.hpp"

#include "CCF/CodeGenerationKit/CommandLine.hpp"

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

class ServantHeaderEmitter
{
public:
  ServantHeaderEmitter (std::ostream& os_,
                        CommandLine const& cl,
                        std::string export_macro,
                        fs::path const& file);

  virtual ~ServantHeaderEmitter () {}

  /// Called by the backend driver.
  virtual void
  generate (CCF::CIDL::SemanticGraph::TranslationUnit& u);

private:
  /// Called from generate().

  virtual void
  pre (CCF::CIDL::SemanticGraph::TranslationUnit& u);

  virtual void
  generate_facets (CCF::CIDL::SemanticGraph::TranslationUnit& u,
                   Context& c);

  virtual void
  post (CCF::CIDL::SemanticGraph::TranslationUnit& u);

private:
  std::ostream& os;
  CommandLine const& cl_;
  std::string export_macro_;
  fs::path file_;
};


#endif // SERVANT_HEADER_GENERATOR_HPP

