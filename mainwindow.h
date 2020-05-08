#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QWebEngineView>
#include <QTabWidget>
#include <QMenu>
#include <QAction>
#include <QWindow>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QTextStream>
#include <QPushButton>
#include <vector>
#include <QKeyEvent>

//make sure to add installer to set path for save data
//add modified QSyntaxHighlighter class and also numbered lines
//after all functionality implemented, make multitasking, then create windows compatible

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void saveAsPlainText();
    void savePlainText();

private slots:
    void openSettings(); //default theme, text font and size
    void save();
    void saveAs();
    void open();
    void newFile();
    void cut();
    void copy();
    void paste();
    void bold();
    void italic();
    void underline();
    void undo();
    void redo();

    void darkPlain();
    void darkTwilight();
    void darkPastel();
    void lightPlain();
    void lightPastel();

    void run();
    void search(int index);
    void closeTab(int index);
    void newTab();

private:
    void createActions();
    void createMenus();

    Ui::MainWindow *ui;
    QMenu* options; //TODO: right click context menu: edit menu
    QMenu* file;
    QMenu* edit;
    QMenu* color;
    QMenu* font; //TODO: change font size, font type
    QMenu* background;
    QMenu* darkMode;
    QMenu* lightMode;

    QAction* openAct = new QAction(tr("Open File"));
    QAction* newAct = new QAction(tr("New File"));
    QAction* cutAct = new QAction(tr("Cut Text"));
    QAction* copyAct = new QAction(tr("Copy Text"));
    QAction* pasteAct = new QAction(tr("Paste Text"));
    QAction* boldAct = new QAction(tr("Bold Text"));
    QAction* italicAct = new QAction(tr("Italic Text"));
    QAction* underlineAct = new QAction(tr("Underline Text"));
    QAction* saveAct = new QAction(tr("Save File"));
    QAction* saveAsAct = new QAction(tr("Save File As"));
    QAction* settingsAct = new QAction(tr("Open Settings"));
    QAction* undoAct = new QAction(tr("Undo Action"));
    QAction* redoAct = new QAction(tr("Redo Action"));

    QAction* plainLight = new QAction(tr("Plain Light"));
    QAction* pastelLight = new QAction(tr("Pastel Light"));
    QAction* plainDark = new QAction(tr("Plain Dark"));
    QAction* pastelDark = new QAction(tr("Pastel Dark"));
    QAction* twilightDark = new QAction(tr("Twilight Dark"));

    QWindow* window;
    QString filePath;
    QString saveData;
    QString exeFilePath;

    QWebEngineView* first = new QWebEngineView;
    std::vector<QWebEngineView*> windows {first};
};
#endif // MAINWINDOW_H
