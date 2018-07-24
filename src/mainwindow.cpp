#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Core.hpp"
#include "Addon.hpp"
#include <iostream>
#include <Connection.h>
#include <thread>

extern std::mutex MainWindow::thumbnailLock;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// ui->setWindowIcon(appIcon);

	// Create model
	searchModel = new QStandardItemModel(0, 0);

	// Prepare the loading icon on the search button
	QMovie *movie = new QMovie(":images/bar-loader");
	ui->loadingIcon->setMovie(movie);
	ui->loadingIcon->setVisible(false);

	// Prepare the loading icon on the installed tab
	QMovie *movieTwo = new QMovie(":images/magnify-loader");
	ui->installedLoadingGif->setMovie(movieTwo);
	ui->installedLoadingGif->setVisible(false);
	ui->installedLoadingText->setVisible(false);

	// Populate the header
	QStringList columnNames;
	columnNames << "Image" << "Name" << "Version" << "Supports" << "Downloads" << " ";
	searchModel->setHorizontalHeaderLabels(columnNames);

	// Make the table stretch to the window size
	ui->searchTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->searchTableView->setModel(searchModel);

	// Get the addon install path
	ui->installPathField->setText(QString("%0").arg(Core::getInstallPath().c_str()));

	// Set row height
	QHeaderView *verticalHeader = ui->searchTableView->verticalHeader();
	verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
	verticalHeader->setDefaultSectionSize(100);

	startIndexAddonsThread();
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
	Core::setInstallPath(defaultPath.absolutePath().toUtf8().constData());
}

// TODO: Create index over unzipped files so cleanup will
// be easy when updating or uninstalling
void MainWindow::downloadAddon() {
	QObject* obj = sender();
	QVariant value = obj->property("link");
	std::string url = value.toString().toUtf8().constData();
	std::cout << "Download link: " << url << std::endl;
	Connection conn{};
	conn.connect(url);

	namespace bf = boost::filesystem;

	bf::path tmpFile = bf::unique_path("%%%%%%%%%%%%%%%%");
	std::string filepath = Core::getInstallPath()
		+ bf::path::preferred_separator
		+ tmpFile.filename().native();
	std::cout << "Saving file to: " << filepath << std::endl;
	conn.save_data_to_file(filepath);

	Core::extractZipArchive(filepath);
}

