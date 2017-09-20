#ifndef PROJECTTREEWIDGETITEM_H
#define PROJECTTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <QWidget>
#include <QAction>
#include <QPalette>
#include <QMessageBox>

class BaseItem : public QWidget, public QTreeWidgetItem
{
    Q_OBJECT
    Q_PROPERTY(CustomItemType customItemType READ customItemType)
    Q_ENUMS(CustomItemType)

public:
    enum    CustomItemType
    {        
        tPartitionItem = 2000,
        tCollectionItem = 3000,
        tFieldItem = 4000
    };

    BaseItem(QWidget *parent = 0, int projectIdentificator = 0/*, int type = UserType*/);

    CustomItemType    customItemType() const { return typeOfItem; }

    CustomItemType    typeOfItem;

    QString     name, baseInName;

    int         projectID;
};

class WellNegativeResultRiskEstimation;
class DescriptionField;
class CreateOrAddWellField;
class CreateOrAddStructureField;
class CreateOrAddStratumField;
class CreateOrAddTrapField;
class CreateOrAddRealizatonField;
class CreateOrAddCustomRiskThresholdField;
class CollectionItem;
class StructuresPartition;
class WellsPartition;
class StratumsPartition;
class TrapsPartition;
class IdField;
class StratumField;
class EstimationResultRealizations;

class PartitionItem : public BaseItem/*, public QTreeWidgetItem*/ /// Объединяет однородные элементы
{
    Q_OBJECT
    Q_PROPERTY(PartitionItemType partitionItemType READ partitionItemType)
    Q_ENUMS(PartitionItemType)

public:
    PartitionItem(QString nameOfPartition = QString(), QWidget *parentItem = 0, int projectIdentificator = 0
                  /*, int type = QTreeWidgetItem::UserType*/);

    enum    PartitionItemType
    {
        tpStructures,
        tpTraps,
        tpWells,
        tpStratums,
        tpEstimationResultRealizations,         // раздел реализаций расчета
        tpCustomRiskThresholdsInResources,      // раздел выбранных порогов риска при оценке ресурсов PRMS
        tp
    };

    PartitionItemType   partitionItemType() const { return typeOfPartition; }

    PartitionItemType   typeOfPartition;

//    void        addSubElement(BaseItem *subElement);
//    void        insertSubElement(BaseItem *subElement, int postition);
};

class CollectionItem : public BaseItem/*, public QTreeWidgetItem*/  /// Объединяет разнородные элементы
{
    Q_OBJECT
    Q_PROPERTY(CollectionType collectionItemType READ collectionItemType)
    Q_ENUMS(CollectionType)

public:
    CollectionItem(QString nameOfCollection = QString(), QWidget *parentItem = 0,  int projectIdentificator = 0);
    ~CollectionItem();

    enum    CollectionType
    {
        clProjectRoot,
        clStructure,
        clTrap,
        clWell,
        clStratum,
        clRealization,                      // колллекция реализации расчета
        clResourcesEstimationResult,        // результат оценки ресурсов (вероятностной)
        clProbableReservesEstimationResult, // результат оценки возможных запасов нефти
        clResourcesGeologicalOfOil,         // геологические ресурсы нефти
        clResourcesRecoverableOfOil,        // извлекаемые ресурсы нефти
        clReservesProbableGeologicalOfOil,  // возможные геологические запасы нефти
        clReservesProbableRecoverableOfOil, // возможные извлекаемые запасы нефти
        clPRMSresources,                    // вероятностная оценка ресурсов PRMS (в группе геол. или извл.)
        clPRMSprobableReserves,             // оценка возможных запасов PRMS (в группе геол. или извл.)
        clCustomRiskThreshold,              // собственный порог риска для для оценки ресурсов / возм. запасов PRMS (геол. или извл.)
        //clGraphOfIntegralCurve,             // график интегральной кривой для оценки ресурсов / возм. запасов PRMS (геол. или извл.)
        //clGraphOfDifferentialCurve,         // график дифференциальной кривой для оценки ресурсов / возм. запасов PRMS (геол. или извл.)
        clMorestProbableValueOfResources,   // наиболее вероятное значение для оценки ресурсов PRMS (геол. или извл.) с подпунктами Pmax и Q(Pmax)
        clMorestProbableValueOfReservesInPRMS,   // наиболее вероятное значение возм. запасов (геол. или извл.) PRMS+Pзал
        clMorestProbableValueOfReservesInCRRU,           // наиболее вероятное значение возм. запасов (геол. или извл.) ЦРН
        clMorestProbableValueOfReservesInCRRU_MethodNoTechnoRisks, // --||-- ЦРН (Pзал)
        clMorestProbableValueOfReservesInCRRU_MethodWithTechnoRisks, // --||-- ЦРН (Pзал+Pтех)
        clIntervalEstimationsOfReserves,        // интервальные оценки возможных запасов (геол. или извл.)
        clQuantil90percents,                    // доверительный интервал 90%
        clQuantil70percents,                    // доверительный интервал 70%
        clChoicedQuantilNNpercents,             // выбранный доверительный интервал NN%
        clMPVofReservesExtendedMethod,          // расширенный метод определения наиболее вероятного значения возможных запасов (геол. или извл.)
        clMPVofReservesExtMtd_WithTechnoRisks,   // --||-- с учетом технических рисков при бурении Rтех.
        clMPVofReservesExtMtd_NoTechnoRisks,     // --||-- без учета технических рисков Rтех.
        clPRMSprobableReserves_ExtMtd_wRisks,    // PRMS с применением Pусп = f(Pзал, Pтех.усп)
        clPRMSprobableReserves_ExtMtd_noRisks    // PRMS с применением Pусп = f(Pзал)
    };

