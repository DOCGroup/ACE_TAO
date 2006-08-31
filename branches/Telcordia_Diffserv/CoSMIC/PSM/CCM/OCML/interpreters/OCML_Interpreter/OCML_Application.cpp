// OCML_Application.cpp: implementation of the OCML_Application class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OCML_Application.h"
#include "fco_utils.h"
#include "OCML_BON_Extension.h"
#include "visitor.h"
#include "OCML_Utility.h"

#include <set>
#include <fstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OCML_Application::OCML_Application(BON::Folder& root)
	: root_(root)
{

}

OCML_Application::~OCML_Application()
{

}

///////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////

/*
 *  print HTML documentation
 */
void
OCML_Application::print_html_documentation(const std::string& file_name)
{
	std::set<BON::Model> unsorted_children = root_->getChildModels();
	std::set<BON::Model, lt_fco> children(unsorted_children.begin(),
										  unsorted_children.end());

	std::ofstream of(file_name.c_str());
	generated_file_header(of, "<!--", "-->");

	of << "<HTML><BODY>";

	// print out Table of Contents
	of << "<UL>";
	for (std::set<BON::Model, lt_fco>::iterator iter = children.begin();
		 iter != children.end(); ++iter)
		if (BON::Option_Category(*iter))
			BON::Option_Category(*iter)->print_toc(of);
	of << "</UL>";

	// print out Descriptions
	for (std::set<BON::Model, lt_fco>::iterator iter1 = children.begin();
		 iter1 != children.end(); ++iter1)
		if (BON::Option_Category(*iter1))
			BON::Option_Category(*iter1)->print_html_body(of);

	of << "</BODY></HTML>";
}

/*
 *  print the C++ code which initializes the option hierarchy
 */
void
OCML_Application::print_initialization_tree(const std::string& file_name)
{
	std::set<BON::Model> children = root_->getChildModels();

	std::ofstream of(file_name.c_str());
	generated_file_header(of, "/*", "*/");

	// Call print initialization tree functions recursively
	for (std::set<BON::Model>::iterator iter2 = children.begin();
	     iter2 != children.end(); ++iter2)
		if (BON::Option_Category(*iter2))
			BON::Option_Category(*iter2)->print_init_body(of);

	// Print the body of the initialize_option_tree() function,
	// which is the entry point of the option tree initialization code
	of << "void initialize_option_tree(Option_Category& root)\n{\n";

	for (std::set<BON::Model>::iterator iter3 = children.begin();
	     iter3 != children.end(); ++iter3)
		if (BON::Option_Category(*iter3))
			of << "    root.push_back(initialize_category_" << BON::Option_Category(*iter3)->getName() << "());\n";

	of << "}\n";
}

/*
 *  print the C++ code which makes rule validation checking
 */
void
OCML_Application::print_rule_validator(const std::string& file_name)
{
	std::set<BON::Model> children = root_->getChildModels();

	// Open the file for writing and print file header
	std::ofstream cpp(file_name.c_str());
	generated_file_header(cpp, "/*", "*/");
	cpp << "\n#include \"rule_validator.h\"\n\n";

	// Print function prototypes at the top
	cpp << "\n/*\n * Function prototypes\n */\n\n";
	BON::Prototype_Visitor visitor1(cpp);
	root_->accept(&visitor1);

	// Print function bodies
	cpp << "\n/*\n * Function definitions\n */\n\n";
	BON::Func_Body_Visitor visitor2(cpp, true);
	// visit BoxItems (Rule, Equal, Not, Or)
	root_->accept(&visitor2);
	// visit all other items
	visitor2.visit_box_items(false);
	root_->accept(&visitor2);

	// Print the main rule function
	cpp << "\n/*\n * Main rule function\n */\n\n";
	cpp << "bool rule_check(Option_Category& root)\n"
		<< "{\n"
		<< "  return \n";

	bool first = true;
	for (std::set<BON::Model>::iterator iter2 = children.begin();
		 iter2 != children.end(); ++iter2)
		if (BON::Rule(*iter2)) {
			cpp << "    ";
			if (!first)
				cpp << "&& ";
			first = false;
			cpp << get_func_name(BON::FCO(*iter2)) << "(root)\n";
		}

	if (first)
		cpp << "    true;\n";
	else
		cpp << "  ;\n";
	cpp << "}\n\n";
}

void OCML_Application::run()
{
	std::string file_name;

	// Process model and generate outputs
	file_name = "Options.html";
	if (save_file("Save - Options documentation", file_name))
		print_html_documentation(file_name);

	file_name = "initialize_tree.i";
	if (save_file("Save - The options hierarchy initialization C++ code", file_name))
		print_initialization_tree(file_name);

	file_name = "rule_validator.cpp";
	if (save_file("Save - Rule validator C++ code", file_name))
		print_rule_validator(file_name);
}
