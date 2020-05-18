#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->centralwidget);
    ui->centralwidget->setLayout(ui->horizontalLayout);
    ui->centralwidget->setContentsMargins(10,10,10,10);

    ui->textEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->tabWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->horizontalLayout->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
    ui->horizontalLayout_2->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
    ui->verticalLayout_2->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);

    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(first, tr("New Tab"));

    ui->tabWidget_2->setTabsClosable(true);
    ui->tabWidget_2->removeTab(0);
    ui->tabWidget_2->removeTab(0);
    ui->tabWidget_2->addTab(edits[0], tr("Untitled"));

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, [=]() {
        MainWindow::search(ui->tabWidget->currentIndex());
    });

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);

    connect(ui->tabWidget_2, &QTabWidget::tabCloseRequested, this, &MainWindow::closeEditor);

    connect(ui->newTabButton, &QPushButton::released, this, &MainWindow::newTab);
    connect(ui->savePlainText, &QPushButton::released, this, &MainWindow::saveAsPlainText);

    createActions();
    createMenus();

    setMinimumSize(160, 160);
    resize(1000, 610);

    mainfont.setFamily("Arial");
    mainfont.setStyleHint(QFont::Monospace);
    mainfont.setFixedPitch(true);
    mainfont.setPointSize(14);
    edits[0]->setFont(mainfont);

    QFontMetrics metrics(mainfont);
    edits[0]->setTabStopDistance(tabStop * metrics.QFontMetrics::horizontalAdvance(' '));

    highlighter = new SyntaxHighlighter(edits[0]->document());

    //SAVE DATA

    QString data[3] = {"1","2","3"};

    QString filename = QFileDialog::getOpenFileName(this, "Open the file");
    sadPath = filename;
    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "save data not open");
        return;
    }
    QTextStream in(&file);
    saveData = in.readAll();
    file.close();

    if(saveData == data[0]) {
        darkPlain();
    }
    else if(saveData == data[1]) {
        darkTwilight();
    }
    else if(saveData == data[2]) {
        darkPastel();
    }
    else {
        QMessageBox::warning(this, "..", "no compatible save data");
    }
}

MainWindow::~MainWindow()
{
    QString filename;
    if(sadPath != tr("")){
        filename = sadPath;
    }
    else {
        filename = QFileDialog::getSaveFileName(this, "Open the file");
    }
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "file not open");
        return;
    }
    QTextStream out(&file);
    out << saveData;
    file.flush();
    file.close();
    delete ui;
}

//PRIVATE SLOTS

void MainWindow::openSettings() {
    window = new QWindow(); //TODO: change background color in settings
    window->setTitle(tr("Settings"));
    window->resize(500,300);
    window->show();
}

void MainWindow::save() {
    QFile file(paths[ui->tabWidget_2->currentIndex()]);
    if(!file.open(QFile::WriteOnly | QFile::Text)) { //if the file doesn't exist or there is no path in memory
        MainWindow::saveAs(); //save file as
        return;
    }
    QTextStream out(&file);
    QString text = edits[ui->tabWidget_2->currentIndex()]->toHtml();
    out << text;
    file.flush();
    file.close();
}

void MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Open the file"); //get file through input
    QFile file(fileName);
    paths[ui->tabWidget_2->currentIndex()] = fileName; //make the current path in memory the path of the new file
    if(!file.open(QFile::WriteOnly | QFile::Text)) { //make sure the file is writeable or open
        QMessageBox::warning(this, "..", "file not open");
        return;
    }
    QTextStream out(&file);
    QString text = edits[ui->tabWidget_2->currentIndex()]->toHtml(); //convert text to html code
    out << text; //output the text
    file.flush();
    file.close();
    this->setWindowTitle(getTabName(fileName));
    ui->tabWidget_2->setTabText(ui->tabWidget_2->currentIndex(),getTabName(fileName));
}

void MainWindow::open() {
    if(ui->tabWidget_2->count() >= 6) {
        QMessageBox::warning(this,"..","Too many tabs open");
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this, "Open the file"); //user input for file and location
    QFile file(fileName); //create a file with the name
    paths.push_back(fileName); //add the filepath to the vector of paths

    if(!file.open(QFile::ReadOnly | QFile::Text)) { //make sure the file is readable and exists
        QMessageBox::warning(this, "..", "file not open");
        return;
    }

    QTextStream in(&file); //read text using stream
    QString text = in.readAll();
    file.close();

    edits.push_back(new QTextEdit); //create new text edit
    ui->tabWidget_2->addTab(edits[ui->tabWidget_2->count()], getTabName(fileName)); //create a new tab with that edit
    ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->count() - 1); //set current tab
    edits[ui->tabWidget_2->currentIndex()]->setText(text); //set the textedit to the text of the file opened

    edits[ui->tabWidget_2->currentIndex()]->setFont(mainfont);

    QFontMetrics metrics(mainfont);
    edits[ui->tabWidget_2->currentIndex()]->setTabStopDistance(tabStop * metrics.QFontMetrics::horizontalAdvance(' '));

    highlighter = new SyntaxHighlighter(edits[ui->tabWidget_2->currentIndex()]->document()); //set the highlighter to the document
}

void MainWindow::newFile() {
    if(ui->tabWidget_2->count() >= 6) {
        QMessageBox::warning(this,"..","Too many tabs open");
        return;
    }

    paths.push_back(tr("")); //add a new path
    edits.push_back(new QTextEdit); //add a new text edit
    ui->tabWidget_2->addTab(edits[ui->tabWidget_2->count()], tr("Untitled")); //add tab
    ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->count() - 1); //set current tab to new

    edits[ui->tabWidget_2->currentIndex()]->setFont(mainfont);

    QFontMetrics metrics(mainfont);
    edits[ui->tabWidget_2->currentIndex()]->setTabStopDistance(tabStop * metrics.QFontMetrics::horizontalAdvance(' '));

    highlighter = new SyntaxHighlighter(edits[ui->tabWidget_2->currentIndex()]->document()); //set highlighter to current doc
}

