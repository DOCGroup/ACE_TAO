// $Id$
#ifndef CCF_IDL2_PARSER_HPP
#define CCF_IDL2_PARSER_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/CompilerElements/Parser.hpp"
#include "CCF/IDL2/IDL2_LexicalAnalyzer.hpp"
#include "CCF/IDL2/IDL2_SemanticAction.hpp"

//@@ tmp
#include <iostream>
using std::cerr;
using std::endl;

namespace IDL2
{
  class Parser : public virtual ::Parser
  {
  public:
    virtual
    ~Parser () throw () {}

  protected:
    LexicalAnalyzer const& lexer_;
    SemanticActionFactory&  actions_;

  protected:

    // @@ this stinks
    // Semanic action types
    //

    typedef
    SemanticAction::Include
    Include;

    typedef
    SemanticAction::Scope
    Scope;

    typedef
    SemanticAction::Module
    Module;

    typedef
    SemanticAction::Interface
    Interface;

    typedef
    SemanticAction::Attribute
    Attribute;

    typedef
    SemanticAction::Operation
    Operation;

  protected:

    //
    // Primitives
    //
    typedef
    Details::KeywordParser
    KeywordParser;

    typedef
    Details::PunctuationParser
    PunctuationParser;

    typedef
    Details::IdentifierParser
    IdentifierParser;

    typedef
    Details::SimpleIdentifierParser
    SimpleIdentifierParser;

    typedef
    Details::StringLiteralParser
    StringLiteralParser;

    // Keyword parsers (alphabetic order).
    KeywordParser ABSTRACT;
    KeywordParser ATTRIBUTE;
    KeywordParser FACTORY;
    KeywordParser IN;
    KeywordParser INCLUDE;
    KeywordParser INOUT;
    KeywordParser INTERFACE;
    KeywordParser LOCAL;
    KeywordParser MODULE;
    KeywordParser OUT;
    KeywordParser SINCLUDE;
    KeywordParser SUPPORTS;

    // Punctuation parsers (alphabetic group order).
    PunctuationParser COLON;
    PunctuationParser COMMA;
    PunctuationParser LBRACE;
    PunctuationParser RBRACE;
    PunctuationParser LPAREN;
    PunctuationParser RPAREN;
    PunctuationParser SEMI;

    IdentifierParser       identifier;
    SimpleIdentifierParser simple_identifier;
    StringLiteralParser    string_literal;


    //
    // Language
    //

    typedef
    Details::Rule
    Rule;

    Rule language;
    Rule import;
    Rule declaration;
    Rule extension;

    Rule include_decl;
    Rule system_include_decl;

    Rule module_decl;

    Rule interface_decl;
    Rule interface_header;
    Rule interface_inheritance_spec;
    Rule interface_body;

    Rule attribute_decl;

    Rule direction_specifier;

    Rule operation_decl;
    Rule operation_parameter_list;
    Rule operation_parameter;

  public:
    Parser (LexicalAnalyzer const& l, SemanticActionFactory& f)
        : lexer_ (l),
          actions_ (f),

          ABSTRACT  (l.abstract),
          ATTRIBUTE (l.attribute),
          FACTORY   (l.factory),
          IN        (l.in),
          INCLUDE   (l.include),
          INOUT     (l.inout),
          INTERFACE (l.interface),
          LOCAL     (l.local),
          MODULE    (l.module),
          OUT       (l.out),
          SINCLUDE  (l.sinclude),
          SUPPORTS  (l.supports),

          COLON  (l.colon),
          COMMA  (l.comma),
          LBRACE (l.lbrace),
          RBRACE (l.rbrace),
          LPAREN (l.lparen),
          RPAREN (l.rparen),
          SEMI   (l.semi),

          act_include_begin (f.include (), &Include::begin),
          act_include_end   (f.include (), &Include::end),

          act_module_begin       (f.module (), &Module::begin),
          act_module_open_scope  (f.module (), &Scope::open_scope),
          act_module_close_scope (f.module (), &Scope::close_scope),
          act_module_end         (f.module (), &Module::end),

          act_interface_begin       (this, &Parser::act_interface_begin_core),
          act_interface_inherits    (f.interface (), &Interface::inherits),
          act_interface_open_scope  (f.interface (), &Scope::open_scope),
          act_interface_close_scope (f.interface (), &Scope::close_scope),
          act_interface_end         (f.interface (), &Interface::end),

          act_attribute_type (f.attribute (), &Attribute::type),
          act_attribute_name (f.attribute (), &Attribute::name),

          act_operation_begin     (f.operation (), &Operation::begin),
          act_operation_parameter (this,
                                   &Parser::act_operation_parameter_core),
          act_operation_end       (f.operation (), &Operation::end)

