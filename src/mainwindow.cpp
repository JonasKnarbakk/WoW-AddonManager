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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	// Create model
	model = new QStandardItemModel(3, 6);

	QStringList columnNames;
	columnNames << "Image" << "Name" << "Version" << "Supports" << "Downloads" << "Download Button";
	model->setHorizontalHeaderLabels(columnNames);

	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableView->setModel(model);

	// QSortFilterProxyModel proxyModel;
	// proxyModel.setSourceModel(model);
	// ui->tableView->sortByColumn(4, Qt::DescendingOrder);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_searchButton_clicked() {
	model->setRowCount(0);

	std::vector<Addon> addons = Core::search(ui->searchField->text().toUtf8().constData());

	int i = 0;
	for(Addon addon : addons) {
		QLabel* label = new QLabel();
		label->setPixmap(QPixmap("/home/jonas/Pictures/classic_tux.png"));
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
		ui->tableView->setIndexWidget(model->index(i, 5), downloadLink);
		i++;
	}

	ui->tableView->sortByColumn(4, Qt::DescendingOrder);

	// Glue model and view togeth {}er
	// ui->tableWidget->;
	// ui->listView->insertRow(ui->listView->rowCount());

	// QModelIndex index = ui->listView->index(ui->listView->rowCount()-1);
	// ui-listView->setData(index, firstEntry);
	// QMessageBox::information(this, "Search button pressed!",
			// ui->searchField->text(), QMessageBox::Ok);
}
