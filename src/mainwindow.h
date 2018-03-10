#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_searchButton_clicked();
	// void on_searchField_textChanged(const QString &value);
	
private:
	Ui::MainWindow *ui;
	QStandardItemModel* model;
};

#endif // MAINWINDOW_H
