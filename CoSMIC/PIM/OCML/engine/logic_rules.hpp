// $Id$

#ifndef _LOGIC_RULES_HPP_
#define _LOGIC_RULES_HPP_

#include <list>
#include <memory>

#include "rule.hpp"

namespace OCML
{

  class Not_Rule: public Basic_Rule<Not_Rule>
  {
  public:
    OCML_Engine_Export Not_Rule(const Not_Rule& rule);

    OCML_Engine_Export Not_Rule(Rule* root, Rule* rule);

    OCML_Engine_Export virtual ~Not_Rule() { };

    OCML_Engine_Export virtual bool test() const;

    OCML_Engine_Export virtual bool apply(bool negate);

  private:
    std::auto_ptr<Rule> rule_;
  };

  class And_Rule: public Basic_Rule<And_Rule>
  {
  public:
    OCML_Engine_Export And_Rule(const And_Rule& rule);

    template <typename InputIterator>
    And_Rule(Rule* root, InputIterator first, InputIterator last);

    OCML_Engine_Export virtual ~And_Rule();

    OCML_Engine_Export virtual bool test() const;

    OCML_Engine_Export virtual bool apply(bool negate);

  private:
    std::list<Rule*> rules_;
  };

  class Or_Rule: public Basic_Rule<Or_Rule>
  {
  public:
    OCML_Engine_Export Or_Rule(const Or_Rule& rule);

    template <typename InputIterator>
    Or_Rule(Rule* root, InputIterator first, InputIterator last);

    OCML_Engine_Export virtual ~Or_Rule();

    OCML_Engine_Export virtual bool test() const;

    OCML_Engine_Export virtual bool apply(bool negate);

  private:
    std::list<Rule*> rules_;
  };

  class If_Rule: public Basic_Rule<If_Rule>
  {
  public:
    OCML_Engine_Export If_Rule(const If_Rule& rule);

    OCML_Engine_Export If_Rule(Rule* root, Rule* condition, Rule* action);

    OCML_Engine_Export virtual ~If_Rule() { };

    OCML_Engine_Export virtual bool test() const;

    OCML_Engine_Export virtual bool apply(bool negate);

  private:
    std::auto_ptr<Rule> condition_;
    std::auto_ptr<Rule> action_;
  };

  class Iff_Rule: public Basic_Rule<Iff_Rule>
  {
  public:
    OCML_Engine_Export Iff_Rule(const Iff_Rule& rule);

    OCML_Engine_Export Iff_Rule(Rule* root, Rule* rule1, Rule* rule2);

    OCML_Engine_Export virtual ~Iff_Rule() { };

    OCML_Engine_Export virtual bool test() const;

    OCML_Engine_Export virtual bool apply(bool negate);

  private:
    std::pair< std::auto_ptr<Rule>, std::auto_ptr<Rule> > rules_;
  };

} // namespace OCML

#include "logic_rules.ipp"
#include "logic_rules.tpp"

#endif // _LOGIC_RULES_HPP_
