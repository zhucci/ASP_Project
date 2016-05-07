#include <qapplication.h>
#include "application.h"
#include "MainFrame.h"
#include "QResource.h"
#include "ostream"

#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>

int main( int argc, char ** argv ) 
{
	//Q_INIT_RESOURCE(iconRes);
    QApplication a( argc, argv );
	 char* logFile="StdOutFile.txt";
	// int outPut = open(logFile.toStdString().c_str(), O_CREAT | O_RDWR);
	 FILE *file;
	 if (fopen_s(&file, logFile, "w") == 0){
		_dup2(_fileno(file),1);
	 }

    MainFrame *w = new MainFrame();
    w->show();
    a.connect( &a, SIGNAL(), &a, SLOT(quit()) );
    a.exec();

	 fflush(stdout);
	 fclose(file);
	 return 0;
}
