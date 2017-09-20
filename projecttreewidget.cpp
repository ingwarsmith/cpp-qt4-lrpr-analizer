#include "projecttreewidget.h"

ProjectTreeWidget::ProjectTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    this->setHeaderHidden(true);
    this->setColumnCount(1);
    this->setItemsExpandable(true);
    this->setExpandsOnDoubleClick(false);

    projectItems = QMap<int, ProjectRootCollection* >();

    previousItem = new BaseItem;

    connect(this,
            SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            SLOT(slotDoubleClickOnItem(QTreeWidgetItem*)));
    connect(this,
            SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            SLOT(slotClickOnProjectsTreeWidgetItem(QTreeWidgetItem*,QTreeWidgetItem*)));
}

ProjectRootCollection *ProjectTreeWidget::retProjectItemByProjectID(int projID)
{
    return projectItems.value(projID);
}

void ProjectTreeWidget::slotCreateEmptyProjectItems(int prjID)
{
    ProjectRootCollection *newEmptyProjElement = new ProjectRootCollection(prjID);

    projectItems.insert(prjID, newEmptyProjElement);

    this->insertTopLevelItem(
                this->topLevelItemCount(),
                newEmptyProjElement
                );
}

void ProjectTreeWidget::slotDoubleClickOnItem(QTreeWidgetItem *itemDoubleClicked)
{
    BaseItem *bi = static_cast<BaseItem* >(itemDoubleClicked);

    BaseItem::CustomItemType tp = bi->customItemType();

    if (tp == BaseItem::tCollectionItem || tp == BaseItem::tPartitionItem)
    {
        if (bi->isExpanded())
            bi->setExpanded(false);
        else
            bi->setExpanded(true);
    }
    else if (tp == BaseItem::tFieldItem)
    {
        FieldItem *fi = qobject_cast<FieldItem* >(bi);

        FieldItem::FieldType tp_f = fi->fieldItemType();

        if (tp_f == FieldItem::ftNewStratum)
        {
            int currentProjectID = fi->projectID;

            BaseItem *bparent = static_cast<BaseItem* >(fi->QTreeWidgetItem::parent());

            bool isInProjectStratumsPartition = false;

            if (bparent->text(0) == trUtf8("Пласты проекта"))
            {
                isInProjectStratumsPartition = true;
            }

            emit sigCreateStratumByDblClickOnAdd(currentProjectID,
                                                 isInProjectStratumsPartition,
                                                 fi);
        }
        else if (tp_f == FieldItem::ftNewWell)
        {
            int currentProjectID = fi->projectID;

            BaseItem *bparent = static_cast<BaseItem* >(fi->QTreeWidgetItem::parent());

            bool isInWellsOfProject = false, isInWellsOfTrap = false;

            if (bparent->text(0) == trUtf8("Скважины проекта"))
            {
                isInWellsOfProject = true;
            }

            if (bparent->text(0) == trUtf8("Скважины"))
            {
                isInWellsOfTrap = true;
            }

            emit sigCreateWellByDblClickOnAdd(currentProjectID,
                                              isInWellsOfProject,
                                              isInWellsOfTrap,
                                              fi);
        }
        else if (tp_f == FieldItem::ftNewStructure)
        {
            int currentProjectID = fi->projectID;

            emit sigCreateStructureByDblClickOnAdd(currentProjectID, fi);
        }
        else if (tp_f == FieldItem::ftNewTrap)
        {
            int currentProjectID = fi->projectID;

            BaseItem *bparent2 = static_cast<BaseItem* >(fi->QTreeWidgetItem::parent()->parent());

            StructureCollection *sc = qobject_cast<StructureCollection* >(bparent2);

            int currentStructID = sc->ID_structure->ID;

            emit sigCreateTrapByDblClickOnAdd(currentProjectID, currentStructID, fi);
        }
    }
}

void ProjectTreeWidget::slotClickOnProjectsTreeWidgetItem(QTreeWidgetItem *itemClicked, QTreeWidgetItem *itemPrevious)
{
    if (this->children().count() == 0) return;

    if (!itemClicked) return; // на случай закрытия проекта и удаления его виджета

    previousItem = static_cast<BaseItem* >(itemPrevious); // на всякий случай

    BaseItem *biClicked = static_cast<BaseItem* >(itemClicked);

    BaseItem::CustomItemType tp_bi = biClicked->customItemType();

    if (tp_bi == BaseItem::tPartitionItem)
    {
        emit sigShowEmptyWidget();
    }
    else if (tp_bi == BaseItem::tFieldItem)
    {
        int currentProjectID = biClicked->projectID;

        FieldItem *fi_ = qobject_cast<FieldItem* >(biClicked);

        if (fi_->fieldItemType() == FieldItem::ftStratum)
        {
            StratumField *sfi = qobject_cast<StratumField* >(fi_);

            int currentStratumID = sfi->ID;

            emit sigShowStratumInfoThroughClickOnPartition(currentProjectID, currentStratumID);
        }
        else /*if (fi_->fieldItemType() == FieldItem::ft)*/
        {
//            emit sigSelectProjectIDThroughClickOnPartition(currentProjectID);
            emit sigShowEmptyWidget();
        }
    }
    else if (tp_bi == BaseItem::tCollectionItem)
    {
        int currentProjectID = biClicked->projectID;

        CollectionItem *ci_ = qobject_cast<CollectionItem* >(biClicked);

        if (ci_->collectionItemType() == CollectionItem::clStructure)
        {
            StructureCollection *sc = qobject_cast<StructureCollection* >(ci_);

            int currentStructureID = sc->ID_structure->ID;

            emit sigShowStructureInfoThroughClickOnPartition(currentProjectID, currentStructureID);
        }
        else if (ci_->collectionItemType() == CollectionItem::clTrap)
        {
            TrapCollection *tc = qobject_cast<TrapCollection* >(ci_);

            int currentTrapID = tc->ID_trap->ID;

            emit sigShowTrapInfoThroughClickOnPartition(currentProjectID, currentTrapID, tc);
        }
        else if (ci_->collectionItemType() == CollectionItem::clWell)
        {
            WellCollection *wc = qobject_cast<WellCollection* >(ci_);

            int currentWellID = wc->ID_well->ID;

            emit sigShowWellpInfoThroughClickOnPartition(currentProjectID,
                                                         currentWellID//,
                                                         //wc
                                                         );
        }
        else if (ci_->collectionItemType() == CollectionItem::clProjectRoot)
        {
            emit sigSelectProjectIDThroughClickOnPartition(currentProjectID);
        }

    }
    else emit sigShowEmptyWidget();
}

