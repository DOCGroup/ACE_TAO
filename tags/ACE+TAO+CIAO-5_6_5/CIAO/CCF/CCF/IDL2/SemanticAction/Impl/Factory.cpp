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

        template <typename X>
        void Factory::
        define_fund_type (Root& root, SimpleName const& name)
        {
          ctx_.tu ().template new_edge<Defines> (
            root,
            ctx_.tu ().template new_node<X> ("fundamental-types", 0), name);
        }

        Factory::
        Factory (CompilerElements::Context& parsing_context,
                 Diagnostic::Stream&,
                 SemanticGraph::TranslationUnit& tu)
            : ctx_ (parsing_context.get ("trace-semantic-action", false),
                    tu,
                    parsing_context),

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
          TranslationRegion& ftr (
            tu.new_node<TranslationRegion> (
              "fundamental-type", 0));

          tu.new_edge<ContainsImplied> (tu, ftr, "fundamental-type");

          Root& fr (tu.new_node<Root> ("fundamental-types", 0));
          tu.new_edge<ContainsRoot> (ftr, fr);

          define_fund_type<Object>           (fr, "Object");
          define_fund_type<ValueBase>        (fr, "ValueBase");
          define_fund_type<Any>              (fr, "any");
          define_fund_type<Boolean>          (fr, "boolean");
          define_fund_type<Char>             (fr, "char");
          define_fund_type<Double>           (fr, "double");
          define_fund_type<Float>            (fr, "float");
          define_fund_type<Long>             (fr, "long");
          define_fund_type<LongDouble>       (fr, "long double");
          define_fund_type<LongLong>         (fr, "long long");
          define_fund_type<Octet>            (fr, "octet");
          define_fund_type<Short>            (fr, "short");
          define_fund_type<String>           (fr, "string");
          define_fund_type<UnsignedLong>     (fr, "unsigned long");
          define_fund_type<UnsignedLongLong> (fr, "unsigned long long");
          define_fund_type<UnsignedShort>    (fr, "unsigned short");
          define_fund_type<Void>             (fr, "void");
          define_fund_type<Wchar>            (fr, "wchar");
          define_fund_type<Wstring>          (fr, "wstring");

          TranslationRegion& principal (
            tu.new_node<TranslationRegion> (ctx_.file (), 0));

          tu.new_edge<ContainsPrincipal> (tu, principal);

          Root& root (tu.new_node<Root> (ctx_.file (), 0));
          tu.new_edge<ContainsRoot> (principal, root);

          ctx_.region (principal);
          ctx_.scope (root);
        }
      }
    }
  }
}
