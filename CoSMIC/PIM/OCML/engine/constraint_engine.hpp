#ifndef _CONSTRAINT_ENGINE_HPP_
#define _CONSTRAINT_ENGINE_HPP_

#include "rule.hpp"
#include "option_value.hpp"
#include "definitions.hpp"
#include <map>
#include <memory>
#include <vector>

namespace OCML
{

  // Forward declarations.
  class Option_Reference;
  class Constraint_Engine;

  /**
   * Base class for the value change listeners (the listener will be invoked
   * whenever the value for the class is changed by the constraint engine).
   */
  class Constraint_Value_Change_Listener
  {
  public:
    OCML_Engine_Export Constraint_Value_Change_Listener();
    OCML_Engine_Export virtual ~Constraint_Value_Change_Listener();

    /**
     * The callback method which should be implemented by the specific value
     * change listener. The implementation is responsible to delete the value.
     */
    OCML_Engine_Export virtual void value_changed(bool flagged, Option_Value* value) = 0;

  private:
    OCML_Engine_Export void set_value(Option_Reference* reference);

    std::auto_ptr<Option_Value> value_;
    bool flag_;

    friend class Constraint_Engine;
  };

  /**
   * Manages the constraint checking process.
   */
  class Constraint_Engine
  {
  private:
    class rule_interface
    {
    public:
      rule_interface(Root_Rule& root_rule, Rule* rule);

      void insert_into_root();

      void delete_from_root();

      /// Returns a rule by duplicating it.
      Rule* rule()
      {
        return rule_->duplicate();
      }

    private:
      /// @todo This should better be static.
      Root_Rule& root_rule_;
      /// If the rule is not in the root_rule, then use this (otherwise null).
      std::auto_ptr<Rule> rule_;
      /// If the rule is in the root rule, then use this.
      Root_Rule::rule_index root_rule_index_;
    };

    typedef std::list<rule_interface*> rule_interface_container;

  public:
    typedef rule_interface_container::iterator rule_index;

  public:
    /// Initialize constraint engine.
    OCML_Engine_Export Constraint_Engine(Option_Category* root_category);

    OCML_Engine_Export void register_listener(Option_Definition* option,
                                              Constraint_Value_Change_Listener* l);

    OCML_Engine_Export void unregister_listener(Option_Definition* option);

    OCML_Engine_Export rule_index define_selection_rule(const Option_Definition* option);
    OCML_Engine_Export rule_index define_equality_rule(const Option_Definition* option,
                                                       const Option_Value* value);
    OCML_Engine_Export rule_index define_not_rule(rule_index index);
    OCML_Engine_Export rule_index define_if_rule(rule_index index1, rule_index index2);
    OCML_Engine_Export rule_index define_iff_rule(rule_index index1, rule_index index2);
    OCML_Engine_Export rule_index define_and_rule(const std::vector<rule_index>& indexes);
    OCML_Engine_Export rule_index define_or_rule(const std::vector<rule_index>& indexes);
    OCML_Engine_Export void delete_rule_definition(rule_index index);

    OCML_Engine_Export void add_rule(rule_index index);
    OCML_Engine_Export void delete_rule(rule_index index);

    /// Ends transaction makes constraint validation and returns the result.
    OCML_Engine_Export bool validate();

  protected:
    template <typename OPTION_TRAIT>
    Rule* try_create_equality_rule(const Option_Definition* option,
                                   const Option_Value* value);

    rule_index create_interface(Rule* rule);
      
    /// Creates option references for the options in the given category.
    void create_references(Option_Category* category);

    /// Insert & delete local copies into/from root rule.
    void commit_changes();
  
    /// Insert & delete the rules copied (with previous commit) into/from
    /// root rule.
    void rollback_changes();

  private:
    /// Option definition to option reference map type.
    typedef std::map<const Option_Definition*, Option_Reference*>
      reference_map;
    typedef reference_map::iterator reference_iterator;

    /// Option definition to listener map.
    typedef std::map<const Option_Definition*,
                     Constraint_Value_Change_Listener*> listener_container;
    typedef listener_container::iterator listener_iterator;

    /// The root rule. @see Root_Rule
    Root_Rule root_rule_;
    /// The root category.
    Option_Category* root_category_;
    /// A map from option definitions to the corresponding references.
    reference_map references_;
    /// The container for the listeners. 
    listener_container listeners_;

    rule_interface_container interfaces_;
    
    /// Holds the rules inserted into the root rule in the current transaction.
    std::list<rule_index> inserted_rules_;
    /// Holds the rules deleted from the root rule in the current transaction.
    std::list<rule_index> deleted_rules_;
 };

} //namespace OCML

#include "constraint_engine.ipp"

#endif // _CONSTRAINT_ENGINE_HPP_
