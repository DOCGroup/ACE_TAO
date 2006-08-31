/*<--$ID  tao.lu@vanderbilt.edu -->*/

#include "XSLTransformer.h"

//Use the xalan and xerces namespace
XALAN_USING_XERCES(XMLPlatformUtils)
//XALAN_USING_XALAN(XalanTransformer)
XALAN_CPP_NAMESPACE_USE

XSLTransformer::~XSLTransformer( )
{
	// Terminate Xalan...
	XalanTransformer::terminate();
	// Terminate Xerces...
	XMLPlatformUtils::Terminate();
	// Clean up the ICU, if it's integrated...
	XalanTransformer::ICUCleanUp();
}

void XSLTransformer::executeTransform( )
{

	int _in[3000];

	// Call the static initializer for Xerces.
	XMLPlatformUtils::Initialize();
	// Initialize Xalan.
	XalanTransformer::initialize();
	// Create a XalanTransformer.
	XalanTransformer theXalanTransformer;


	// The return form the transform method
	int theResult = -1;
	try
	{
		// Do the transform.
		cout<<"return Code: "<<theResult<<endl;

		const char * p1 = inputFile.c_str();
		const char * p2 = xsltFile.c_str();
		const char * p3 = outputFile.c_str();
		cout<<p1<<endl;

		theResult = theXalanTransformer.transform( p1, p2, p3);
		cout<<"return Code: "<<theResult<<endl;
		if( theResult != 0 )
			  cout<<"Error: " << theXalanTransformer.getLastError( ) << endl;
	}
	catch(...)
	{
		string res;
		res = "An unknown error occurred during final XSLTransformation. \n";
		res += "Input name=" + inputFile + "output name=" + outputFile + "transform name=" + xsltFile;
		cout << res << endl;
	}

}
