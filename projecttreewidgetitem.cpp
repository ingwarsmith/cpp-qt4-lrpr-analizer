#include "projecttreewidgetitem.h"

BaseItem::BaseItem(QWidget *parent/*, int type*/, int projectIdentificator) : QWidget(parent),
    QTreeWidgetItem(QTreeWidgetItem::UserType)
{
    name = baseInName = QString();

    projectID = projectIdentificator;
}

PartitionItem::PartitionItem(QString nameOfPartition, QWidget *parentItem, int projectIdentificator)
    : BaseItem(parentItem, projectIdentificator)
{
    Q_UNUSED(nameOfPartition);

    typeOfItem = tPartitionItem;

    setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

    QFont fnt = ::QTreeWidgetItem::font(0);
    fnt.setItalic(true);
    int fSz = fnt.pointSize();
    fnt.setPointSize(fSz+2);
    ::QTreeWidgetItem::setFont(0, fnt);
}


ProjectRootCollection::ProjectRootCollection(int idProject, QString nameOfCollection, QWidget *prnt)
    : CollectionItem(nameOfCollection, prnt, idProject)
{
    typeOfCollection = clProjectRoot;

    ID_project = //new IdField(idProject, this);
            new IdField(idProject, idProject, this);

    name = trUtf8("Проект: ...");

    baseInName = trUtf8("Проект: ");

    if (!nameOfCollection.isEmpty())
    {
        name = baseInName + nameOfCollection;
    }

    QFont fnt = ::QTreeWidgetItem::font(0);
    fnt.setBold(true);
    ::QTreeWidgetItem::setFont(0, fnt);

    setTextColor(0, Qt::darkCyan);
    setText(0, name);

    Begin_decsription = new DescriptionField(idProject, QString(), this);
    this->insertChild(0, Begin_decsription);

    this->insertChild(1, ID_project);

    projectPart_Structures = //new StructuresPartition(this);
            new StructuresPartition(idProject, this);
    this->insertChild(2, projectPart_Structures);

    projectPart_Wells_ReadOnly_List = new WellsPartition(idProject, this, trUtf8("Скважины проекта (список)"));
    projectPart_Wells_ReadOnly_List->removeLastElement();
    this->insertChild(3, projectPart_Wells_ReadOnly_List);

    projectPart_Stratums = new StratumsPartition(idProject, this, trUtf8("Пласты проекта"));
    this->insertChild(4, projectPart_Stratums);

    projectPart_Realizations = new EstimationResultRealizations(idProject, this);
    this->insertChild(5, projectPart_Realizations);
}


StructuresPartition::StructuresPartition(int idPrj, QWidget *prnt, QString nameOfPartition)
    : PartitionItem(nameOfPartition, prnt, idPrj)
{
    typeOfPartition = tpStructures;

    name = nameOfPartition;

    setTextColor(0, Qt::darkGreen);
    ::QTreeWidgetItem::setText(0, name);

    lastElementAddOrCreate = new CreateOrAddStructureField(idPrj, this);
    this->insertChild(this->childCount(), lastElementAddOrCreate);
}

void StructuresPartition::addSubElement(StructureCollection *subElement)
{
    if (this->childCount() == 0)
    {
        this->insertChild(0, subElement);
    }
    else
    {
        int lastElementIndex = childCount()-1;

        this->insertChild(lastElementIndex, subElement);
    }
}

void StructuresPartition::insertSubElement(StructureCollection *subElement, int position)
{
    this->insertChild(position, subElement);
}

StructureCollection *StructuresPartition::retStructureCollectionElementByStructureID(int IDstructure)
{
    StructureCollection *sc = new StructureCollection(IDstructure,
                                                      this->projectID,
                                                      QString(),
                                                      this);

    for (int i = 0; i < this->childCount(); ++i)
    {
        BaseItem *childItem = static_cast<BaseItem* >(this->child(i));

        if (childItem->customItemType() == BaseItem::tCollectionItem) // значит - коллекция (структур)
        {
            StructureCollection *sc2 = qobject_cast<StructureCollection* >(childItem);
            //break;

            if (sc2->ID_structure->ID == IDstructure)
            {
                return sc2;
            }
        }
    }

    return sc;
}