void MainWindow::cut() {
    ui->textEdit->cut();
}

void MainWindow::copy() {
    ui->textEdit->copy();
}

void MainWindow::paste() {
    ui->textEdit->paste();
}

void MainWindow::undo() {
    ui->textEdit->undo();
}

void MainWindow::redo() {
    ui->textEdit->redo();
}

//THEMES

void MainWindow::darkPlain() {
    this->setStyleSheet("background-color: DimGray; color: white");
    saveData = "1";
}

void MainWindow::darkTwilight() {
    this->setStyleSheet("background-color: DarkSlateBlue; color: LightSteelBlue");
    saveData = "2";
}

void MainWindow::darkPastel() {
    this->setStyleSheet("background-color: DarkSlateGray; color: MediumAquaMarine");
    saveData = "3";
}

//CODE
void MainWindow::saveAsPlainText() {
    QString fileName = (QFileDialog::getSaveFileName(this, "Open the file"));
    QFile file(fileName);
    exeFilePath = fileName;
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "file not open");
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toHtml();
    out << text;
    file.flush();
    file.close();
}

void MainWindow::savePlainText() {
    QFile file(exeFilePath);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        MainWindow::saveAsPlainText();
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
}

void MainWindow::run() {

}

//WEB SEARCH

void MainWindow::search(int index = 0) {
    QString address = "";
    address.append("https://");
    QString webname = ui->lineEdit->text();
    address.append(webname);
    windows[index]->load(address);
}

void MainWindow::closeTab(int index) {
    windows.erase(windows.begin() + index);
    ui->tabWidget->removeTab(index);
}

void MainWindow::newTab() {
    if(ui->tabWidget->count() >= 6) {
        QMessageBox::warning(this,"..","Too many tabs open");
        return;
    }

    QWebEngineView* temp = new QWebEngineView;
    windows.push_back(temp);
    int nextIndex = ui->tabWidget->count();
    ui->tabWidget->addTab(windows[nextIndex],tr("New Tab"));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

int MainWindow::findEndOfName(QString path) {
    for(int i{path.length() - 1}; i > 0; --i) {
        if(path[i] == tr("/")) {
            return i;
        }
    }
    return -1;
}

QString MainWindow::getTabName(QString path) {
    int index = findEndOfName(path);
    QString name = path.mid(index + 1, path.length() - index);
    return name;
}

//CODE EDITOR

void MainWindow::closeEditor(int index) {
    paths.erase(paths.begin() + index);
    edits.erase(edits.begin() + index);
    ui->tabWidget_2->removeTab(index);
}

//PRIVATE

void MainWindow::createActions() {

    QAction* nonShortcutActions[4] {plainDark,pastelDark,twilightDark,settingsAct};

    QAction* shortcutActions[9] {openAct,newAct,saveAct,saveAsAct,cutAct,copyAct,pasteAct,redoAct,undoAct};

    QKeySequence shortcuts[9] {QKeySequence::Open,QKeySequence::New,QKeySequence::Save,QKeySequence::SaveAs,
                              QKeySequence::Cut,QKeySequence::Copy,QKeySequence::Paste,QKeySequence::Redo,
                              QKeySequence::Undo};

    void (MainWindow::*shortcutSlotFunction[9])() {&MainWindow::open,&MainWindow::newFile,&MainWindow::save,
                                                    &MainWindow::saveAs,&MainWindow::cut,&MainWindow::copy,
                                                    &MainWindow::paste,&MainWindow::redo,&MainWindow::undo};
    void (MainWindow::*nonShortcutSlotFunction[4])() {&MainWindow::darkPlain,&MainWindow::darkPastel,
                                                      &MainWindow::darkTwilight,&MainWindow::openSettings};

    //initialise array of functors (function pointers)

    for(int i{}; i < 9; ++i) {
        shortcutActions[i]->setShortcut(shortcuts[i]);
        connect(shortcutActions[i], &QAction::triggered, this, shortcutSlotFunction[i]);
    }

    for(int i{}; i < 4; ++i) {
        connect(nonShortcutActions[i], &QAction::triggered, this, nonShortcutSlotFunction[i]);
    }


    //OPTIONS

    settingsAct = new QAction(tr("&Open Settings"), this); //QAction object can have qstring type
    settingsAct->setStatusTip(tr("Open settings")); //also a status tip
    settingsAct->setShortcuts(QKeySequence::Preferences);
    connect(settingsAct, &QAction::triggered, this, &MainWindow::openSettings); //connect to slot and triggered is emitter

    //MISC BUTTONS

    connect(ui->runProgram, &QPushButton::released, this, &MainWindow::run);
}

void MainWindow::createMenus() {
    options = menuBar()->addMenu(tr("&Options"));
    options->addAction(settingsAct);

    file = menuBar()->addMenu(tr("&File"));
    file->addAction(newAct);
    file->addAction(openAct);
    file->addAction(saveAct);
    file->addAction(saveAsAct);

    edit = menuBar()->addMenu(tr("&Edit"));
    edit->addAction(undoAct);
    edit->addAction(redoAct);
    edit->addSeparator();
    edit->addAction(cutAct);
    edit->addAction(copyAct);
    edit->addAction(pasteAct);

    background = menuBar()->addMenu(tr("&Themes"));
    darkMode = background->addMenu(tr("&Dark"));
    darkMode->addAction(plainDark);
    darkMode->addAction(twilightDark);
    darkMode->addAction(pastelDark);
}