    CollectionType collectionItemType() { return typeOfCollection; }

    CollectionType typeOfCollection;

//private:
    QAction *actionRemove;

public slots:
    void slotChangeName(QString newName);
    void slotForRemovingOfCollection();

//    protected void mouseDoubleClickEvent();
};

class FieldItem : public BaseItem/*, public QTreeWidgetItem*/ /// Поле с информацией по какому-либо признаку
{
    Q_OBJECT
    Q_PROPERTY(FieldType fieldItemType READ fieldItemType)
    Q_ENUMS(FieldType)

public:
    FieldItem(QWidget *parentItem = 0,  int projectIdentificator = 0);

    enum    FieldType
    {
        ftDesription = 1,
        ftWellNegativeResultRiskEstimation,
        ftNewWell,
        ftNewStructure,
        ftNewStratum,
        ftNewTrap,
        ftID,
        ftStratum,
        ftAddRealization,
        ftAddNewCustomRiskThreshold,
        //ftProbabilityByThreshold_Pxx,
        //ftAdjustingOfGraphOfIntegralCurve,
        //ftAdjustingOfGraphOfDifferentialCurve,
        ftMorestProbableValueOfResourcesOrReserves,
        ftMorestProbability_Value_Pmax,
        ftProbabilityOfAbsolutelyNegativeResult_akaFullRisk
    };

    FieldType typeOfField;

    FieldType fieldItemType() { return typeOfField; }

public slots:
    void slotSetItemText(QString newText);
    void slotSetItemToolTipText(QString newToolTipText);
};

class ProjectRootCollection : public CollectionItem /// Коллекция: Корень проекта в дереве проектов
{
    Q_OBJECT

public:
    ProjectRootCollection(int idProject, QString nameOfCollection = QString(), QWidget *prnt = 0);
    DescriptionField    *Begin_decsription;
    StructuresPartition *projectPart_Structures;
    WellsPartition      *projectPart_Wells_ReadOnly_List;
    StratumsPartition   *projectPart_Stratums;
    EstimationResultRealizations
                        *projectPart_Realizations;
    IdField             *ID_project;
};

class StructureCollection : public CollectionItem /// Коллекция: описание структуры
{
    Q_OBJECT

public:
    StructureCollection(int idStructure, int idProject, QString nameOfCollection = QString(), QWidget *prnt = 0);
    DescriptionField    *Begin_decsription;
    TrapsPartition      *structurePart_Traps;
    IdField             *ID_structure;
    WellsPartition      *structurePart_Wells_ReadOnly;
    StratumsPartition   *structurePart_Stratums_ReadOnly;

    bool    isStructureContainWellByID(int wellID);
    bool    isStructureContainStratumByID(int stratumID);
};

class TrapCollection : public CollectionItem /// Коллекция: описание ловушки
{
    Q_OBJECT

public:
    TrapCollection(int idTrap, int idProject, QString nameOfCollection = QString(), QWidget *prnt = 0);
    DescriptionField    *Begin_decsription;
    WellsPartition      *trapPart_Wells;
    IdField             *ID_trap;
    StratumField        *StratumOfTrap;

    void                setStratumFieldID(int ID);
    void                setStratumFieldNameOfStratum(QString nameStrat);

    StructureCollection     *retStructureCollectionElementByTrapCollItem();
};

class WellCollection : public CollectionItem /// Коллекция: описание скважины
{
    Q_OBJECT

public:
    WellCollection(int idWell, int idProject, QString nameOfCollection = QString(), QWidget *prnt = 0);
    WellCollection(WellCollection *other);
    DescriptionField    *Begin_decsription;
    IdField             *ID_well;

    WellNegativeResultRiskEstimation *elementRisk;

    void                setWellFieldID(int ID);
};

class PRMSforResourcesEstimationCollection : public CollectionItem
{
    Q_OBJECT

public:
    PRMSforResourcesEstimationCollection(int idRealization, int idProject,
                                         QWidget *prnt = 0);
    DescriptionField    *Begin_description;

    int                 IDofRealization;
};

class RealizationOfEstimationResultCollection : public CollectionItem /// Коллекция: описание реализации расчета
{
    Q_OBJECT

public:
    RealizationOfEstimationResultCollection(int idRealization, int idProject,
                                            QString nameOfRealizationCollection = QString(),
                                            QWidget *prnt = 0);
    DescriptionField    *Begin_description;
    IdField             *ID_realization;