TrapCollection *StructuresPartition::retTrapCollectionSubSubElementByTrapID(int IDtrap)
{
    TrapCollection *tc = new TrapCollection(IDtrap,
                                            0,
                                            QString(),
                                            this);

    for (int i = 0; i < this->childCount(); ++i)
    {
        BaseItem *childItem = static_cast<BaseItem* >(this->child(i));

        if (childItem->customItemType() == BaseItem::tCollectionItem) // значит - коллекция (структур)
        {
            StructureCollection *sc = qobject_cast<StructureCollection* >(childItem);

            TrapCollection *tc2 = sc->structurePart_Traps->retTrapCollectionElementByTrapID(IDtrap);

            if (tc2->projectID == 0)
                continue;
            else
                return tc2;
        }
    }

    return tc;
}


TrapsPartition::TrapsPartition(int idPrj, QWidget *prnt, QString nameOfPartition)
    : PartitionItem(nameOfPartition, prnt, idPrj)
{
    typeOfPartition = tpTraps;

    name = nameOfPartition;

    setTextColor(0, Qt::darkRed);
    ::QTreeWidgetItem::setText(0, name);

    lastElementAddOrCreate = new CreateOrAddTrapField(idPrj, this);
    this->insertChild(this->childCount(), lastElementAddOrCreate);
}

void TrapsPartition::addSubElement(TrapCollection *subElement)
{
    if (this->childCount() == 0)
    {
        this->insertChild(0, subElement);
    }
    else
    {
        int lastElementIndex = childCount()-1;

        this->insertChild(lastElementIndex, subElement);
    }
}

void TrapsPartition::insertSubElement(TrapCollection *subElement, int position)
{
    this->insertChild(position, subElement);
}

TrapCollection *TrapsPartition::retTrapCollectionElementByTrapID(int IDtrap)
{
    TrapCollection *tc = new TrapCollection(IDtrap,
                                            0,
                                            QString(),
                                            this);

    for (int i = 0; i < this->childCount(); ++i)
    {
        BaseItem *childItem = static_cast<BaseItem* >(this->child(i));

        if (childItem->customItemType() == BaseItem::tCollectionItem) // значит - коллекция (ловушек)
        {
            TrapCollection *tc2 = qobject_cast<TrapCollection* >(childItem);

            if (tc2->ID_trap->ID == IDtrap)
            {
                return tc2;
            }
        }
    }

    return tc;
}


WellsPartition::WellsPartition(int idPrj, QWidget *prnt, QString nameOfPartition)
    : PartitionItem(nameOfPartition, prnt, idPrj)
{
    lastElementIsRemoved = false;

    typeOfPartition = tpWells;

    name = nameOfPartition;

    setTextColor(0, Qt::darkMagenta);
    ::QTreeWidgetItem::setText(0, name);

    lastElementAddOrCreate = new CreateOrAddWellField(idPrj, this);
    this->insertChild(this->childCount(), lastElementAddOrCreate);
}

void WellsPartition::addSubElement(WellCollection *subElement)
{
    if (this->childCount() == 0)
    {
        this->insertChild(0, subElement);
    }
    else if (lastElementIsRemoved)
    {
        int lastElementIndex = childCount();

        this->insertChild(lastElementIndex, subElement);
    }
    else
    {
        int lastElementIndex = childCount()-1;

        this->insertChild(lastElementIndex, subElement);
    }
}

void WellsPartition::insertSubElement(WellCollection *subElement, int position)
{
    this->insertChild(position, subElement);
}

void WellsPartition::setLastElementDisabled(bool yes)
{
    lastElementAddOrCreate->setEnabled(!yes);
}

void WellsPartition::removeLastElement()
{
    if (!lastElementIsRemoved)
    {
        lastElementIsRemoved = true;

        this->removeChild(lastElementAddOrCreate);
    }
}

WellCollection *WellsPartition::retWellCollectionElementByWellID(int IDwell)
{
    WellCollection *wc = new WellCollection(IDwell, 0);

    for (int ichild = 0; ichild < this->childCount(); ++ichild)
    {
        if (!qobject_cast<WellCollection* >(static_cast<BaseItem* >(this->child(ichild)))) return wc;

        wc = qobject_cast<WellCollection* >(static_cast<BaseItem* >(this->child(ichild)));

        if (wc->ID_well->ID == IDwell)
        {
            break;
        }
    }

    return wc;
}


