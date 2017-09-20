#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QMap>
#include <QStandardItemModel>
#include <QProgressBar>

#include "project.h"
#include "importopenedfiledlg.h"

class ProjectManager : public QObject
{
    Q_OBJECT
public:
    explicit ProjectManager(QObject *parent = 0);
    ~ProjectManager();
    void        selectCurrentProjectByNumber(int projectNumber);
    Project     *returnCurrentProjectsPointer();
    QMap<int, Project* >  arrayOfProjects;

    int                 currentProjectNumber;
    Project             *createNewProject(bool forOpen);

private:
    QString             baseWindowTitle;
    QMap<QString, int>  headerColumnsByCode;
    QStandardItemModel  *mdl;
    ImportOpenedFileDlg *DialogImport;

    QObject             *commonParentPointer;

signals:
    void        sigChangeWindowTitle(QString);
//    void        sigForCreateProject();
    void        sigToCreateEmptyProjectTree(int projectID);
    void        sigToFillProjectTreeByDataFromProject(int projectID);

public slots:
    void        slotCurrentProjectNameChange(QString newName);
    void        slotSelectProjectBeetwenProjectWindows(int prjNum);
    void        slotRetreiveDataForSelectedItemInProjectsTree();
    void        slotCreateProject();
    void        slotOpenProject();
    void        slotSelectProjectByNumber(int);
    void        slotSaveCurrentProject();
    void        slotSaveCurrentProjectAs();
    void        slotSaveAllProjects();
    void        slotCLoseCurrentProject();
    void        slotCloseAllProjects();
};

#endif // PROJECTMANAGER_H
