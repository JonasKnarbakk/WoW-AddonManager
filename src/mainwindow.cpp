#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include <QMessageBox>
// #include <QStandardItemModel>
// #include <QSortFilterProxyModel>
// #include <QtWidgets>
// #include <QTableWidgetItem>
#include "Core.hpp"
#include "Addon.hpp"
#include <iostream>
#include <Connection.h>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	// Create model
	model = new QStandardItemModel(0, 0);

	// Prepare the loading icon
	QMovie *movie = new QMovie("resources/ajax-loader.gif");
	ui->loadingIcon->setMovie(movie);
	ui->loadingIcon->setVisible(false);

	QStringList columnNames;
	columnNames << "Image" << "Name" << "Version" << "Supports" << "Downloads" << " ";
	model->setHorizontalHeaderLabels(columnNames);

	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableView->setModel(model);

	QHeaderView *verticalHeader = ui->tableView->verticalHeader();
	verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
	verticalHeader->setDefaultSectionSize(100);
	// QSortFilterProxyModel proxyModel;
	// proxyModel.setSourceModel(model);
	// ui->tableView->sortByColumn(4, Qt::DescendingOrder);
}

MainWindow::~MainWindow()
{
	workerThread.quit();
	workerThread.wait();
	delete ui;
}

void MainWindow::on_searchButton_released() {
	startSearchThread();
}

void MainWindow::on_searchField_returnPressed() {
	startSearchThread();
}

void MainWindow::on_installPathButton_released() {
	// QFileDialog fileDialog(this);
	// fileDialog.setFileMode(QFileDialog::DirectoryOnly);
	// fileDialog.setViewMode(QFileDialog::Detail);
	QDir defaultPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
			"/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui->installPathField->setText(defaultPath.absolutePath());
	std::cout << defaultPath.absolutePath().toUtf8().constData() << std::endl;
}

void MainWindow::downloadAddon() {
	QObject* obj = sender();
	QVariant value = obj->property("link");
	std::string url = value.toString().toUtf8().constData();
	std::cout << "Download link: " << url << std::endl;
	Connection conn{};
	conn.connect(url);
	conn.save_data_to_file("test.zip");
}

void Worker::run() {
	std::cout << "doWork started" << std::endl;
	/* Expensive operation */
	Curse curse{};
	curse.updateDatabase();
	std::vector<Addon> addons = curse.search(searchParam);

	std::vector<QPixmap> thumbnails;
	for (Addon addon : addons) {
		if(addon.getImageLink().compare("Not set") != 0) {
			// Download the thumbnail image
			Connection conn{};
			conn.connect(addon.getImageLink());
			conn.save_data_to_file("resources/tmp_thumbnail");
			QPixmap pixmap("resources/tmp_thumbnail");
			thumbnails.push_back(pixmap);
		} else {
			// If there is no image we will display a happy little penguin for now :)
			QPixmap pixmap("resources/Tux-icon-mono.svg");
			thumbnails.push_back(pixmap);
		}
	}

	std::cout << "doWork finished" << std::endl;

	// Pack the addons into a QVariant for sending
	QVariant variantAddons;
	variantAddons.setValue(addons);
	// Do the same to the thumbnails
	QVariant variantThumbnails;
	variantThumbnails.setValue(thumbnails);
	emit resultReady(variantAddons, variantThumbnails);
}

void MainWindow::handleResults(const QVariant& variantAddons, const QVariant& variantThumbnails) {
	std::vector<Addon> addons;
	std::vector<QPixmap> thumbnails;
	if (variantAddons.canConvert<std::vector<Addon>>()
			&& variantThumbnails.canConvert<std::vector<QPixmap>>()) {
		addons = variantAddons.value<std::vector<Addon>>();
		thumbnails = variantThumbnails.value<std::vector<QPixmap>>();
	} else {
		return;
	}
	model->setRowCount(0);
	int i = 0;
	for(Addon addon : addons) {
		QLabel* label = new QLabel();
		label->setPixmap(thumbnails.at(i));
		label->setScaledContents(true);
		model->setItem(i, 0, nullptr);
		ui->tableView->setIndexWidget(model->index(i, 0), label);

		// ui->tableView->setIndexWidget(model->index(i, 0), thumbnail);
		QStandardItem* name = new QStandardItem(QString("%0").arg(addon.getName().c_str()));
		model->setItem(i, 1, name);
		QStandardItem* version = new QStandardItem(QString("%0").arg(addon.getVersion().c_str()));
		model->setItem(i, 2, version);
		QStandardItem* supported = new QStandardItem(QString("%0").arg(addon.getSupportedVersion().c_str()));
		model->setItem(i, 3, supported);
		QStandardItem* downloads = new QStandardItem;
		downloads->setData(addon.getTotalDownloads(), Qt::DisplayRole);
		model->setItem(i, 4, downloads);
		model->setItem(i, 5, nullptr);
		QPushButton* downloadLink = new QPushButton();
		downloadLink->setText("Download");
		downloadLink->setProperty("link", QVariant(addon.getDownloadLink().c_str()));
		connect(downloadLink, SIGNAL(released()), this, SLOT(downloadAddon()));
		ui->tableView->setIndexWidget(model->index(i, 5), downloadLink);
		i++;
	}

	ui->tableView->sortByColumn(4, Qt::DescendingOrder);

	searching = false;
	showSearchButton();
}

void MainWindow::startSearchThread() {
	if(!searching) {
		showLoadingIcon();
		Worker* worker = new Worker(ui->searchField->text().toUtf8().constData());
		connect(worker, &Worker::resultReady, this, &MainWindow::handleResults);
		connect(worker, &Worker::finished, worker, &QObject::deleteLater);
		worker->start();
	}
}

void MainWindow::showLoadingIcon() {
	ui->searchButton->setVisible(false);
	ui->loadingIcon->setVisible(true);
	ui->loadingIcon->movie()->start();
}

void MainWindow::showSearchButton() {
	ui->searchButton->setVisible(true);
	ui->loadingIcon->movie()->stop();
	ui->loadingIcon->setVisible(false);
}
