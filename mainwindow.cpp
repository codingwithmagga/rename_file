#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    fillComboBox(m_ui->cb_year, "years.txt", { "2024", "2025" }, "2025");
    fillComboBox(m_ui->cb_month, "months.txt", getMonthsInGerman(), "Januar");
    fillComboBox(m_ui->cb_type, "types.txt", { "Rechnung (RE)", "Kontoauszug (KA)" }, "Rechnung (RE)");

    connect(m_ui->pb_chooseFile, &QPushButton::clicked, this, &MainWindow::chooseFile);
    connect(m_ui->pb_renameFile, &QPushButton::clicked, this, &MainWindow::renameFile);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::chooseFile()
{
    const QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select a PDF file",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "PDF Files (*.pdf);;All Files (*)");

    m_ui->le_file->setText(fileName);
}

void MainWindow::renameFile()
{
    const QString oldFileName = m_ui->le_file->text();
    QFile file(oldFileName);

    if (!file.exists()) {
        QMessageBox::critical(this, "Error", "Datei nicht gefunden.", QMessageBox::Ok);
        return;
    }

    const QString typeText = m_ui->cb_type->currentText();
    QString type;
    if (typeText.length() >= 3) {
        type = typeText.mid(typeText.length() - 3, 2);
    } else {
        QMessageBox::critical(this, "Error", "Typ konnte nicht korrekt identifziert werden.", QMessageBox::Ok);
        return;
    }

    const int month = m_ui->cb_month->currentIndex() + 1;
    QString monthText = QString::number(month);
    if (month < 10) {
        monthText.prepend("0");
    }

    QFileInfo fileInfo(oldFileName);
    QString newFileName = QStringList { m_ui->cb_year->currentText(), monthText, type, m_ui->le_additionalInformation->text() }.join("_");
    newFileName.append("." + fileInfo.suffix());
    const QString newFileNamePath = QDir(fileInfo.absolutePath()).absoluteFilePath(newFileName);

    if (file.rename(newFileNamePath)) {
        QMessageBox::information(this, "Erfolg", "Umbenennung der Datei erfolgreich.", QMessageBox::Ok);
        return;
    }

    QMessageBox::critical(this, "Error", "Umbenennung der Datei fehlgeschlagen.", QMessageBox::Ok);
}

void MainWindow::fillComboBox(QComboBox* const box, const QString fileName, const QStringList& initialValues, const QString defaultValue)
{
    QString filePath = QDir::current().absoluteFilePath(fileName);
    QFile file(filePath);

    if (!file.exists()) {
        // Create the file and write initial values
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream out(&file);

            for (const auto value : initialValues) {
                out << value << "\n";
            }

            file.close();
        } else {
            qDebug() << "Unable to create file.";
            return;
        }
    }

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            box->addItem(line);
        }
        file.close();
    }

    box->setCurrentText(defaultValue);
}

QStringList MainWindow::getMonthsInGerman() const
{
    QStringList months;
    months << "Januar"
           << "Februar"
           << "März"
           << "April"
           << "Mai"
           << "Juni"
           << "Juli"
           << "August"
           << "September"
           << "Oktober"
           << "November"
           << "Dezember";
    return months;
}
