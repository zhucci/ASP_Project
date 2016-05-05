/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionEXIT;
    QAction *actionSAVESEQ;
    QAction *actionSTEP;
    QAction *actionCONTROLPANEL;
    QAction *actionABOUT;
    QAction *actionSTEPBack;
    QAction *actionPlayBack;
    QAction *actionStop;
    QAction *actionNextStep;
    QAction *actionPlayFoward;
    QAction *actionFoto;
    QAction *actionDBGShow;
    QAction *actionSTEPFile;
    QAction *actionPrevSession;
    QAction *actionSaveSession;
    QWidget *mainwidget;
    QMenuBar *menubar;
    QMenu *menu_FILE_2;
    QMenu *menu_IMPORT;
    QMenu *menu_EXPORT;
    QMenu *menu_VIEW;
    QMenu *menu_HELP;
    QStatusBar *ProgressBar;
    QToolBar *asmCotrolToolBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setMinimumSize(QSize(100, 50));
        QFont font;
        font.setFamily(QString::fromUtf8("Segoe UI"));
        font.setPointSize(10);
        font.setItalic(false);
        font.setKerning(true);
        MainWindow->setFont(font);
        MainWindow->setMouseTracking(false);
        actionEXIT = new QAction(MainWindow);
        actionEXIT->setObjectName(QString::fromUtf8("actionEXIT"));
        actionSAVESEQ = new QAction(MainWindow);
        actionSAVESEQ->setObjectName(QString::fromUtf8("actionSAVESEQ"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("PNG/32/download-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSAVESEQ->setIcon(icon);
        actionSTEP = new QAction(MainWindow);
        actionSTEP->setObjectName(QString::fromUtf8("actionSTEP"));
        QIcon icon1;
        QString iconThemeName = QString::fromUtf8("New");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8("PNG/32/file-32.png"), QSize(), QIcon::Normal, QIcon::Off);
            icon1.addFile(QString::fromUtf8("PNG/32/documents-32.png"), QSize(), QIcon::Normal, QIcon::On);
        }
        actionSTEP->setIcon(icon1);
        actionCONTROLPANEL = new QAction(MainWindow);
        actionCONTROLPANEL->setObjectName(QString::fromUtf8("actionCONTROLPANEL"));
        actionABOUT = new QAction(MainWindow);
        actionABOUT->setObjectName(QString::fromUtf8("actionABOUT"));
        actionSTEPBack = new QAction(MainWindow);
        actionSTEPBack->setObjectName(QString::fromUtf8("actionSTEPBack"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("PNG/32/stepback-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSTEPBack->setIcon(icon2);
        actionPlayBack = new QAction(MainWindow);
        actionPlayBack->setObjectName(QString::fromUtf8("actionPlayBack"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("PNG/32/playaback-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlayBack->setIcon(icon3);
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("PNG/32/pause-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon4);
        actionNextStep = new QAction(MainWindow);
        actionNextStep->setObjectName(QString::fromUtf8("actionNextStep"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("PNG/32/stepfoward-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNextStep->setIcon(icon5);
        actionPlayFoward = new QAction(MainWindow);
        actionPlayFoward->setObjectName(QString::fromUtf8("actionPlayFoward"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("PNG/32/play-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlayFoward->setIcon(icon6);
        actionFoto = new QAction(MainWindow);
        actionFoto->setObjectName(QString::fromUtf8("actionFoto"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("PNG/32/camera-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFoto->setIcon(icon7);
        actionDBGShow = new QAction(MainWindow);
        actionDBGShow->setObjectName(QString::fromUtf8("actionDBGShow"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8("PNG/32/share-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDBGShow->setIcon(icon8);
        actionSTEPFile = new QAction(MainWindow);
        actionSTEPFile->setObjectName(QString::fromUtf8("actionSTEPFile"));
        actionPrevSession = new QAction(MainWindow);
        actionPrevSession->setObjectName(QString::fromUtf8("actionPrevSession"));
        actionSaveSession = new QAction(MainWindow);
        actionSaveSession->setObjectName(QString::fromUtf8("actionSaveSession"));
        mainwidget = new QWidget(MainWindow);
        mainwidget->setObjectName(QString::fromUtf8("mainwidget"));
        mainwidget->setMinimumSize(QSize(100, 50));
        MainWindow->setCentralWidget(mainwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menu_FILE_2 = new QMenu(menubar);
        menu_FILE_2->setObjectName(QString::fromUtf8("menu_FILE_2"));
        menu_IMPORT = new QMenu(menu_FILE_2);
        menu_IMPORT->setObjectName(QString::fromUtf8("menu_IMPORT"));
        menu_EXPORT = new QMenu(menu_FILE_2);
        menu_EXPORT->setObjectName(QString::fromUtf8("menu_EXPORT"));
        menu_VIEW = new QMenu(menubar);
        menu_VIEW->setObjectName(QString::fromUtf8("menu_VIEW"));
        menu_HELP = new QMenu(menubar);
        menu_HELP->setObjectName(QString::fromUtf8("menu_HELP"));
        MainWindow->setMenuBar(menubar);
        ProgressBar = new QStatusBar(MainWindow);
        ProgressBar->setObjectName(QString::fromUtf8("ProgressBar"));
        MainWindow->setStatusBar(ProgressBar);
        asmCotrolToolBar = new QToolBar(MainWindow);
        asmCotrolToolBar->setObjectName(QString::fromUtf8("asmCotrolToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, asmCotrolToolBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menu_FILE_2->menuAction());
        menubar->addAction(menu_VIEW->menuAction());
        menubar->addAction(menu_HELP->menuAction());
        menu_FILE_2->addSeparator();
        menu_FILE_2->addAction(menu_IMPORT->menuAction());
        menu_FILE_2->addAction(menu_EXPORT->menuAction());
        menu_FILE_2->addAction(actionEXIT);
        menu_IMPORT->addAction(actionSTEPFile);
        menu_IMPORT->addAction(actionPrevSession);
        menu_EXPORT->addAction(actionSaveSession);
        menu_VIEW->addAction(actionCONTROLPANEL);
        menu_HELP->addAction(actionABOUT);
        asmCotrolToolBar->addSeparator();
        asmCotrolToolBar->addAction(actionSTEP);
        asmCotrolToolBar->addAction(actionSAVESEQ);
        toolBar->addAction(actionPlayBack);
        toolBar->addAction(actionSTEPBack);
        toolBar->addAction(actionStop);
        toolBar->addAction(actionNextStep);
        toolBar->addAction(actionPlayFoward);
        toolBar->addAction(actionDBGShow);
        toolBar->addAction(actionFoto);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\320\241\320\270\321\201\321\202\320\265\320\274\320\260 \320\220\320\237\320\237\320\241", 0, QApplication::UnicodeUTF8));
        actionEXIT->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        actionSAVESEQ->setText(QApplication::translate("MainWindow", "\320\237\320\276\321\201\320\273\320\265\320\264\320\276\320\262\320\260\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\321\214", 0, QApplication::UnicodeUTF8));
        actionSTEP->setText(QApplication::translate("MainWindow", "STEP \321\204\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        actionCONTROLPANEL->setText(QApplication::translate("MainWindow", "\320\237\320\260\320\275\320\265\320\273\321\214 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217", 0, QApplication::UnicodeUTF8));
        actionABOUT->setText(QApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\270\320\273\320\276\320\266\320\265\320\275\320\270\320\270...", 0, QApplication::UnicodeUTF8));
        actionSTEPBack->setText(QApplication::translate("MainWindow", "STEPBack", 0, QApplication::UnicodeUTF8));
        actionPlayBack->setText(QApplication::translate("MainWindow", "PlayBack", 0, QApplication::UnicodeUTF8));
        actionStop->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        actionNextStep->setText(QApplication::translate("MainWindow", "NextStep", 0, QApplication::UnicodeUTF8));
        actionPlayFoward->setText(QApplication::translate("MainWindow", "PlayFoward", 0, QApplication::UnicodeUTF8));
        actionFoto->setText(QApplication::translate("MainWindow", "Foto", 0, QApplication::UnicodeUTF8));
        actionDBGShow->setText(QApplication::translate("MainWindow", "DBGShow", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionDBGShow->setToolTip(QApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\321\213\320\262\320\260\321\202\321\214 \321\201\320\262\320\276\320\261\320\276\320\264\320\275\321\213\320\265 \320\275\320\260\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217 \320\270\320\267\320\262\320\273\320\265\320\272\320\260\320\265\320\274\320\276\320\271 \320\264\320\265\321\202\320\260\320\273\320\270", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSTEPFile->setText(QApplication::translate("MainWindow", "STEP \321\204\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        actionPrevSession->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\265\320\264\321\213\320\264\321\203\321\211\321\203\321\216 \321\201\320\265\321\201\321\201\320\270\321\216", 0, QApplication::UnicodeUTF8));
        actionSaveSession->setText(QApplication::translate("MainWindow", "\320\242\320\265\320\272\321\203\321\211\321\203\321\216 \321\201\320\265\321\201\321\201\320\270\321\216", 0, QApplication::UnicodeUTF8));
        menu_FILE_2->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        menu_IMPORT->setTitle(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", 0, QApplication::UnicodeUTF8));
        menu_EXPORT->setTitle(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        menu_VIEW->setTitle(QApplication::translate("MainWindow", "\320\222\320\270\320\264", 0, QApplication::UnicodeUTF8));
        menu_HELP->setTitle(QApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", 0, QApplication::UnicodeUTF8));
        asmCotrolToolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