    void                setRealizationFieldID(int ID);
};

class StructuresPartition : public PartitionItem /// Раздел: Описатель перспективных структур                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       оооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооо (проекта)
{
    Q_OBJECT

public:
    StructuresPartition(int idPrj, QWidget *prnt = 0, QString nameOfCollection = QObject::trUtf8("Структуры"));

    void        addSubElement(StructureCollection *subElement);
    void        insertSubElement(StructureCollection *subElement, int position);
    StructureCollection        *retStructureCollectionElementByStructureID(int IDstructure);

    TrapCollection             *retTrapCollectionSubSubElementByTrapID(int IDtrap);

    CreateOrAddStructureField *lastElementAddOrCreate;
};

class TrapsPartition : public PartitionItem /// Раздел: Описатель перспективных ловушек                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       оооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооооо (проекта)
{
    Q_OBJECT

public:
    TrapsPartition(int idPrj, QWidget *prnt = 0, QString nameOfPartition = QObject::trUtf8("Ловушки"));

    void        addSubElement(TrapCollection *subElement);
    void        insertSubElement(TrapCollection *subElement, int position);

    TrapCollection             *retTrapCollectionElementByTrapID(int IDtrap);

    CreateOrAddTrapField *lastElementAddOrCreate;
};

class WellsPartition : public PartitionItem /// Раздел: Рекомендуемые скважины
{
    Q_OBJECT

public:
    WellsPartition(int idPrj, QWidget *prnt = 0, QString nameOfPartition = QObject::trUtf8("Скважины"));

    void        addSubElement(WellCollection *subElement);
    void        insertSubElement(WellCollection *subElement, int position);
    void        setLastElementDisabled(bool yes);
    void        removeLastElement();

    WellCollection      *retWellCollectionElementByWellID(int IDwell);

    bool        lastElementIsRemoved;

    CreateOrAddWellField *lastElementAddOrCreate;
};

class StratumsPartition : public PartitionItem /// Раздел: Описатель пластов
{
    Q_OBJECT

public:
    StratumsPartition(int idPrj, QWidget *prnt = 0, QString nameOfPartition = QObject::trUtf8("Пласты"));

    void        addSubElement(StratumField *subElement);
    void        insertSubElement(StratumField *subElement, int position);
    void        setLastElementDisabled(bool yes);
    void        removeLastElement();

    StratumField        *retStratumFieldElementByStratumID(int IDstratum);

    bool        lastElementIsRemoved;

    CreateOrAddStratumField *lastElementAddOrCreate;
};

class EstimationResultRealizations : public PartitionItem /// Раздел: Описатель набора реализаций расчетов
{
    Q_OBJECT

public:
    EstimationResultRealizations(int idPrj, QWidget *prnt = 0,
                                 QString nameOfPartition = QObject::trUtf8("Реализации"));

    CreateOrAddRealizatonField *lastElementAddOrCreate;
};


class DescriptionField : public FieldItem
{
    Q_OBJECT

public:
    DescriptionField(int idProject, QString descriptionTextInCreating = QString(), QWidget *prnt = 0);

    QString text;
};


class WellNegativeResultRiskEstimation : public FieldItem
{
    Q_OBJECT

public:
    WellNegativeResultRiskEstimation(int idProject, double riskValue = 0.3, QWidget *prnt = 0);

    double risk;
    QString txtToolTipBase;

public slots:
    void slotSetRiskItemText(double riskNew);
};


class CreateOrAddWellField : public FieldItem
{
    Q_OBJECT

public:
    CreateOrAddWellField(int idProject, QWidget *prnt = 0);
};


class CreateOrAddStructureField : public FieldItem
{
    Q_OBJECT

public:
    CreateOrAddStructureField(int idProject, QWidget *prnt = 0);
};


class CreateOrAddStratumField : public FieldItem
{
    Q_OBJECT

public:
    CreateOrAddStratumField(int idProject, QWidget *prnt = 0);
};


class CreateOrAddTrapField : public FieldItem
{
    Q_OBJECT

public:
    CreateOrAddTrapField(int idProject, QWidget *prnt = 0);
};


class CreateOrAddRealizatonField : public FieldItem
{
    Q_OBJECT

public:
    CreateOrAddRealizatonField(int idProject, QWidget *prnt = 0);
};


class CreateOrAddCustomRiskThresholdField : public FieldItem
{
    Q_OBJECT

public:
    CreateOrAddCustomRiskThresholdField(int idProject, QWidget *prnt = 0);
};


class IdField : public FieldItem
{
    Q_OBJECT

public:
    IdField(int ID, int idProject, QWidget *prnt = 0);

    int ID;

public slots:
    void    slotChangeID(int newIdValue);
};


class StratumField : public IdField
{
    Q_OBJECT

public:
    StratumField(int ID, int idProject, QString nmSTratum = QString(), QWidget *prnt = 0);

    QString nameOfStratum;

public slots:
    void    slotChangeName(QString nm);
    void    slotSetItemText(QString newText = QString());
};


#endif // PROJECTTREEWIDGETITEM_H
