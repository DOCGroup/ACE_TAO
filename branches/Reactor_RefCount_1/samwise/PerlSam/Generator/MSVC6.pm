# $Id$

package PerlSam::Generator::MSVC6;

use Cwd;
use Data::Dumper;
use FileHandle;
use strict;

###############################################################################
# Forward Declarations

sub GenerateExeProject (\%);
sub GenerateDLLProject (\%);
sub GenerateLIBProject (\%);

sub WorkspaceProject ($$\@);

sub ProjectHeader (\%\@$);
sub ProjectAllGroups ($\%\@);
sub ProjectGroup ($$$\%\@);
sub ProjectSource ($$\%\@);
sub ProjectFooter ();
sub ConfigSection (%);

###############################################################################
# Constructor

sub new (@)
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    bless ($self, $class);
    return $self;
}

###############################################################################
# Methods

my %tempdirs = ( 'Win32 Debug'          => 'Debug',
                 'Win32 Release'        => 'Release',
                 'Win32 Static Debug'   => 'Debug\Static',
                 'Win32 Static Release' => 'Release\Static',
                 'Win32 MFC Debug'      => 'Debug\MFC',
                 'Win32 MFC Release'    => 'Release\MFC'
               );  

sub GenerateWorkspace (\%)
{
    my $self = shift;
    my $data = shift;
  
    # Check to see if we even need to generate a workspace file
    if (!defined @{$data->{WORKSPACE}->{PROJECTLINKS}->{LIST}}) {
        return;
    }    
      
    my $string;
    
    $string .= <<EOWH;
Microsoft Developer Studio Workspace File, Format Version 6.00
# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!

EOWH

    foreach my $project (sort @{$data->{WORKSPACE}->{PROJECTLINKS}->{LIST}}) {
        my $description = $data->{PROJECTS}->{$project}->{DESCRIPTION};
        
        if (!defined $data->{PROJECTS}->{$project}->{NAME}) {
            print STDERR "Error: Cannot find project <$project>\n";
            next;
        }
        
        $string .= WorkspaceProject ($description, $project . ".dsp", @{$data->{PROJECTS}->{$project}->{DEPENDS}});
    
        if ($data->{PROJECTS}->{$project}->{TYPE} eq "library") {
            my @empty = ();
            $string .= WorkspaceProject ($description. " Static" , $project . "_static.dsp", @empty);
        }
    }
    $string .= <<EOWF;
###############################################################################

Global:

Package=<5>
{{{
}}}

Package=<3>
{{{
}}}

###############################################################################

EOWF

    my $file_name;

    if (defined $data->{WORKSPACE}->{NAME}) {
        $file_name = $data->{WORKSPACE}->{NAME};
    }
    else {
        $file_name = getcwd ();

        if ($file_name =~ m/\/([^\/]*)$/) {
            $file_name = $1;
        }
    }

    $file_name = $file_name . ".dsw";

    print "Creating Workspace: $file_name\n";

    my $file_handle = new FileHandle ($file_name, "w");
    binmode $file_handle;
    $string =~ s/\n/\r\n/g;
    print $file_handle $string;    
}

