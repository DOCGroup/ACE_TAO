package MonitorControl;

# ************************************************************
# Description   : Graphical interface for the Notify Service Monitor
# Author        : Chad Elliott
# Create Date   : 7/17/2007
# $Id$
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use Tk;
use Tk::Tree;
use Tk::ROText;
use Tk::DialogBox;

use GeometryStore;

use Error qw(:try);

use NotifyMonitoringExt::EventChannelCreationTime;
use NotifyMonitoringExt::EventChannelFactoryNames;
use NotifyMonitoringExt::ActiveEventChannelNames;
use NotifyMonitoringExt::InactiveEventChannelNames;
use NotifyMonitoringExt::EventChannelSupplierNames;
use NotifyMonitoringExt::EventChannelConsumerNames;
use NotifyMonitoringExt::EventChannelQueueElementCount;
use NotifyMonitoringExt::EventChannelQueueSize;
use NotifyMonitoringExt::EventChannelOldestEvent;
use NotifyMonitoringExt::EventChannelSlowestConsumers;

# ************************************************************
# Data Section
# ************************************************************

my($headingFont) = '-adobe-helvetica-bold-i-*-*-24-*-*-*-*-*-*-*';
my(%help) = ('Basics',      => 'Click on an item in the tree view ' .
                               'to obtain more information.',
             'Auto Refresh' => 'The item selected in the tree view ' .
                               "will be refreshed every second.\nNOTE: " .
                               'This may slow down the notify service ' .
                               'due to internal locking.',
             'Update'       => 'Refresh the tree view with up-to-date ' .
                               'information from the monitor.',
             'Shutdown EC'  => 'Send a "shutdown event channel" command ' .
                               'to the notify service monitor.  An ' .
                               'event channel must already be selected.',
            );

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class) = shift;
  my($orb)   = shift;
  my($loc)   = shift;
  return bless {'orb'     => $orb,
                'loc'     => $loc,
                'rc'      => 'NotifyServiceMonitor',
                'title'   => 'Notify Service Monitor',
                'version' => '1.0',
                'auto'    => 0,
               }, $class;
}

sub Show {
  my($self) = shift;

  ## Create the main window and other widgets
  my($main) = new MainWindow();
  my($dim, $geom) = GeometryStore::retrieve($self->{'rc'});
  my($full) = ($dim eq '' ? '800x600' : $dim) . $geom;
  $main->geometry($full);
  $main->title($self->{'title'});
  $main->protocol('WM_DELETE_WINDOW',
                  sub { GeometryStore::store($self->{'rc'},
                                             $main->geometry());
                        $self->{'auto'} = 0;
                        $self->repeat();
                        exit(0);
                      });
  $self->{'main'} = $main;
  $self->setup();

  ## Update the widgets
  $self->update();

  ## Run the Tk event loop
  MainLoop();
}

