#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void chooseFile();
    void renameFile();

private:
    void fillComboBox(QComboBox* const box, const QString fileName, const QStringList& initialValues, const QString defaultValue);

    QStringList getMonthsInGerman() const;

    Ui::MainWindow* m_ui;
};
#endif // MAINWINDOW_H
