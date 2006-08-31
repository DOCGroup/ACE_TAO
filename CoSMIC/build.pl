eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'  # -*- PERL -*-
    & eval 'exec perl -S $0 $argv:q'
    if 0;

$dsp_builder = "$ENV{'ACE_ROOT'}/bin/msvc_mpc_auto_compile.pl";
$dsp_args = "-v -s -Release"; # -rebuild

@projects = (
  "PIM/PICML/decorators",
  "PIM/PICML/interpreters/BGML_Interpreter",
  "PIM/PICML/interpreters/cidl",
  "PIM/PICML/interpreters/domain",
  "PIM/PICML/interpreters/EQAL",
  "PIM/PICML/interpreters/MPC",
  "PIM/PICML/interpreters/packaging",
  "PIM/PICML/interpreters/plan",
# "PSM/CCM/CADML/Component",
  "PSM/CCM/OCML/interpreters/OCML_Interpreter",
  "PSM/CCM/OCML/interpreters/OCMLConfiguratorLibrary",
  "PSM/CCM/OCML/interpreters/OCMLConfiguratorInterpreter",
  "PSM/CCM/OCML/interpreters/TAOServiceConfExporter",
  "PSM/CCM/OCML/interpreters/TAOServiceConfExportInterpreter",
  "PIM/PICML/interpreters/IDLGenerator"
#  "PSM/CCM/COMPASS/interpreters",
#  "PSM/CCM/EQAL/interpreters",
#  "PSM/CCM/FESML/interpreters",
#  "PSM/CCM/MIDCESS/interpreters"
);

$dsp_args_no_env = "-v -s -u -Release"; # -rebuild
@projects_no_env = (
  "Installer/RegisterParadigm",
   "PIM/PICML/interpreters/IDLImporter"
);

foreach $project (@projects) {
    $args = "$dsp_args -dir $project"; 
    system "$dsp_builder $args";
};

foreach $project (@projects_no_env) {
    $args = "$dsp_args_no_env -dir $project";
    system "$dsp_builder $args";
};

