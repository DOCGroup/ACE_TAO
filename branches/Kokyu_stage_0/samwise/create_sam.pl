eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$

use FindBin;
use lib $FindBin::Bin;
use PerlACE::MSProject::DSP;
use DirHandle;
use FileHandle;
use Cwd;

use strict;

################################################################################

my $version = "0.1";

my $recurse = 0;
my $list = 0;
my $verbose = 0;
my @arguments;
my @configs;
my @subdirs;
my @roots;
my $auto_compile = 0;
my $clean = 0;
my $debug = 0;

my $aceroot = 0;

my $output;
my $outfile;

################################################################################

# Parse command line arguments

while ( $#ARGV >= 0)
{
    if ($ARGV[0] =~ m/^-file/i) {
        $outfile = $ARGV[1];
        shift;
    }
    elsif ($ARGV[0] =~ m/^-r/i) {
        $recurse = 1;
    }
    elsif ($ARGV[0] =~ m/^-(\?|h)/i) {     # Help information
        print "Options\n";
        print " -file <file>\n";
        exit;
    }
    elsif ($ARGV[0] =~ m/^-v/i) {
        $verbose = 1;
    }
    elsif ($ARGV[0] =~ m/^-/) {
        warn "$0:  unknown option $ARGV[0]\n";
        exit 1;
    }
    else {
        push @arguments, $ARGV[0];
    }
    shift;
}

@configs = ("Win32 Debug");

if (!defined $outfile) {
    $outfile = "sam.xml";
}

$output = new FileHandle ($outfile, "w");

################################################################################

# I like these variables

# %projects->{$file}->{BUILD} <- Are we supposed to build this file?
#                   ->{PROJ} <- MSProject object
#                   ->{ACEDEPS}
#                   ->{TAODEPS}
#                   ->{LIBS}
#                   ->{CONFIGS}->{$config}->{DEPS} <- List of dependencies
#                                         ->{DONE} <- Have we compiled it yet?

my %projects;

# %names->{$output} <- points to the $file used in the above %projects

my %names;

################################################################################

# Expand all the files/directories passed in on the command line

sub ProjectSearch ($@)
{
    my $build = shift;
    my @targets = @_;

    while ($#targets >= 0) {
        my $target = $targets[0];
        if (-d $target) {
            print "    Reading Directory $target\n" if ($verbose);
            if ($recurse) {
                my $dh = new DirHandle ($target);

                if (defined $dh) {
                    foreach my $entry ($dh->read ()) {
                        if (-d "$target/$entry" && $entry ne "." && $entry ne "..") {
                            $entry =~ s/^.\\//;
                            $entry =~ s/^.\///;
                            push @targets, ($target . "\\". $entry);
                        }
                    }
                }
                else {
                    print STDERR "Error: Cannot read $target: $!\n";
                }
            }

            foreach my $t (glob ($target . "/*.dsp")) {
                $t =~ s/^.\\//;
                $t =~ s/^.\///;

                print "    Adding project $t\n" if ($verbose);
                %projects->{$t}->{BUILD} = $build;
            }
        }
        else {
            foreach my $t (glob ($target)) {
                print "    Adding project $t\n" if ($verbose);
                %projects->{$t}->{BUILD} = $build;
            }
        }
        shift @targets;
    }
}

print "=== Expanding Command line Arguments\n" if ($verbose);

if ($#arguments < 0) {
    print "    No files specified, defaulting to \".\"\n" if ($verbose);
    push @arguments, (".");
}

ProjectSearch (1, @arguments);

print "=== Expanding Root Arguments\n" if ($verbose);

ProjectSearch (0, @roots);

if ($aceroot == 1) {
    my $oldrecurse = $recurse;
    $recurse = 1;
    my @aceroots = ($ENV{ACE_ROOT}."\\ace",
                    $ENV{ACE_ROOT}."\\apps\\gperf\\src",
                    $ENV{ACE_ROOT}."\\TAO\\TAO_IDL",
                    $ENV{ACE_ROOT}."\\TAO\\tao",
                    $ENV{ACE_ROOT}."\\TAO\\orbsvcs\\orbsvcs");
    ProjectSearch (0, @aceroots);
    $recurse = $oldrecurse;
}

################################################################################

# Read each project file to gather dependency and output information

print "=== Reading Project Files\n" if ($verbose);

foreach my $project (keys %projects) {
    my $proj;

    if ($project =~ m/\.dsp$/i) {
        $proj = new PerlACE::MSProject::DSP ($project);
    }
    elsif ($project =~ m/\.vcp$/i) {
        $proj = new PerlACE::MSProject::VCP ($project);
    }
    elsif ($project =~ m/\.vcproj$/i) {
        print STDERR "Error: MSVC7 not supported yet\n";
    }
    else {
        print STDERR "Error: Unrecognized file: $project\n";
    }

    print "    Loading $project:" if ($verbose);

    $proj->Load ();

    foreach my $config (@configs) {
        foreach my $proj_config ($proj->Configs ()) {
            if ($proj_config =~ m/\Q$config\E/i) {
                print " \"$proj_config\"" if ($verbose);
                my $name = $proj->DepOutputFile ($proj_config);

                %names->{lc $name} = $project;

                if (defined $proj->Libs ($proj_config)) {
                    @{%projects->{$project}->{CONFIGS}->{$proj_config}->{DEPS}} = split / /, $proj->Libs ($proj_config);
                }
            }
        }
    }

    print "\n" if ($verbose);

    %projects->{$project}->{PROJ} = $proj;
}
################################################################################
# Random function

sub CSConvertPathToRelative ($)
{
    my $args = shift;
    $args =~ s/\s\s*/ /g;
    $args =~ s/\s$//g;
    my @list = split / /, $args;
    my $result = "";
    my $root;

    my $current = getcwd ();
    my $traverse = $current;
    my $external = 0;
    my @current_list;

    # This little bit of code walks up the path looking ACE_ROOT.  If
    # not found, just default to using "$ACE_ROOT/"

    while (1) {
        # Are we in the "root" yet?
        if (-r "$traverse/samwise/PerlSam/Generator.pm") {
            last;
        }

        # Move up in the directory tree by lopping off the last part of
        # the path
        if ($traverse =~ m/(.*)\/[^\/]*/) {
            $traverse = $1;
            $root .= "../";
        }
        else {
            # Ran out of path, default to environment variable
            $root = "\$ACE_ROOT/";
            $external = 1;
            last;
        }
    }

    # Remove the trailing slash
    $root =~ s/\/$//;

    if (!$external) {
        # Figure out what our relative current directory is
        $current =~ s/^\Q$traverse\E\///;
        @current_list = split /\//, $current;
    }

    # Rebuild the stringified list
    foreach my $entry (@list) {
        my $this_root = $root . "/";
        my @this_current = @current_list;

        # Loop off any common parts.  So if current directory is
        # "\TAO\tests" and the entry is "\TAO\" then reduce the root
        if (!$external) {
            while ($#this_current >= 0) {
                my $top_dir = shift @this_current;
                if ($entry && $entry =~ s/^\/$top_dir//) {
                    $this_root =~ s/^\.\.\///;
                }
                else {
                    last;
                }
            }
        }

        $entry =~ s/^\///;

        $result .= $this_root . $entry . " ";
    }

    # Remove the trailing space from the stringified list.
    $result =~ s/ $//;

    return $result;
}

################################################################################

# Clean out the dependency lists, we only keep the libraries which we know
# how to generate

print "=== Cleaning out Dependency Lists\n" if ($verbose);

my %libs_table = (
    'ACEd.lib'                    => ['ACE',    'ACE',              '../',                   'ACE ACE_OS'          ],
    'ACE_SSLd.lib'                => ['ACE',    'SSL',              '../../',                'ACE_SSL'             ],
    'ACE_QoSd.lib'                => ['ACE',    'QoS',              '../../',                'ACE_QoS'             ],
    'ACE_RMCastd.lib'             => ['ACE',    'RMCast',           '../../',                'ACE_RMCast'          ],
    'TAO_IDL_BEd.lib'             => ['TAOIDL', 'Back_End',         'include be_include',    'TAO_IDL_BE'          ],
    'TAO_IDL_FEd.lib'             => ['TAOIDL', 'Front_End',        'include fe',            'TAO_IDL_FE'          ],
    'TAOd.lib'                    => ['TAO',    'TAO',              '../',                   'TAO'                 ],
    'TAO_PortableServerd.lib'     => ['TAO',    'PortableServer',   '../../',                'TAO_PortableServer'  ],
    'TAO_BiDirGIOPd.lib'          => ['TAO',    'BiDirGIOP',        '../../',                'TAO_BiDirGIOP'       ],
    'TAO_Domaind.lib'             => ['TAO',    'Domain',           '../../',                'TAO_Domain'          ],
    'TAO_DynamicAnyd.lib'         => ['TAO',    'DynamicAny',       '../../',                'TAO_DynamicAny'      ],
    'TAO_DynamicInterfaced.lib'   => ['TAO',    'DynamicInterface', '../../',                'TAO_DynamicInterface'],
    'TAO_IORManipd.lib'           => ['TAO',    'IORManip',         '../../',                'TAO_IORManip'        ],
    'TAO_RTCORBAd.lib'            => ['TAO',    'RTCORBA',          '../../',                'TAO_RTCORBA'         ],
    'TAO_RTPortableServerd.lib'   => ['TAO',    'RTPortableServer', '../../',                'TAO_RTPortableServer'],
    'TAO_SmartProxiesd.lib'       => ['TAO',    'SmartProxies',     '../../',                'TAO_SmartProxies'    ],
    'TAO_Strategiesd.lib'         => ['TAO',    'Strategies',       '../../',                'TAO_Strategies'      ],
    'TAO_IORTabled.lib'           => ['TAO',    'IORTable',         '../../',                'TAO_IORTable'        ],
    'TAO_TypeCodeFactoryd.lib'    => ['TAO',    'TypeCodeFactory',  '../../',                'TAO_TypeCodeFactory' ],
    'TAO_IFR_Clientd.lib'         => ['TAO',    'IFR_Client',       '../../',                'TAO_IFR_Client'      ],
    'TAO_AVd.lib'                 => ['TAO',    'AVStreams',        '../../',                'TAO_AV'              ],
    'TAO_CosLifeCycled.lib'       => ['TAO',    'CosLifeCycle',     '../../',                'TAO_LifeCycle'       ],
    'TAO_CosNamingd.lib'          => ['TAO',    'CosNaming',        '../../',                'TAO_Naming'          ],
    'TAO_DsLogAdmind.lib'         => ['TAO',    'DsLogAdmin',       '../../',                'TAO_Log'             ],
    'TAO_RTEventd.lib'            => ['TAO',    'RTEvent',          '../../',                'TAO_RTEvent'         ],
    'TAO_RTOLDEventd.lib'         => ['TAO',    'RTOldEvent',       '../../',                'TAO_RTOLDEvent'      ],
    'TAO_RTSchedd.lib'            => ['TAO',    'RTSched',          '../../',                'TAO_RTSched'         ],
    'TAO_RTSchedEventd.lib'       => ['TAO',    'RTSchedEvent',     '../../',                'TAO_RTSchedEvent'    ],
    'TAO_Securityd.lib'           => ['TAO',    'Security',         '../../',                'TAO_Security'        ],
    'TAO_SSLIOPd.lib'             => ['TAO',    'SSLIOP',           '../../',                'TAO_SSLIOP'          ],
    'TAO_Svc_Utilsd.lib'          => ['TAO',    'SvcUtils',         '../../',                'TAO_Svc_Utils'       ],
    'TAO_CosConcurrencyd.lib'     => ['TAO',    'CosConcurrency',   '../../',                'TAO_Concurrency'     ],
    'TAO_CosEventd.lib'           => ['TAO',    'CosEvent',         '../../',                'TAO_Event'           ],
    'TAO_CosNotificationd.lib'    => ['TAO',    'CosNotification',  '../../',                'TAO_Notify'          ],
    'TAO_CosPropertyd.lib'        => ['TAO',    'CosProperty',      '../../',                'TAO_Property'        ],
    'TAO_CosTimed.lib'            => ['TAO',    'CosTime',          '../../',                'TAO_Time'            ],
    'TAO_CosTradingd.lib'         => ['TAO',    'CosTrading',       '../../',                'TAO_Trading'         ],
    'TAO_Fault_Toleranced.lib'    => ['TAO',    'FaultTolerance',   '../../',                'TAO_FT'              ],
    'TAO_LoadBalancingd.lib'      => ['TAO',    'LoadBalancing',    '../../',                'TAO_LoadBalancing'   ],
    'CECTestd.lib'                => [undef,    undef,              '',                      'CEC_Test'            ],
    'coll_test_stubsd.lib'        => [undef,    undef,              '',                      undef                 ],
    'Diamondd.lib'                => [undef,    undef,              '',                      'DIAMOND'             ],
    'ECTestd.lib'                 => [undef,    undef,              '',                      'EC_TEST'             ],
    'TAO_RTEC_COSECd.lib'         => [undef,    undef,              '',                      'RTEC_COSEC'          ],
    'RTCORBA_Commond.lib'         => [undef,    undef,              '',                      'RTCORBA_COMMON'      ],
    'TAO_IFR_BE_DLLd.lib'         => [undef,    undef,              '',                      'TAO_IFR_BE'          ],
    'TAO_NotifyTestsd.lib'        => [undef,    undef,              '',                      'TAO_NOTIFY_TEST'     ],
    'TradingLibd.lib'             => [undef,    undef,              '',                      undef                 ],
    'Generic_Servantd.lib'        => [undef,    undef,              '',                      'GENERIC_SERVANT'     ],
    'Client_Testd.lib'            => [undef,    undef,              '',                      'ACE_SVC'             ],
    'export_dlld.lib'             => [undef,    undef,              '',                      'TEST'                ],
    'Gateway.lib'                 => [undef,    undef,              '',                      'ACE_SVC'             ],
    'Gatewayd.lib'                => [undef,    undef,              '',                      'ACE_SVC'             ],
    'HTTPUd.lib'                  => [undef,    undef,              '',                      'HTTPU'               ],
    'JAWSd.lib'                   => [undef,    undef,              '',                      'JAWS'                ],
    'netsvcsd.lib'                => [undef,    undef,              '',                      'ACE_SVC'             ],
    'paced.lib'                   => [undef,    undef,              '',                      undef                 ],
    'Peerd.lib'                   => [undef,    undef,              '',                      'ACE_SVC'             ],
    'Peer.lib'                    => [undef,    undef,              '',                      'ACE_SVC'             ],
    'Synch_Libd.lib'              => [undef,    undef,              '',                      'SYNCHLIB'            ],
    'websvcsd.lib'                => [undef,    undef,              '../../',                'ACE_WEBSVCS'         ],
    'Service_Config_DLLd.lib'     => [undef,    undef,              '',                      'Service_Config_DLL'  ],
    'DLL_Testd.lib'               => [undef,    undef,              '',                      'ACE_Svc'             ],
    'Test_DLLd.lib'               => [undef,    undef,              '',                      'OLT'                 ],
    'Test_Server_Moduled.lib'     => [undef,    undef,              '',                      'TEST_SERVER_MODULE'  ],
    'Test_Client_Moduled.lib'     => [undef,    undef,              '',                      'TEST_CLIENT_MODULE'  ],
    'Collocation_Test_Stubsd.lib' => [undef,    undef,              '',                      'MY_STUB'             ],
    'Time_Dated.lib'              => [undef,    undef,              '',                      'ACE_SVC ' .
                                                                                             'Alt_Resource_Factory'],
    'Base_Testd.lib'              => [undef,    undef,              '',                      'ACE_SVC'             ],
    'Perf_Testd.lib'              => [undef,    undef,              '',                      'ACE_SVC'             ],
    'Timerd.lib'                  => [undef,    undef,              '',                      'ACE_SVC'             ],
    'Todayd.lib'                  => [undef,    undef,              '',                      'ACE_SVC'             ],
    'Newsweekd.lib'               => [undef,    undef,              '',                      'ACE_SVC'             ],
    'Serverd.lib'                 => [undef,    undef,              '',                      'ACE_SVC'             ],
    'CCM_Appd.lib'                => [undef,    undef,              '',                      'ACE_SVC'             ],
    'Dump_Restored.lib'           => [undef,    undef,              '',                      'ACE_SVC'             ],
    'Dump_Restore.lib'            => [undef,    undef,              '',                      'ACE_SVC'             ],
    'Acceptor_Serverd.lib'        => [undef,    undef,              '',                      'ACE_SVC'             ],
    'TAO_IDL_BE_DLLd.lib'         => ['TAOIDL', 'Back_End',         'include fe',            'TAO_IDL_BE'          ],
    'TAO_IDL_FE_DLLd.lib'         => ['TAOIDL', 'Front_End',        'include fe be_include', 'TAO_IDL_FE'          ]
    );

my %local_libs_table = (
    'Service_Config_DLLd.lib'     => ['/tests'                                                  ],
    'DLL_Testd.lib'               => ['/tests'                                                  ],
    'Test_DLLd.lib'               => ['/TAO/tests/Object_Loader'                                ],
    'Test_Server_Moduled.lib'     => ['/TAO/tests/DLL_ORB'                                      ],
    'Test_Client_Moduled.lib'     => ['/TAO/tests/DLL_ORB'                                      ],
    'Collocation_Test_Stubsd.lib' => ['/TAO/tests/Collocation'                                  ],
    'Timerd.lib'                  => ['/examples/Service_Configurator/Misc'                     ],
    'Todayd.lib'                  => ['/examples/DLL'                                           ],
    'Newsweekd.lib'               => ['/examples/DLL'                                           ],
    'Serverd.lib'                 => ['/examples/Service_Configurator/IPC-tests/server'         ],
    'CCM_Appd.lib'                => ['/examples/ASX/CCM_App'                                   ],
    'Dump_Restored.lib'           => ['/netsvcs/clients/Naming/Dump_Restore'                    ],
    'Dump_Restore.lib'            => ['/netsvcs/clients/Naming/Dump_Restore'                    ],
    'Acceptor_Serverd.lib'        => ['/docs/tutorials/022'                                     ],
    'CECTestd.lib'                => ['/TAO/orbsvcs/tests/CosEvent/lib'                         ],
    'coll_test_stubsd.lib'        => ['/TAO/tests/Smart_Proxies/Collocation'                    ],
    'Diamondd.lib'                => ['/TAO/tests/Collocation'                                  ],
    'ECTestd.lib'                 => ['/TAO/orbsvcs/tests/Event/lib'                            ],
    'TAO_RTEC_COSECd.lib'         => ['/TAO/orbsvcs/examples/CosEC/RtEC_Based/lib'              ],
    'RTCORBA_Commond.lib'         => ['/TAO/performance-tests/RTCorba/Multiple_Endpoints/Common'],
    'TAO_IFR_BE_DLLd.lib'         => ['/TAO/orbsvcs/IFR_Service'                                ],
    'NotifyTestsd.lib'            => ['/TAO/orbsvcs/tests/Notify/lib'                           ],
    'TAO_NotifyTestsd.lib'        => ['/TAO/orbsvcs/tests/Notify/lib'                           ],
    'TradingLibd.lib'             => ['/TAO/orbsvcs/tests/Trading'                              ],
    'Generic_Servantd.lib'        => ['/TAO/examples/POA/Generic_Servant'                       ],
    'Client_Testd.lib'            => ['/netsvcs/clients/Naming/Client'                          ],
    'export_dlld.lib'             => ['/examples/Export'                                        ],
    'Gateway.lib'                 => ['/apps/Gateway/Gateway'                                   ],
    'Gatewayd.lib'                => ['/apps/Gateway/Gateway'                                   ],
    'HTTPUd.lib'                  => ['/apps/JAWS2/HTTPU'                                       ],
    'JAWSd.lib'                   => ['/apps/JAWS2/JAWS'                                        ],
    'netsvcsd.lib'                => ['/netsvcs/lib'                                            ],
    'paced.lib'                   => ['/PACE/pace'                                              ],
    'Peerd.lib'                   => ['/apps/Gateway/Peer'                                      ],
    'Synch_Libd.lib'              => ['/performance-tests/Synch-Benchmarks/Synch_Lib'           ],
    'Base_Testd.lib'              => ['/performance-tests/Synch-Benchmarks/Base_Test'           ],
    'Perf_Testd.lib'              => ['/performance-tests/Synch-Benchmarks/Perf_Test'           ],
    'websvcsd.lib'                => ['/websvcs/lib'                                            ]
);

my @system_libs = ( 'kernel32.lib', 'user32.lib', 'gdi32.lib', 'winspool.lib', 'comdlg32.lib',
                   'advapi32.lib', 'shell32.lib', 'ole32.lib', 'oleaut32.lib', 'uuid.lib',
                   'odbc32.lib', 'odbccp32.lib', 'ssleay32.lib', 'libeay32.lib', 'ws2_32.lib',
                   'wsock32.lib'
                 );

foreach my $project (keys %projects) {
    foreach my $config (keys %{%projects->{$project}->{CONFIGS}}) {
        print "    Cleaning Dependencies: $project ($config)\n" if ($verbose);

        foreach my $dep (@{%projects->{$project}->{CONFIGS}->{$config}->{DEPS}}) {
            my $found = 0;

            # Remove dirs from $dep
            $dep =~ s/.*\///g;
            $dep =~ s/.*\\//g;

            if (lc $dep eq "diamondd.lib") {
                my $fulllib = "/TAO/tests/Collocation/Diamond";
                if (getcwd () =~ m/Smart_Proxies/i) {
                    $fulllib = "/TAO/tests/Smart_Proxies/Collocation/Diamond"
                }
                $fulllib = CSConvertPathToRelative ($fulllib);
                push @{%projects->{$project}->{LOCAL_DEPS}}, $fulllib;
            }


            foreach my $lib (keys %libs_table) {
                if (lc $dep eq lc $lib) {
                    $found = 1;

                    if (!defined %libs_table->{$lib}[0]) {
                        my $fulllib = %local_libs_table->{$dep}[0] . '/' . $dep;
                        $fulllib =~ s/d\.lib$//;
                        $fulllib =~ s/\.lib$//;
                        $fulllib = CSConvertPathToRelative ($fulllib);
                        push @{%projects->{$project}->{LOCAL_DEPS}}, $fulllib;
                    }
                    elsif (%libs_table->{$lib}[0] eq "ACE") {
                        push @{%projects->{$project}->{ACE_DEPS}}, %libs_table->{$lib}[1];
                    }
                    elsif (%libs_table->{$lib}[0] eq "TAOIDL") {
                        push @{%projects->{$project}->{TAOIDL_DEPS}}, %libs_table->{$lib}[1];
                    }
                    elsif (%libs_table->{$lib}[0] eq "TAO") {
                        push @{%projects->{$project}->{TAO_DEPS}}, %libs_table->{$lib}[1];
                    }
                    last;
                }
            }

            next if ($found);

            foreach my $lib (@system_libs) {
                if (lc $dep eq lc $lib) {
                    $found = 1;
                    last;
                }
            }

            next if ($found);

            %projects->{$project}->{BADLIB} = 1;

            if ($dep) {
                print "        Unknown Dependency: $dep\n";
            }
        }
    }
}

################################################################################

# Clean out the dependency lists, we only keep the libraries which we know
# how to generate

print "=== Reading Makefile for DIRS\n" if ($verbose);

my $makefile = new FileHandle ("Makefile", "r");

if (defined $makefile) {
    my $found = 0;

    while (<$makefile>) {
        chomp;
        if (m/^\s*DIRS\s*[\+]*=\s*([^\\]*)/i) {
            my $dirs = $1;
            if (m/\\\s*$/) {
                $found = 1;
            }
            foreach my $dir (split / /, $dirs) {
                push @subdirs, $dir;
            }
            next;
        }

        if ($found == 1) {
            if (!m/\\\s*$/) {
                $found = 0;
            }

            if (m/^\s*([^\\]*[^\s\\])/i) {
                foreach my $dir (split / /, $1) {
                    push @subdirs, $dir;
                }
            }
        }
    }

    $makefile->close ();
}
else {
    print STDERR "Error: Could not open Makefile\n";
}

################################################################################

# Prune idl generated files and get the type of generation

print "=== Messing with IDL files\n" if ($verbose);

foreach my $project (keys %projects) {
    my @idlfiles;
    my %idltypes;

    my @filelist = %projects->{$project}->{PROJ}->Sources ();

    foreach my $file (@filelist) {
        if ($file =~ m/^\.\\(.*)$/ || $file =~ m/^\.\/(.*)$/) {
            $file = $1;
        }
        if ($file =~ m/(.*)\.idl$/) {
            push @idlfiles, $1;
        }
    }

    foreach my $file (@filelist) {
        if ($file =~ m/^\.\\(.*)$/ || $file =~ m/^\.\/(.*)$/) {
            $file = $1;
        }
        if ($#idlfiles < 0) {
            push @{%projects->{$project}->{SOURCES}}, $file;
        }
        else {
            my $banned = 0;
            foreach my $idl (@idlfiles) {
                if ($file =~ m/(\Q$idl\E)S(_T|)\.(h|i|inl|cpp)/) {
                    %idltypes->{$idl} = 1;
                    $banned = 1;
                }
                elsif ($file =~ m/(\Q$idl\E)C\.(h|i|inl|cpp)$/ || $file =~ m/\.idl$/) {
                    $banned = 1;
                }
            }

            if (!$banned) {
                push @{%projects->{$project}->{SOURCES}}, $file;
            }
        }
    }

    foreach my $file (%projects->{$project}->{PROJ}->IgnoredSources ()) {
        if ($file =~ m/^\.\\(.*)$/ || $file =~ m/^\.\/(.*)$/) {
            $file = $1;
        }

        if ($#idlfiles < 0) {
            push @{%projects->{$project}->{T_SOURCES}}, $file;
        }
        else {
            my $banned = 0;
            foreach my $idl (@idlfiles) {
                if ($file =~ m/($idl)S(_T\.|\.)(h|i|inl|cpp)/) {
                    %idltypes->{$idl} = 1;
                    $banned = 1;
                }
                elsif ($file =~ m/($idl)C\.(h|i|inl|cpp)/ || $file =~ m/\.idl$/) {
                    $banned = 1;
                }
            }
            if (!$banned) {
                push @{%projects->{$project}->{T_SOURCES}}, $file;
            }
        }
    }

    foreach my $idl (@idlfiles) {
        if (defined %idltypes->{$idl}) {
            push @{%projects->{$project}->{S_IDL}}, $idl . ".idl";
        }
        else {
            push @{%projects->{$project}->{C_IDL}}, $idl . ".idl";
        }
    }
}

################################################################################

# Output the XML

sub UpdateProjectName ($)
{
    my $filename = shift;

    if ($filename eq "ace_dll") {
        $filename = "ACE";
    }
    elsif ($filename eq "TAO_IDL_BE_DLL") {
        $filename = "TAOIDL_Back_End";
    }
    elsif ($filename eq "TAO_IDL_FE_DLL") {
        $filename = "TAOIDL_Front_End";
    }
    elsif ($filename eq "TAO_BiDir_GIOP") {
        $filename = "TAO_BiDirGIOP";
    }

    return $filename;
}

sub UpdateProjectTarget ($)
{
    my $target = shift;

    if ($target eq "ace") {
        $target = "ACE";
    }
    elsif ($target eq "TAO_IDL_BE_DLL") {
        $target = "TAO_IDL_BE"
    }
    elsif ($target eq "TAO_IDL_FE_DLL") {
        $target = "TAO_IDL_FE"
    }

    return $target;
}

sub ExtraSourceFiles ($)
{
    my $name = shift;

    if ($name eq "AV"
        || $name eq "CosConcurrency"
        || $name eq "CosEvent"
        || $name eq "CosLifeCycle"
        || $name eq "CosNaming"
        || $name eq "CosNotification"
        || $name eq "CosProperty"
        || $name eq "CosTime"
        || $name eq "CosTrading"
        || $name eq "DsLogAdmin"
        || $name eq "Fault_Tolerance"
        || $name eq "LoadBalancing"
        || $name eq "RTEvent"
        || $name eq "RTOLDEvent"
        || $name eq "RTSched"
        || $name eq "RTSchedEvent"
        || $name eq "SSLIOP"
        || $name eq "Security"
        || $name eq "Svc_Utils")
    {
        return ("orbsvcs.rc");
    }

    return ();
}

sub ExtraTemplateFiles ($)
{
    my $name = shift;

    if ($name eq "ACE") {
        return ("Array_Base.cpp",
                "CORBA_Ref.cpp",
                "Dynamic_Service.cpp",
                "Event_Handler_T.cpp",
                "Future_Set.cpp",
                "Intrusive_List.cpp",
                "Intrusive_List_Node.cpp",
                "LOCK_SOCK_Acceptor.cpp",
                "Node.cpp",
                "Svc_Handler.cpp",
                "Timer_Queue_Adapters.cpp",
                "Typed_SV_Message.cpp",
                "Typed_SV_Message_Queue.cpp",
                "Unbounded_Set.cpp",
                "Unbounded_Queue.cpp");
    }

    if ($name eq "TAO") {
        return ("TAO_Singleton.cpp",
                "Connector_Impl.cpp");
    }

    return ();
}

my $cwd = getcwd ();

print "=== Producing XML file\n" if ($verbose);

print "    Workspace\n" if ($verbose);

print $output "<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>\n";
print $output "<sam version=\"$version\">\n";

print $output "    <workspace>\n";
if ($cwd =~ /\/websvcs\/lib/) {
    print $output "        <name>websvcs</name>\n";
}

foreach my $project (sort keys %projects) {
    my $filename = %projects->{$project}->{PROJ}->Filename ();

    if ($filename =~ m/([^\\]*)\.dsp/i) {
        $filename = $1;
    }

    if (%projects->{$project}->{PROJ}->OutputFile (@configs) !~ m/\.lib$/i) {
        $filename = UpdateProjectName ($filename);
        print $output "        <projectlink>$filename</projectlink>\n";
    }
}
foreach my $subdir (@subdirs) {
    print $output "        <subdir>$subdir</subdir>\n";
}
print $output "    </workspace>\n";

foreach my $project (sort keys %projects) {
    print "    Project $project\n" if ($verbose);

    my $filename = %projects->{$project}->{PROJ}->Filename ();
    my $target = %projects->{$project}->{PROJ}->OutputFile (@configs);

    my $type = "unknown";

    if ($filename =~ m/([^\\]*)\.dsp/i) {
        $filename = $1;
    }

    if ($target =~ m/\.lib$/i) {
        next;
    } elsif ($target =~ m|([^/\\]*)\.exe|i) {
        $target = $1;
        $type = "executable";
    } elsif ($target =~ m|([^/\\]*)d\.dll|i
             || $target =~ m|([^/\\]*)\.dll|i) {
        $target = $1;
        $type = "library";
    }

    $target = UpdateProjectTarget ($target);

    $filename = UpdateProjectName ($filename);

    print $output "    <project>\n";
    print $output "        <name>$filename</name>\n";

    if ($filename eq "ACE") {
        print $output "        <description>ACE</description>\n";
    }
    elsif ($filename eq "TAO") {
        print $output "        <description>TAO</description>\n";
    }
    elsif ($filename eq "TAOIDL_Back_End") {
        print $output "        <description>TAOIDL Back End Library</description>\n";
    }
    elsif ($filename eq "TAOIDL_Front_End") {
        print $output "        <description>TAOIDL Front End Library</description>\n";
    }
    else {
        print $output "        <description>", %projects->{$project}->{PROJ}->Name (), "</description>\n";
    }

    if ($filename eq "gperf" || $filename eq "tao_idl") {
        print $output "        <target type=\"$type\" install=\"yes\">$target</target>\n";
    }
    else {
        print $output "        <target type=\"$type\">$target</target>\n";
    }

    if ($type eq "library") {
        my $namespace;
        my $name = "";
        my $include = "";
        my $export = "";
        my $base = "";

        my $lib = $target . "d.lib";

        $lib =~ s/(.*)\///g;

        $namespace = %libs_table->{$lib}[0] if (defined %libs_table->{$lib});
        $name = %libs_table->{$lib}[1];
        $include = %libs_table->{$lib}[2];
        $export = %libs_table->{$lib}[3];

        $base = $target;

        $base =~ s/(.*)\///g;

        $base = "ACE" if ($base eq "ace");

        if (defined $namespace) {
            print $output "        <libinfo>\n";
            print $output "            <name>$name</name>\n";
            print $output "            <namespace>$namespace</namespace>\n";
            print $output "            <include>$include</include>\n";
            print $output "            <base>$base</base>\n";
            print $output "            <export>$export</export>\n";
            print $output "        </libinfo>\n";
        }
        else {
            print $output "        <libinfo>\n";
            print $output "            <export>$export</export>\n";
            print $output "        </libinfo>\n";
        }
    }

    print $output "        <sources>\n";

    push @{%projects->{$project}->{SOURCES}}, ExtraSourceFiles ($filename);

    if (defined %projects->{$project}->{SOURCES}) {
        foreach my $file (sort @{%projects->{$project}->{SOURCES}}) {
            if ($file =~ m/^\.\\(.*)$/ || $file =~ m/\.\/(.*)$/) {
                $file = $1;
            }

            if ($file eq "Event_Handler_T.cpp") {
                next;
            }

            $file =~ s/\\/\//g;

            print $output "            <source>\n";
            print $output "                <file>$file</file>\n";
            print $output "            </source>\n";
        }
    }

    push @{%projects->{$project}->{T_SOURCES}}, ExtraTemplateFiles ($filename);

    if (defined %projects->{$project}->{T_SOURCES}) {
        foreach my $file (sort @{%projects->{$project}->{T_SOURCES}}) {
            if ($file =~ m/^\.\\(.*)$/ || $file =~ m/\.\/(.*)$/) {
                $file = $1;
            }

            $file =~ s/\\/\//g;

            print $output "            <source type=\"template\">\n";
            print $output "                <file>$file</file>\n";
            print $output "            </source>\n";
        }
    }

    if (defined %projects->{$project}->{C_IDL}) {
        foreach my $file (sort @{%projects->{$project}->{C_IDL}}) {
            if ($file =~ m/^\.\\(.*)$/ || $file =~ m/\.\/(.*)$/) {
                $file = $1;
            }

            my $opts = %projects->{$project}->{PROJ}->IDLOpts ();

            $opts =~ s/\\/\//g;
            $opts =~ s/\s+/ /g;

            print $output "            <source type=\"clientidl\">\n";
            print $output "                <file>$file</file>\n";
            print $output "                <options>$opts</options>\n";
            print $output "            </source>\n";
        }
    }
    if (defined %projects->{$project}->{S_IDL}) {
        foreach my $file (sort @{%projects->{$project}->{S_IDL}}) {
            if ($file =~ m/^\.\\(.*)$/ || $file =~ m/\.\/(.*)$/) {
                $file = $1;
            }

            my $opts = %projects->{$project}->{PROJ}->IDLOpts ();

            $opts =~ s/\\/\//g;
            $opts =~ s/\s+/ /g;

            print $output "            <source type=\"idl\">\n";
            print $output "                <file>$file</file>\n";
            print $output "                <options>$opts</options>\n";
            print $output "            </source>\n";
        }
    }
    print $output "        </sources>\n";
    print $output "        <libs>\n";
    if (defined %projects->{$project}->{ACE_DEPS}) {
        foreach my $dep (sort @{%projects->{$project}->{ACE_DEPS}}) {
            print $output "            <lib namespace=\"ACE\">$dep</lib>\n";
        }
    }
    if (defined %projects->{$project}->{TAOIDL_DEPS}) {
        foreach my $dep (sort @{%projects->{$project}->{TAOIDL_DEPS}}) {
            print $output "            <lib namespace=\"TAOIDL\">$dep</lib>\n";
        }
    }
    if (defined %projects->{$project}->{TAO_DEPS}) {
        foreach my $dep (sort @{%projects->{$project}->{TAO_DEPS}}) {
            print $output "            <lib namespace=\"TAO\">$dep</lib>\n";
        }
    }
    if (defined %projects->{$project}->{LOCAL_DEPS}) {
        foreach my $dep (sort @{%projects->{$project}->{LOCAL_DEPS}}) {
            $dep =~ s/\n//g;
            $dep =~ s/.lib$//g;
            print $output "            <lib>$dep</lib>\n";
        }
    }
    print $output "        </libs>\n";
    print $output "    </project>\n";
}
print $output "</sam>\n";
