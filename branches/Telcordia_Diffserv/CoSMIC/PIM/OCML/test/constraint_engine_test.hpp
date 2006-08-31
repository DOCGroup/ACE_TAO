#ifndef _CONSTRAINT_ENGINE_TEST_HPP_
#define _CONSTRAINT_ENGINE_TEST_HPP_

#include <engine/constraint_engine.hpp>
#include <engine/definitions.hpp>
#include <engine/option_value.hpp>
#include <map>
#include <list>
#include <memory>
#include "option_value_test.hpp"

class Value_Change_Listener
  : public OCML::Constraint_Value_Change_Listener
{
public:
  Value_Change_Listener(OCML::Option_Definition* option);

  virtual void value_changed(bool flagged, OCML::Option_Value* value);

private:
  OCML::Option_Definition* option_;
};

typedef std::map<OCML::Option_Definition*, Value_Change_Listener*>
  listener_map;

listener_map* create_listeners(OCML::Option_Category* category,
                               OCML::Constraint_Engine& engine);

void delete_listeners(listener_map* listeners,
                      OCML::Constraint_Engine& engine);

void assign_values(OCML::Option_Category* category,
                   OCML::Constraint_Engine& engine);

#endif // _CONSTRAINT_ENGINE_TEST_HPP_
