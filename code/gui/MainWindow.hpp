#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QListWidgetItem>

#include <QDialog>
#include <QLabel>
#include <QStatusBar>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <stack>

#include "ProcessingStepParams.hpp"
#include "ProcessingStepInputs.hpp"

#include "../framework/Configuration.hpp"
#include "../framework/Logger.hpp"
#include "../framework/ProcessingStep.hpp"
#include "../framework/ModuleManager.hpp"

#include <QStandardItemModel>


namespace Ui {
class MainWindow;
}

namespace uipf {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	// loads a new configuration from file
	void loadDataFlow(std::string);

private slots:
	// Buttons addStep/deleteStep
    void on_addButton_clicked();
    void on_deleteButton_clicked();

    // Activation of Step (via clicking)
    void on_listProcessingSteps_activated(const QModelIndex & index);
    void on_appendToLog(const Logger::LogType&, const std::string& );

	// menu bar
	// File
	void new_Data_Flow();
	void load_Data_Flow();
	void save_Data_Flow();
	void save_Data_Flow_as();
	// Help
	void about();
	// Edit
	void undo();
	void redo();
	// Configuration
	void run();
	void stop();

	//stepName changed
	void stepNameChanged();

private:
	// default window title that appears next to the file name
	const std::string WINDOW_TITLE = "uipf";

    Ui::MainWindow *ui;

	// the module manager instance
	ModuleManager mm_;

	// model for the listView of processing steps
    QStringListModel *modelStep;
	// model for the module selector
    QStringListModel *modelModule;
	// model for the params editor table
    ProcessingStepParams *modelTableParams;
	// model for the input editor table
    ProcessingStepInputs *modelTableInputs;
    QStandardItemModel  *model; // TODO replaces the above

	// the file name of the currently loaded configuration
	std::string currentFileName;
	// the currently loaded configuration represented in the window
   	Configuration conf_;

	// current name of a precessing step
	std::string currentStepName;

   	// Redo and Undo stacks, which store configurations
   	std::stack<Configuration> undoStack;
   	std::stack<Configuration> redoStack;
	// fills the undo and redo stacks
	void beforeConfigChange();

	// sets a Module name list
	void setModuleList();

	// menu bar
    void createActions();
    void deleteActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *configMenu;
    QMenu *helpMenu;

    // actions in fileMenu
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    // actions in editMenu
   	QAction *undoAct;
    QAction *redoAct;

    // actions in configMenu
   	QAction *runAct;
    QAction *stopAct;

    // actions in helpMenu
    QAction *aboutAct;
};

}; // namespace

#endif // MAINWINDOW_H
