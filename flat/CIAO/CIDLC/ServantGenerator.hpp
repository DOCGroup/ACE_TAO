// file      : CIDLC/ServantGenerator.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef SERVANT_GENERATOR_HPP
#define SERVANT_GENERATOR_HPP

#include <string>
#include <iosfwd>

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

class ServantGenerator
{
public:
  static void
  options (CL::Description& d);

  ServantGenerator (CommandLine const& cl);

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


#endif // SERVANT_GENERATOR_HPP

/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
