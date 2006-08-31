// $Id$

//
// init
//
CUTS_INLINE
void CUTS_BE_File_Generator_Manager::init (const std::string & outdir)
{
  this->outdir_ = outdir;
}

//
// preprocess_data
//
CUTS_INLINE
const CUTS_BE_Preprocess_Data & CUTS_BE_File_Generator_Manager::
preprocess_data (void) 
{
  return this->ppd_;
}
