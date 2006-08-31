// OCML_Application.h: interface for the OCML_Application class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OCML_APPLICATION_H__A3B4F86C_AC59_499F_B216_18E4AAACC62E__INCLUDED_)
#define AFX_OCML_APPLICATION_H__A3B4F86C_AC59_499F_B216_18E4AAACC62E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BON.h"

/*
 *  class OCML_Application
 *
 *  Provides the methods which processes the model and produces output
 */
class OCML_Application
{
public:
	OCML_Application(BON::Folder& root);
	virtual ~OCML_Application();

	// Generates the output files
	void run();

protected:
	// Prints out html documentation
	void print_html_documentation(const std::string& file_name);
	// Prints out C++ tree initialization code -included in Configurator-
	void print_initialization_tree(const std::string& file_name);
	// Prints out C++ rule validator code -included in Configurator-
	void print_rule_validator(const std::string& file_name);

private:
	// The root folder set at constructor
	BON::Folder& root_;
};

#endif // !defined(AFX_OCML_APPLICATION_H__A3B4F86C_AC59_499F_B216_18E4AAACC62E__INCLUDED_)