void Worker::run() {
	std::cout << "doWork started" << std::endl;
	/* Expensive operation */
	std::cout << "Searching for: " << searchParam << std::endl;
	std::vector<Addon> addons = Core::search(searchParam, false);

	std::vector<QPixmap> thumbnails;
	std::vector<std::thread> threads;

	for (Addon addon : addons) {
		if(addon.getImageLink().compare("Not set") != 0) {
			threads.push_back(std::thread(MainWindow::getThumbnail,
					addon.getImageLink()));

			// We don't want the amount of threads to get out of hand
			if(threads.size() >= std::thread::hardware_concurrency()) {
				for(auto& thread : threads) {
					thread.join();
				}
				threads.clear();
			}

			QPixmap pixmap(":images/tux");
			thumbnails.push_back(pixmap);
		} else {
			// If there is no image we will display a happy little penguin for now :)
			QPixmap pixmap(":images/tux");
			thumbnails.push_back(pixmap);
		}
	}

	for(auto& thread : threads) {
		thread.join();
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
	searchModel->setRowCount(0);
	int i = 0;
	for(Addon addon : addons) {
		QLabel* label = new QLabel();
		label->setPixmap(thumbnails.at(i));
		label->setScaledContents(true);
		searchModel->setItem(i, 0, nullptr);
		ui->searchTableView->setIndexWidget(searchModel->index(i, 0), label);

		// ui->searchTableView->setIndexWidget(searchModel->index(i, 0), thumbnail);
		QStandardItem* name = new QStandardItem(QString("%0").arg(addon.getName().c_str()));
		searchModel->setItem(i, 1, name);
		QStandardItem* version = new QStandardItem(QString("%0").arg(addon.getVersion().c_str()));
		searchModel->setItem(i, 2, version);
		QStandardItem* supported = new QStandardItem(QString("%0").arg(addon.getSupportedVersion().c_str()));
		searchModel->setItem(i, 3, supported);
		QStandardItem* downloads = new QStandardItem;
		downloads->setData(addon.getTotalDownloads(), Qt::DisplayRole);
		searchModel->setItem(i, 4, downloads);
		searchModel->setItem(i, 5, nullptr);
		QPushButton* downloadLink = new QPushButton();
		downloadLink->setText("Download");
		downloadLink->setProperty("link", QVariant(addon.getDownloadLink().c_str()));
		connect(downloadLink, SIGNAL(released()), this, SLOT(downloadAddon()));
		ui->searchTableView->setIndexWidget(searchModel->index(i, 5), downloadLink);
		i++;
	}

	ui->searchTableView->sortByColumn(4, Qt::DescendingOrder);

	searching = false;
	showSearchButton();
}

void AddonIndexer::run() {
	std::vector<Addon> results = Core::indexInstalled();
	QVariant variantAddons;
	variantAddons.setValue(results);
	emit addonIndexResultReady(variantAddons);
}

void MainWindow::handleAddonIndexResuslts(const QVariant& variantAddons) {
	installedModel = new QStandardItemModel(0, 0);
	// Populate the header
	QStringList columnNames;
	columnNames << "Image" << "Name" << "Version" << "Supports" << "Downloads" << " ";
	installedModel->setHorizontalHeaderLabels(columnNames);

	// Make the table stretch to the window size
	ui->installedTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->installedTableView->setModel(installedModel);

	// Set row height
	QHeaderView *verticalHeader = ui->installedTableView->verticalHeader();
	verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
	verticalHeader->setDefaultSectionSize(100);

	std::vector<Addon> addons;
	if (variantAddons.canConvert<std::vector<Addon>>()) {
		addons = variantAddons.value<std::vector<Addon>>();
	} else {
		return;
	}
	installedModel->setRowCount(0);
	int i = 0;
	for(Addon addon : addons) {
		QLabel* label = new QLabel();
		QPixmap pixmap(":images/tux");
		label->setPixmap(pixmap);
		label->setScaledContents(true);
		installedModel->setItem(i, 0, nullptr);
		ui->installedTableView->setIndexWidget(installedModel->index(i, 0), label);

		QStandardItem* name = new QStandardItem(QString("%0").arg(addon.getName().c_str()));
		installedModel->setItem(i, 1, name);
		QStandardItem* version = new QStandardItem(QString("%0").arg(addon.getVersion().c_str()));
		installedModel->setItem(i, 2, version);
		QStandardItem* supported = new QStandardItem(QString("%0").arg(addon.getSupportedVersion().c_str()));
		installedModel->setItem(i, 3, supported);
		QStandardItem* downloads = new QStandardItem;
		downloads->setData(addon.getTotalDownloads(), Qt::DisplayRole);
		installedModel->setItem(i, 4, downloads);
		installedModel->setItem(i, 5, nullptr);
		// QPushButton* downloadLink = new QPushButton();
		// downloadLink->setText("Download");
		// downloadLink->setProperty("link", QVariant(addon.getDownloadLink().c_str()));
		// connect(downloadLink, SIGNAL(released()), this, SLOT(downloadAddon()));
		// ui->installedTableView->setIndexWidget(installedModel->index(i, 5), downloadLink);
		i++;
	}

	ui->installedTableView->sortByColumn(4, Qt::DescendingOrder);

	std::cout << "Indexing done!" << std::endl;
	indexing = false;
	showLoadingIconInstalledTab(false);
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

void MainWindow::startIndexAddonsThread() {
	if(!indexing) {
		showLoadingIconInstalledTab(true);
		indexing = true;
		AddonIndexer* indexer = new AddonIndexer();
		connect(indexer, &AddonIndexer::addonIndexResultReady, this, &MainWindow::handleAddonIndexResuslts);
		connect(indexer, &Worker::finished, indexer, &QObject::deleteLater);
		indexer->start();
	}
}

void MainWindow::showLoadingIcon() {
	ui->searchButton->setVisible(false);
	ui->loadingIcon->setVisible(true);
	ui->loadingIcon->movie()->start();
}

void MainWindow::showLoadingIconInstalledTab(bool show) {
	ui->installedLoadingGif->setVisible(show);
	ui->installedLoadingText->setVisible(show);
	ui->installedTableView->setVisible(!show);
	if(show) {
		ui->installedLoadingGif->movie()->start();
	} else {
		ui->installedLoadingGif->movie()->stop();
	}
}

void MainWindow::showSearchButton() {
	ui->searchButton->setVisible(true);
	ui->loadingIcon->movie()->stop();
	ui->loadingIcon->setVisible(false);
}


void MainWindow::getThumbnail(const std::string url) {
	namespace bf = boost::filesystem;

	bf::path tmpStorage = bf::temp_directory_path();
	bf::path tmpFile = bf::unique_path("%%%%%%%%%%%%%%%%");

	std::string tmpFileLocation = "resources/"
		+ tmpStorage.filename().native()
		+ tmpFile.filename().native();
	// Download the thumbnail image
	Connection conn{};
	conn.connect(url);
	conn.save_data_to_file(tmpFileLocation.c_str());
	QPixmap pixmap(tmpFileLocation.c_str());

	// thumbnailLock.lock();
	std::cout << "Saving thumbail as: " << tmpFileLocation << std::endl;
	// thumbnails.push_back(pixmap);
	bf::remove(tmpFileLocation);
	// thumbnailLock.unlock();
}