sub setup {
  my($self) = shift;
  my($main) = $self->{'main'};
  my($bg)   = '#dfdfdf';

  $self->{'factory_image'} = $main->Getimage('factory');
  $self->{'active_image'} = $main->Getimage('active');
  $self->{'inactive_image'} = $main->Getimage('inactive');

  my($menuFrame) = $main->Frame(-relief      => 'raised',
                                -borderwidth => 1,
                               )->pack(-side => 'top',
                                       -fill => 'x');
  my($fileMenu) = $menuFrame->Menubutton(-text => 'File',
                                         -underline => 0,
                                         -tearoff => 0
                                        )->pack(-side => 'left');
  $fileMenu->command(-label => 'Exit',
                     -underline => 1,
                     -command => sub { GeometryStore::store($self->{'rc'},
                                                            $main->geometry());
                                       $self->{'auto'} = 0;
                                       $self->repeat();
                                       exit(0); },
                    );

  my($viewMenu) = $menuFrame->Menubutton(-text => 'View',
                                         -underline => 0,
                                         -tearoff => 0,
                                        )->pack(-side => 'left');

  $viewMenu->command(-label => 'Update',
                     -underline => 1,
                     -command => sub { $self->update(); },
                    );

  $viewMenu->checkbutton(-label => 'Auto Refresh',
                         -underline => 1,
                         -command => sub { $self->repeat() },
                         -variable => \$self->{'auto'},
                         );

  my($contMenu) = $menuFrame->Menubutton(-text => 'Control',
                                         -underline => 0,
                                         -tearoff => 0,
                                        )->pack(-side => 'left');

  $contMenu->command(-label => 'Shutdown EC',
                     -underline => 1,
                     -command => sub { $self->shutdownEC(); },
                    );

  my($helpMenu) = $menuFrame->Menubutton(-text => 'Help',
                                         -underline => 0,
                                         -tearoff => 0
                                        )->pack(-side => 'right');

  foreach my $key (sort keys %help) {
    $helpMenu->command(-label => "Help on $key...",
                       -underline => 8,
                       -command => sub { $self->help($help{$key},
                                                     $key) },
                      );
  }

  $helpMenu->separator();
  $helpMenu->command(-label => 'About...',
                     -underline => 0,
                     -command => sub { $self->about() },
                    );

  $self->{'tree'} = $main->Scrolled('Tree',
                                    -background => $bg,
                                    -scrollbars => 'ose',
                                    -separator  => '/',
                                    -selectmode => 'single',
                                    -width      => 40,
                                    -browsecmd  => sub { $self->{'current'} = $_[0];
                                                         $self->more();
                                                       },
                                   )->pack(-expand => 1,
                                           -fill   => 'both',
                                           -side   => 'left');
  my($font) = $self->{'tree'}->cget(-font);

  if ($^O eq 'MSWin32') {
    $self->{'tree'}->configure(-cursor => 'hand2');
  }
  else {
    my($xbmdir) = "$INC[0]/Tk";
    $self->{'tree'}->configure(-cursor => ["\@$xbmdir/hand.xbm",
                                           "$xbmdir/mask.xbm",
                                           'black', 'white']);
  }

  $self->{'text'} = $main->Scrolled('ROText',
                                    -background => $bg,
                                    -scrollbars => 'osoe',
                                    -height     => 1,
                                    -font       => $font,
                                    -wrap       => 'none',
                                   );
  ## Assume that the 'File' portion of the popup menu is at
  ## index 0 and the 'View' portion is at the end and delete them.
  my($lmenu) = $self->{'text'}->menu();
  $lmenu->delete(0);
  $lmenu->delete('end');

  $self->{'text'}->pack(-expand => 1,
                        -fill => 'both',
                        -side => 'left');
}

sub update {
  my($self) = shift;
  my($tree) = $self->{'tree'};
  my($dis)  = '#bfbfbf';

  ## Remove everything from view
  $self->{'current'} = undef;
  $self->more();
  $tree->delete('all');

  ## Start with the event channel factories
  my($names) = $self->getStatisticData(
                        NotifyMonitoringExt::EventChannelFactoryNames);
  if (defined $names) {
    foreach my $name (@$names) {
      ## Add the factory name and creation time
      my($num) = $self->getStatisticData(
                          $name . '/' .
                          NotifyMonitoringExt::EventChannelCreationTime);
      next if (!defined $num);
      my($time) = $num->last();
      $time = $self->getTimeString($time);
      $tree->add($name,
                 -text  => $name,
                 -data  => $time,
                 -image => $self->{'factory_image'});

      ## Add the active event channels
      my($ecnames) = $self->getStatisticData(
                              $name . '/' .
                              NotifyMonitoringExt::ActiveEventChannelNames);
      next if (!defined $ecnames);
      foreach my $ecname (@$ecnames) {
        $num = $self->getStatisticData(
                        $ecname . '/' .
                        NotifyMonitoringExt::EventChannelCreationTime);
        next if (!defined $num);
        $time = $num->last();
        $time = $self->getTimeString($time);
        $tree->add($ecname,
                   -text  => basename($ecname),
                   -data  => $time,
                   -image => $self->{'active_image'});
      }

      ## Add the inactive event channels
      $ecnames = $self->getStatisticData(
                          $name . '/' .
                          NotifyMonitoringExt::InactiveEventChannelNames);
      next if (!defined $ecnames);
      foreach my $ecname (@$ecnames) {
        $num = $self->getStatisticData(
                        $ecname . '/' .
                        NotifyMonitoringExt::EventChannelCreationTime);
        next if (!defined $num);
        $time = $num->last();
        $time = $self->getTimeString($time);
        $tree->add($ecname,
                   -text  => basename($ecname),
                   -data  => $time,
                   -image => $self->{'inactive_image'});
      }
    }
  }

  ## Expand the tree by default
  $tree->autosetmode();
}

