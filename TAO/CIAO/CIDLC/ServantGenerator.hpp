// $Id$
#ifndef SERVANT_GENERATOR_HPP
#define SERVANT_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/Traversal.hpp"

class ServantGenerator
{
public:
  ServantGenerator (CommandLine const& cl);

  void
  options (CL::Description& d);

  void
  generate (CCF::CIDL::SyntaxTree::TranslationUnitPtr const&);

private:
  void
  compute_export_macro (const fs::path& file_path);

  ostream&
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
