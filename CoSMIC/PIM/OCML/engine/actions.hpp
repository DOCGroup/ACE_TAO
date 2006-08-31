#ifndef OCML_RULE_PARSER_ACTIONS_HPP
#define OCML_RULE_PARSER_ACTIONS_HPP

#include <map>
#include <list>
#include <string>

#include <boost/bind.hpp>
#include <boost/spirit.hpp>
#include <boost/spirit/phoenix.hpp>
#include <boost/spirit/phoenix/actor.hpp>

#include "configurator.hpp"

namespace OCML
{

  namespace Rule_Parser
  {

    typedef std::map<std::string, OCML::Constraint_Engine::rule_index>
      rule_map_t;
    extern rule_map_t rules;

    struct insert_rule_map
    {
      template <typename InitT>
      void operator()(InitT val) const
      {
        rules[val.first] = val.second;
      }
    };

    struct do_find_rule_t
    {
      template <typename Arg>
      struct result { typedef OCML::Constraint_Engine::rule_index type; };

      OCML::Constraint_Engine::rule_index
      operator()(const std::string& name) const
      {
        // @todo check here if the name is found. else show some error message.
        return rules[name];
      }
    };

    struct do_push_back_t
    {
      template <typename Arg1, typename Arg2>
      struct result { typedef void type; };

      template <typename C, typename V>
      void operator()(C& c, V& v) const
      {
        c.push_back(v);
      }
    };

    struct insert_rule
    {
      template <typename InitT>
      void operator()(InitT val) const
      {
        Configurator* configurator = Configurator::instance();
        Constraint_Engine* constraint_engine =
          configurator->constraint_engine();
        constraint_engine->add_rule(val);
      }
    };

    struct do_define_selection_rule_t
    {
      template <typename Arg>
      struct result { typedef Constraint_Engine::rule_index type; };

      typedef Constraint_Engine::rule_index rule_index;

      rule_index operator() (const Option_Path& path) const
      {
        Configurator* configurator = Configurator::instance();
        Constraint_Engine* constraint_engine =
          configurator->constraint_engine();
        Option_Definition *defn = configurator->definition(path);
        return constraint_engine->define_selection_rule(defn);
      }
    };

    struct do_define_equality_rule_t
    {
      template <typename Arg1, typename Arg2>
      struct result { typedef Constraint_Engine::rule_index type; };

      typedef Constraint_Engine::rule_index rule_index;

      rule_index operator() (const Option_Path& path,
                             Option_Value* value) const
      {
        Configurator* configurator = Configurator::instance();
        Constraint_Engine* constraint_engine =
          configurator->constraint_engine();
        Option_Definition *defn = configurator->definition(path);
        return constraint_engine->define_equality_rule(defn, value);
      }
    };
 
    struct do_define_not_rule_t
    {
      template <typename Arg>
      struct result { typedef Constraint_Engine::rule_index type; };

      typedef Constraint_Engine::rule_index rule_index;

      rule_index operator() (rule_index index) const
      {
        Configurator* configurator = Configurator::instance();
        Constraint_Engine* constraint_engine =
          configurator->constraint_engine();
        return constraint_engine->define_not_rule(index);
      }
    };
 
    struct do_define_if_rule_t
    {
      template <typename Arg1, typename Arg2>
      struct result { typedef Constraint_Engine::rule_index type; };

      typedef Constraint_Engine::rule_index rule_index;

      rule_index operator() (rule_index index1, rule_index index2) const
      {
        Configurator* configurator = Configurator::instance();
        Constraint_Engine* constraint_engine =
          configurator->constraint_engine();
        return constraint_engine->define_if_rule(index1, index2);
      }
    };
 
    struct do_define_iff_rule_t
    {
      template <typename Arg1, typename Arg2>
      struct result { typedef Constraint_Engine::rule_index type; };

      typedef Constraint_Engine::rule_index rule_index;

      rule_index operator() (rule_index index1, rule_index index2) const
      {
        Configurator* configurator = Configurator::instance();
        Constraint_Engine* constraint_engine =
          configurator->constraint_engine();
        return constraint_engine->define_iff_rule(index1, index2);
      }
    };
 
    struct do_define_and_rule_t
    {
      template <typename Arg>
      struct result { typedef Constraint_Engine::rule_index type; };

      typedef Constraint_Engine::rule_index rule_index;

      rule_index operator() (const std::vector<rule_index>& indexes) const
      {
        Configurator* configurator = Configurator::instance();
        Constraint_Engine* constraint_engine =
          configurator->constraint_engine();
        return constraint_engine->define_and_rule(indexes);
      }
    };
 
    struct do_define_or_rule_t
    {
      template <typename Arg>
      struct result { typedef Constraint_Engine::rule_index type; };

      typedef Constraint_Engine::rule_index rule_index;

      rule_index operator() (const std::vector<rule_index>& indexes) const
      {
        Configurator* configurator = Configurator::instance();
        Constraint_Engine* constraint_engine =
          configurator->constraint_engine();
        return constraint_engine->define_or_rule(indexes);
      }
    };
 
    extern phoenix::function<do_find_rule_t> do_find_rule;
    extern phoenix::function<do_push_back_t> do_push_back;
    extern phoenix::function<do_define_equality_rule_t> do_define_equality_rule;
    extern phoenix::function<do_define_not_rule_t> do_define_not_rule;
    extern phoenix::function<do_define_if_rule_t> do_define_if_rule;
    extern phoenix::function<do_define_iff_rule_t> do_define_iff_rule;
    extern phoenix::function<do_define_and_rule_t> do_define_and_rule;
    extern phoenix::function<do_define_or_rule_t> do_define_or_rule;
    extern phoenix::function<do_define_selection_rule_t>
      do_define_selection_rule;
  } // namespace Rule_Parser
 
} // namespace OCML

#endif // OCML_RULE_PARSER_ACTIONS_HPP