sub GenerateProjects (\%)
{
    my $self = shift;
    my $data = shift;

    foreach my $name (sort keys %{$data->{PROJECTS}}) {
        # Expand the IDL files
        
        PerlSam::Generator::ExpandIDLFiles (%{$data->{PROJECTS}->{$name}});

        # Store the data in easier variables
        
        my $description = $data->{PROJECTS}->{$name}->{DESCRIPTION};
        my $target = $data->{PROJECTS}->{$name}->{TARGET};
        my $type = $data->{PROJECTS}->{$name}->{TYPE};
        my $export = "";
        my $include = "";
        
        if (defined $data->{PROJECTS}->{$name}->{LIBINFO}->{EXPORT}) {
            $export = $data->{PROJECTS}->{$name}->{LIBINFO}->{EXPORT};
        }
        
        if (defined $data->{PROJECTS}->{$name}->{LIBINFO}->{INCLUDE}) {
            $include = $data->{PROJECTS}->{$name}->{LIBINFO}->{INCLUDE};
        }

        if (!defined $data->{PROJECTS}->{$name}->{LIBS}) {
            @{$data->{PROJECTS}->{$name}->{LIBS}} = [];
        }
         
        if ($type eq "executable") 
        {
            my $string = GenerateExeProject (%{$data->{PROJECTS}->{$name}});
        
            my $file_name = $name . ".dsp";

            print "Creating Project: $file_name\n";

            my $file_handle = new FileHandle ($file_name, "w");
            binmode $file_handle;
            $string =~ s/\n/\r\n/g;
            print $file_handle $string;    
        }
        elsif ($type eq "library") {
            my $string;
            my $file_name;
            my $file_handle;

            $string = GenerateDLLProject (%{$data->{PROJECTS}->{$name}});
            
            $file_name = $name . ".dsp";

            print "Creating Project: $file_name\n";

            $file_handle = new FileHandle ($file_name, "w");
            binmode $file_handle;
            $string =~ s/\n/\r\n/g;
            print $file_handle $string;    
            $file_handle->close ();
            
            my %newdata = %{$data->{PROJECTS}->{$name}};
            
            %newdata->{DESCRIPTION} .= " Static";
            
            $string = GenerateLIBProject (%newdata);
            
            $file_name = $name . "_static.dsp";

            print "Creating Project: $file_name\n";

            $file_handle = new FileHandle ($file_name, "w");
            binmode $file_handle;
            $string =~ s/\n/\r\n/g;
            print $file_handle $string;    

        }
        else {
            print STDERR "Error: Unrecognized type <$type> for $name\n";
            next;
        }
    }
}

###############################################################################
# Internal Methods

sub GenerateExeProject (\%)
{
    my $data = shift;

    my @configs = ('Win32 Release', 'Win32 Debug');
    my $type = 'executable';
    
    my $string = ProjectHeader (%{$data}, @configs, 0);

    my $libraries = PerlSam::Generator::ExpandLibraries (@{$data->{LIBS}});
    
    my $debug_link_opts;
    my $release_link_opts;
  
    foreach my $lib (split / /, $libraries) {
        $debug_link_opts .= $lib . "d.lib ";
        $release_link_opts .= $lib . ".lib ";
    }
    
    if ($data->{INSTALL} eq 'yes') {
        my $bindir = PerlSam::Generator::ConvertPathToRelative ('/bin');
        $debug_link_opts .= "/out:\"$bindir/$data->{TARGET}.exe\"";
        $release_link_opts .= "/out:\"$bindir/Release/$data->{TARGET}.exe\"";
    }
    else {
        $debug_link_opts .= "/out:\"$data->{TARGET}.exe\"";
        $release_link_opts .= "/out:\"$data->{TARGET}.exe\"";
    }
        
    $debug_link_opts =~ s/\s$//;
    $release_link_opts =~ s/\s$//;
  
    my $cpp_opts = "";

    my $dirs = PerlSam::Generator::ExpandIncludeDirs (@{$data->{LIBS}});
    my $include_dirs = PerlSam::Generator::ConvertPathToRelative ($dirs);
    foreach my $dir (split / /, $include_dirs) {
        $cpp_opts .= "/I \"$dir\" ";
    }
    
    $cpp_opts =~ s/ $//;
    
    $string .= "!IF  \"\$(CFG)\" == \"$data->{DESCRIPTION} - Win32 Release\"\n\n";
    $string .= ConfigSection ({TYPE => $type,
                               OUTPUTDIR => %tempdirs->{'Win32 Release'} . "\\$data->{NAME}",
                               INTERDIR => %tempdirs->{'Win32 Release'} . "\\$data->{NAME}",
                               ADDCPPOPTS => $cpp_opts,
                               ADDLINKOPTS => $release_link_opts});
    $string .= "!ELSEIF  \"\$(CFG)\" == \"$data->{DESCRIPTION} - Win32 Debug\"\n\n";
    $string .= ConfigSection ({TYPE => $type,
                               OUTPUTDIR => '',
                               INTERDIR => %tempdirs->{'Win32 Debug'} . "\\$data->{NAME}",
                               DEBUG => 1,
                               ADDCPPOPTS => $cpp_opts,
                               ADDLINKOPTS => $debug_link_opts});
    $string .= "!ENDIF\n\n";
 
    $string .= "# Begin Target\n\n";

    foreach my $config (@configs) {
        $string .= "# Name \"$data->{DESCRIPTION} - $config\"\n";
    }
    
    $string .= ProjectAllGroups ($data->{DESCRIPTION}, %{$data->{SOURCES}}, @configs);
    $string .= ProjectFooter ();
    
    return $string;
}

