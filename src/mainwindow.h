#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
// #include <QStandardItemModel>
#include <QThread>
#include <addonsources/curse.hpp>
#include <QMetaType>

Q_DECLARE_METATYPE(std::vector<Addon>);
Q_DECLARE_METATYPE(std::vector<QPixmap>);

namespace Ui {
class MainWindow;
}

class Worker : public QThread {

	Q_OBJECT

	public:
		Worker(std::string search) {
			searchParam = search;
		}
	private:
		std::string searchParam;
		void run() override;
	signals:
	void resultReady(const QVariant& variantAddons, const QVariant& variantThumbnails);
};

class MainWindow : public QMainWindow
{
	Q_OBJECT
	QThread workerThread;
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

public slots:
	void handleResults(const QVariant& variantAddons, const QVariant& variantThumbnails);
private slots:
	void on_searchButton_released();
	void on_searchField_returnPressed();
	void on_installPathButton_released();
	void downloadAddon();

signals:
	void operate(const QString &);

private:
	Ui::MainWindow* ui;
	QStandardItemModel* model;
	bool searching = false;

	void startSearchThread();
	void showLoadingIcon();
	void showSearchButton();
	void extractZipArchive(std::string filepath);
};

#endif // MAINWINDOW_H
