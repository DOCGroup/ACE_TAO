// file      : CIDLC/cidlc.cpp
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
#include "ServantGenerator.hpp"
#include "ExecImplGenerator.hpp"
#include "RepositoryIdGenerator.hpp"
#include "DescriptorGenerator.hpp"
#include "SizeTypeCalculator.hpp"

#include "ciao/Version.h" // for --version

using std::cerr;
using std::endl;

class ErrorDetector : public std::streambuf
{
public:
  ErrorDetector (std::ostream& os)
    : error_ (false),
      os_ (os),
      prev_ (*os_.rdbuf ())
  {
    os_.rdbuf (this);
  }

  ~ErrorDetector ()
  {
    os_.rdbuf (&prev_);
  }

  virtual int_type
  overflow (int_type c)
  {
    error_ = true;
    return prev_.sputc (c);
  }

  virtual int
  sync ()
  {
    return prev_.pubsync ();
  }

  bool
  error () const throw ()
  {
    return error_;
  }

private:
  bool error_;
  std::ostream& os_;
  std::streambuf& prev_;
};

using namespace CCF::CompilerElements;
using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

//FUZZ: disable check_for_improper_main_declaration

int
main (int argc, char* argv[])
{
  try
  {
    // Parsing command line options and arguments.
    //

    CL::Description cld (argv[0]);

    cld.add_option (CL::OptionDescription (
                      "version",
                      "Display version information and exit.",
                      CL::OptionType::flag));

    cld.add_option (CL::OptionDescription (
                      "help",
                      "Display usage information and exit.",
                      CL::OptionType::flag));

    cld.add_option (CL::OptionDescription (
                      "help-html",
                      "Dump usage information in html format and exit.",
                      CL::OptionType::flag));

    cld.add_option (CL::OptionDescription (
                      "I",
                      "dir",
                      "Add the directory dir to the list of directories to "
                      "be searched for header files.",
                      CL::OptionType::value));

    cld.add_option (CL::OptionDescription (
                      "D",
                      "name[=definition]",
                      "Predefine name as a macro with the value definition "
                      "if present, 1 otherwise.",
                      CL::OptionType::value));

    cld.add_option (CL::OptionDescription (
                      "gen-exec-impl",
                      "Generate the executor implementation classes.",
                      CL::OptionType::flag));

    ExecutorMappingGenerator::options (cld);
    ServantGenerator::options (cld);
    DescriptorGenerator::options (cld);
    ExecImplGenerator::options (cld);

    cld.add_option (CL::OptionDescription (
                      "trace-semantic-actions",
                      "Turn on semantic actions tracing facility.",
                      CL::OptionType::flag));

    cld.add_option (CL::OptionDescription (
                      "preprocess-only",
                      "Run preprocessor only and output result to stdout.",
                      CL::OptionType::flag));

    cld.add_argument ("cidl file");


    CommandLine cl;

    if (!parse (argc, argv, cld, cl))
    {
      cerr << "try " << argv[0] << " --help for usage information" << endl;
      return -1;
    }

    if (cl.get_value ("version", false))
    {
      cerr << "CIAO CIDL Compiler " << CIAO_VERSION
           << " (built on " << __DATE__ << " at " << __TIME__ << ")" << endl;
      return 0;
    }

    if (cl.get_value ("help", false))
    {
      CL::print_text (cerr, cld);
      return 0;
    }

    if (cl.get_value ("help-html", false))
    {
      CL::print_html (cerr, cld);
      return 0;
    }


    fs::ifstream ifs;
    ifs.exceptions (std::ios_base::badbit | std::ios_base::failbit);

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
    ifs.exceptions (std::ios_base::iostate (0));

    std::istream& is = ifs.is_open ()
      ? static_cast<std::istream&> (ifs)
      : static_cast<std::istream&> (std::cin);

    InputStreamAdapter isa (is);


    // Extract preprocessor symbol definitions (-D). We are not
    // interested in the values, just symbols.
    //
    CPP::Symbols symbols;

    for (CommandLine::OptionsIterator
           i (cl.options_begin ()), e (cl.options_end ()); i != e; ++i)
    {
      if (i->name () == "D")
      {
        std::string def (i->value ());

        // Get rid of '=value' in 'symbol=value' definitions.
        //
        std::size_t p (def.find ('='));
        if (p != std::string::npos)
          symbols.insert (std::string (def, 0, p));
        else
          symbols.insert (def);
      }
    }

    CPP::Preprocessor pp (isa, symbols);

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

    Diagnostic::Stream dout;
    ErrorDetector detector (cerr);

    LexicalAnalyzer lexer (pp);

    TokenList token_stream;

    //@@ bad token comparison
    for (TokenPtr token = lexer.next ();; token = lexer.next ())
    {
      // cerr << typeid(*(token.in ())).name () << " : " << token << endl;
      token_stream.push_back (token);
      if (ReferenceCounting::strict_cast<EndOfStream> (token) != 0) break;
    }

    if (token_stream.size () < 2)
    {
      cerr << "no tokens produced so nothing to parse" << endl;
      return 0;
    }

    TranslationUnit tu (file_path, 0);

    // Initialize compilation context.
    //
    CCF::CompilerElements::Context context;
    context.set ("file-path", file_path);
    context.set ("trace-semantic-action",
                 cl.get_value ("trace-semantic-actions", false));
    context.set ("cpp-symbols", symbols);


    // Extract include search paths.
    //

    std::vector<fs::path> include_paths;

    for (CommandLine::OptionsIterator
           i (cl.options_begin ()), e (cl.options_end ()); i != e; ++i)
    {
      std::string path;

      if (i->name () == "I")
      {
        path = i->value ();

        try
        {
          include_paths.push_back (fs::path (path, fs::native));
        }
        catch (fs::filesystem_error const&)
        {
          cerr << "error: invalid filesystem path '" << path << "' "
               << "provided with the -I option" << endl;

          return 1;
        }
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

    if (dout.error_count () != 0) return 1;

    // This is a little hack to make CIDL compiler signal
    // error conditions by return value. Once I redesign
    // diagnostic in CCF this should be removed.

    if (detector.error ()) return 1;


    // Generate executor mapping.
    {
      ExecutorMappingGenerator lem_gen;
      lem_gen.generate (cl, tu, file_path);
    }

    // Calculate the size type of everything in the AST.
    // This must be executed before the servant code generator.
    {
      SizeTypeCalculator sizetype_calc;
      sizetype_calc.calculate (tu);
    }

    // Compute repository IDs in a separate pass.
    {
      RepositoryIdGenerator repid_gen;
      if (!repid_gen.generate (tu)) return 1;
    }

    // Generate servant code.
    {
      ServantGenerator svnt_gen (cl);
      svnt_gen.generate (tu, file_path);
    }

    // Generate executor implementation code.
    if (cl.get_value ("gen-exec-impl", false))
    {
      ExecImplGenerator impl_gen (cl);
      impl_gen.generate (tu, file_path);
    }

    // Generate descriptor code.
    {
      DescriptorGenerator desc_gen;
      desc_gen.generate (cl, tu);
    }

    return 0;
  }
  catch (std::bad_cast const&)
  {
    cerr << "bad cast exception" << endl;
  }
  catch (InvalidName const&)
  {
    cerr << "invalid name exception" << endl;
  }
  /*
  catch (...)
  {
    cerr << "caught unknown exception" << endl;
  }
  */

  return 1;
}
