#include "definitions.hpp"
#include "constraint_engine.hpp"
#include "option_reference.hpp"

namespace OCML
{
  /*
   * Value change listener methods.
   */

  Constraint_Value_Change_Listener::~Constraint_Value_Change_Listener()
  {
  }
 
  void Constraint_Value_Change_Listener::set_value(Option_Reference* reference)
  {
    bool changed = false;
    std::auto_ptr<Option_Value> ref_value(reference->option_value());

    // Check if the flag value is changed.
    if (!changed)
    {
      changed = (flag_ != reference->flagged());
    }

    // Handle the special case where the current value ptr is null.
    if (value_.get() == 0)
    {
      if (!changed)
        changed = !(ref_value->is_null());
    }
    else
    {
      if (!changed)
        changed = !(value_->compare(ref_value.get()));
    }

    if (changed)
    {
      flag_ = reference->flagged();
      value_.reset(ref_value->duplicate());
      value_changed(flag_, value_.get());
    }
  }
  
  /*
   * Constraint engine methods.
   */

  Constraint_Engine::Constraint_Engine(Option_Category* root_category)
    : root_category_(root_category)
  {
    // Create the references for the given root option category.
    create_references(root_category_);
  }

  void Constraint_Engine::delete_rule_definition(Constraint_Engine::rule_index index)
  {
    delete *index;
    interfaces_.erase(index);
  }

  bool Constraint_Engine::validate()
  {
    // Insert & delete local copies into/from root rule.
    commit_changes();

    // Clear the references.
    for (reference_iterator iter = references_.begin();
         iter != references_.end(); ++iter)
    {
      iter->second->clear();
    }

    // Check if validation succeeds.
    bool result = root_rule_.apply();

    if (result == true) // If the validation is successful:
    {
      // Call back the registered listeners.
      for (listener_iterator iter = listeners_.begin();
           iter != listeners_.end(); ++iter)
      {
        iter->second->set_value(references_[iter->first]);
      }

      // Delete the deleted rules from the interface container
      // also delete the rule interface (which deletes the rules themselves).
      for (std::list<rule_index>::iterator iter = deleted_rules_.begin();
           iter != deleted_rules_.end(); ++iter)
      {
        delete_rule_definition(*iter);
      }
    }
    else                // If the validation is not successful
    {
      rollback_changes();
    }

    // Reset the transaction data.
    inserted_rules_.clear();
    deleted_rules_.clear();
 
    // Return the result of the validation to the caller.
    return result;
  }

  void Constraint_Engine::create_references(Option_Category* category)
  {
    // Traverse the options of the given categories
    // and create corresponding option references.
    for (Option_Category::option_iterator iter = category->begin_options();
         iter != category->end_options(); ++iter)
    {
      references_[*iter] =
        Option_Reference_Factory::create_option_reference(*iter);
    }

    // Traverse the categories of the given category
    // and call this method recursively.
    for (Option_Category::category_iterator iter=category->begin_categories();
         iter != category->end_categories(); ++iter)
    {
      create_references(*iter);
    }
  }

  void Constraint_Engine::commit_changes()
  {
    // Traverse the insert list and insert them into the root rule.
    for (std::list<rule_index>::iterator iter = inserted_rules_.begin();
         iter != inserted_rules_.end(); ++iter)
    {
      (**iter)->insert_into_root();
    }

    // Traverse the delete list and delete them from the root rule.
    for (std::list<rule_index>::iterator iter = deleted_rules_.begin();
         iter != deleted_rules_.end(); ++iter)
    {
      (**iter)->delete_from_root();
    }
  }

  void Constraint_Engine::rollback_changes()
  {
    // Put the deleted rules back into root rule.
    for (std::list<rule_index>::iterator iter = deleted_rules_.begin();
         iter != deleted_rules_.end(); ++iter)
    {
      (**iter)->insert_into_root();
    }

    // Delete the inserted rules from the root rule.
    for (std::list<rule_index>::iterator iter = inserted_rules_.begin();
         iter != inserted_rules_.end(); ++iter)
    {
      (**iter)->delete_from_root();
    }
  }

} //namespace OCML
