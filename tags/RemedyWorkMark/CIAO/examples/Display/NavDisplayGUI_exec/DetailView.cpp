// $Id$

#include "DetailView.h"
#include <Qt/q3table.h>
#include <Qt/qlayout.h>
#include <Qt/qpushbutton.h>
#include <Qt/qtabwidget.h>
#include <Qt/qlabel.h>
#include <Qt/qlineedit.h>
#include <Qt/qspinbox.h>

DetailView::DetailView(QWidget *parent, const char *name)
: QWidget(parent, name), current_unit(0)
{
  QGridLayout *grid = new QGridLayout(this, 8, 10);

  // Create a label containing a QMovie
  separatorlabel = new QLabel(this, "label0" );

  grid->addMultiCellWidget(separatorlabel, 0, 0, 0, 9);

  QLabel *sem_id_label = new QLabel("FDN:", this);
  grid->addWidget(sem_id_label, 1, 0);
  sem_id_val = new QLineEdit(this);
  sem_id_val->setReadOnly(1);
  grid->addMultiCellWidget(sem_id_val, 1, 1, 1, 7);

  QLabel *descr_label = new QLabel("Description:", this);
  grid->addWidget(descr_label, 2, 0);
  descr_edit = new QLineEdit(this);
  grid->addMultiCellWidget(descr_edit, 2, 2, 1, 7);

  QLabel *x_label = new QLabel("X coordinate:", this);
  grid->addWidget(x_label, 3, 0);
  x_edit = new QSpinBox(this);
  x_edit->setMinValue(0);
  x_edit->setMaxValue(65535);
  grid->addWidget(x_edit, 3, 1);

  QLabel *y_label = new QLabel("Y coordinate:", this);
  grid->addWidget(y_label, 3, 3);
  y_edit = new QSpinBox(this);
  y_edit->setMinValue(0);
  y_edit->setMaxValue(65535);
  grid->addWidget(y_edit, 3, 4);

  QLabel *z_label = new QLabel("Z coordinate:", this);
  grid->addWidget(z_label, 3, 6);
  z_edit = new QSpinBox(this);
  z_edit->setMinValue(0);
  z_edit->setMaxValue(65535);
  grid->addWidget(z_edit, 3, 7);

  // Create a label containing a QMovie
  QString path_to_movie;
  char *ace_root = ACE_OS::getenv("ACE_ROOT");
  if(ace_root)
  {
    path_to_movie += ace_root;
    path_to_movie += "/TAO/CIAO/examples/handcrafted/Display/NavDisplayGUI_exec/trolltech.gif";
  }
  else
  {
    path_to_movie = "../NavDisplayGUI_exec/trolltech.gif";
  }
  movie = new QMovie(path_to_movie);
  movielabel = new QLabel(this, "label1" );
//  movie->connectStatus(this, SLOT(movieStatus(int)));
//  movie->connectUpdate(this, SLOT(movieUpdate(const QRect&)));
  movielabel->setFrameStyle( QFrame::Box | QFrame::Plain );
  movielabel->setMovie(movie);
  movielabel->setFixedSize( 128+movielabel->frameWidth()*2,
                            64+movielabel->frameWidth()*2 );
  grid->addMultiCellWidget(new QLabel("", this), 1, 4, 8, 8);
  grid->addMultiCellWidget(movielabel, 1, 4, 9, 9);


  QPushButton *apply = new QPushButton("Apply", this);
  apply->setDefault(1);
  grid->addWidget(apply, 7, 9);

  // FUZZ: disable check_for_lack_ACE_OS
  connect(apply, SIGNAL(clicked()), this, SLOT(apply()));
  // FUZZ: enable check_for_lack_ACE_OS

  QTabWidget *tabs = new QTabWidget(this);


  child_list = new Q3Table(0, 6, tabs);
  child_list->setSelectionMode(Q3Table::Single);
  child_list->horizontalHeader()->setLabel(0, "FDN");
  child_list->horizontalHeader()->setLabel(1, "Description");
  child_list->horizontalHeader()->setLabel(2, "X Coordinate");
  child_list->horizontalHeader()->setLabel(3, "Y Coordinate");
  child_list->horizontalHeader()->setLabel(4, "Z Coordinate");
  child_list->horizontalHeader()->setLabel(5, "Color");

  parent_list = new Q3Table(0, 6, tabs);
  parent_list->setSelectionMode(Q3Table::Single);
  parent_list->horizontalHeader()->setLabel(0, "FDN");
  parent_list->horizontalHeader()->setLabel(1, "Description");
  parent_list->horizontalHeader()->setLabel(2, "X Coordinate");
  parent_list->horizontalHeader()->setLabel(3, "Y Coordinate");
  parent_list->horizontalHeader()->setLabel(4, "Z Coordinate");
  parent_list->horizontalHeader()->setLabel(5, "Color");

  tabs->addTab(child_list, "Property set &1");
  tabs->addTab(parent_list, "Property set &2");

  grid->addMultiCellWidget(new QLabel(this), 4, 4, 0, 9);
  grid->addMultiCellWidget(tabs, 5, 5, 0, 9);
}

DetailView::~DetailView()
{
  delete movie;
}

void
DetailView::currentNode(NavUnit *unit)
{
  UnitLocation loc = unit->getLocation();
  descr_edit->setText(unit->getShortDescr());
  this->updateLocation(loc);
}

void
DetailView::updateLocation(const UnitLocation &loc)
{
  x_edit->setValue(static_cast<int>(loc.x_));
  y_edit->setValue(static_cast<int>(loc.y_));
  z_edit->setValue(static_cast<int>(loc.z_));
}

void
DetailView::apply()
{
}


void
DetailView::movieUpdate( const QRect& )
{
  // Uncomment this to test animated icons on your window manager
  //setIcon( movie.framePixmap() );
}


void
DetailView::movieStatus( int )
{
/*  switch ( s )
    {
    case QMovie::SourceEmpty:
    case QMovie::UnrecognizedFormat:
      {
        QPixmap pm("tt-logo.png");
        movielabel->setPixmap(pm);
        movielabel->setFixedSize(pm.size());
      }
      break;
    default:
      if ( movielabel->movie() ) // for flicker-free animation:
        movielabel->setBackgroundMode( Qt::NoBackground );
    }*/
}
