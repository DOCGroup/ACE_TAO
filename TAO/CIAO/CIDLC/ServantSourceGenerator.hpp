// file      : CIDLC/ServantSourceGenerator.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef SERVANT_SOURCE_GENERATOR_HPP
#define SERVANT_SOURCE_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

class ServantSourceEmitter
{
public:
  ServantSourceEmitter (std::ostream& os_,
                        CommandLine const& cl,
                        string export_macro,
                        fs::path const& file);


  virtual void
  pre (CCF::CIDL::SemanticGraph::TranslationUnit& u);

  virtual void
  generate (CCF::CIDL::SemanticGraph::TranslationUnit& u);

private:
  std::ostream& os;
  CommandLine const& cl_;
  std::string export_macro_;
  fs::path file_;
};

#endif // SERVANT_SOURCE_GENERATOR_HPP

/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