StratumsPartition::StratumsPartition(int idPrj, QWidget *prnt, QString nameOfCollection)
    : PartitionItem(nameOfCollection, prnt, idPrj)
{
    lastElementIsRemoved = false;

    typeOfPartition = tpStratums;

    name = nameOfCollection;

    setTextColor(0, Qt::darkRed);
    ::QTreeWidgetItem::setText(0, name);

    lastElementAddOrCreate = new CreateOrAddStratumField(idPrj, this);
    this->insertChild(this->childCount(), lastElementAddOrCreate);
}

void StratumsPartition::addSubElement(StratumField *subElement)
{
    if (this->childCount() == 0)
    {
        this->insertChild(0, subElement);
    }
    else if (lastElementIsRemoved)
    {
        int lastElementIndex = childCount();

        this->insertChild(lastElementIndex, subElement);
    }
    else
    {
        int lastElementIndex = childCount()-1;

        this->insertChild(lastElementIndex, subElement);
    }
}

void StratumsPartition::insertSubElement(StratumField *subElement, int position)
{
    this->insertChild(position, subElement);
}

void StratumsPartition::setLastElementDisabled(bool yes)
{
    lastElementAddOrCreate->setEnabled(!yes);
    //    lastElementAddOrCreate->QWidget::setVisible(!yes);
}

void StratumsPartition::removeLastElement()
{
    if (!lastElementIsRemoved)
    {
        lastElementIsRemoved = true;

        this->removeChild(lastElementAddOrCreate);
    }
}

StratumField *StratumsPartition::retStratumFieldElementByStratumID(int IDstratum)
{
    StratumField *sf = new StratumField(IDstratum, 0);

    for (int ichild = 0; ichild < this->childCount(); ++ichild)
    {
        if (!qobject_cast<StratumField* >(static_cast<BaseItem* >(this->child(ichild)))) return sf;

        sf = qobject_cast<StratumField* >(static_cast<BaseItem* >(this->child(ichild)));

        if (sf->ID == IDstratum)
        {
            break;
        }
    }

    return sf;
}


EstimationResultRealizations::EstimationResultRealizations(int idPrj, QWidget *prnt, QString nameOfCollection)
    : PartitionItem(nameOfCollection, prnt, idPrj)
{
    typeOfPartition = tpEstimationResultRealizations;

    name = nameOfCollection;

    setTextColor(0, Qt::blue);
    ::QTreeWidgetItem::setText(0, name);

    lastElementAddOrCreate = new CreateOrAddRealizatonField(idPrj, this);
    this->insertChild(this->childCount(), lastElementAddOrCreate);
}


CollectionItem::CollectionItem(QString nameOfCollection, QWidget *parentItem, int projectIdentificator)
    : BaseItem(parentItem, projectIdentificator)
{
    //Q_UNUSED(parentItem);

    typeOfItem = tCollectionItem;

    setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

    if (!nameOfCollection.isEmpty())
    {
        name = baseInName + nameOfCollection;
        setText(0, name);
    }

    QFont fnt = ::QTreeWidgetItem::font(0);
    int fSz = fnt.pointSize();
    fnt.setPointSize(fSz+2);
    ::QTreeWidgetItem::setFont(0, fnt);

//    Q_UNUSED(nameOfCollection);

    actionRemove = new QAction(trUtf8("Удалить элемент"), this);

    connect(actionRemove,
            SIGNAL(triggered(bool)),
            this,
            SLOT(slotForRemovingOfCollection()));

    setContextMenuPolicy(Qt::ActionsContextMenu);
    this->addAction(actionRemove);
}

CollectionItem::~CollectionItem()
{
    delete actionRemove;
}

void CollectionItem::slotChangeName(QString newName)
{
    if (newName == QString()) newName = QString("...");
    name = baseInName + newName;
//    this->setText(0, name);
    this->QTreeWidgetItem::setText(0, name);
}

