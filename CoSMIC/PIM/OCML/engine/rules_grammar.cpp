#include "spirit_helpers.hpp"  // Include this first.

#include "rules_grammar.hpp"

namespace OCML
{
  namespace Rule_Parser
  {
    rule_map_t rules;
    phoenix::function<do_find_rule_t> do_find_rule;
    phoenix::function<do_push_back_t> do_push_back;
    phoenix::function<do_define_equality_rule_t> do_define_equality_rule;
    phoenix::function<do_define_not_rule_t> do_define_not_rule;
    phoenix::function<do_define_if_rule_t> do_define_if_rule;
    phoenix::function<do_define_iff_rule_t> do_define_iff_rule;
    phoenix::function<do_define_and_rule_t> do_define_and_rule;
    phoenix::function<do_define_or_rule_t> do_define_or_rule;
    phoenix::function<do_define_selection_rule_t> do_define_selection_rule;
  }
}
