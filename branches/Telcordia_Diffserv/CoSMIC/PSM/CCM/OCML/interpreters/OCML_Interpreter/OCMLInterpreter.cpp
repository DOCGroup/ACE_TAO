#include "stdafx.h"

#include "windows.h"
#include "commdlg.h"

#include <iostream>

#include "OCMLUtils.h"
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include "OCMLInterpreter.h"

OCMLInterpreter::OCMLInterpreter()
{
  // Initialize the XML4C2 system.
  try
    {
      xercesc::XMLPlatformUtils::Initialize();
    }
  catch(const xercesc::XMLException& toCatch)
    {
      char *pMsg = xercesc::XMLString::transcode(toCatch.getMessage());
      std::cerr << "Error during Xerces-c Initialization.\n"
		<< "  Exception message:" << pMsg;
      xercesc::XMLString::release(&pMsg);
      throw;
    }
  
  impl_ =
    xercesc::DOMImplementationRegistry::getDOMImplementation(XStr("Core"));
}

OCMLInterpreter::~OCMLInterpreter()
{
  xercesc::XMLPlatformUtils::Terminate();
}

void
OCMLInterpreter::write(const char* file_name, xercesc::DOMDocument* doc)
{
  xercesc::DOMWriter* theSerializer = impl_->createDOMWriter();
  xercesc::XMLFormatTarget *myFormTarget =
    new xercesc::LocalFileFormatTarget(file_name);
  xercesc::DOMNode* root_node = doc->getDocumentElement()->getFirstChild();
  theSerializer->writeNode(myFormTarget, *root_node);
}

xercesc::DOMDocument* 
OCMLInterpreter::create_document(std::set<BON::Model>& root_models)
{
  // create DOMDocument
  xercesc::DOMDocument* doc = impl_->createDocument(0, XStr("OCML"), 0);

  // Traverse the root models and ...
  for (std::set<BON::Model>::iterator iter = root_models.begin();
       iter != root_models.end(); ++iter)
    {
      if (OCML_BON::Option_Category(*iter))
	{
	  // call xml_export functions of each Option_Category
	  // and insert the returning node into the xml node
	  xercesc::DOMNode* child =
	    OCML_BON::Option_Category(*iter)->xml_export(doc);
	  doc->getDocumentElement()->appendChild(child);
	}
    }
  return doc;
}

bool
OCMLInterpreter::save_file(const char* title, std::string& file_name)
{
  OPENFILENAME ofn;

  // allocate memory for the file name
  char *szFile = new char[MAX_PATH];
  strcpy(szFile, file_name.c_str());

  // Initialize the OPENFILENAME structure
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = 0;
  ofn.lpstrFile = szFile;
  ofn.lpstrTitle = title;

  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = "All\0*.*\0"; // "All\0*.*\0Text\0*.txt\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.Flags = OFN_PATHMUSTEXIST;

  // GetSaveFileName() system call shows up the save file dialog

  bool result = false;
  if (GetSaveFileName (&ofn) != 0)
  {
    file_name = szFile;
	result = true;
  }
  delete[] szFile;
  return result;
}
