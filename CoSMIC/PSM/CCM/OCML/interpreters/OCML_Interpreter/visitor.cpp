#include "visitor.h"

#include <map>
#include <string>
#include <list>
#include <algorithm>

#include "OCML_BON_Extension.h"
#include "fco_utils.h"

namespace BON {

void Prototype_Visitor::visitFCOImpl(const FCO& object)
{
	// print the function prototype of rule validatior
	if (Rule(object->getParent())) {
		os_ << "bool " << get_func_name(object)
			<< "(Option_Category& root);  // "
			<< object->getName() << std::endl;
	}
}

// template functor used by the Func_Body_Visitor
template<class RULE_CLASS>
class print_rule_body {
public:
	void operator()(const FCO& object, std::ostream& os)
	{
		// print the function body of the rule validator
		if (RULE_CLASS(object)) {
			os << "bool " << get_func_name(object) << "(Option_Category& root) // "
			   << object->getName() << "\n"
			   << "{\n"
			   << "  return\n";
			RULE_CLASS r = object;
			r->print_rule_body(os);
			os << "}\n\n";
		}
	}
};

void Func_Body_Visitor::visitFCOImpl(const FCO& object)
{
	if (visit_box_item_) {
		print_rule_body<Rule>()(object, os_);
		print_rule_body<Or_Logic>()(object, os_);
		print_rule_body<Not_Logic>()(object, os_);
		print_rule_body<Equal_Logic>()(object, os_);
	}
	else {
		print_rule_body<Select_Conn>()(object, os_);
		print_rule_body<Range_Conn>()(object, os_);
		print_rule_body<Int_Eq_Conn>()(object, os_);
		print_rule_body<Str_Eq_Conn>()(object, os_);
		print_rule_body<Flag_Eq_Conn>()(object, os_);
		print_rule_body<Logic_Conn>()(object, os_);
		print_rule_body<Int_Val_Eq_Conn>()(object, os_);
		print_rule_body<Str_Val_Eq_Conn>()(object, os_);
		print_rule_body<Flag_Val_Eq_Conn>()(object, os_);
	}
}
}; // namespace BON
