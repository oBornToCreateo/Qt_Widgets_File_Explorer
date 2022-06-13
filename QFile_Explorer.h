#include <QtWidgets/QMainWindow>
#include "ui_QFile_Explorer.h"
#include <QtCore>
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

class QFile_Explorer : public QMainWindow
{
	Q_OBJECT

public:
	QFile_Explorer(QWidget* parent = nullptr);
	~QFile_Explorer();


private slots:
	void NotifieChanegs();
	void EnterPath();
	void SaveLogs();

private:
	QString dirPath, filePath;
	QFileSystemWatcher* watcher;

	QStringList newList, oldList;
	QStringList RefreshContents(QString& dirpath);

	Ui::QtWidgetsFile_ExplorerClass* ui = new Ui::QtWidgetsFile_ExplorerClass;

};