void CollectionItem::slotForRemovingOfCollection()
{
}


//ElementOfCollectionOrPartition::ElementOfCollectionOrPartition(QWidget *parentItem)
//    : QTreeWidgetItem(QTreeWidgetItem::UserType)
//{
//    Q_UNUSED(parentItem);

//    setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
//}


FieldItem::FieldItem(QWidget *parentItem, int projectIdentificator)
    : BaseItem(parentItem, projectIdentificator)
{
//    Q_UNUSED(parentItem);

    setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);

    typeOfItem = tFieldItem;
}

void FieldItem::slotSetItemText(QString newText)
{
    name = newText;
//    QString txt =
    setText(0, newText);
}

void FieldItem::slotSetItemToolTipText(QString newToolTipText)
{
    ::QTreeWidgetItem::setToolTip(0, newToolTipText);
}


StructureCollection::StructureCollection(int idStructure, int idProject, QString nameOfCollection, QWidget *prnt)
    : CollectionItem(nameOfCollection, prnt, idProject)
{
    typeOfCollection = clStructure;

    Begin_decsription = new DescriptionField(idProject, QString(), this);
    this->insertChild(0, Begin_decsription);

    ID_structure = //new IdField(idStructure, this);
            new IdField(idStructure, idProject, this);
    this->insertChild(1, ID_structure);

    structurePart_Traps = //new TrapsPartition(this);
            new TrapsPartition(idProject, this);
    this->insertChild(2, structurePart_Traps);

    structurePart_Wells_ReadOnly = //new WellsPartition(this, trUtf8("Скважины на структуре"));
            new WellsPartition(idProject, this, trUtf8("Скважины на структуре"));
    this->insertChild(3, structurePart_Wells_ReadOnly);
//    structurePart_Wells_ReadOnly->setLastElementDisabled(true);
    structurePart_Wells_ReadOnly->removeLastElement();

    structurePart_Stratums_ReadOnly = //new StratumsPartition(this, trUtf8("Пласты структуры"));
            new StratumsPartition(idProject, this, trUtf8("Пласты структуры"));
    this->insertChild(4, structurePart_Stratums_ReadOnly);
//    structurePart_Stratums_ReadOnly->setLastElementDisabled(true);
    structurePart_Stratums_ReadOnly->removeLastElement();
}

bool StructureCollection::isStructureContainWellByID(int wellID)
{
    for (int i = 0; structurePart_Wells_ReadOnly->childCount(); ++i)
    {
        WellCollection *i_well = qobject_cast<WellCollection* >(
                    static_cast<BaseItem* >(
                        structurePart_Wells_ReadOnly->child(i)
                        )
                    );

        if (i_well->ID_well->ID == wellID) return true;
    }

    return false;
}

bool StructureCollection::isStructureContainStratumByID(int stratumID)
{
    for (int i = 0; structurePart_Stratums_ReadOnly->childCount(); ++i)
    {
        StratumField *i_stratum = qobject_cast<StratumField* >(
                    static_cast<BaseItem* >(
                        structurePart_Stratums_ReadOnly->child(i)
                        )
                    );

        if (i_stratum->ID == stratumID) return true;
    }

    return false;
}


TrapCollection::TrapCollection(int idTrap, int idProject, QString nameOfCollection, QWidget *prnt)
    : CollectionItem(nameOfCollection, prnt, idProject)
{
    typeOfCollection = clTrap;

    Begin_decsription = new DescriptionField(idProject, QString(), this);
    this->insertChild(0, Begin_decsription);

    ID_trap = new IdField(idTrap, idProject, this);
    this->insertChild(1, ID_trap);

    trapPart_Wells = new WellsPartition(idProject, this);
    this->insertChild(2, trapPart_Wells);

    StratumOfTrap = new StratumField(-1, idProject, QString(), this);
    this->insertChild(3, StratumOfTrap);
}

void TrapCollection::setStratumFieldID(int ID)
{
    StratumOfTrap->slotChangeID(ID);
}

void TrapCollection::setStratumFieldNameOfStratum(QString nameStrat)
{
//    StratumOfTrap->nameOfStratum = nameStrat;
    StratumOfTrap->slotChangeName(nameStrat);
}

