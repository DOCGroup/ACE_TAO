// $Id$
#ifndef SERVANT_HEADER_GENERATOR_HPP
#define SERVANT_HEADER_GENERATOR_HPP

#include "CCF/CodeGenerationKit/CommandLine.hpp"

#include "Collectors.hpp"

#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/Traversal.hpp"

// HeaderEmitterBase is a base class that holds the ostream member
// common to every other class in this file.
//
class HeaderEmitterBase
{
protected:
  HeaderEmitterBase (std::ostream&);

  std::ostream& os;
};

// Emitter generates the servant source mapping for declarations collected
// by Collectors. Note that the original structure of modules is preserved.
//

class ServantHeaderEmitter
  : public HeaderEmitterBase,
    public CCF::CIDL::Traversal::TranslationUnit,
    public CCF::CIDL::Traversal::UnconstrainedInterfaceDef
{
public:
  ServantHeaderEmitter (std::ostream& os_,
                        CommandLine const& cl,
                        string export_macro,
                        Declarations const& declarations);


  virtual void
  pre (CCF::CIDL::SyntaxTree::TranslationUnitPtr const& u);

  virtual void
  generate (CCF::CIDL::SyntaxTree::TranslationUnitPtr const& u);

  virtual void
  post (CCF::CIDL::SyntaxTree::TranslationUnitPtr const& u);

private:
  CommandLine const& cl_;
  std::string export_macro_;

  Declarations const& declarations_;
};


#endif // SERVANT_HEADER_GENERATOR_HPP

/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
