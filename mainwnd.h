#ifndef MAINWND_H
#define MAINWND_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QCloseEvent>

#include "specialwidget.h"

#include "projecttreewidget.h"

#include "projectmanager.h"

namespace Ui {
class MainWnd;
}

class MainWnd : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWnd(QWidget *parent = 0);
    ~MainWnd();

private:
    Ui::MainWnd *ui;

    ProjectManager *prjMgr;

    QMap<int, int> NumbersOfProjectsByNumberOfItemInWindowMenu;

    QHBoxLayout *_lt_main;

    ProjectTreeWidget *projectsTreeWgt;

    QStackedWidget *stackOfWidgets;

    const QString baseWindowTitle = QString("Local Resources Probability and Risks  a n a l i z e r ");

    void    setConnectedSlotsOfChangingsInBaseWidgets(bool connect, SpecialWidget::SpecialWidgetType type);

    void    synchronizeDataAfterChangingInProjectInfoWidget(QObject *senderObj);
    void    synchronizeDataAfterChangingInStratumInfoWidget(QObject *senderObj);
    void    synchronizeDataAfterChangingInStructureInfoWidget(QObject *senderObj);
    void    synchronizeDataAfterChangingInTrapInfoWidget(QObject *senderObj);
    void    synchronizeDataAfterChangingInWellInfoWidget(QObject *senderObj);

    void    refreshWellListOnProject();
    void    refreshWellListOnProjectAfterChaingeWellID(int oldIDwell, int newIDwell);

    void    aboutToCloseProject_CheckForUnSavedChanges();

protected:
    virtual void    closeEvent(QCloseEvent *evnt);

signals:
    void    signalCreateProject();
    void    signalOpenProject();
    void    signalSelectProjectByNumber(int);
    void    signalSaveCurrentProject();
    void    signalSaveAllProjects();
    void    signalCLoseCurrentProject();
    void    signalCloseAllProjects();

    void    signalDoubleClickOnProjectsTreeWidgetItemWithExtInfo(QTreeWidgetItem *item, QModelIndex absItemNumb);

private slots:
    void    slotAboutToQuit_CheckForUnSavedChanges();
    void    slotForCloseCurrentProject_CheckForUnSavedChanges();
    void    slotForCloseAllProjects_CheckForUnSavedChanges();

public slots:
    void    slotCreateElementsForEmptyProject(int projID);
    void    slotSomethingChangedInCurrentWidget();
    void    slotChangeWindowTitle(QString str);

    //------------------------------------------------

    void    slotAddStratumElementInCurrentProjectAtProjectsTreeWgt(int currentProject, bool isInProjectStratumsPartition,
                                                                   BaseItem *itemOfAdd);
    void    slotAddWellElementInCurrentProjectAtProjectsTreeWgt(int projectID,
                                                                bool isInWellsOfProject,
                                                                bool isInWellsOfTrap,
                                                                BaseItem *itemOfAdd);
    void    slotAddStructureElementInCurrentProjectAtProjectsTreeWgt(int currentProject, BaseItem *itemOfAdd);
    void    slotAddTrapElementInCurrentProjectAtProjectsTreeWgt(int currentProject, int currentStructure,
                                                                BaseItem *itemOfAdd);
    void    slotSelectProjectIDThroughClickOnPartition(int projectID);
    void    slotShowEmptyWidget();
    void    slotShowSelectedStratumInfo(int projectID, int stratumID);
    void    slotShowSelectedStructureInfo(int projectID, int structureID);
    void    slotShowSelectedWellInfo(int projectID, int wellID);
    void    slotShowSelectedTrapInfo(int projectID, int trapID, TrapCollection *tc);

    void    slotFillProjectItemsFromItsData(int projectID);
};

#endif // MAINWND_H