sub GenerateDLLProject (\%)
{
    my $data = shift;

    my @configs = ('Win32 MFC Release', 'Win32 MFC Debug', 'Win32 Release', 'Win32 Debug');
    my $type = 'library';

    my $export = "";
    my $include = "";
    
    if (defined $data->{LIBINFO}->{EXPORT}) {
        $export = $data->{LIBINFO}->{EXPORT};
    }
    
    if (defined $data->{LIBINFO}->{INCLUDE}) {
        $include = $data->{LIBINFO}->{INCLUDE};
    }

    my $string = ProjectHeader (%{$data}, @configs, 0);

    my $libraries = PerlSam::Generator::ExpandLibraries (@{$data->{LIBS}});
    
    my $mfc_debug_link_opts;
    my $mfc_release_link_opts;
    my $debug_link_opts;
    my $release_link_opts;
  
    foreach my $lib (split / /, $libraries) {
        $mfc_debug_link_opts .= $lib . "mfcd.lib ";
        $mfc_release_link_opts .= $lib . "mfc.lib ";
        $debug_link_opts .= $lib . "d.lib ";
        $release_link_opts .= $lib . ".lib ";
    }
    
    my $bindir = PerlSam::Generator::ConvertPathToRelative ("/bin") . "/";
    
    $mfc_debug_link_opts .= "/out:\"$bindir$data->{TARGET}" . "mfcd.dll\" ";
    $mfc_release_link_opts .= "/out:\"$bindir$data->{TARGET}" . "mfc.dll\" ";
    $debug_link_opts .= "/out:\"$bindir$data->{TARGET}" . "d.dll\" ";
    $release_link_opts .= "/out:\"$bindir$data->{TARGET}.dll\" ";
    
    $mfc_debug_link_opts =~ s/\s$//;
    $mfc_release_link_opts =~ s/\s$//;
    $debug_link_opts =~ s/\s$//;
    $release_link_opts =~ s/\s$//;
  
    my $cpp_opts = "";
    
    foreach my $ex (split / /, $export) {
        $cpp_opts .= "/D \"" . $ex . "_BUILD_DLL\" ";
    }

    my $include_dirs = PerlSam::Generator::ExpandIncludeDirs (@{$data->{LIBS}});
    $include_dirs = PerlSam::Generator::ConvertPathToRelative ($include_dirs);
    foreach my $dir (split / /, $include_dirs) {
        $cpp_opts .= "/I \"$dir\" ";
    }
   
    if ($cpp_opts !~ m/\/I "$include"/) {
        $cpp_opts .= "/I \"$include\" ";
    }

    $cpp_opts =~ s/ $//;
    
    $string .= "!IF  \"\$(CFG)\" == \"$data->{DESCRIPTION} - Win32 MFC Release\"\n\n";
    $string .= ConfigSection ({TYPE => $type,
                               OUTPUTDIR => '',
                               INTERDIR => %tempdirs->{'Win32 MFC Release'} . "\\$data->{NAME}",
                               ADDCPPOPTS => $cpp_opts . " /D ACE_HAS_MFC=1",
                               ADDLINKOPTS => $mfc_release_link_opts});
    $string .= "!ELSEIF  \"\$(CFG)\" == \"$data->{DESCRIPTION} - Win32 MFC Debug\"\n\n";
    $string .= ConfigSection ({TYPE => $type,
                               OUTPUTDIR => '',
                               INTERDIR => %tempdirs->{'Win32 MFC Debug'} . "\\$data->{NAME}",
                               DEBUG => 1,
                               ADDCPPOPTS => $cpp_opts . " /D ACE_HAS_MFC=1",
                               ADDLINKOPTS => $mfc_debug_link_opts});
    $string .= "!ELSEIF  \"\$(CFG)\" == \"$data->{DESCRIPTION} - Win32 Release\"\n\n";
    $string .= ConfigSection ({TYPE => $type,
                               OUTPUTDIR => '',
                               INTERDIR => %tempdirs->{'Win32 Release'} . "\\$data->{NAME}",
                               ADDCPPOPTS => $cpp_opts,
                               ADDLINKOPTS => $release_link_opts});
    $string .= "!ELSEIF  \"\$(CFG)\" == \"$data->{DESCRIPTION} - Win32 Debug\"\n\n";
    $string .= ConfigSection ({TYPE => $type,
                               OUTPUTDIR => '',
                               INTERDIR => %tempdirs->{'Win32 Debug'} . "\\$data->{NAME}",
                               DEBUG => 1,
                               ADDCPPOPTS => $cpp_opts,
                               ADDLINKOPTS => $debug_link_opts});
    $string .= "!ENDIF\n\n";
 
    $string .= "# Begin Target\n\n";

    foreach my $config (@configs) {
        $string .= "# Name \"$data->{DESCRIPTION} - $config\"\n";
    }

    $string .= ProjectAllGroups ($data->{DESCRIPTION},
                                 %{$data->{SOURCES}},
                                 @configs);
    $string .= ProjectFooter ();
    
    return $string;
}