sub shutdownEC {
  my($self) = shift;
  my($name) = $self->{'current'};

  if (defined $name && index($name, '/') >= 0) {
    if (defined $self->{'nsm'}) {
      $self->{'nsm'}->shutdown_event_channel($name);
      $self->update();
    }
  }
  else {
    $self->{'main'}->messageBox(-title   => 'Error',
                                -type    => 'Ok',
                                -icon    => 'error',
                                -message => 'You must select an ' .
                                            'event channel.');
  }
}

sub more {
  my($self) = shift;
  my($name) = $self->{'current'};

  ## Remove all the text
  $self->{'text'}->delete('1.0', 'end');

  if (defined $name) {
    ## Every entry has a creation time
    my($time) = $self->{'tree'}->info('data', $name);
    $self->{'text'}->insert('end', "Created on $time\n\n");

    if (index($name, '/') >= 0) {
      my($suppliers) = $self->getStatisticData(
                                $name . '/' .
                                NotifyMonitoringExt::EventChannelSupplierNames);
      return if (!defined $suppliers);
      if (scalar(@$suppliers) > 0) {
        $self->{'text'}->insert('end', "Suppliers:\n");
        foreach my $supplier (@$suppliers) {
          $self->{'text'}->insert('end', '  ' . $supplier . "\n");
        }
        $self->{'text'}->insert('end', "\n");
      }

      my($consumers) = $self->getStatisticData(
                                $name . '/' .
                                NotifyMonitoringExt::EventChannelConsumerNames);
      return if (!defined $consumers);
      my($concount) = scalar(@$consumers);
      if ($concount > 0) {
        $self->{'text'}->insert('end', "Consumers:\n");
        foreach my $consumer (@$consumers) {
          $self->{'text'}->insert('end', '  ' . $consumer . "\n");
        }
        $self->{'text'}->insert('end', "\n");
      }

      my($queuecount) = $self->getStatisticData(
                           $name . '/' .
                           NotifyMonitoringExt::EventChannelQueueElementCount);
      return if (!defined $queuecount);
      $queuecount = $queuecount->last();

      if ($concount > 0) {
        $self->{'text'}->insert('end',
                                "Queue: $queuecount messages");
      }

      my($queuesize) = $self->getStatisticData(
                           $name . '/' .
                           NotifyMonitoringExt::EventChannelQueueSize);
      return if (!defined $queuesize);
      $queuesize = $queuesize->last();

      ## Since we're not obtaining queuecount and queuesize atomically,
      ## we have to make sure that there's still something in the queue.
      if ($queuecount == 0 && $queuesize > 0) {
        if ($concount > 0) {
          $self->{'text'}->insert('end', "\n");
        }
      }
      else {
        $self->{'text'}->insert('end',
                                ' approximately totaling ' .
                                "$queuesize bytes\n\n");

        my($oldest) = $self->getStatisticData(
                             $name . '/' .
                             NotifyMonitoringExt::EventChannelOldestEvent);
        return if (!defined $oldest);

        $oldest = $self->getTimeString($oldest->last());
        $self->{'text'}->insert('end',
                                "Oldest event creation time: $oldest\n\n");

        $consumers = $self->getStatisticData(
                              $name . '/' .
                              NotifyMonitoringExt::EventChannelSlowestConsumers);
        return if (!defined $consumers);

        if (scalar(@$consumers) > 0) {
          $self->{'text'}->insert('end', "Slowest Consumers:\n");
          foreach my $consumer (@$consumers) {
            $self->{'text'}->insert('end', '  ' . $consumer . "\n");
          }
          $self->{'text'}->insert('end', "\n");
        }
      }
    }
  }
}

