#ifndef PROJECTTREEWIDGET_H
#define PROJECTTREEWIDGET_H

#include <QMap>
#include <QVector>
#include <QTreeWidget>

#include "projecttreewidgetitem.h"

class ProjectTreeWidget : public QTreeWidget
{
    Q_OBJECT

//    struct ForProject
//    {
//        int ProjectID;
//        QVector<int>
//    };

public:
    explicit ProjectTreeWidget(QWidget *parent = 0);

    ProjectRootCollection *retProjectItemByProjectID(int projID);

private:
    QMap<int, ProjectRootCollection* > projectItems;
    BaseItem                           *previousItem;

signals:
    void    sigCreateStratumByDblClickOnAdd(int projectID,
                                            bool isInProjectStratumsPartition,
                                            BaseItem *itemOfAdd);
    void    sigCreateWellByDblClickOnAdd(int projectID,
                                         bool isInWellsOfProject,
                                         bool isInWellsOfTrap,
                                         BaseItem *itemOfAdd);
    void    sigCreateStructureByDblClickOnAdd(int projectID,
                                              BaseItem *itemOfAdd);
    void    sigCreateTrapByDblClickOnAdd(int projectID, int structureIDs, BaseItem *itemOfAdd);
    void    sigSelectProjectIDThroughClickOnPartition(int projectID);
    void    sigShowEmptyWidget();
    void    sigShowStratumInfoThroughClickOnPartition(int projectID, int stratumID);
    void    sigShowStructureInfoThroughClickOnPartition(int projectID, int structureID);
    void    sigShowTrapInfoThroughClickOnPartition(int projectID, int trapID, TrapCollection *tc);
    void    sigShowWellpInfoThroughClickOnPartition(int projectID, int wellID/*, WellCollection *wc*/);

public slots:
    void    slotCreateEmptyProjectItems(int prjID);

private slots:
    void    slotDoubleClickOnItem(QTreeWidgetItem *itemDoubleClicked);
    void    slotClickOnProjectsTreeWidgetItem(QTreeWidgetItem *itemClicked,
                                              QTreeWidgetItem *itemPrevious);
};

#endif // PROJECTTREEWIDGET_H
