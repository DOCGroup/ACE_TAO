// file      : CCF/IDL2/SemanticAction/Impl/Factory.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Fundamental.hpp"

#include "CCF/IDL2/SemanticAction/Impl/Factory.hpp"


namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        using namespace SemanticGraph;

        Factory::
        Factory (CompilerElements::Context& context,
                 Diagnostic::Stream&,
                 SemanticGraph::TranslationUnit& tu)
            : ctx_ (context.get ("trace-semantic-action", false), tu),

              attribute_ (ctx_),
              const__ (ctx_),
              enum__ (ctx_),
              exception_ (ctx_),
              include_ (ctx_),
              interface_ (ctx_),
              member_ (ctx_),
              module_ (ctx_),
              native_ (ctx_),
              numeric_expression_ (ctx_),
              operation_ (ctx_),
              struct__ (ctx_),
              typedef__ (ctx_),
              type_id_ (ctx_),
              type_prefix_ (ctx_),
              union__ (ctx_),
              value_type_ (ctx_),
              value_type_factory_ (ctx_),
              value_type_member_ (ctx_)
        {
          // Implied translation region with fundamental types.
          //
          TranslationRegion& ftr (tu.new_node<TranslationRegion> ());
          tu.new_edge<ContainsImplied> (tu, ftr, ".fundamental");

          Root& fr (tu.new_node<Root> ());
          tu.new_edge<ContainsRoot> (ftr, fr);

          tu.new_edge<Defines> (fr, tu.new_node<Object> (), "Object");
          tu.new_edge<Defines> (fr, tu.new_node<ValueBase> (), "ValueBase");
          tu.new_edge<Defines> (fr, tu.new_node<Any> (), "any");
          tu.new_edge<Defines> (fr, tu.new_node<Boolean> (), "boolean");
          tu.new_edge<Defines> (fr, tu.new_node<Char> (), "char");
          tu.new_edge<Defines> (fr, tu.new_node<Double> (), "double");
          tu.new_edge<Defines> (fr, tu.new_node<Float> (), "float");
          tu.new_edge<Defines> (fr, tu.new_node<Long> (), "long");
          tu.new_edge<Defines> (fr, tu.new_node<LongDouble> (), "long double");
          tu.new_edge<Defines> (fr, tu.new_node<LongLong> (), "long long");
          tu.new_edge<Defines> (fr, tu.new_node<Octet> (), "octet");
          tu.new_edge<Defines> (fr, tu.new_node<Short> (), "short");
          tu.new_edge<Defines> (fr, tu.new_node<String> (), "string");
          tu.new_edge<Defines> (fr, tu.new_node<UnsignedLong> (), "unsigned long");
          tu.new_edge<Defines> (fr, tu.new_node<UnsignedLongLong> (), "unsigned long long");
          tu.new_edge<Defines> (fr, tu.new_node<UnsignedShort> (), "unsigned short");
          tu.new_edge<Defines> (fr, tu.new_node<Void> (), "void");
          tu.new_edge<Defines> (fr, tu.new_node<Wchar> (), "wchar");
          tu.new_edge<Defines> (fr, tu.new_node<Wstring> (), "wstring");

          TranslationRegion& principal (tu.new_node<TranslationRegion> ());
          tu.new_edge<ContainsPrincipal> (tu, principal);

          Root& root (tu.new_node<Root> ());
          tu.new_edge<ContainsRoot> (principal, root);

          ctx_.region (principal);
          ctx_.scope (root);
        }
      }
    }
  }
}
