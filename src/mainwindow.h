#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStandardItemModel>
#include <addonsources/curse.hpp>

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
	void on_searchButton_released();
	void on_installPathButton_released();
	void downloadAddon();
	
private:
	Ui::MainWindow *ui;
	QStandardItemModel* model;
	Curse curse{};
};

#endif // MAINWINDOW_H
