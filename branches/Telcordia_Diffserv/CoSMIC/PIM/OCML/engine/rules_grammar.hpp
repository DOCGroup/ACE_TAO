#ifndef OCML_RULES_GRAMMAR_HPP
#define OCML_RULES_GRAMMAR_HPP

#include "actions.hpp"
#include "common_parsers.hpp"

namespace OCML
{

  namespace Rule_Parser
  {

    /*
     * Option_Value grammar
     */
 
    struct Option_Value_Closure
      : public boost::spirit::closure<Option_Value_Closure, Option_Value*>
    {
      member1 value;
    };

    struct Option_Value_Grammar
      : public boost::spirit::grammar<Option_Value_Grammar,
                                      Option_Value_Closure::context_t >
    {
      template <typename ScannerT>
      struct definition
      {
        definition(Option_Value_Grammar const& self);

        boost::spirit::rule<ScannerT> top;
        boost::spirit::rule<ScannerT, Option_Value_Closure::context_t>
          int_value, bool_value, str_value;

        const boost::spirit::rule<ScannerT>& start() const {
          return top;
        }
      };
    };

    /*
     * Option_Path grammar
     */
 
    struct Option_Path_Closure
      : public boost::spirit::closure<Option_Path_Closure, Option_Path>
    {
      member1 value;
    };

    struct Option_Path_Grammar
      : public boost::spirit::grammar<Option_Path_Grammar,
                                      Option_Path_Closure::context_t >
    {
      template <typename ScannerT>
      struct definition
      {
        definition(Option_Path_Grammar const& self);

        boost::spirit::rule<ScannerT> option;
        common::Word_Grammar word;

        const boost::spirit::rule<ScannerT>& start() const {
          return option;
        }
      };
    };

    /*
     * Expression grammar
     */
 
     struct Expression_Closure
       : public boost::spirit::closure<Expression_Closure,
                             OCML::Constraint_Engine::rule_index,
                             std::vector<OCML::Constraint_Engine::rule_index>,
                             OCML::Constraint_Engine::rule_index,
                             OCML::Constraint_Engine::rule_index,
                             Option_Path,
                             Option_Value*>
    {
      member1 value;
      member2 exprs;
      member3 lhs;
      member4 rhs;
      member5 o_path;
      member6 o_value;
    };

    struct Expression_Grammar
      : public boost::spirit::grammar<Expression_Grammar,
                                      Expression_Closure::context_t>
    {
    public:
      template <typename ScannerT>
      struct definition
      {
        definition(Expression_Grammar const& self);

        boost::spirit::rule<ScannerT, Expression_Closure::context_t>
          expression,
          option_selection,
          option_equality,
          negated_expression,
          and_combination,
          or_combination,
          if_rule;

        boost::spirit::rule<ScannerT> top;

        Option_Value_Grammar option_value;
        common::Word_Grammar word;
        Option_Path_Grammar option_path;

        const boost::spirit::rule<ScannerT>& start() const {
          return top;
        }
      };
    };

    /*
     * Rule_Definition grammar
     */
    
     struct Rule_Definition_Closure
       : public boost::spirit::closure<Rule_Definition_Closure,
                  std::pair<std::string, OCML::Constraint_Engine::rule_index> >
     {
       member1 value;
     };
 
     struct Rule_Definition_Grammar
       : public boost::spirit::grammar<Rule_Definition_Grammar,
                                       Rule_Definition_Closure::context_t>
     {
       template <typename ScannerT>
       struct definition
       {
         definition(Rule_Definition_Grammar const& self);
 
         boost::spirit::rule<ScannerT> rule_definition;
         common::Word_Grammar word;
         Expression_Grammar expression;
         
         const boost::spirit::rule<ScannerT>& start() const {
          return rule_definition;
        }
      };
    };
 
    /*
     * Syntax grammar  -- spirit entry point --
     */
    
    struct Syntax: public boost::spirit::grammar<Syntax>
    {
    public:
      template <typename ScannerT>
      struct definition
      {
        definition(Syntax const& self);

        boost::spirit::rule<ScannerT> file;
         Rule_Definition_Grammar rule_definition;
        Expression_Grammar expression;

        const boost::spirit::rule<ScannerT>& start() const {
          return file;
        }
      };
    };

    
  } // namespace Rule_Parser

} // namespace OCML

#include "rules_grammar.tpp"

#endif // OCML_RULES_GRAMMAR_HPP
