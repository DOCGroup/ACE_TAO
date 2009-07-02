// file      : Test/CIDL/Compiler/driver.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CompilerElements/Context.hpp"
#include "CCF/CompilerElements/FileSystem.hpp"
#include "CCF/CompilerElements/Diagnostic.hpp"
#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/Preprocessor.hpp"

#include "CCF/CIDL/LexicalAnalyzer.hpp"
#include "CCF/CIDL/Parser.hpp"
#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/SemanticAction/Impl/Factory.hpp"

#include "Generator.hpp"

#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

using namespace CCF::CompilerElements;
using namespace CCF::CIDL;
using namespace SemanticGraph;

//FUZZ: disable check_for_improper_main_declaration

int
main ()
{
  try
  {
    Diagnostic::Stream dout;

    fs::path file_path ("stdout");

    InputStreamAdapter isa (std::cin);
    CPP::Preprocessor pp (isa);

    LexicalAnalyzer lexer (pp);

    TokenList token_stream;

    //@@ bad token comparison
    for (TokenPtr token = lexer.next ();; token = lexer.next ())
    {
      token_stream.push_back (token);
      if (ReferenceCounting::strict_cast<EndOfStream> (token) != 0) break;
    }

    if (token_stream.size () < 2)
    {
      cerr << "no tokens produced so nothing to parse" << endl;
      return 0;
    }

    TranslationUnit tu;

    // Compilation context.
    //
    CCF::CompilerElements::Context context;
    context.set ("file-path", file_path);
    context.set ("trace-semantic-action", false);


    SemanticAction::Impl::Factory actions (context, dout, tu);

    Parser parser (context, dout, lexer, actions);

    //@@ should be able to use IDL3 here. Or better yet get rid of this
    //   function completely.
    //
    CCF::IDL2::Parsing::parse (token_stream.begin (),
                               token_stream.end (),
                               parser.start ());

    if (dout.error_count () != 0) return -1;

    CIDL::Generator g;

    g.generate (tu);

  }
  catch (std::bad_cast const&)
  {
    cerr << "bad cast exception" << endl;
  }
  catch (InvalidName const&)
  {
    cerr << "invalid name exception" << endl;
  }
  catch (...)
  {
    cerr << "caught unknown exception" << endl;
    return -1;
  }
}