    {
      language = *import >> +declaration
        ;

      import =
          include_decl
        | system_include_decl
        ;

      include_decl =
           INCLUDE
        >> string_literal[act_include_begin]
        >> SEMI[act_include_end]
        ;

      system_include_decl =
           SINCLUDE
        >> string_literal
        >> SEMI[act_include_end]
        ;

      declaration =
           interface_decl
        |  module_decl
        |  extension
        ;

      module_decl =
           MODULE
        >> simple_identifier[act_module_begin]
        >> LBRACE[act_module_open_scope]
        >> +declaration
        >> RBRACE[act_module_close_scope]
        >> SEMI[act_module_end]
        ;


      //
      // interface
      //
      interface_decl =
           interface_header
        >>
           (
                 SEMI[act_interface_end]
              |
                 (
                       !(COLON >> interface_inheritance_spec)
                    >> LBRACE[act_interface_open_scope]
                    >> interface_body
                    >> RBRACE[act_interface_close_scope]
                    >> SEMI[act_interface_end]
                 )
           )
        ;

      interface_header =
        (
              !(LOCAL | ABSTRACT)
           >> INTERFACE
           >> simple_identifier
        )[act_interface_begin]
        ;


      interface_inheritance_spec =
           identifier[act_interface_inherits]
        >> *(COMMA >> identifier[act_interface_inherits])
        ;

      interface_body =
        *(
             attribute_decl
           | operation_decl
        )
        ;

      //
      // attribute
      //
      attribute_decl =
           ATTRIBUTE
        >> identifier[act_attribute_type]
        >> simple_identifier[act_attribute_name]
        >> SEMI
        ;


      //
      // direction specifier
      //
      direction_specifier =
           IN
        |  OUT
        |  INOUT
        ;


      //
      // operation
      //
      operation_decl =
           (identifier >> simple_identifier)[act_operation_begin]
        >> LPAREN
        >> operation_parameter_list
        >> RPAREN
        >> SEMI[act_operation_end]
        ;

      operation_parameter_list =
        *(
              operation_parameter
           >> *(COMMA >> operation_parameter)
        )
        ;

      operation_parameter =
        (
              direction_specifier
           >> identifier
           >> simple_identifier
        )[act_operation_parameter]
        ;
    }

    virtual Rule const&
    start() const
    {
      return language;
    }

  protected:

    //
    // Semantic actions
    //

    typedef
    NoArgAction<Scope>
    ScopeAction;

    //
    // Include
    //
    OneArgAction<StringLiteralPtr, Include> act_include_begin;
    NoArgAction<Include> act_include_end;

    //
    // Module
    //

    OneArgAction<SimpleIdentifierPtr, Module> act_module_begin;
    ScopeAction act_module_open_scope;
    ScopeAction act_module_close_scope;
    NoArgAction<Module> act_module_end;

    //
    // Interface
    //
    void
    act_interface_begin_core (Iterator begin, Iterator end) const
    {
      Interface::Qualifier::Value q = Interface::Qualifier::UNCONSTRAINED;

      if(end - begin == 3)
      {
        if(identical (lexer_.local, *begin))
        {
          q = Interface::Qualifier::LOCAL;
        }
        else
        {
          q = Interface::Qualifier::ABSTRACT;
        }
        begin++;
      }

      begin++;

      actions_.interface ().begin (q, typical<SimpleIdentifierPtr>(*begin));
    }
    ActionExecutor<Parser> act_interface_begin;

    OneArgAction<IdentifierPtr, Interface> act_interface_inherits;

    ScopeAction act_interface_open_scope;
    ScopeAction act_interface_close_scope;

    NoArgAction<Interface> act_interface_end;

    //
    // Attribute
    //
    OneArgAction<IdentifierPtr, Attribute> act_attribute_type;
    OneArgAction<SimpleIdentifierPtr, Attribute> act_attribute_name;

    //
    // Operation
    //
    TwoArgAction<IdentifierPtr,
                 SimpleIdentifierPtr,
                 Operation> act_operation_begin;

    void
    act_operation_parameter_core (Iterator begin, Iterator end) const
    {
      Operation::Direction::Value d = Operation::Direction::INOUT;

      if(identical (lexer_.in, *begin))
      {
        d = Operation::Direction::IN;
      }
      else if(identical (lexer_.out, *begin))
      {
        d = Operation::Direction::OUT;
      }
      begin++;

      actions_.operation ().parameter (
        d,
        typical<IdentifierPtr>(*begin),
        typical<SimpleIdentifierPtr>(*(begin + 1)));
    }
    ActionExecutor<Parser> act_operation_parameter;

    NoArgAction<Operation> act_operation_end;


  };
}

#endif // CCF_IDL2_PARSER_HPP
