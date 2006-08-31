#include "MPC_Interpreter/MPC_Visitor.h"
#include <fstream>

namespace PICML
{
  MPC_Visitor::MPC_Visitor (const std::string& outputPath)
    : outputPath_ (outputPath)
  {

  }

  MPC_Visitor::~MPC_Visitor ()
  {

  }

  void MPC_Visitor::Visit_StubProject (const StubProject &stub_proj)
  {

  }

  void MPC_Visitor::Visit_ServantProject (const ServantProject &skeleton_proj)
  {

  }

  void MPC_Visitor::Visit_ExecutorProject (const ExecutorProject &exec_proj)
  {

  }

  // Visit Component Assembly
  void MPC_Visitor::Visit_MPC (const MPC &proj_folder)
  {
	  // Start for interpretation
	  // Step 1: Create Workspace for the projects
	  std::set<PICML::Workspaces> workspace = proj_folder.Workspaces_children();
	  for (std::set<PICML::Workspaces>::iterator iter = workspace.begin ();
	  iter != workspace.end ();
	  iter ++)
	  {
		  // Step 1: Get the name of the workspace to be generated
		  std::string wspace_name;
		  iter->GetStrValue ("name", wspace_name);

		  // Step 2: Write out the header for the workspace
		  // Outputpath refers to a directory
		  // The target file is : <dir> + "\" + fileName + ".cidl"
		  std::string file_name = this->outputPath_ + "\\" + wspace_name + ".mwc";
		  std::ofstream file_stream (file_name.c_str ());

		  MPCStream mpc_stream (file_stream, this->outputPath_);
		  mpc_stream.create_workspace (* iter);
	  }

	  /// Create the individual MPC files for each artifact
	  std::set<PICML::Project> projects = proj_folder.Project_children ();
	  for (std::set<PICML::Project>::iterator proj_iter = projects.begin ();
	  proj_iter != projects.end ();
	  proj_iter ++)
	  {
		 // Step 1: Get the name of the project file
		  std::string proj_name;
		  proj_iter->GetStrValue ("name", proj_name);

		  // Step 2: Write the stub skeleton and executor IDL projects
		  std::string file_name = this->outputPath_ + "\\" + proj_name + ".mpc";
		  std::ofstream file_stream (file_name.c_str ());

		  MPCStream mpc_stream (file_stream, this->outputPath_);
		  mpc_stream.create_project (* proj_iter);
	  }
  }

}
