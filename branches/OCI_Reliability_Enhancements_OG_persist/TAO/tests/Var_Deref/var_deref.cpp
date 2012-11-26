// $Id$

#include "TestC.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int errors = 0;
  Test::FixedStruct *ft = 0;
  Test::FixedStruct_var ftvar ( new Test::FixedStruct );
  Test::FixedStruct_var nftvar (0);

  Test::FixedSeq *fs = 0;
  Test::FixedSeq_var fsvar ( new Test::FixedSeq (0) );
  Test::FixedSeq_var nfsvar (0);

  Test::VarStruct *vt = 0;
  Test::VarStruct_var vtvar ( new Test::VarStruct );
  Test::VarStruct_var nvtvar (0);

  Test::VarSeq *vs = 0;
  Test::VarSeq_var vsvar (new Test::VarSeq (0) );
  Test::VarSeq_var nvsvar (0);

  ft = ftvar;
  if (ft == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Expected a non-null FixedStruct ref, got null\n"));
      ++errors;
    }
  ft = nftvar;
  if (ft != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Expected a null FixedStruct ref, got non-null\n"));
      ++errors;
    }

  fs = fsvar;
  if (fs == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Expected a non-null FixedSeq ref, got null\n"));
      ++errors;
    }
  fs = nfsvar;
  if (fs != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Expected a null FixedSeq ref, got non-null\n"));
      ++errors;
    }

  vt = vtvar;
  if (vt == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Expected a non-null VarStruct rev, got null\n"));
      ++errors;
    }
  vt = nvtvar;
  if (vt != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Expected a null VarStruct rev, got non-null\n"));
      ++errors;
    }

  vs = vsvar;
  if (vs == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Expected a non-null VarSeq rev, got null\n"));
      ++errors;
    }
  vs = nvsvar;
  if (vs != 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Expected a null VarSeq rev, got non-null\n"));
      ++errors;
    }

  if (errors == 0)
    ACE_DEBUG ((LM_DEBUG, "Success: all dereferences worked as expected\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "Failure: %d dereferences didn't work\n", errors));

  return errors;
}
