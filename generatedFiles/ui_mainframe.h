/********************************************************************************
** Form generated from reading UI file 'mainframe.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFRAME_H
#define UI_MAINFRAME_H

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

class Ui_MainFrame
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
    QAction *actionT1;
    QAction *actionVoxel;
    QAction *actionT2;
    QAction *actionT3;
    QAction *actionT4;
    QAction *actionT5;
    QWidget *mainwidget;
    QMenuBar *menubar;
    QMenu *menu_FILE;
    QMenu *menu_IMPORT;
    QMenu *menu_EXPORT;
    QMenu *menu_VIEW;
    QMenu *menu_HELP;
    QStatusBar *ProgressBar;
    QToolBar *MainBar;
    QToolBar *AsmPlaneBar;
    QToolBar *TestFunction;

    void setupUi(QMainWindow *MainFrame)
    {
        if (MainFrame->objectName().isEmpty())
            MainFrame->setObjectName(QString::fromUtf8("MainFrame"));
        MainFrame->resize(800, 600);
        MainFrame->setMinimumSize(QSize(100, 50));
        QFont font;
        font.setFamily(QString::fromUtf8("Segoe UI"));
        font.setPointSize(10);
        font.setItalic(false);
        font.setKerning(true);
        MainFrame->setFont(font);
        MainFrame->setMouseTracking(false);
        actionEXIT = new QAction(MainFrame);
        actionEXIT->setObjectName(QString::fromUtf8("actionEXIT"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/check.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEXIT->setIcon(icon);
        actionSAVESEQ = new QAction(MainFrame);
        actionSAVESEQ->setObjectName(QString::fromUtf8("actionSAVESEQ"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/downloads.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSAVESEQ->setIcon(icon1);
        actionSTEP = new QAction(MainFrame);
        actionSTEP->setObjectName(QString::fromUtf8("actionSTEP"));
        QIcon icon2;
        QString iconThemeName = QString::fromUtf8("New");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QString::fromUtf8(":/file.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionSTEP->setIcon(icon2);
        actionCONTROLPANEL = new QAction(MainFrame);
        actionCONTROLPANEL->setObjectName(QString::fromUtf8("actionCONTROLPANEL"));
        actionABOUT = new QAction(MainFrame);
        actionABOUT->setObjectName(QString::fromUtf8("actionABOUT"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionABOUT->setIcon(icon3);
        actionSTEPBack = new QAction(MainFrame);
        actionSTEPBack->setObjectName(QString::fromUtf8("actionSTEPBack"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/stepback.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSTEPBack->setIcon(icon4);
        actionPlayBack = new QAction(MainFrame);
        actionPlayBack->setObjectName(QString::fromUtf8("actionPlayBack"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/playback.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlayBack->setIcon(icon5);
        actionStop = new QAction(MainFrame);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon6);
        actionNextStep = new QAction(MainFrame);
        actionNextStep->setObjectName(QString::fromUtf8("actionNextStep"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/stepfoward.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNextStep->setIcon(icon7);
        actionPlayFoward = new QAction(MainFrame);
        actionPlayFoward->setObjectName(QString::fromUtf8("actionPlayFoward"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlayFoward->setIcon(icon8);
        actionFoto = new QAction(MainFrame);
        actionFoto->setObjectName(QString::fromUtf8("actionFoto"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/camera.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFoto->setIcon(icon9);
        actionDBGShow = new QAction(MainFrame);
        actionDBGShow->setObjectName(QString::fromUtf8("actionDBGShow"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/dbg.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDBGShow->setIcon(icon10);
        actionSTEPFile = new QAction(MainFrame);
        actionSTEPFile->setObjectName(QString::fromUtf8("actionSTEPFile"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/file.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSTEPFile->setIcon(icon11);
        actionPrevSession = new QAction(MainFrame);
        actionPrevSession->setObjectName(QString::fromUtf8("actionPrevSession"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrevSession->setIcon(icon12);
        actionSaveSession = new QAction(MainFrame);
        actionSaveSession->setObjectName(QString::fromUtf8("actionSaveSession"));
        actionSaveSession->setIcon(icon1);
        actionT1 = new QAction(MainFrame);
        actionT1->setObjectName(QString::fromUtf8("actionT1"));
        actionT1->setCheckable(false);
        actionVoxel = new QAction(MainFrame);
        actionVoxel->setObjectName(QString::fromUtf8("actionVoxel"));
        actionT2 = new QAction(MainFrame);
        actionT2->setObjectName(QString::fromUtf8("actionT2"));
        actionT3 = new QAction(MainFrame);
        actionT3->setObjectName(QString::fromUtf8("actionT3"));
        actionT4 = new QAction(MainFrame);
        actionT4->setObjectName(QString::fromUtf8("actionT4"));
        actionT5 = new QAction(MainFrame);
        actionT5->setObjectName(QString::fromUtf8("actionT5"));
        mainwidget = new QWidget(MainFrame);
        mainwidget->setObjectName(QString::fromUtf8("mainwidget"));
        mainwidget->setMinimumSize(QSize(100, 50));
        MainFrame->setCentralWidget(mainwidget);
        menubar = new QMenuBar(MainFrame);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menu_FILE = new QMenu(menubar);
        menu_FILE->setObjectName(QString::fromUtf8("menu_FILE"));
        menu_IMPORT = new QMenu(menu_FILE);
        menu_IMPORT->setObjectName(QString::fromUtf8("menu_IMPORT"));
        menu_EXPORT = new QMenu(menu_FILE);
        menu_EXPORT->setObjectName(QString::fromUtf8("menu_EXPORT"));
        menu_VIEW = new QMenu(menubar);
        menu_VIEW->setObjectName(QString::fromUtf8("menu_VIEW"));
        menu_HELP = new QMenu(menubar);
        menu_HELP->setObjectName(QString::fromUtf8("menu_HELP"));
        MainFrame->setMenuBar(menubar);
        ProgressBar = new QStatusBar(MainFrame);
        ProgressBar->setObjectName(QString::fromUtf8("ProgressBar"));
        MainFrame->setStatusBar(ProgressBar);
        MainBar = new QToolBar(MainFrame);
        MainBar->setObjectName(QString::fromUtf8("MainBar"));
        MainFrame->addToolBar(Qt::TopToolBarArea, MainBar);
        AsmPlaneBar = new QToolBar(MainFrame);
        AsmPlaneBar->setObjectName(QString::fromUtf8("AsmPlaneBar"));
        MainFrame->addToolBar(Qt::TopToolBarArea, AsmPlaneBar);
        TestFunction = new QToolBar(MainFrame);
        TestFunction->setObjectName(QString::fromUtf8("TestFunction"));
        TestFunction->setFloatable(false);
        MainFrame->addToolBar(Qt::TopToolBarArea, TestFunction);

        menubar->addAction(menu_FILE->menuAction());
        menubar->addAction(menu_VIEW->menuAction());
        menubar->addAction(menu_HELP->menuAction());
        menu_FILE->addSeparator();
        menu_FILE->addAction(menu_IMPORT->menuAction());
        menu_FILE->addAction(menu_EXPORT->menuAction());
        menu_FILE->addAction(actionEXIT);
        menu_IMPORT->addAction(actionSTEP);
        menu_IMPORT->addAction(actionPrevSession);
        menu_EXPORT->addAction(actionSAVESEQ);
        menu_VIEW->addAction(actionCONTROLPANEL);
        menu_HELP->addAction(actionABOUT);
        MainBar->addSeparator();
        MainBar->addAction(actionSTEP);
        MainBar->addAction(actionSAVESEQ);
        AsmPlaneBar->addAction(actionPlayBack);
        AsmPlaneBar->addAction(actionSTEPBack);
        AsmPlaneBar->addAction(actionStop);
        AsmPlaneBar->addAction(actionNextStep);
        AsmPlaneBar->addAction(actionPlayFoward);
        AsmPlaneBar->addAction(actionDBGShow);
        AsmPlaneBar->addAction(actionFoto);
        TestFunction->addAction(actionT1);
        TestFunction->addAction(actionT2);
        TestFunction->addAction(actionT3);
        TestFunction->addAction(actionT4);
        TestFunction->addAction(actionT5);
        TestFunction->addAction(actionVoxel);

        retranslateUi(MainFrame);

        QMetaObject::connectSlotsByName(MainFrame);
    } // setupUi

    void retranslateUi(QMainWindow *MainFrame)
    {
        MainFrame->setWindowTitle(QApplication::translate("MainFrame", "\320\241\320\270\321\201\321\202\320\265\320\274\320\260 \320\220\320\237\320\237\320\241", 0, QApplication::UnicodeUTF8));
        actionEXIT->setText(QApplication::translate("MainFrame", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        actionSAVESEQ->setText(QApplication::translate("MainFrame", "\320\237\320\276\321\201\320\273\320\265\320\264\320\276\320\262\320\260\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\321\214", 0, QApplication::UnicodeUTF8));
        actionSTEP->setText(QApplication::translate("MainFrame", "STEP \321\204\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        actionCONTROLPANEL->setText(QApplication::translate("MainFrame", "\320\237\320\260\320\275\320\265\320\273\321\214 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217", 0, QApplication::UnicodeUTF8));
        actionABOUT->setText(QApplication::translate("MainFrame", "\320\236 \320\277\321\200\320\270\320\273\320\276\320\266\320\265\320\275\320\270\320\270...", 0, QApplication::UnicodeUTF8));
        actionSTEPBack->setText(QApplication::translate("MainFrame", "STEPBack", 0, QApplication::UnicodeUTF8));
        actionPlayBack->setText(QApplication::translate("MainFrame", "PlayBack", 0, QApplication::UnicodeUTF8));
        actionStop->setText(QApplication::translate("MainFrame", "Stop", 0, QApplication::UnicodeUTF8));
        actionNextStep->setText(QApplication::translate("MainFrame", "NextStep", 0, QApplication::UnicodeUTF8));
        actionPlayFoward->setText(QApplication::translate("MainFrame", "PlayFoward", 0, QApplication::UnicodeUTF8));
        actionFoto->setText(QApplication::translate("MainFrame", "Foto", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFoto->setToolTip(QApplication::translate("MainFrame", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionDBGShow->setText(QApplication::translate("MainFrame", "DBGShow", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionDBGShow->setToolTip(QApplication::translate("MainFrame", "\320\237\320\276\320\272\320\260\320\267\321\213\320\262\320\260\321\202\321\214 \321\201\320\262\320\276\320\261\320\276\320\264\320\275\321\213\320\265 \320\275\320\260\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217 \320\270\320\267\320\262\320\273\320\265\320\272\320\260\320\265\320\274\320\276\320\271 \320\264\320\265\321\202\320\260\320\273\320\270", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSTEPFile->setText(QApplication::translate("MainFrame", "STEP \321\204\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        actionPrevSession->setText(QApplication::translate("MainFrame", "\320\237\321\200\320\265\320\264\321\213\320\264\321\203\321\211\321\203\321\216 \321\201\320\265\321\201\321\201\320\270\321\216", 0, QApplication::UnicodeUTF8));
        actionSaveSession->setText(QApplication::translate("MainFrame", "\320\242\320\265\320\272\321\203\321\211\321\203\321\216 \321\201\320\265\321\201\321\201\320\270\321\216", 0, QApplication::UnicodeUTF8));
        actionT1->setText(QApplication::translate("MainFrame", "T1", 0, QApplication::UnicodeUTF8));
        actionVoxel->setText(QApplication::translate("MainFrame", "Voxel", 0, QApplication::UnicodeUTF8));
        actionT2->setText(QApplication::translate("MainFrame", "T2", 0, QApplication::UnicodeUTF8));
        actionT3->setText(QApplication::translate("MainFrame", "T3", 0, QApplication::UnicodeUTF8));
        actionT4->setText(QApplication::translate("MainFrame", "T4", 0, QApplication::UnicodeUTF8));
        actionT5->setText(QApplication::translate("MainFrame", "T5", 0, QApplication::UnicodeUTF8));
        menu_FILE->setTitle(QApplication::translate("MainFrame", "\320\244\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        menu_IMPORT->setTitle(QApplication::translate("MainFrame", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", 0, QApplication::UnicodeUTF8));
        menu_EXPORT->setTitle(QApplication::translate("MainFrame", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        menu_VIEW->setTitle(QApplication::translate("MainFrame", "\320\222\320\270\320\264", 0, QApplication::UnicodeUTF8));
        menu_HELP->setTitle(QApplication::translate("MainFrame", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", 0, QApplication::UnicodeUTF8));
        MainBar->setWindowTitle(QApplication::translate("MainFrame", "Main ToolBar", 0, QApplication::UnicodeUTF8));
        AsmPlaneBar->setWindowTitle(QApplication::translate("MainFrame", "Assembly Sequence Planning Bar", 0, QApplication::UnicodeUTF8));
        TestFunction->setWindowTitle(QApplication::translate("MainFrame", "toolBar_2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainFrame: public Ui_MainFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFRAME_H