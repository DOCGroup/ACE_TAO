// file      : CCF/Example/CIDL/LocalExecutorMapping/cidlc.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CompilerElements/FileSystem.hpp"
#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/Preprocessor.hpp"
#include "CCF/CompilerElements/Diagnostic.hpp"
#include "CCF/CompilerElements/Context.hpp"

#include "CCF/CodeGenerationKit/CommandLine.hpp"
#include "CCF/CodeGenerationKit/CommandLineParser.hpp"
#include "CCF/CodeGenerationKit/CommandLineDescriptor.hpp"

#include "CCF/CIDL/LexicalAnalyzer.hpp"
#include "CCF/CIDL/Parser.hpp"
#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/SemanticAction/Impl/Factory.hpp"

#include "ExecutorMappingGenerator.hpp"

#include <iostream>

using std::cerr;
using std::endl;

using namespace CCF;
using namespace CIDL;
using namespace SyntaxTree;

int main (int argc, char* argv[])
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
    Preprocessor pp (isa);

    if (cl.get_value ("preprocess-only", false))
    {
      while (true)
      {
        Preprocessor::int_type i = pp.next ();

        if (pp.eos (i)) break;

        Preprocessor::char_type c = pp.to_char_type (i);

        std::cout << c ;
      }
      return 0;
    }


    Diagnostic::Stream diagnostic_stream;


    LexicalAnalyzer lexer (pp);
    TokenList token_stream;

    //@@ bad token comparison
    for (TokenPtr token = lexer.next ();; token = lexer.next ())
    {
      token_stream.push_back (token);
      // cerr << token << endl;
      if (ReferenceCounting::strict_cast<EndOfStream> (token) != 0) break;
    }

    if (token_stream.size () < 2)
    {
      cerr << "no tokens produced so nothing to parse" << endl;
      return 0;
    }

    TranslationUnitPtr unit (new TranslationUnit);

    //-----------------------------------------------------------------
    //@@ exeprimental code

    //Create .builtin region
    {
      TranslationRegionPtr builtin (
        new TranslationRegion (fs::path (".builtin"),
                               unit->table (),
                               unit->create_order ()));
      unit->insert (builtin);

      // Inject built-in types into the file scope of this
      // translation region

      ScopePtr s = builtin->scope ();

      s->insert (BuiltInTypeDeclPtr (new ObjectDecl           (s)));
      s->insert (BuiltInTypeDeclPtr (new ValueBaseDecl        (s)));
      s->insert (BuiltInTypeDeclPtr (new AnyDecl              (s)));
      s->insert (BuiltInTypeDeclPtr (new BooleanDecl          (s)));
      s->insert (BuiltInTypeDeclPtr (new CharDecl             (s)));
      s->insert (BuiltInTypeDeclPtr (new DoubleDecl           (s)));
      s->insert (BuiltInTypeDeclPtr (new FloatDecl            (s)));
      s->insert (BuiltInTypeDeclPtr (new LongDecl             (s)));
      s->insert (BuiltInTypeDeclPtr (new LongDoubleDecl       (s)));
      s->insert (BuiltInTypeDeclPtr (new LongLongDecl         (s)));
      s->insert (BuiltInTypeDeclPtr (new OctetDecl            (s)));
      s->insert (BuiltInTypeDeclPtr (new ShortDecl            (s)));
      s->insert (BuiltInTypeDeclPtr (new StringDecl           (s)));
      s->insert (BuiltInTypeDeclPtr (new UnsignedLongDecl     (s)));
      s->insert (BuiltInTypeDeclPtr (new UnsignedLongLongDecl (s)));
      s->insert (BuiltInTypeDeclPtr (new UnsignedShortDecl    (s)));
      s->insert (BuiltInTypeDeclPtr (new VoidDecl             (s)));
      s->insert (BuiltInTypeDeclPtr (new WcharDecl            (s)));
      s->insert (BuiltInTypeDeclPtr (new WstringDecl          (s)));

    }

    //Create implied #include <Components.idl>
    {
      TranslationRegionPtr builtin (
        new ImpliedIncludeTranslationRegion (fs::path ("Components.idl"),
                                             unit->table (),
                                             unit->create_order ()));
      unit->insert (builtin);

      ScopePtr fs = builtin->scope ();
      ModulePtr m (new Module (SimpleName("Components"), fs));
      fs->insert (m);

      LocalInterfaceDefPtr i (
        new LocalInterfaceDef (SimpleName ("EnterpriseComponent"),
                               m,
                               ScopedNameSet ()));

      m->insert (i);
    }

    TranslationRegionPtr tr (
      new PrincipalTranslationRegion (file_path,
                                      unit->table (),
                                      unit->create_order ()));
    unit->insert (tr);


    CompilerElements::Context context;
    context.set ("file-path", file_path);

    bool trace = cl.get_value ("trace-semantic-actions", false);

    context.set ("idl2::semantic-action::trace", trace);
    context.set ("idl3::semantic-action::trace", trace);
    context.set ("cidl::semantic-action::trace", trace);

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

    SemanticAction::Impl::Factory actions (context, diagnostic_stream, tr);

    //-----------------------------------------------------------------

    Parser parser (context, diagnostic_stream, lexer, actions);

    IDL2::Parsing::parse (token_stream.begin (),
                          token_stream.end (),
                          parser.start ());

    if (diagnostic_stream.error_count () != 0) return -1;

    // Generate executor mapping
    {
      lem_gen.generate (cl, unit);
    }
  }
  catch (Declaration::NotInScope const&)
  {
    cerr << "exception: " << "Declaration::NotInScope" << endl;
  }
  catch (NodePtr::NotInitialized const& e)
  {
    cerr << "exception: " << e.what () << endl;
  }
  catch (...)
  {
    cerr << "exception: " << "unknow" << endl;
  }
}
