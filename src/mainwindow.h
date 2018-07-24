#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
// #include <QStandardItemModel>
#include <QThread>
#include <addonsources/curse.hpp>
#include <QMetaType>
#include <mutex>

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

class AddonIndexer : public QThread {
	Q_OBJECT

	public:
		AddonIndexer() {}

	private:
		void run() override;
	signals:
	void addonIndexResultReady(const QVariant& variantAddons);
};

class MainWindow : public QMainWindow
{
	Q_OBJECT
	QThread workerThread;
	QThread addonIndexerThread;
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
	static void getThumbnail(const std::string url);

public slots:
	void handleResults(const QVariant& variantAddons, const QVariant& variantThumbnails);
	void handleAddonIndexResuslts(const QVariant& variantAddons);
private slots:
	void on_searchButton_released();
	void on_searchField_returnPressed();
	void on_installPathButton_released();
	void onAddonRightClick(QPoint point);
	void downloadAddon();

signals:
	void operate(const QString &);

private:
	Ui::MainWindow* ui;
	QStandardItemModel* searchModel;
	QStandardItemModel* installedModel;
	bool searching = false;
	bool indexing = false;

	void startSearchThread();
	void startIndexAddonsThread();
	void showLoadingIcon();
	void showLoadingIconInstalledTab(bool show);
	void showSearchButton();
	static std::mutex thumbnailLock;
};

#endif // MAINWINDOW_H
