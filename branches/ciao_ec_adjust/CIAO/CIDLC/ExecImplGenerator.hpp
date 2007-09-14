// file      : CIDLC/ExecImplGenerator.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef EXEC_IMPL_GENERATOR_HPP
#define EXEC_IMPL_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

class ExecImplGenerator
{
public:
  static void
  options (CL::Description& d);

  ExecImplGenerator (CommandLine const& cl);

  void
  generate (CCF::CIDL::SemanticGraph::TranslationUnit&,
            fs::path const& file);

private:
  void
  compute_export_macro (const fs::path& file_path);

  std::ostream&
  configure_stream (std::string const& suffix_option,
                    std::string const& default_suffix,
                    std::string const& regex_option,
                    fs::ofstream& ofs);

private:
  CommandLine const& cl_;
  std::string file_name_;
  std::string export_macro_;
};


#endif // EXEC_IMPL_GENERATOR_HPP