sub help {
  my($self)      = shift;
  my($text)      = shift;
  my($topic)     = shift;
  my($db)        = $self->{'main'}->DialogBox(-title   => 'Help',
                                              -buttons => [ 'Ok' ]);
  my($top)       = $db->Subwidget('top');
  my($width)     = 248;
  my($height)    = 200;
  my($headFrame) = $top->Frame()->pack();

  $headFrame->Label(-text => "Help on $topic",
                    -font => $headingFont
                   )->pack(-anchor => 'n',
                           -side => 'top');

  $top->Label(-text    => $text,
              -wrap    => $width - 12,
              -justify => 'left',
             )->pack(-anchor => 'w',
                     -side   => 'top',
                     -padx   => 2);
  $db->Show();
}

sub about {
  my($self) = shift;
  my($text) = "The $self->{'title'}\n" .
              "Version $self->{'version'}\n" .
              "by Chad Elliott";
  my($db)   = $self->{'main'}->DialogBox(-title   => 'About',
                                         -buttons => [ 'Ok' ]);
  my($top)  = $db->Subwidget('top');

  $top->Label(-text       => $text,
              -justify    => 'center',
             )->pack(-side => 'left',
                     -padx => 2);
  $db->Show();
}

sub getTimeString {
  my($self) = shift;
  my($time) = shift;

  ## Get the digits to the right of the decimal point and remove
  ## the leading zero
  my($dec)  = sprintf("%0.03f", $time - int($time));
  $dec =~ s/^0//;

  ## Get the time as a string and add in the digits to the right of
  ## the decimal point for the seconds
  my($str)  = scalar(localtime($time));
  $str =~ s/(\d+:\d+:\d+)/${1}$dec/;

  return $str;
}

sub getStatisticData {
  my($self) = shift;
  my($name) = shift;
  my($val)  = undef;

  $self->{'tk_die'} = $SIG{__DIE__};
  $SIG{__DIE__} = sub {};

  try {
    if (!defined $self->{'nsm'}) {
      my($obj) = $self->{'orb'}->string_to_object($self->{'loc'});
      $self->{'nsm'} = CosNotification::NotificationServiceMonitorControl::_narrow($obj);
    }

    if (defined $self->{'nsm'}) {
      my($data) = $self->{'nsm'}->get_statistic($name);

      if ($data->_d() == CosNotification::NotificationServiceMonitorControl::DATA_NUMERIC) {
        $val = $data->num();
      }
      else {
        $val = $data->list();
      }
    }
    else {
      if ($self->{'auto'}) {
        $self->{'auto'} = 0;
        $self->repeat();
      }

      $self->{'main'}->messageBox(-title   => 'Error',
                                  -type    => 'Ok',
                                  -icon    => 'error',
                                  -message => 'Unable to locate the ' .
                                              'monitor service.');
    }
  }
  catch CORBA::Exception with {
    if ($self->{'auto'}) {
      $self->{'auto'} = 0;
      $self->repeat();
    }

    my($ex) = shift;
    my(%opts) = (-title   => 'Error',
                 -type    => 'Ok',
                 -icon    => 'error',
                 -message => "Received an exception:\n" . $ex);
    $opts{-wraplength} = '4i' if ($^O ne 'MSWin32');

    $self->{'main'}->messageBox(%opts);
    $self->{'nsm'} = undef;
  };

  $SIG{__DIE__} = $self->{'tk_die'};
  return $val;
}

sub repeat {
  my($self) = shift;
  if ($self->{'auto'}) {
    if (!defined $self->{'repeat'}) {
      my($id) = $self->{'main'}->repeat(1000,
                                        sub { $self->more(); });
      $self->{'repeat'} = $id;
    }
  }
  else {
    if (defined $self->{'repeat'}) {
      $self->{'main'}->afterCancel($self->{'repeat'});
      $self->{'repeat'} = undef;
    }
  }
}

1;
