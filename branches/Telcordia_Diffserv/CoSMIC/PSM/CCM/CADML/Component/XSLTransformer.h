/*<--$ID  tao.lu@vanderbilt.edu -->*/

#ifndef XSLTRANSFORMER_H
#define XSLTRANSFORMER_H

#include <vector>
#include <string>
#include <iostream>
#include <Include/PlatformDefinitions.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <XalanTransformer/XalanTransformer.hpp>


using namespace std;

class XSLTransformer
{
public:
  XSLTransformer( const string& input,
                  const string& xslt,
                  const string& output )
      : inputFile(input),
        xsltFile(xslt),
        outputFile(output)   {	}

	~XSLTransformer( );

  void executeTransform( );
private:
  string inputFile;
  string xsltFile;
  string outputFile;
};

#endif // XSLTRANSFORMER_H
