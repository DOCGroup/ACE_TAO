// $Id$
#ifndef SERVANT_SOURCE_GENERATOR_HPP
#define SERVANT_SOURCE_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"

#include "Collectors.hpp"

#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/Traversal.hpp"

// SourceEmitterBase is a base class that holds the ostream member
// common to every other class in this file.
//
class SourceEmitterBase
{
protected:
  SourceEmitterBase (std::ostream&);

  std::ostream& os;
};

// Emitter generates the servant source mapping for declarations collected
// by Collector. Note that the original structure of modules is preserved.
//
class ServantSourceEmitter
  : public SourceEmitterBase,
    public CCF::CIDL::Traversal::TranslationUnit
{
public:
  ServantSourceEmitter (std::ostream& os_,
                        std::string export_macro,
                        CommandLine const& cl,
                        Declarations const& declarations);

  virtual void
  generate (CCF::CIDL::SyntaxTree::TranslationUnitPtr const& u);

  virtual void
  pre (CCF::CIDL::SyntaxTree::TranslationUnitPtr const&);

private:
  CommandLine const& cl_;
  std::string export_macro_;

  Declarations const& declarations_;
};

#endif // SERVANT_SOURCE_GENERATOR_HPP

/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
