#include "QFile_Explorer.h"



QFile_Explorer::QFile_Explorer(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	// Connect button signal to appropriate slot
	connect(ui.Action_Btn, &QPushButton::released, this, &QFile_Explorer::EnterPath);
	connect(ui.Action_Btn_2, &QPushButton::released, this, &QFile_Explorer::SaveLogs);

	// Setup QFileSystemWatcher and link with apriopraite slots
	watcher = new QFileSystemWatcher(this);
	
	QObject::connect(watcher, SIGNAL(directoryChanged(QString)),
		this,   SLOT(DirectoryChanged(QString)));
	
	QObject::connect(watcher, SIGNAL(fileChanged(QString)),
		this,   SLOT(FileChanged(QString)));
}

QFile_Explorer::~QFile_Explorer() {

}

void QFile_Explorer::EnterPath(){

	dirpath = ui.Input_Wnd->text();
	DispalyContents(dirpath);

}


void QFile_Explorer::DispalyContents(QString& path){


	QDirIterator folderIt(path, {}, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

	while (folderIt.hasNext()) {

		QFile file(folderIt.next());
		filepath = file.fileName();

		if (QFile::exists(filepath))
			watcher->addPath(filepath);

	}

	ui.Output_Wnd->append(" ---------------------------[ Monitored Content ]--------------------------- ");

	QStringList monitored = watcher->directories() + watcher->files();

	Q_FOREACH(QString dirs, monitored)
		ui.Output_Wnd->append(dirs);

	ui.Output_Wnd->append(" ---------------------------[ MSGS ]--------------------------- ");

}

void QFile_Explorer::DirectoryChanged(const QString& path)
{

	Q_UNUSED(path)
	ui.Output_Wnd->append(" [dir changed]  " + path );

}

void QFile_Explorer::FileChanged(const QString& path)
{

	Q_UNUSED(path)
	ui.Output_Wnd->append( " [file changed]  "  + path );

}

void QFile_Explorer::SaveLogs()
{

	auto logpath = ui.Input_Wnd_2->text();
	auto TableContents = ui.Output_Wnd->toPlainText();

	QString filename = logpath +  "/LOG.txt";
	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly)) {
		file.close();
	}
	else {
		file.write(TableContents.toUtf8());
		ui.Output_Wnd->append(" The log file was created at :" + filename);
		file.close();
	}


}
