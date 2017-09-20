#include "projectmanager.h"

ProjectManager::ProjectManager(QObject *parent) : QObject(parent)
{
    arrayOfProjects = QMap<int, Project* >();

    commonParentPointer = parent; // предком для всех Project'ов, производных от QTreeWidget,
                                  // будет родитель объекта ProjectManager
}

ProjectManager::~ProjectManager()
{
}

void ProjectManager::selectCurrentProjectByNumber(int projectNumber)
{
    currentProjectNumber = projectNumber;
}

Project *ProjectManager::createNewProject(bool forOpen)
{
    Project *newPrjPtr = new Project(commonParentPointer);

    connect(newPrjPtr,
            SIGNAL(changeWindowTitle(QString)),
            this,
            SIGNAL(sigChangeWindowTitle(QString)));

    connect(newPrjPtr,
            SIGNAL(toCreateEmptyProjectTree(int)),
            this,
            SIGNAL(sigToCreateEmptyProjectTree(int)));

    int toset = 0;

    QList<int> lstkeys = arrayOfProjects.keys();

    for (int i = 0; i < lstkeys.size(); ++i)
    {
        if (lstkeys.at(i) > toset)
            toset = lstkeys.at(i);
    }

    toset++;

    newPrjPtr->setThisProjectNumb(toset);

    arrayOfProjects.insert(toset, newPrjPtr);

    currentProjectNumber = toset;

//    emit sigToCreateEmptyProjectTree(toset); - Будет вызываться, если всё нормально открылось:

    if (!forOpen) // создание пустого проекта
    {
        emit sigToCreateEmptyProjectTree(toset);
    }

    return newPrjPtr;
}

void ProjectManager::slotCurrentProjectNameChange(QString newName)
{
    arrayOfProjects.value(currentProjectNumber)->slotChangeProjectName(newName);
}

void ProjectManager::slotSelectProjectBeetwenProjectWindows(int prjNum)
{
    selectCurrentProjectByNumber(prjNum);
}

void ProjectManager::slotRetreiveDataForSelectedItemInProjectsTree()
{
}

void ProjectManager::slotCreateProject()
{
    Project *newPrj = createNewProject(false);
}

void ProjectManager::slotOpenProject()
{
    Project *openedPrj = createNewProject(true);

    openedPrj->slotLoad();

    if (openedPrj->getProjectName().isEmpty())
    {
        arrayOfProjects.remove(
                    arrayOfProjects.key(openedPrj)
                    );

        delete openedPrj;
    }
    else
    {
        //emit sigToCreateEmptyProjectTree(arrayOfProjects.key(openedPrj));
        emit sigToFillProjectTreeByDataFromProject(arrayOfProjects.key(openedPrj));
    }
}

void ProjectManager::slotSelectProjectByNumber(int)
{

}

void ProjectManager::slotSaveCurrentProject()
{
    if (arrayOfProjects.isEmpty())
    {
        QMessageBox::critical(NULL,
                              trUtf8("Отсутствуют проекты"),
                              trUtf8("Необходимо создать/открыть хотя бы один проект"));
        return;
    }

    if (arrayOfProjects.value(currentProjectNumber)->getStringFileNameOfProject().isEmpty())
    {
        arrayOfProjects.value(currentProjectNumber)->slotSaveAs();
    }
    else
    {
        arrayOfProjects.value(currentProjectNumber)->slotSave();
    }
}

void ProjectManager::slotSaveCurrentProjectAs()
{
    if (arrayOfProjects.isEmpty())
    {
        QMessageBox::critical(NULL,
                              trUtf8("Отсутствуют проекты"),
                              trUtf8("Необходимо создать/открыть хотя бы один проект"));
        return;
    }

    arrayOfProjects.value(currentProjectNumber)->slotSaveAs();
}

void ProjectManager::slotSaveAllProjects()
{
    if (arrayOfProjects.isEmpty())
    {
        QMessageBox::critical(NULL,
                              trUtf8("Отсутствуют проекты"),
                              trUtf8("Необходимо создать/открыть хотя бы один проект"));
        return;
    }

    bool newProjectsExist = false;

    for (int i_prj = 0; i_prj < arrayOfProjects.size(); ++i_prj)
    {
        int prjID = arrayOfProjects.keys().at(i_prj);

        if (arrayOfProjects.value(prjID)->getStringFileNameOfProject().isEmpty())
        {
            newProjectsExist = true;
            break;
        }
    }

    if (newProjectsExist)
    {
        QMessageBox::critical(NULL,
                              trUtf8("Имеются проекты, созданные в памяти"),
                              trUtf8("Необходимо сохранить проекты, существующие только в памяти (созданные, но ни разу не сохраненные)."));
        return;
    }

    QProgressBar progressBar;

    progressBar.setMinimum(0);
    progressBar.setMaximum(arrayOfProjects.size());
    progressBar.setWindowTitle(trUtf8("Сохранение всех открытых проектов"));

    progressBar.show();

    for (int i_prj2save = 0; i_prj2save < arrayOfProjects.size(); ++i_prj2save)
    {
        arrayOfProjects.values().at(i_prj2save)->slotSave();

        progressBar.setValue(i_prj2save);
    }

    progressBar.hide();
}

void ProjectManager::slotCLoseCurrentProject()
{
    Project *project = arrayOfProjects.value(currentProjectNumber);

    arrayOfProjects.remove(currentProjectNumber);

    if (arrayOfProjects.isEmpty()) currentProjectNumber = 0;
    else
    {
        currentProjectNumber = arrayOfProjects.keys().at(0);
    }

//    project = new Project;

    delete project;
}

void ProjectManager::slotCloseAllProjects()
{

}
