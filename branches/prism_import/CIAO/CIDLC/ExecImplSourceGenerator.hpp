// file      : CIDLC/ExecImplSourceGenerator.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef EXEC_IMPL_SOURCE_GENERATOR_HPP
#define EXEC_IMPL_SOURCE_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

using std::string;

class ExecImplSourceEmitter
{
public:
  ExecImplSourceEmitter (std::ostream& os_,
                         CommandLine const& cl,
                         string export_macro,
                         fs::path const& file);

  virtual ~ExecImplSourceEmitter () {}

  virtual void
  pre (CCF::CIDL::SemanticGraph::TranslationUnit& u);

  virtual void
  generate (CCF::CIDL::SemanticGraph::TranslationUnit& u);

  virtual void
  post (CCF::CIDL::SemanticGraph::TranslationUnit& u);

private:
  std::ostream& os;
  CommandLine const& cl_;
  string export_macro_;
  fs::path file_;
};


#endif // EXEC_IMPL_SOURCE_GENERATOR_HPP

