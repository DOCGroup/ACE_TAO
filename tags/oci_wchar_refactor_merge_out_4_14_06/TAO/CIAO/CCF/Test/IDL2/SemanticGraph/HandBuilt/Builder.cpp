// file      : Test/IDL2/SemanticGraph/HandBuilt/Builder.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include <iostream>

#include "Builder.hpp"

using namespace CCF::IDL2::SemanticGraph;

//@@ tmp
using std::cerr;
using std::endl;

TranslationUnit* Builder::
build ()
{
  /*
  0
  1  #include "../foo/quote.idl"
  2  #include <ui/bracket.idl>
  3
  4  module M
  5  {
  6
  7    interface I;
  8
  9    typedef I J;
  10
  11   interface I
  12   {
  13     void f (in long id, out string name);
  14
  15     attribute long a;
  16   };
  17
  18   interface Q : J {};
  19
  20   typeid I "Foo";
  21   typeprefix M "Bar";
  22
  23   struct S
  24   {
  25     long member;
  26   };
  27
  28   typedef sequence<long> LongSeq;
  29
  30 };
  31
  32
  */

  //@@ names can be spcified without explicit construction.
  //

  TranslationUnit& tu (*(new TranslationUnit));


  // 0: Implied translation region with fundamental types.
  //
  //
  TranslationRegion& fundamental (tu.new_node<TranslationRegion> ());
  tu.new_edge<ContainsImplied> (tu, fundamental, ".fundamental");

  Root& fundamental_root (tu.new_node<Root> ());
  tu.new_edge<ContainsRoot> (fundamental, fundamental_root);

  Void& void_ (tu.new_node<Void> ());
  tu.new_edge<Defines> (fundamental_root, void_, SimpleName ("void"));

  Long& long_ (tu.new_node<Long> ());
  tu.new_edge<Defines> (fundamental_root, long_, SimpleName ("long"));

  String& string_ (tu.new_node<String> ());
  tu.new_edge<Defines> (fundamental_root, string_, SimpleName ("string"));

  // Principal translation region.
  //
  TranslationRegion& principal (tu.new_node<TranslationRegion> ());
  tu.new_edge<ContainsPrincipal> (tu, principal);

  // 1: Quote included translation region.
  //
  TranslationRegion& quote_included (tu.new_node<TranslationRegion> ());
  tu.new_edge<QuoteIncludes> (principal, quote_included, "../foo/quote.idl");

  // 2: Bracket included translation region.
  //
  TranslationRegion& braket_included (tu.new_node<TranslationRegion> ());
  tu.new_edge<BracketIncludes> (principal, braket_included, "ui/bracket.idl");

  Root& root (tu.new_node<Root> ());
  tu.new_edge<ContainsRoot> (principal, root);

  // 4-5:
  //
  Module& m (tu.new_node<Module> ());
  tu.new_edge<Defines> (root, m, SimpleName ("M"));

  // 7-11:
  //
  UnconstrainedInterface& i (tu.new_node<UnconstrainedInterface> ());
  tu.new_edge<Mentions> (m, i, SimpleName ("I"));
  tu.new_edge<Aliases>  (m, i, SimpleName ("J"));
  tu.new_edge<Defines>  (m, i, SimpleName ("I"));

  // 13:
  //
  TwoWayOperation& f (tu.new_node<TwoWayOperation> ());
  tu.new_edge<Returns> (f, void_);

  Parameter& id (tu.new_node<InParameter> ("id"));
  tu.new_edge<Belongs> (id, long_);
  tu.new_edge<Receives> (f, id);

  Parameter& name (tu.new_node<OutParameter> ("name"));
  tu.new_edge<Belongs> (name, string_);
  tu.new_edge<Receives> (f, name);

  tu.new_edge<Defines> (i, f, SimpleName ("f"));

  // 15:
  //
  Attribute& a (tu.new_node<Attribute> ());
  tu.new_edge<Belongs> (a, long_);
  tu.new_edge<Defines> (i, a, SimpleName ("a"));

  // 18:
  //
  UnconstrainedInterface& q (tu.new_node<UnconstrainedInterface> ());
  tu.new_edge<Inherits> (q, i);
  tu.new_edge<Defines> (m, q, SimpleName ("Q"));

  // 20:
  //
  TypeId& ti (tu.new_node<TypeId> (
                ScopedName ("::M::I"), StringLiteral ("Foo")));
  tu.new_edge<Defines> (m, ti, SimpleName ("typeid"));

  // 21:
  //
  TypePrefix& tp (tu.new_node<TypePrefix> (
                    ScopedName ("::M"), StringLiteral ("Bar")));
  tu.new_edge<Defines> (m, tp, SimpleName ("typeprefix"));

  // 23-24:
  //
  Struct& s (tu.new_node<Struct> ());
  tu.new_edge<Defines>  (m, s, SimpleName ("S"));

  // 25:
  //
  Member& member (tu.new_node<Member> ());
  tu.new_edge<Belongs> (member, long_);
  tu.new_edge<Defines> (s, member, SimpleName ("member"));

  // 28:
  //
  UnboundedSequence& long_seq (tu.new_node<UnboundedSequence> ());
  tu.new_edge<Specialized> (long_seq, long_);
  tu.new_edge<Aliases> (m, long_seq, SimpleName ("LongSeq"));

  return &tu;
}