StructureCollection *TrapCollection::retStructureCollectionElementByTrapCollItem()
{
    QTreeWidgetItem *twi = this->QTreeWidgetItem::parent()->parent();

    BaseItem *bi = static_cast<BaseItem* >(twi);

    StructureCollection *sc = qobject_cast<StructureCollection* >(bi);

    return sc;
}

WellCollection::WellCollection(int idWell, int idProject, QString nameOfCollection, QWidget *prnt)
    : CollectionItem(nameOfCollection, prnt, idProject)
{
    typeOfCollection = clWell;

    elementRisk = new WellNegativeResultRiskEstimation(idProject, 0.3, this);

    Begin_decsription = new DescriptionField(idProject, QString(), this);
    this->insertChild(0, Begin_decsription);

    ID_well = new IdField(idWell, idProject, this);

    this->insertChild(1, ID_well);

    this->insertChild(2, elementRisk);
}

WellCollection::WellCollection(WellCollection *other)
{
    typeOfCollection = clWell;

    elementRisk = new WellNegativeResultRiskEstimation(other->projectID, 0.3, this);

    elementRisk->baseInName = other->elementRisk->baseInName;
    elementRisk->name = other->elementRisk->name;
    elementRisk->risk = other->elementRisk->risk;

    Begin_decsription = new DescriptionField(other->projectID, QString(), this);;
    Begin_decsription->baseInName = other->Begin_decsription->baseInName;
    Begin_decsription->name = other->Begin_decsription->name;
    Begin_decsription->text = other->Begin_decsription->text;

    this->insertChild(0, Begin_decsription);

    ID_well = new IdField(other->ID_well->ID, other->projectID, this);

    this->insertChild(1, ID_well);

    this->insertChild(2, elementRisk);

    this->slotChangeName(other->name);
}

void WellCollection::setWellFieldID(int ID)
{
    ID_well->slotChangeID(ID);
}


DescriptionField::DescriptionField(int idProject, QString descriptionTextInCreating, QWidget *prnt)
    : FieldItem(prnt, idProject)
{
//    Q_UNUSED(prnt);

    typeOfField = FieldItem::ftDesription;

    text = descriptionTextInCreating;

    slotSetItemText(trUtf8("Описание"));
    setTextColor(0, Qt::gray);
}


WellNegativeResultRiskEstimation::WellNegativeResultRiskEstimation(int idProject, double riskValue, QWidget *prnt)
    : FieldItem(prnt, idProject)
{
//    Q_UNUSED(prnt);

    typeOfField = FieldItem::ftWellNegativeResultRiskEstimation;

    risk = riskValue;

    txtToolTipBase = trUtf8("Риск отрицательного результата бурения по техническим и иным причинам");

    setText(0, trUtf8("Риск : ") + QString::number(risk));
    setTextColor(0, Qt::red);
    ::QTreeWidgetItem::setToolTip(0, txtToolTipBase);
}

void WellNegativeResultRiskEstimation::slotSetRiskItemText(double riskNew)
{
    risk = riskNew;

    setText(0, trUtf8("Риск : ") + QString::number(risk));
}


CreateOrAddWellField::CreateOrAddWellField(int idProject, QWidget *prnt)
    : FieldItem(prnt, idProject)
{
//    Q_UNUSED(prnt);

    typeOfField = FieldItem::ftNewWell;

    this->slotSetItemText(trUtf8("*** создать скважину ***"));

    ::QTreeWidgetItem::setToolTip(0, trUtf8("Создать/добавить скважину"));
}


CreateOrAddStructureField::CreateOrAddStructureField(int idProject, QWidget *prnt)
    : FieldItem(prnt, idProject)
{
//    Q_UNUSED(prnt);

    typeOfField = FieldItem::ftNewStructure;

    this->slotSetItemText(trUtf8("*** создать структуру ***"));

    ::QTreeWidgetItem::setToolTip(0, trUtf8("Создать/добавить структуру"));
}


