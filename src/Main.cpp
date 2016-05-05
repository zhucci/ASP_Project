#include <qapplication.h>
#include "application.h"
#include "MainFrame.h"
#include "QResource.h"


int main( int argc, char ** argv ) 
{
	//Q_INIT_RESOURCE(iconRes);
    QApplication a( argc, argv );

    MainFrame *w = new MainFrame();
    w->show();
    a.connect( &a, SIGNAL(), &a, SLOT(quit()) );
    return a.exec();
}
