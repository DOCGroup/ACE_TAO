#ifndef MPCSTREAM_H
#define MPCSTREAM_H

#include <ostream>
#include <string>
#include <vector>
#include "PICML/PICML.h"

#include "MPC_Interpreter/MPC_Export.h"

// A dummy structure to inform TAO_OutStream's << operator to  put a newline
// and use the current indentation for the succeeding line
struct NL
{
  NL (void);
};

struct INDENT
{
  //   Increase the indentation level, if the "do_now" parameter is
  //   not zero then the <indent> method is called on the stream.
  //
  INDENT (int do_now = 0);

  const int do_now_;
};

struct UNINDENT
{
  //   Decrease the indentation level, if the "do_now" parameter is
  //   not zero then the <indent> method is called on the stream.
  //
  UNINDENT (int do_now = 0);

  const int do_now_;
};

extern const NL nl;
extern const INDENT idt;
extern const INDENT idt_nl;
extern const UNINDENT uidt;
extern const UNINDENT uidt_nl;

class  MPCStream
{
public:
  MPC_Export MPCStream (std::ostream& strm, std::string output_path_);
  // constructor.

  MPC_Export ~MPCStream (void);
  // destructor.

  MPC_Export void incr_indent (unsigned short flag = 1);
  // increment the indentation level and by default actually indent the output
  // accordingly

  MPC_Export void decr_indent (unsigned short flag = 1);
  // decrease the indentation level and by default actually indent the output
  // accordingly

  MPC_Export inline void reset (void);
  // reset indentation level to 0

  MPC_Export void indent (void);
  // indent starting next line

  MPC_Export void nl (void);
  // put a newline and indent on the next line

  // Workspace definitions
  MPC_Export void create_workspace (const PICML::Workspaces &workspace);
  MPC_Export void write_mwc_footer ();
  MPC_Export void write_mwc_header ();

  // Project related definitions
  MPC_Export void create_project (const PICML::Project &project);
  MPC_Export void create_stub_definition (const PICML::StubProject &);
  MPC_Export void create_skeleton_definition (const PICML::ServantProject &, bool);
  MPC_Export void create_executor_definition (const PICML::ExecutorProject &);
  MPC_Export bool dependent_project (PICML::ImplementationArtifact &artifact,
									 std::string &dependent_proj);
  MPC_Export void create_project_defn (std::string& proj_name,
									   std::string root_proj);
  MPC_Export void create_after_defn (std::string& deo_proj_name);
  MPC_Export void create_shared_defn (std::string& name);
  MPC_Export void create_export_defn (std::string& shared_name);
  MPC_Export void create_close_proj_defn ();
  MPC_Export void create_dynamic_flags_defn (std::string& shared_name);
  MPC_Export void create_idl_file_defn (std::vector<std::string>& idl_files);
  MPC_Export void create_source_file_defn (std::vector<std::string>& idl_c_files);
  MPC_Export void create_lib_path_defn (std::string& lib_path);
  MPC_Export void create_cidl_defn (PICML::ImplementationArtifact& artifact);
  MPC_Export void create_export_macro (std::string& shared_name);
  MPC_Export std::string skel_export_macro (PICML::Project project);
  MPC_Export std::vector<std::string> generate_dependant_libs (std::set<PICML::ArtifactDependsOn>& art_refs);
  MPC_Export void create_dependant_libs_defn (std::vector<std::string>& list);

  MPC_Export MPCStream &operator<< (const std::string &);
  MPC_Export MPCStream &operator<< (const char *);
  MPC_Export MPCStream &operator<< (const long &);
  MPC_Export MPCStream &operator<< (const unsigned long &);
  MPC_Export MPCStream &operator<< (const double &);
  MPC_Export MPCStream &operator<< (const char &);

  // = MANIPULATORS
  MPC_Export MPCStream &operator<< (const NL &);
  MPC_Export MPCStream &operator<< (const INDENT &);
  MPC_Export MPCStream &operator<< (const UNINDENT &);

private:
  std::ostream& strm_;
  std::string output_path_;
  std::string stub_project_;
  std::string servant_project_;
  int indent_level_;
  std::vector<std::string> cidl_file_;
  std::vector<std::string> idl_files_;

private:
  void upcase (const char *);
};

#endif // MPCStream_H
