#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QtWidgets>
#include <QTableWidgetItem>
#include "Core.hpp"
#include "Addon.hpp"
#include <iostream>
#include <Connection.h>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	curse.updateDatabase();
	ui->setupUi(this);
	// Create model
	model = new QStandardItemModel(0, 0);

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
	delete ui;
}

void MainWindow::on_searchButton_released() {
	model->setRowCount(0);

	std::vector<Addon> addons = curse.search(ui->searchField->text().toUtf8().constData());

	int i = 0;
	for(Addon addon : addons) {
		if(addon.getImageLink().compare("Not set") != 0) {
			// Download the thumbnail image
			Connection conn{};
			conn.connect(addon.getImageLink());
			conn.save_data_to_file("resources/tmp_thumbnail");
			QLabel* label = new QLabel();
			label->setPixmap(QPixmap("resources/tmp_thumbnail"));
			label->setScaledContents(true);
			model->setItem(i, 0, nullptr);
			ui->tableView->setIndexWidget(model->index(i, 0), label);
		} else {
			// If there is no image we will display a happy little penguin for now :)
			QLabel* label = new QLabel();
			label->setPixmap(QPixmap("resources/Tux-icon-mono.svg"));
			label->setScaledContents(true);
			model->setItem(i, 0, nullptr);
			ui->tableView->setIndexWidget(model->index(i, 0), label);
		}

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
