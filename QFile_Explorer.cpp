#include "QFile_Explorer.h"



QFile_Explorer::QFile_Explorer(QWidget* parent) : QMainWindow(parent)
{
	ui->setupUi(this);

	// Connect button signal to appropriate slot
	connect(ui->Action_Btn, &QPushButton::released, this, &QFile_Explorer::EnterPath);
	connect(ui->Action_Btn_2, &QPushButton::released, this, &QFile_Explorer::SaveLogs);
	
	// Inicialize file Monitoring System
	watcher = new QFileSystemWatcher(this);
	QObject::connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(NotifieChanegs()));
	QObject::connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(NotifieChanegs()));
}

QFile_Explorer::~QFile_Explorer() {

	delete ui;
}

void QFile_Explorer::EnterPath(){

	ui->Output_Wnd->clear();
	oldList.erase(newList.begin(), newList.end());

	//display the input window & set file path to top input_wnd
	dirPath = QFileDialog::getExistingDirectory(this, " Choose the directory to be monitored : ", QDir::homePath());
	ui->Input_Wnd->setText(dirPath);
	
	// Refrresh contents to be monitored & add paths to watcherr
	oldList = RefreshContents(dirPath);
	oldList.sort();

	ui->Output_Wnd->append(" ---------------------------[ Monitored Content ]--------------------------- ");

	//Dispalay contents in sorted order,
	auto moniDirs = watcher->directories();
	moniDirs.sort();
	auto moniFiles = watcher->files(); 
	moniFiles.sort();

	auto monitored = moniDirs + moniFiles;

	Q_FOREACH(QString dirs, monitored)
		ui->Output_Wnd->append(dirs);

	ui->Output_Wnd->append(" ---------------------------[ MSGS ]--------------------------- ");

}

void QFile_Explorer::NotifieChanegs()
{
	// trochę niebezpieczny sposób przrekazania śćieżki

	dirPath = ui->Input_Wnd->text();
	newList = RefreshContents(dirPath);
	newList.sort();

	QSet<QString> oldSet(oldList.begin(), oldList.end());
	QSet<QString> newSet(newList.begin(), newList.end());


	if (oldList < newList) {
		
		// file was delited
		auto intersection = oldSet.subtract(newSet);

		foreach(QString name, intersection)
		{
			ui->Output_Wnd->setTextColor(Qt::red);
			ui->Output_Wnd->append("  [!] File was delited"  + name);
		}
		
	}
	else if (oldList > newList) {

		// file was created
		auto intersection = newSet.subtract(oldSet);

		foreach (QString name ,intersection)
		{
			ui->Output_Wnd->setTextColor(Qt::darkBlue);
			ui->Output_Wnd->append("  [!] File was created  " + name);
		}

	}
	else if (oldList == newList) {

		ui->Output_Wnd->setTextColor(Qt::darkGreen);
		ui->Output_Wnd->append("  [!] File was modified  ");

	}
	else {

		ui->Output_Wnd->setTextColor(Qt::darkMagenta);
		ui->Output_Wnd->append("[!] Other Modification  ");
	}

	ui->Output_Wnd->setTextColor(Qt::black);

	oldList = newList;

}

QStringList QFile_Explorer::RefreshContents(QString& dirpath) {

	QDir dir(dirpath);
	QStringList list;
	dir.setSorting(QDir::SortFlag::DirsFirst | QDir::SortFlag::Name);
	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

	// Iterate throught folder contents and add paths to the monitoring system ;
	// Add main directory to watch
	watcher->addPath(dirpath);
	list.append(dirpath);

	QDirIterator folderIt(dir);
	while (folderIt.hasNext()) {

		QFile file(folderIt.next());
		filePath = file.fileName();

		// add dirrectory files to wach
		if (QFile::exists(filePath)) {
			watcher->addPath(filePath);
			list.append(filePath);
		}

	}

	return list;
}

void QFile_Explorer::SaveLogs()
{

	auto logpath = QFileDialog::getExistingDirectory(this, " Choose the directory to be monitored : ", QDir::homePath());
	ui->Input_Wnd_2->setText(logpath);

	auto TableContents = ui->Output_Wnd->toPlainText();

	QString filename = logpath +  "/LOG.txt";
	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly)) {
		file.close();
	}
	else {
		file.write(TableContents.toUtf8());
		ui->Output_Wnd->append(" The log file was created at :" + filename);
		file.close();
	}


}
