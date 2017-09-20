#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QTreeWidget>
#include <QMessageBox>
#include <QMap>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QDir>

#include "structure.h"
#include "well.h"
#include "realization.h"

class Project : public QObject
{
    Q_OBJECT

public:
    explicit Project(QObject *parent = 0);
    ~Project();

    int  getThisProjectNumb();
    void setThisProjectNumb(int nmb);

    QString getDescription();
    void setDescription(QString newDescription);

    QString getProjectName();
    void setProjectName(QString newName);

    QString getStringFileNameOfProject();

    bool isSavedProjectWithoutUnsavedChanges();
    void setUnSavedChangesExist();

    QMap<int, Structure* >    structuresInProject;
    QMap<int, Stratum* >      stratumsNamesByNumber;
    QMap<int, Well* >         wellsInProject;
    QMap<int, Trap* >         trapsInProjectCommonMap;
    QMap<int, Realization* >  realizationsInProject;

    QVector<int>        retWellsIDsOnStructure(int structureID);

private:
    QString                     m_projectName;

    QString                     m_stringFileName;
    int                         m_thisProjectNumber;
    QString                     m_Description;
    QFile                       *m_project_header_file;
    bool                        m_notNewProjectInMemory;
    bool                        m_unsavedChangesExist;

    void    loadStratums();
    void    loadTraps();
    void    loadStructures();
    void    loadWells();
    void    loadRealizations();

    void    saveStratums();
    void    saveTraps();
    void    saveStructures();
    void    saveWells();
    void    saveRealizations();

signals:
    void    changeWindowTitle(const QString&);
    void    toCreateEmptyProjectTree(int);

public slots:
    void    slotLoad();
    void    slotImportFromTextFileWithDelimeters();
    void    slotSave();
    void    slotSaveAs();
    void    slotClose();

    void    slotChangeProjectName(QString newProjectName);

private slots:
};

#endif // PROJECT_H
