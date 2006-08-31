/*
 * @file   visitor.h
 *
 * BON - model visitor classes
 *
 * @author Emre Turkay <emre.turkay@vanderbilt.edu>
 */

#ifndef _OCML_VISITOR_H_
#define _OCML_VISITOR_H_

#include "BON.h"
#include "Extensions.h"
#include "BONImpl.h"
#include <ComponentConfig.h>
#include <string>
#include <iostream>

namespace BON {

/*
 * class Prototype_Visitor
 *
 * Traverses model to generate the rule validator function prototypes
 */
class Prototype_Visitor: public Visitor {
public :
	Prototype_Visitor(std::ostream& os)
		: os_(os) { }
	void visitFCOImpl(const FCO& object);

private :
	// The output stream to write the generated output
	std::ostream &os_;
};

/*
 * class Func_Body_Visitor
 *
 * Traverses model to generate the rule validator function bodies
 */
class Func_Body_Visitor: public Visitor {
public :
	Func_Body_Visitor(std::ostream& os, bool box_items = false)
		: os_(os), visit_box_item_(box_items) { }

	void visitFCOImpl( const FCO& object );
	// set method for visit_box_items
	void visit_box_items(bool value);

private :
	// The output stream to write the generated output
	std::ostream &os_;
	// If this is set to true visits only the BoxItems (see OCML metamodel Rule/BoxItem)
	bool visit_box_item_;
};

inline void
Func_Body_Visitor::visit_box_items(bool value)
{
	visit_box_item_ = value;
}

};  // namespace BON

#endif // _OCML_VISITOR_H_