sub GenerateLIBProject (\%)
{
    my $data = shift;

    my @configs = ('Win32 Static Release', 'Win32 Static Debug');
    my $type = 'static library';
    my $include = "";
    my $namespace = "";
    
    if (defined $data->{LIBINFO}->{INCLUDE}) {
        $include = $data->{LIBINFO}->{INCLUDE};
    }
    
    if (defined $data->{LIBINFO}->{NAMESPACE}) {
        $namespace = $data->{LIBINFO}->{NAMESPACE};
    }

    my $string = ProjectHeader (%{$data}, @configs, 1);

    my $debug_link_opts;
    my $release_link_opts;

    $debug_link_opts .= "/out:\"$data->{TARGET}sd.lib\" ";
    $release_link_opts .= "/out:\"$data->{TARGET}s.lib\" ";
    
    my $cpp_opts = "";
    
    my $include_dirs = PerlSam::Generator::ExpandIncludeDirs (@{$data->{LIBS}});
    $include_dirs = PerlSam::Generator::ConvertPathToRelative ($include_dirs);
    
    foreach my $dir (split / /, $include_dirs) {
        $cpp_opts .= "/I \"$dir\" ";
    }
   
    if ($cpp_opts !~ m/\/I "$include"/) {
        $cpp_opts .= "/I \"$include\" ";
    }
    
    if ($#{$data->{LIBS}} > 0 || $namespace eq "ACE") {
        $cpp_opts .= "/D \"ACE_AS_STATIC_LIBS\" ";
    }

    if ($#{$data->{LIBS}} > 0 || $namespace eq "TAO") {
        $cpp_opts .= "/D \"TAO_AS_STATIC_LIBS\" ";
    }

    my $export = '';
    if (defined $data->{LIBINFO}->{EXPORT}) {
        $export = $data->{LIBINFO}->{EXPORT};
    }
    foreach my $ex (split / /, $export) {
        $cpp_opts .= "/D \"" . $ex . "_HAS_DLL=0\" ";
    }

    $cpp_opts =~ s/ $//;

    $string .= "!IF  \"\$(CFG)\" == \"$data->{DESCRIPTION} - Win32 Static Release\"\n\n";
    $string .= ConfigSection ({TYPE => $type,
                               OUTPUTDIR => '',
                               INTERDIR => %tempdirs->{'Win32 Release'} . "\\$data->{NAME}",
                               ADDCPPOPTS => $cpp_opts,
                               ADDLINKOPTS => $release_link_opts});
    $string .= "!ELSEIF  \"\$(CFG)\" == \"$data->{DESCRIPTION} - Win32 Static Debug\"\n\n";
    $string .= ConfigSection ({TYPE => $type,
                               OUTPUTDIR => '',
                               INTERDIR => %tempdirs->{'Win32 Debug'} . "\\$data->{NAME}",
                               DEBUG => 1,
                               ADDCPPOPTS => $cpp_opts,
                               ADDLINKOPTS => $debug_link_opts});
    $string .= "!ENDIF\n\n";
    
    $string .= "# Begin Target\n\n";

    foreach my $config (@configs) {
        $string .= "# Name \"$data->{DESCRIPTION} - $config\"\n";
    }
    
    $string .= ProjectAllGroups ($data->{DESCRIPTION},
                                 %{$data->{SOURCES}},
                                 @configs);
    $string .= ProjectFooter ();
    
    return $string;
}

