inline
Environment::Environment(OCML::Configurator* configurator)
  : configurator_(configurator)
{
  ACE_DEBUG((LM_TRACE, "Environment::Environment(OCML::Configurator*)\n"));
}

inline OCML::Constraint_Engine::rule_index
Environment::rule(const char* name)
{
  return rules_[name];
}
 
inline void
Environment::add_rule(const char* name,
                      OCML::Constraint_Engine::rule_index rule)
{
  rules_[name] = rule;
}

inline void
Environment::delete_rule(const char* name)
{
  rules_.erase(rules_.find(name));
}

inline OCML::Constraint_Engine*
Environment::constraint_engine()
{
  return configurator_->constraint_engine();
}

