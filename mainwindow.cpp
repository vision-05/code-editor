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

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, [=]() {
        MainWindow::search(ui->tabWidget->currentIndex());
    });

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, [=]() {
        MainWindow::closeTab(ui->tabWidget->currentIndex());
    });

    connect(ui->newTabButton, &QPushButton::released, this, &MainWindow::newTab);
    connect(ui->savePlainText, &QPushButton::released, this, &MainWindow::saveAsPlainText);

    createActions();
    createMenus();

    setWindowTitle(tr("Untitled"));
    setMinimumSize(160, 160);
    resize(1000, 610);

    QString data[5] = {"1","2","3","4","5"};

    QFile file(tr("/Users/tim/Desktop/saveData.sad"));
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "save data not open");
        return;
    }
    QTextStream in(&file);
    saveData = in.readAll();
    file.close();

    if(saveData == data[0]) {
        lightPlain();
    }
    else if(saveData == data[1]) {
        lightPastel();
    }
    else if(saveData == data[2]) {
        darkPlain();
    }
    else if(saveData == data[3]) {
        darkTwilight();
    }
    else if(saveData == data[4]) {
        darkPastel();
    }
    else {
        QMessageBox::warning(this, "..", "no compatible save data");
    }
}

MainWindow::~MainWindow()
{
    QFile file(tr("/Users/tim/Desktop/saveData.sad"));
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
    QFile file(filePath);
    if(!file.open(QFile::WriteOnly | QFile::Text)) { //if the file doesn't exist or there is no path in memory
        MainWindow::saveAs(); //save file as
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toHtml();
    out << text;
    file.flush();
    file.close();
}

void MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Open the file"); //get file through input
    QFile file(fileName);
    filePath = fileName; //make the current path in memory the path of the new file
    if(!file.open(QFile::WriteOnly | QFile::Text)) { //make sure the file is writeable or open
        QMessageBox::warning(this, "..", "file not open");
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toHtml(); //convert text to html code
    out << text; //output the text
    file.flush();
    file.close();
    this->setWindowTitle(fileName);
}

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file"); //user input for file and location
    QFile file(fileName); //create a file with the name
    filePath = fileName;
    if(!file.open(QFile::ReadOnly | QFile::Text)) { //make sure the file is readable and exists
        QMessageBox::warning(this, "..", "file not open");
        return;
    }
    QTextStream in(&file); //read text using stream
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    this->setWindowTitle(fileName);
}

void MainWindow::newFile() {
    filePath = "";
    this->setWindowTitle(tr("Untitled_1"));
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

void MainWindow::bold() {
    if(!ui->textEdit->fontWeight() || ui->textEdit->fontWeight() < 99) { //font weight 99 is bold
        ui->textEdit->setFontWeight(99);
    }
    else {
        ui->textEdit->setFontWeight(0); //font weight 0 is thin
    }
}

void MainWindow::underline() {
    if(!ui->textEdit->fontUnderline()) {
        ui->textEdit->setFontUnderline(true);
    }
    else {
        ui->textEdit->setFontUnderline(false);
    }
}

void MainWindow::italic() {
    if(!ui->textEdit->fontItalic()) {
        ui->textEdit->setFontItalic(true);
    }
    else {
        ui->textEdit->setFontItalic(false);
    }
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
    saveData = "3";
}

void MainWindow::darkTwilight() {
    this->setStyleSheet("background-color: DarkSlateBlue; color: LightSteelBlue");
    saveData = "4";
}

void MainWindow::darkPastel() {
    this->setStyleSheet("background-color: DarkSlateGray; color: MediumAquaMarine");
    saveData = "5";
}

void MainWindow::lightPlain() {
    this->setStyleSheet("background-color: white; color: black");
    saveData = "2";
}

void MainWindow::lightPastel() {
    this->setStyleSheet("background-color: white; color: LightBlue");
    saveData = "1";
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
    QWebEngineView* temp = new QWebEngineView;
    windows.push_back(temp);
    int nextIndex = ui->tabWidget->count();
    ui->tabWidget->addTab(windows[nextIndex],tr("New Tab"));

}

//PRIVATE

void MainWindow::createActions() {

    QAction* nonShortcutActions[6] {plainDark,pastelDark,twilightDark,plainLight,pastelLight,settingsAct};

    QAction* shortcutActions[12] {openAct,newAct,saveAct,saveAsAct,cutAct,copyAct,pasteAct,redoAct,undoAct,boldAct,
                                italicAct,underlineAct};

    QKeySequence shortcuts[12] {QKeySequence::Open,QKeySequence::New,QKeySequence::Save,QKeySequence::SaveAs,
                              QKeySequence::Cut,QKeySequence::Copy,QKeySequence::Paste,QKeySequence::Redo,
                              QKeySequence::Undo,QKeySequence::Bold,QKeySequence::Italic,QKeySequence::Underline};

    void (MainWindow::*shortcutSlotFunction[12])() {&MainWindow::open,&MainWindow::newFile,&MainWindow::save,
                                                    &MainWindow::saveAs,&MainWindow::cut,&MainWindow::copy,
                                                    &MainWindow::paste,&MainWindow::redo,&MainWindow::undo,
                                                    &MainWindow::bold,&MainWindow::italic,&MainWindow::underline};
    void (MainWindow::*nonShortcutSlotFunction[6])() {&MainWindow::darkPlain,&MainWindow::darkPastel,
                                                      &MainWindow::darkTwilight, &MainWindow::lightPlain,
                                                      &MainWindow::lightPastel,&MainWindow::openSettings};

    //initialise array of functors (function pointers)

    for(int i{}; i < 12; ++i) {
        shortcutActions[i]->setShortcut(shortcuts[i]);
        connect(shortcutActions[i], &QAction::triggered, this, shortcutSlotFunction[i]);
    }

    for(int i{}; i < 6; ++i) {
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
    edit->addSeparator();
    edit->addAction(boldAct);
    edit->addAction(italicAct);
    edit->addAction(underlineAct);

    background = menuBar()->addMenu(tr("&Themes"));
    lightMode = background->addMenu(tr("&Light"));
    darkMode = background->addMenu(tr("&Dark"));
    darkMode->addAction(plainDark);
    darkMode->addAction(twilightDark);
    darkMode->addAction(pastelDark);
    lightMode->addAction(plainLight);
    lightMode->addAction(pastelLight);
}