sub WorkspaceProject ($$\@)
{
    my $name = shift;
    my $file = shift;
    my $depends = shift;
    my $string;

    $string .= "###############################################################################\n\n";
    $string .= "Project: \"$name\"=.\\$file - Package Owner=<4>\n\n";
    $string .= "Package=<5>\n";
    $string .= "{{{\n";
    $string .= "}}}\n\n";
    $string .= "Package=<4>\n";
    $string .= "{{{\n";

    foreach my $dep (@{$depends}) {
        $string .= "    Begin Project Dependency\n";
        $string .= "    Project_Dep_Name $dep\n";
        $string .= "    End Project Dependency\n";
    }

    $string .= "}}}\n\n";

    return $string;
}

sub ProjectHeader (\%\@$)
{
    my $data = shift;
    my $configs = shift;
    my $static = shift;
        
    my $default_config = (reverse @{$configs})[0];
    
    my $string = "";

    # Standard header.  Do not edit?  heh heh
        
    $string .= "# Microsoft Developer Studio Project File - Name=\"$data->{DESCRIPTION}\" - Package Owner=<4>\n";
    $string .= "# Microsoft Developer Studio Generated Build File, Format Version 6.00\n";
    $string .= "# ** DO NOT EDIT **\n";
    $string .= "\n";
    
    # Pick a target
    
    $string .= "# TARGTYPE \"Win32 (x86) Dynamic-Link Library\" 0x0102\n" if ($data->{TYPE} eq "library" && !$static);
    $string .= "# TARGTYPE \"Win32 (x86) Console Application\" 0x0103\n" if ($data->{TYPE} eq "executable");
    $string .= "# TARGTYPE \"Win32 (x86) Static Library\" 0x0104\n" if ($data->{TYPE} eq "library" && $static);
    $string .= "\n";
    
    # Next block of dsp goodness
    
    $string .= <<EOPH1;
CFG=$data->{DESCRIPTION} - $default_config
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "$data->{NAME}.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "$data->{NAME}.mak" CFG="$data->{DESCRIPTION} - $default_config"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
EOPH1

    # Now we need to output the list of configs possible
    
    my $target = "Unknown";
    $target = "Win32 (x86) Dynamic-Link Library" if ($data->{TYPE} eq "library" && !$static);
    $target = "Win32 (x86) Console Application" if ($data->{TYPE} eq "executable");
    $target = "Win32 (x86) Static Library" if ($data->{TYPE} eq "library" && $static);
    
    foreach my $config (@{$configs}) {
        $string .= "!MESSAGE \"$data->{DESCRIPTION} - $config\" (based on \"$target\")\n";
    }
  
    # More goodness
    
    $string .= <<EOPH2;
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

EOPH2
    return $string;
}

sub ProjectAllGroups ($\%\@)
{
    my $description = shift;
    my $sources = shift;
    my $configs = shift;
    
    my $string;
    
    $string .= ProjectGroup ($description,
                             "Source Files", 
                             "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90", 
                             %{$sources},
                             @{$configs});
    $string .= ProjectGroup ($description,
                             "Header Files", 
                             "h;hpp;hxx;hm;fi;fd", 
                             %{$sources},
                             @{$configs});
    $string .= ProjectGroup ($description,
                             "IDL Files", 
                             "idl;pidl", 
                             %{$sources},
                             @{$configs});
    $string .= ProjectGroup ($description,
                             "Inline Files", 
                             "inl;i", 
                             %{$sources},
                             @{$configs});
    $string .= ProjectGroup ($description,
                             "Resource Files", 
                             "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe", 
                             %{$sources},
                             @{$configs});
    return $string;
}