CreateOrAddStratumField::CreateOrAddStratumField(int idProject, QWidget *prnt)
    : FieldItem(prnt, idProject)
{
//    Q_UNUSED(prnt);

    typeOfField = FieldItem::ftNewStratum;

    this->slotSetItemText(trUtf8("*** создать пласт ***"));

    ::QTreeWidgetItem::setToolTip(0, trUtf8("Создать/добавить пласт"));
}


CreateOrAddTrapField::CreateOrAddTrapField(int idProject, QWidget *prnt)
    : FieldItem(prnt, idProject)
{
//    Q_UNUSED(prnt);

    typeOfField = FieldItem::ftNewTrap;

    this->slotSetItemText(trUtf8("*** создать ловушку ***"));

    ::QTreeWidgetItem::setToolTip(0, trUtf8("Создать/добавить ловушку"));
}


IdField::IdField(int ID, int idProject, QWidget *prnt)
    : FieldItem(prnt, idProject)
{
//    Q_UNUSED(prnt);

    typeOfField = FieldItem::ftID;

    this->ID = ID;

    this->slotSetItemText(QString("ID: ") + QString::number(this->ID));

    ::QTreeWidgetItem::setToolTip(0, trUtf8("Уникальный идентификатор объекта данных, сопоставляемого с элементом"));
}

void IdField::slotChangeID(int newIdValue)
{
    ID = newIdValue;
}

StratumField::StratumField(int ID, int idProject, QString nmSTratum, QWidget *prnt)
    : IdField(ID, idProject, prnt)
{
    //Q_UNUSED(prnt);

    typeOfField = FieldItem::ftStratum;

    //this->slotSetItemText();

    //nameOfStratum = nmSTratum;
    slotChangeName(nmSTratum);

    QFont fnt = ::QTreeWidgetItem::font(0);
//    fnt.setItalic(true);
    int fSz = fnt.pointSize();
    fnt.setPointSize(fSz+2);
    ::QTreeWidgetItem::setFont(0, fnt);

    ::QTreeWidgetItem::setToolTip(0, QString());
}

void StratumField::slotChangeName(QString nm)
{
    nameOfStratum = nm;

    slotSetItemText(nameOfStratum);
}

void StratumField::slotSetItemText(QString newText)
{
    if (newText.isEmpty())
    {
        name = trUtf8("Пласт: ...");
    }
    else
    {
        name = trUtf8("Пласт: ") + newText;
    }

    QString txt = name + QString(", ID: ") + QString::number(this->ID);
    QTreeWidgetItem::setText(0, txt);
}


CreateOrAddRealizatonField::CreateOrAddRealizatonField(int idProject, QWidget *prnt)
    : FieldItem(prnt, idProject)
{
    typeOfField = FieldItem::ftAddRealization;

    this->slotSetItemText(trUtf8("*** создать реализацию ***"));

    ::QTreeWidgetItem::setToolTip(0, trUtf8("Создать/добавить реализацию расчета"));
}


RealizationOfEstimationResultCollection::RealizationOfEstimationResultCollection(int idRealization, int idProject, QString nameOfRealizationCollection, QWidget *prnt)
{
    typeOfCollection = clRealization;

    Begin_description = new DescriptionField(idProject, QString(), this);
    this->insertChild(0, Begin_description);

    ID_realization = new IdField(idRealization, idProject, this);
    this->insertChild(1, ID_realization);
}

void RealizationOfEstimationResultCollection::setRealizationFieldID(int ID)
{
    ID_realization->slotChangeID(ID);
}


CreateOrAddCustomRiskThresholdField::CreateOrAddCustomRiskThresholdField(int idProject, QWidget *prnt)
    : FieldItem(prnt, idProject)
{
    typeOfField = FieldItem::ftAddNewCustomRiskThreshold;

    this->slotSetItemText(trUtf8("*** создать новое пороговое значение ***"));

    ::QTreeWidgetItem::setToolTip(0, trUtf8("Создать/добавить пороговое значение риска"));
}


PRMSforResourcesEstimationCollection::PRMSforResourcesEstimationCollection(int idRealization, int idProject, QWidget *prnt)
{
    typeOfCollection = clPRMSresources;

    Begin_description = new DescriptionField(idProject, QString(), this);
    this->insertChild(0, Begin_description);

    IDofRealization = idRealization;
}
