#include <QtWidgets/QMainWindow>
#include "ui_QFile_Explorer.h"
#include <QtCore>
#include <QWidget>
#include <QMessageBox>
#include <QAbstractItemModel>

class QFile_Explorer : public QMainWindow
{
	Q_OBJECT

public:
	QFile_Explorer(QWidget* parent = nullptr);
	~QFile_Explorer();


private slots:
	void DirectoryChanged(const QString& path);
	void FileChanged(const QString& path);
	void EnterPath();
	void SaveLogs();

private:
	void DispalyContents(QString& path);
	Ui::QtWidgetsFile_ExplorerClass ui;
	QFileSystemWatcher* watcher;
	QString dirpath, filepath;


};