sub ProjectGroup ($$$\%\@)
{
    my $description = shift;
    my $name = shift;
    my $filter = shift;
    my $data = shift;
    my $configs = shift;
    my $string;
        
    $string .= "# Begin Group \"$name\"\n";
    $string .= "\n";
    $string .= "# PROP Default_Filter \"$filter\"\n";

    foreach my $source (sort keys %{$data}) {
        my $ext = "";
        if ($source =~ m/\.([^\.]*)$/) {
            $ext = $1;
        }
        
        if ($filter =~ m/^$ext;/ ||
            $filter =~ m/;$ext;/ ||
            $filter =~ m/;$ext$/ ||
            $filter =~ m/^$ext$/)
        {
            $string .= ProjectSource ($description, $source, %{$data->{$source}}, @{$configs});
        }
    }
    $string .= "# End Group\n";
    return $string;
}

sub ProjectSource ($$\%\@)
{
    my $description = shift;
    my $file = shift;
    my $data = shift;
    my $configs = shift;
    my $string;
 
    $file =~ s/\//\\/g;
    
    if ($file !~ m/\\/) {
        $file = ".\\" . $file;
    }
    
    $string .= "# Begin Source File\n\nSOURCE=$file\n";

    if (defined $data->{TYPE} && lc ($data->{TYPE}) eq 'template') {
        $string .= "# PROP Exclude_From_Build 1\n";
    }
    
    if ($file =~ m/(.*)\.idl$/) {
        my $idl = $file;
        my $idlroot = $1;
        my $taoidl_opts;
        
        if ($idlroot =~ m/([^\\]*)$/) {
            $idlroot = $1;
        }
        
        if (defined $data->{OPTS}) {
            $taoidl_opts = $data->{OPTS};
        }
        else {
            $taoidl_opts = "-Ge 1";
        }
        
        
        my $first = 1;
        foreach my $config (@{$configs}) {
            my $taoidl_path = PerlSam::Generator::ConvertPathToRelative('/bin/' . %tempdirs->{$config} . '/tao_idl.exe');

            # Special case for Win32 Debug            
            if ($config eq "Win32 Debug") {
                $taoidl_path = PerlSam::Generator::ConvertPathToRelative('/bin/tao_idl.exe');
            }
                     
            $taoidl_path =~ s/\//\\/g;
                                                   
            if ($first) {
                $first = 0;
                $string .= "!IF  \"\$(CFG)\" == \"$description - $config\"\n\n"
            }
            else {
                $string .= "!ELSEIF  \"\$(CFG)\" == \"$description - $config\"\n\n"
            }
                        $string .= <<EOPS
USERDEP__IDL_="$taoidl_path"	

# Begin Custom Build - Invoking TAO's IDL Compiler on \$(InputPath)
InputPath=$idl
InputName=$idlroot

BuildCmds= \\
	$taoidl_path $taoidl_opts \$(InputName).idl

"\$(InputName)C.h" : \$(SOURCE) "\$(INTDIR)" "\$(OUTDIR)"
   \$(BuildCmds)

"\$(InputName)C.i" : \$(SOURCE) "\$(INTDIR)" "\$(OUTDIR)"
   \$(BuildCmds)

"\$(InputName)C.cpp" : \$(SOURCE) "\$(INTDIR)" "\$(OUTDIR)"
   \$(BuildCmds)

"\$(InputName)S.h" : \$(SOURCE) "\$(INTDIR)" "\$(OUTDIR)"
   \$(BuildCmds)

"\$(InputName)S.i" : \$(SOURCE) "\$(INTDIR)" "\$(OUTDIR)"
   \$(BuildCmds)

"\$(InputName)S.cpp" : \$(SOURCE) "\$(INTDIR)" "\$(OUTDIR)"
   \$(BuildCmds)

"\$(InputName)S_T.h" : \$(SOURCE) "\$(INTDIR)" "\$(OUTDIR)"
   \$(BuildCmds)

"\$(InputName)S_T.i" : \$(SOURCE) "\$(INTDIR)" "\$(OUTDIR)"
   \$(BuildCmds)

"\$(InputName)S_T.cpp" : \$(SOURCE) "\$(INTDIR)" "\$(OUTDIR)"
   \$(BuildCmds)
# End Custom Build
                        
EOPS
        }
        
        $string .= "!ENDIF\n";
    }
    
    $string .= "# End Source File\n";

    return $string;
}

