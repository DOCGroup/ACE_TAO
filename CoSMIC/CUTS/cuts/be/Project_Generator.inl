// $Id$

//
// init
//
CUTS_INLINE
void CUTS_BE_Project_Generator::init (const std::string & outdir,
                                      CUTS_Dependency_Graph * graph)
{
  this->outdir_ = outdir;
  this->graph_ = graph;
}

CUTS_INLINE
void CUTS_BE_Project_Generator::
write_project (const PICML::ComponentImplementationContainer & container,
               const CUTS_BE_Preprocess_Data & ppd)
{

}
