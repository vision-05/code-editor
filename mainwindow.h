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
#include <syntaxhighlighter.h>

//make sure to add installer to set path for save data
//auto indent and auto complete, auto add brackets
//add numbered lines
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
    int findEndOfName(QString path);
    QString getTabName(QString path);

private slots:
    void openSettings(); //default theme, text font and size
    void save();
    void saveAs();
    void open();
    void newFile();
    void cut();
    void copy();
    void paste();
    void undo();
    void redo();

    void darkPlain();
    void darkTwilight();
    void darkPastel();

    void run();
    void search(int index);
    void closeTab(int index);
    void newTab();

    void closeEditor(int index);

private:
    void createActions();
    void createMenus();

    Ui::MainWindow *ui;
    QMenu* options;
    QMenu* file;
    QMenu* edit;
    QMenu* color;
    QMenu* font;
    QMenu* background;
    QMenu* darkMode;

    QAction* openAct = new QAction(tr("Open File"));
    QAction* newAct = new QAction(tr("New File"));
    QAction* cutAct = new QAction(tr("Cut Text"));
    QAction* copyAct = new QAction(tr("Copy Text"));
    QAction* pasteAct = new QAction(tr("Paste Text"));
    QAction* saveAct = new QAction(tr("Save File"));
    QAction* saveAsAct = new QAction(tr("Save File As"));
    QAction* settingsAct = new QAction(tr("Open Settings"));
    QAction* undoAct = new QAction(tr("Undo Action"));
    QAction* redoAct = new QAction(tr("Redo Action"));

    QAction* plainDark = new QAction(tr("Plain Dark"));
    QAction* pastelDark = new QAction(tr("Pastel Dark"));
    QAction* twilightDark = new QAction(tr("Twilight Dark"));

    QWindow* window;
    QString filePath;
    QString saveData;
    QString exeFilePath;

    QWebEngineView* first = new QWebEngineView;
    std::vector<QWebEngineView*> windows {first};

    QTextEdit* textEdit = new QTextEdit;
    std::vector<QTextEdit*> edits {textEdit};
    std::vector<QString> paths {filePath};

    SyntaxHighlighter* highlighter;

    QFont mainfont;
    const int tabStop = 4;

    QString sadPath;
};
#endif // MAINWINDOW_H