sub ProjectFooter ()
{
    return <<EOPF;
# End Target
# End Project
EOPF
}

sub ConfigSection (%) 
{
    my $data = shift;
    
    my $type = $data->{TYPE};
    $type = "" if (!defined $type);

    my $output_dir = "Output";
    my $inter_dir = "Output";
    my $target_dir = "";
    my $base_cpp_opts = "/nologo /W3 /GX /D \"WIN32\" /D \"_WINDOWS\" /D \"_CONSOLE\" /D \"_MBCS\" /FD /c";
    my $base_link_opts = "kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386";
    my $use_debug_libs = 0;
    my $debug_macro = "NDEBUG";
    my $other_cpp_opts = "/D \"NDEBUG\" /O2 /MD ";
    my $link_opts = "/nologo /subsystem:console /machine:I386 ";

    if ($type eq "library") {
        $base_link_opts = "kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /machine:I386";
        $link_opts = "/nologo /dll /machine:I386 ";
    }
    
    if (defined $data->{DEBUG} && $data->{DEBUG} == 1) {
        $use_debug_libs = 1;
        $debug_macro = "_DEBUG";
        $other_cpp_opts = "/D \"_DEBUG\" /Od /MDd ";
        $base_cpp_opts .= " /Gm /Zi";
        $base_link_opts .= " /debug /pdbtype:sept";
        $link_opts .= "/debug /pdbtype:sept ";
    }
        
    if ($type eq "static library") {
        $base_link_opts = "/nologo ";
        $link_opts = "/nologo ";
        
        if (defined $data->{DEBUG} && $data->{DEBUG} == 1) {
            $other_cpp_opts = "/D \"_DEBUG\" /Od /Gy /MDd ";
        }
        else {
            $other_cpp_opts = "/D \"NDEBUG\" /O1 /MD ";
        }
    }
    
    # Override defaults    
    
    $output_dir = $data->{OUTPUTDIR} if (defined $data->{OUTPUTDIR});
    $inter_dir = $data->{INTERDIR} if (defined $data->{INTERDIR});
    $target_dir = $data->{TARGETDIR} if (defined $data->{TARGETDIR});
    $other_cpp_opts = $data->{CPPOPTS} if (defined $data->{CPPOPTS});
    $link_opts = $data->{LINKOPTS} if (defined $data->{LINKOPTS});
    
    $other_cpp_opts .= $data->{ADDCPPOPTS} if (defined $data->{ADDCPPOPTS});
    $link_opts .= $data->{ADDLINKOPTS} if (defined $data->{ADDLINKOPTS});
        
    my $string = <<EOCS1;
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries $use_debug_libs
# PROP BASE Output_Dir "$output_dir"
# PROP BASE Intermediate_Dir "$inter_dir"
# PROP BASE Target_Dir "$target_dir"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries $use_debug_libs
# PROP Output_Dir "$output_dir"
# PROP Intermediate_Dir "$inter_dir"
EOCS1
    
    $string .= "# PROP Ignore_Export_Lib 0\n" if ($type ne "static library");
    
    $string .= <<EOCS2;
# PROP Target_Dir "$target_dir"
# ADD BASE CPP $base_cpp_opts
# ADD CPP $base_cpp_opts $other_cpp_opts
EOCS2
    
    if ($type ne "static library") {
        $string .= "# ADD BASE MTL /nologo /D \"$debug_macro\" /mktyplib203 /win32\n";
        $string .= "# ADD MTL /nologo /D \"$debug_macro\" /mktyplib203 /win32\n";
    }
    
    $string .= <<EOCS3;
# ADD BASE RSC /l 0x409 /d "$debug_macro"
# ADD RSC /l 0x409 /d "$debug_macro"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
EOCS3
    
    if ($type eq "static library") {
        $string .= "LIB32=link.exe -lib\n";
        $string .= "# ADD BASE LIB32 $base_link_opts\n";
        $string .= "# ADD LIB32 $link_opts\n";
    }
    else {
        $string .= "LINK32=link.exe\n";
        $string .= "# ADD BASE LINK32 $base_link_opts\n";
        $string .= "# ADD LINK32 $link_opts \n";
    }
    
    $string .= "\n";
    
    return $string;
}

1;
