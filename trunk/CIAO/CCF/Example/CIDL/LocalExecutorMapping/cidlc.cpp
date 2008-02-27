// file      : CCF/Example/CIDL/LocalExecutorMapping/cidlc.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include <vector>
#include <iostream>

#include "CCF/CompilerElements/Context.hpp"
#include "CCF/CompilerElements/FileSystem.hpp"
#include "CCF/CompilerElements/Diagnostic.hpp"
#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/Preprocessor.hpp"

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineParser.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

#include "CCF/CIDL/LexicalAnalyzer.hpp"
#include "CCF/CIDL/Parser.hpp"
#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/SemanticAction/Impl/Factory.hpp"

#include "ExecutorMappingGenerator.hpp"

using std::cerr;
using std::endl;

using namespace CCF::CompilerElements;
using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

//FUZZ: disable check_for_improper_main_declaration

int
main (int argc, char* argv[])
{
  try
  {
    // Parsing command line options and arguments
    //
    //
    CommandLine cl;

    if (!parse (argc, argv, cl))
    {
      cerr << "command line syntax error" << endl;
      cerr << "try " << argv[0] << " --help for usage information" << endl;
      return -1;
    }

    ExecutorMappingGenerator lem_gen;

    if (cl.get_value ("help", false) || cl.get_value ("help-html", false))
    {
      CL::Description d (argv[0]);

      lem_gen.options (d);

      d.add_option (CL::OptionDescription (
                      "trace-semantic-actions",
                      "Turn on semnatic actions tracing facility.",
                      true));

      d.add_option (CL::OptionDescription (
                      "preprocess-only",
                      "Run preprocessor only and output result to stdout.",
                      true));

      d.add_option (CL::OptionDescription (
                      "help",
                      "Display usage information and exit.",
                      true));

      d.add_option (CL::OptionDescription (
                      "help-html",
                      "Dump usage information in html format and exit.",
                      true));

      d.add_argument ("cidl file");

      if (cl.get_value ("help-html", false)) CL::print_html (cerr, d);
      else CL::print_text (cerr, d);

      return 0;
    }

    fs::ifstream ifs;
    ifs.exceptions (ios_base::badbit | ios_base::failbit);

    fs::path file_path;

    CommandLine::ArgumentsIterator i = cl.arguments_begin ();

    if (i != cl.arguments_end ())
    {
      try
      {
        file_path = fs::path (*i, fs::native);
        ifs.open (file_path, std::ios_base::in);
      }
      catch (fs::filesystem_error const&)
      {
        cerr << *i << ": error: unable to open in read mode" << endl;
        return -1;
      }
      catch (std::ios_base::failure const&)
      {
        cerr << *i << ": error: unable to open in read mode" << endl;
        return -1;
      }
    }

    //@@ for some reason ifs throws exception if I don't reset it to
    //   original state. It probably has something to do with call to
    //   get after eof.
    ifs.exceptions (ios_base::iostate (0));

    std::istream& is = ifs.is_open ()
      ? static_cast<std::istream&> (ifs)
      : static_cast<std::istream&> (std::cin);

    InputStreamAdapter isa (is);
    CPP::Preprocessor pp (isa);

    if (cl.get_value ("preprocess-only", false))
    {
      while (true)
      {
        CPP::Token t (pp.next ());

        if (t == CPP::Token::eos) break;

        std::cout << t;
      }
      return 0;
    }

    //}

    Diagnostic::Stream dout;

    LexicalAnalyzer lexer (pp);

    TokenList token_stream;

    //@@ bad token comparison
    for (TokenPtr token = lexer.next ();; token = lexer.next ())
    {
      //cerr << token << endl;
      token_stream.push_back (token);
      if (ReferenceCounting::strict_cast<EndOfStream> (token) != 0) break;
    }

    if (token_stream.size () < 2)
    {
      cerr << "no tokens produced so nothing to parse" << endl;
      return 0;
    }

    TranslationUnit tu;

    // Initialize compilation context.
    //
    CCF::CompilerElements::Context context;
    context.set ("file-path", file_path);
    context.set ("trace-semantic-action",
                 cl.get_value ("trace-semantic-actions", false));


    // Extract include search paths.
    //

    std::vector<fs::path> include_paths;

    for (CommandLine::OptionsIterator
           i (cl.options_begin ()), e (cl.options_end ()); i != e; ++i)
    {
      if (i->name () == "I")
      {
        include_paths.push_back (fs::path (i->value (), fs::native));
      }
      else if (i->name ()[0] == 'I')
      {
        std::string opt (i->name ());
        std::string path (opt.begin () + 1, opt.end ());
        include_paths.push_back (fs::path (path, fs::native));
      }
    }

    context.set ("include-search-paths", include_paths);

    // Instantiate semantic actions factory.
    //
    SemanticAction::Impl::Factory actions (context, dout, tu);

    Parser parser (context, dout, lexer, actions);

    //@@ should be able to use CIDL here. Or better yet get rid of this
    //   function completely.
    //
    CCF::IDL2::Parsing::parse (token_stream.begin (),
                               token_stream.end (),
                               parser.start ());

    if (dout.error_count () != 0) return -1;


    // Generate executor mapping.
    //
    lem_gen.generate (cl, tu, file_path);

  }
  catch (std::bad_cast const&)
  {
    cerr << "bad cast exception" << endl;
  }
  catch (InvalidName const&)
  {
    cerr << "invalid name exception" << endl;
  }
  catch (std::exception const& e)
  {
    cerr << "caught standard exception " << e.what () << endl;
  }
  catch (...)
  {
    cerr << "caught unknown exception" << endl;
    return -1;
  }
}
