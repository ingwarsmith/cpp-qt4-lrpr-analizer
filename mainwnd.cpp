#include "mainwnd.h"
#include "ui_mainwnd.h"

MainWnd::MainWnd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWnd)
{
    ui->setupUi(this);

    this->setWindowState(Qt::WindowMaximized);

    this->slotChangeWindowTitle(QString());

    ui->centralWidget->setLayout(new QHBoxLayout);

    _lt_main = qobject_cast<QHBoxLayout* >(ui->centralWidget->layout());

    ui->actionOpenFddImage->setEnabled(false);
    ui->actionOpenFddImage->setVisible(false);

    ui->mainToolBar->setEnabled(false);
    ui->mainToolBar->setVisible(false);

    prjMgr = new ProjectManager(this);

    connect(ui->actionCreateEmptyProject,
            SIGNAL(triggered(bool)),
            prjMgr,
            SLOT(slotCreateProject()));

    connect(ui->actionSaveCurrentProject,
            SIGNAL(triggered(bool)),
            prjMgr,
            SLOT(slotSaveCurrentProject()));

    connect(ui->actionSaveCurrentProjectAs,
            SIGNAL(triggered(bool)),
            prjMgr,
            SLOT(slotSaveCurrentProjectAs()));

    connect(ui->actionSaveAllOpenedProjects,
            SIGNAL(triggered(bool)),
            prjMgr,
            SLOT(slotSaveAllProjects()));

    connect(ui->actionOpenProject,
            SIGNAL(triggered(bool)),
            prjMgr,
            SLOT(slotOpenProject()));

    connect(ui->actionCloseCurrentProject,
            SIGNAL(triggered(bool)),
            this,
            SLOT(slotForCloseCurrentProject_CheckForUnSavedChanges()));

    connect(ui->actionCloseAllOpenedProjects,
            SIGNAL(triggered(bool)),
            this,
            SLOT(slotForCloseAllProjects_CheckForUnSavedChanges()));

// по причине переориентирования на closeEvent()
//    connect(ui->actionExit,
//            SIGNAL(triggered(bool)),
//            this,
//            SLOT(slotAboutToQuit_CheckForUnSavedChanges()));

    connect(ui->actionExit,
            SIGNAL(triggered(bool)),
            this,
            SLOT(close()));

    NumbersOfProjectsByNumberOfItemInWindowMenu = QMap<int, int>();

    projectsTreeWgt = new ProjectTreeWidget(this);
    stackOfWidgets = new QStackedWidget(this);

    _lt_main->addWidget(projectsTreeWgt, 5);
    _lt_main->addWidget(stackOfWidgets, 13);

    connect(prjMgr,
            SIGNAL(sigToCreateEmptyProjectTree(int)),
            this,
            SLOT(slotCreateElementsForEmptyProject(int)));

    connect(prjMgr,
            SIGNAL(sigToFillProjectTreeByDataFromProject(int)),
            this,
            SLOT(slotFillProjectItemsFromItsData(int)));

//    connect(projectsTreeWgt,
//            SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
//            this,
//            SLOT(slotDoubleClickOnProjectsTreeWidgetItem(QTreeWidgetItem*)));

//    connect(projectsTreeWgt,
//            SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
//            this,
//            SLOT(slotClickOnProjectsTreeWidgetItem(QTreeWidgetItem*,QTreeWidgetItem*)));

    connect(projectsTreeWgt,
            SIGNAL(sigCreateStratumByDblClickOnAdd(int,bool,BaseItem*)),
            this,
            SLOT(slotAddStratumElementInCurrentProjectAtProjectsTreeWgt(int,bool,BaseItem*)));
    connect(projectsTreeWgt,
            SIGNAL(sigCreateWellByDblClickOnAdd(int,bool,bool,BaseItem*)),
            this,
            SLOT(slotAddWellElementInCurrentProjectAtProjectsTreeWgt(int,bool,bool,BaseItem*)));
    connect(projectsTreeWgt,
            SIGNAL(sigCreateStructureByDblClickOnAdd(int,BaseItem*)),
            this,
            SLOT(slotAddStructureElementInCurrentProjectAtProjectsTreeWgt(int,BaseItem*)));
    connect(projectsTreeWgt,
            SIGNAL(sigCreateTrapByDblClickOnAdd(int,int,BaseItem*)),
            this,
            SLOT(slotAddTrapElementInCurrentProjectAtProjectsTreeWgt(int,int,BaseItem*)));
    connect(projectsTreeWgt,
            SIGNAL(sigSelectProjectIDThroughClickOnPartition(int)),
            this,
            SLOT(slotSelectProjectIDThroughClickOnPartition(int)));
    connect(projectsTreeWgt,
            SIGNAL(sigShowEmptyWidget()),
            this,
            SLOT(slotShowEmptyWidget()));
    connect(projectsTreeWgt,
            SIGNAL(sigShowStratumInfoThroughClickOnPartition(int,int)),
            this,
            SLOT(slotShowSelectedStratumInfo(int,int)));
    connect(projectsTreeWgt,
            SIGNAL(sigShowStructureInfoThroughClickOnPartition(int,int)),
            this,
            SLOT(slotShowSelectedStructureInfo(int,int)));
    connect(projectsTreeWgt,
            SIGNAL(sigShowWellpInfoThroughClickOnPartition(int,int)),
            this,
            SLOT(slotShowSelectedWellInfo(int,int)));
    connect(projectsTreeWgt,
            SIGNAL(sigShowTrapInfoThroughClickOnPartition(int,int,TrapCollection*)),
            this,
            SLOT(slotShowSelectedTrapInfo(int,int,TrapCollection*)));

    QWidget *wgtEmpty = new QWidget;
    stackOfWidgets->insertWidget(SpecialWidget::typeEmpty, wgtEmpty);

    BaseProjectPropertiesWidget *wgtPrj = new BaseProjectPropertiesWidget;

    BaseStructurePropertiesWidget *wgtStruct = new BaseStructurePropertiesWidget;

    BaseStratumPropertiesWidget *wgtStratum = new BaseStratumPropertiesWidget;

    BaseTrapPropertiesWidget *wgtTrap = new BaseTrapPropertiesWidget;

    BaseWellPropertiesWidget *wgtWell = new BaseWellPropertiesWidget;

    stackOfWidgets->insertWidget((int) SpecialWidget::typeProjectProperties, wgtPrj);
    stackOfWidgets->insertWidget((int) SpecialWidget::typeStructureProperties, wgtStruct);
    stackOfWidgets->insertWidget((int) SpecialWidget::typeStratumProperties, wgtStratum);
    stackOfWidgets->insertWidget((int) SpecialWidget::typeTrapProperties, wgtTrap);
    stackOfWidgets->insertWidget((int) SpecialWidget::typeWellProperties, wgtWell);    

    setConnectedSlotsOfChangingsInBaseWidgets(true, SpecialWidget::typeProjectProperties);
    setConnectedSlotsOfChangingsInBaseWidgets(true, SpecialWidget::typeStructureProperties);
    setConnectedSlotsOfChangingsInBaseWidgets(true, SpecialWidget::typeStratumProperties);
    setConnectedSlotsOfChangingsInBaseWidgets(true, SpecialWidget::typeTrapProperties);
    setConnectedSlotsOfChangingsInBaseWidgets(true, SpecialWidget::typeWellProperties);
}

MainWnd::~MainWnd()
{    
    delete ui;
//    delete mdiArea;
    delete prjMgr;
}

void MainWnd::setConnectedSlotsOfChangingsInBaseWidgets(bool connect,
                                                                      SpecialWidget::SpecialWidgetType type)
{
    if (type == SpecialWidget::typeProjectProperties)
    {
        BaseProjectPropertiesWidget *wgt = qobject_cast<BaseProjectPropertiesWidget* >(
                    stackOfWidgets->widget((int) type)
                    );

        if (connect)
        {
            this->connect(wgt,
                    SIGNAL(signalSomethingChanged()),
                    this,
                    SLOT(slotSomethingChangedInCurrentWidget()));
        }
        else
        {
            this->disconnect(wgt,
                    SIGNAL(signalSomethingChanged()),
                    this,
                    SLOT(slotSomethingChangedInCurrentWidget()));
        }
    }
    else if (type == SpecialWidget::typeStratumProperties)
    {
        BaseStratumPropertiesWidget *wgt = qobject_cast<BaseStratumPropertiesWidget* >(
                    stackOfWidgets->widget((int) type)
                    );

        if (connect)
        {
            this->connect(wgt,
                    SIGNAL(signalSomethingChanged()),
                    this,
                    SLOT(slotSomethingChangedInCurrentWidget()));
        }
        else
        {
            this->disconnect(wgt,
                    SIGNAL(signalSomethingChanged()),
                    this,
                    SLOT(slotSomethingChangedInCurrentWidget()));
        }
    }
    else if (type == SpecialWidget::typeStructureProperties)
    {
        BaseStructurePropertiesWidget *wgt = qobject_cast<BaseStructurePropertiesWidget* >(
                    stackOfWidgets->widget((int) type)
                    );

        if (connect)
        {
            this->connect(wgt,
                    SIGNAL(signalSomethingChanged()),
                    this,
                    SLOT(slotSomethingChangedInCurrentWidget()));
        }
        else
        {
            this->disconnect(wgt,
                    SIGNAL(signalSomethingChanged()),
                    this,
                    SLOT(slotSomethingChangedInCurrentWidget()));
        }
    }
    else if (type == SpecialWidget::typeTrapProperties)
    {
        BaseTrapPropertiesWidget *wgt = qobject_cast<BaseTrapPropertiesWidget* >(
                    stackOfWidgets->widget((int) type)
                    );

        if (connect)
        {
            this->connect(wgt,
                    SIGNAL(signalSomethingChanged()),
                    this,
                    SLOT(slotSomethingChangedInCurrentWidget()));
        }
        else
        {
            this->disconnect(wgt,
                    SIGNAL(signalSomethingChanged()),
                    this,
                    SLOT(slotSomethingChangedInCurrentWidget()));
        }
    }
    else if (type == SpecialWidget::typeWellProperties)
    {
        BaseWellPropertiesWidget *wgt = qobject_cast<BaseWellPropertiesWidget* >(
                    stackOfWidgets->widget((int) type)
                    );

        if (connect)
        {
            this->connect(wgt,
                    SIGNAL(signalSomethingChanged()),
                    this,
                    SLOT(slotSomethingChangedInCurrentWidget()));

            wgt->setCmbSignalsDisconnected(true);
        }
        else
        {
            this->disconnect(wgt,
                    SIGNAL(signalSomethingChanged()),
                    this,
                    SLOT(slotSomethingChangedInCurrentWidget()));

            wgt->setCmbSignalsDisconnected(false);
        }
    }
}

void MainWnd::synchronizeDataAfterChangingInProjectInfoWidget(QObject *senderObj)
{
    Project *currentProject = prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber);

    ProjectRootCollection *currPrjRootColl =
            projectsTreeWgt->retProjectItemByProjectID(prjMgr->currentProjectNumber);

    BaseProjectPropertiesWidget *wgt = qobject_cast<BaseProjectPropertiesWidget* >(senderObj);

    currentProject->setProjectName(wgt->lned_PrjName->text());
    slotChangeWindowTitle(currentProject->getProjectName());
    currentProject->setDescription(wgt->te_Description->toPlainText());

    currPrjRootColl->slotChangeName(currentProject->getProjectName());
    currPrjRootColl->Begin_decsription->slotSetItemToolTipText(currentProject->getDescription());

    slotChangeWindowTitle(currentProject->getProjectName());
}

void MainWnd::synchronizeDataAfterChangingInStratumInfoWidget(QObject *senderObj)
{
    Project *currentProject = prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber);

    BaseStratumPropertiesWidget *wgt = qobject_cast<BaseStratumPropertiesWidget* >(senderObj);

    Stratum *stratum = currentProject->stratumsNamesByNumber.value(wgt->lned_stratumID->text().toInt());

    stratum->description = wgt->te_DescriptionStratum->toPlainText();
    stratum->nameOfStratum = wgt->lned_stratumName->text();

    ProjectRootCollection *currPrjRootColl =
            projectsTreeWgt->retProjectItemByProjectID(prjMgr->currentProjectNumber);

    StratumField *sf = currPrjRootColl->projectPart_Stratums
            ->retStratumFieldElementByStratumID(stratum->numberOfStratumInCommonMapOfProject);

    sf->slotChangeName(stratum->nameOfStratum);

    for (int istr_ch = 0; istr_ch < currPrjRootColl->projectPart_Structures->childCount(); ++istr_ch)
    {
        StructureCollection *sc =
                qobject_cast<StructureCollection* >(static_cast<BaseItem* >(
                                                        currPrjRootColl->projectPart_Structures->child(istr_ch)
                                                        ));

        if (!sc) continue;

        for (int itrap_on_str = 0;
             itrap_on_str < sc->structurePart_Traps->childCount();
             ++itrap_on_str)
        {
            TrapCollection *tc =
                    qobject_cast<TrapCollection* >(static_cast<BaseItem* >(
                                                       sc->structurePart_Traps->child(itrap_on_str)
                                                       ));

            if (!tc) continue;

            Trap *trap = currentProject->trapsInProjectCommonMap.value(tc->ID_trap->ID);

            if (trap->nameOfStratum == sf->nameOfStratum)
            {
                tc->StratumOfTrap->slotChangeName(stratum->nameOfStratum);
                tc->Begin_decsription->slotSetItemToolTipText(stratum->description);
            }
        }

        for (int istrat_on_str = 0;
             istrat_on_str < sc->structurePart_Stratums_ReadOnly->childCount();
             ++istrat_on_str)
        {
            StratumField *sf_on_str = qobject_cast<StratumField* >
                    (
                        static_cast<BaseItem* >
                        (
                            sc->structurePart_Stratums_ReadOnly->child(istrat_on_str)
                            )
                        );

            if (!sf_on_str) continue;

            if (sf_on_str->ID == sf->ID)
            {
                sf_on_str->slotChangeName(stratum->nameOfStratum);
            }
        }
    }
}

void MainWnd::synchronizeDataAfterChangingInStructureInfoWidget(QObject *senderObj)
{
    Project *currentProject = prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber);

    BaseStructurePropertiesWidget *wgt = qobject_cast<BaseStructurePropertiesWidget* >(senderObj);

    Structure *structure = currentProject->structuresInProject.value(wgt->lned_structID->text().toInt());

    structure->setDescription(wgt->te_DescriptionStruct->toPlainText());
    structure->setNameStruct(wgt->lned_structName->text());

    ProjectRootCollection *currPrjRootColl =
            projectsTreeWgt->retProjectItemByProjectID(prjMgr->currentProjectNumber);

    StructureCollection *sc = currPrjRootColl
            ->projectPart_Structures->retStructureCollectionElementByStructureID(wgt->lned_structID->text().toInt());

    sc->Begin_decsription->slotSetItemToolTipText(structure->getDescription());
    sc->slotChangeName(structure->getNameStruct());
}

void MainWnd::synchronizeDataAfterChangingInTrapInfoWidget(QObject *senderObj)
{
    Project *currentProject = prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber);

    //ProjectRootCollection *currPrjRootCl = projectsTreeWgt->retProjectItemByProjectID(prjMgr->currentProjectNumber);

    BaseTrapPropertiesWidget *wgt = qobject_cast<BaseTrapPropertiesWidget* >(senderObj);

    Trap *trap = currentProject->trapsInProjectCommonMap.value(wgt->lineEdit_IDtrap->text().toInt());

    trap->commentForTrap = trap->description = wgt->te_Comment->toPlainText();

    trap->nameTrap = wgt->lineEdit_TrapName->text();

    trap->baseData->Area.minimum = wgt->lineEdit_AreaMin->text().toDouble();

    trap->baseData->Area.average = wgt->lineEdit_AreaAvg->text().toDouble();

    trap->baseData->Area.maximum = wgt->lineEdit_AreaMax->text().toDouble();

    trap->baseData->DensityOfOil.minimum = wgt->lineEdit_OilDensityMin->text().toDouble();

    trap->baseData->DensityOfOil.average = wgt->lineEdit_OilDensityAvg->text().toDouble();

    trap->baseData->DensityOfOil.maximum = wgt->lineEdit_OilDensityMax->text().toDouble();

    trap->baseData->determinedKIN = wgt->lineEdit_KIN->text().toDouble();

    trap->baseData->EffectiveThick.minimum = wgt->lineEdit_EffThickMin->text().toDouble();

    trap->baseData->EffectiveThick.average = wgt->lineEdit_EffThickAvg->text().toDouble();

    trap->baseData->EffectiveThick.maximum = wgt->lineEdit_EffThickMax->text().toDouble();

    trap->baseData->numberOfAllocationType = wgt->cmb_AllocationType->currentIndex();

    trap->baseData->OilSaturationCoeff.minimum = wgt->lineEdit_OilSaturationCoeffMin->text().toDouble();

    trap->baseData->OilSaturationCoeff.average = wgt->lineEdit_OilSaturationCoeffAvg->text().toDouble();

    trap->baseData->OilSaturationCoeff.maximum = wgt->lineEdit_OilSaturationCoeffMax->text().toDouble();

    trap->baseData->PorosityCoeff.minimum = wgt->lineEdit_PorosityCoeffMin->text().toDouble();

    trap->baseData->PorosityCoeff.average = wgt->lineEdit_PorosityCoeffAvg->text().toDouble();

    trap->baseData->PorosityCoeff.maximum = wgt->lineEdit_PorosityCoeffMax->text().toDouble();

    trap->baseData->ShrinkingCoeff.minimum = wgt->lineEdit_OilShrinkingMin->text().toDouble();

    trap->baseData->ShrinkingCoeff.average = wgt->lineEdit_OilShrinkingAvg->text().toDouble();

    trap->baseData->ShrinkingCoeff.maximum = wgt->lineEdit_OilShrinkingMax->text().toDouble();

    trap->allocationType->setAllocTypeInt_Key(trap->baseData->numberOfAllocationType);

    trap->commentForTrap = wgt->te_Comment->toPlainText();

    trap->baseData->probabilityOfDepositExisting = wgt->lineEdit_ProbaboilityOfDeposite->text().toDouble();

    TrapCollection *currItem =
            qobject_cast<TrapCollection* >(static_cast<BaseItem* >(projectsTreeWgt->currentItem()));

    currItem->slotChangeName(trap->nameTrap);

    currItem->Begin_decsription->slotSetItemToolTipText(trap->commentForTrap);

    if (!wgt->cmb_StratumIndex->currentText().isEmpty())
    {
        QList<int> keys_strat = currentProject->stratumsNamesByNumber.keys();

        int key = 0;

        trap->nameOfStratum = wgt->cmb_StratumIndex->currentText();

        for (int i = 0; i < keys_strat.size(); ++i)
        {

            if (currentProject->stratumsNamesByNumber.value(keys_strat.at(i))
                    ->nameOfStratum == trap->nameOfStratum
                    )
            {
                key = keys_strat.at(i);
                break;
            }
        }

        trap->numberOfStratumInCommonMapOfProject = key;

        currItem->setStratumFieldID(
                    trap->numberOfStratumInCommonMapOfProject
                    );
        currItem->setStratumFieldNameOfStratum(trap->nameOfStratum);


        // обновление данных о пластах на структуре

        StructureCollection *collStr =
                qobject_cast<StructureCollection* >(
                    static_cast<BaseItem* >(currItem->QTreeWidgetItem::parent()->parent())
                    );

//        StratumField *sf_old = collStr->structurePart_Stratums_ReadOnly
//                ->retStratumFieldElementByStratumID(key);

        Structure *str_c = currentProject->structuresInProject.value(collStr->ID_structure->ID);

        for (int iCh = 0; iCh < collStr->structurePart_Stratums_ReadOnly->childCount(); ++iCh)
        {
//            StratumField *sf = collStr->structurePart_Stratums_ReadOnly
//                    ->retStratumFieldElementByStratumID(trap->numberOfStratumInCommonMapOfProject)
        }

        for (int iTrap = 0; iTrap < str_c->trapsNumbersFromCommonMap_onStructure.size(); ++iTrap)
        {
            Trap *trapOnStr = prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber)
                    ->trapsInProjectCommonMap.value(
                        str_c->trapsNumbersFromCommonMap_onStructure.at(iTrap)
                        );

            if (trapOnStr->nameOfStratum.isEmpty()) continue;

            StratumField *stratFldNew = new StratumField(
                        trapOnStr->numberOfStratumInCommonMapOfProject,
                        prjMgr->currentProjectNumber,
                        trapOnStr->nameOfStratum
                        );

            stratFldNew->slotChangeID(trapOnStr->numberOfStratumInCommonMapOfProject);

            stratFldNew->slotChangeName(trapOnStr->nameOfStratum);

            bool neededToAdd = true;

            int chc = collStr->structurePart_Stratums_ReadOnly->childCount();

            for (int iElem = 0; iElem < chc; ++iElem)
            {
                BaseItem *itmb = static_cast<BaseItem* >(collStr->structurePart_Stratums_ReadOnly->child(iElem));

                if (itmb->customItemType() == BaseItem::tFieldItem)
                {
                    FieldItem *itmf = qobject_cast<FieldItem* >(itmb);

                    if (itmf->fieldItemType() == FieldItem::ftStratum)
                    {
                        StratumField *elem = qobject_cast<StratumField* >(itmf);

                        if (elem->ID != stratFldNew->ID)
                        {
                            continue;
                        }
                        else
                        {
                            neededToAdd = false;
                            break;
                        }
                    }
                }
            }

            if (!neededToAdd) continue;

            if (stratFldNew->ID == -1) continue;

            collStr->structurePart_Stratums_ReadOnly->addChild(stratFldNew);

            collStr->structurePart_Stratums_ReadOnly->sortChildren(0, Qt::AscendingOrder);
        }
    }
}

void MainWnd::synchronizeDataAfterChangingInWellInfoWidget(QObject *senderObj)
{
    Project *currentProject = prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber);

    BaseWellPropertiesWidget *wgt = qobject_cast<BaseWellPropertiesWidget* >(senderObj);

    WellCollection *wc_curr = qobject_cast<WellCollection* >(
                static_cast<BaseItem* >(projectsTreeWgt->currentItem())
                );

    int id_well_fromCmb = wgt->cmb_ID->currentText().split(trUtf8(" (id), имя: ")).at(0).toInt();

    int id_well = wgt->lned_ID->text().toInt();

    if (id_well_fromCmb != id_well) // смена ID скважины
    {
        wgt->oldID = id_well;

        id_well = id_well_fromCmb;

        wgt->lned_ID->setText(QString::number(id_well));

        wgt->ID_valueChanged = true;

        Well *well = currentProject
                ->wellsInProject.value(id_well);

        if (!well) return;

        wgt->lned_NegResProb->setText(QString::number(well->negativeResultProbability));

        wgt->lned_wellName->setText(well->wellName);

        wgt->te_Description->setPlainText(well->wellDescription);

        TrapCollection *tc_for_wc_curr = qobject_cast<TrapCollection* >(
                    static_cast<BaseItem* >(projectsTreeWgt->currentItem()
                                            ->parent()->parent())
                    );

        Trap *trap = currentProject->trapsInProjectCommonMap.value(tc_for_wc_curr->ID_trap->ID);

        if (trap->IDofWellsRecommendedForExplorationOfTrap.contains(id_well))
        {
            QMessageBox::critical(this,
                                  trUtf8("Ошибка пользователя"),
                                  trUtf8("Нельзя размещать одну и ту же скважину на ловушке дважды!"));

            return;
        }

        int indexOfID = trap->IDofWellsRecommendedForExplorationOfTrap.value(wgt->oldID);

        trap->IDofWellsRecommendedForExplorationOfTrap.remove(indexOfID);

        trap->IDofWellsRecommendedForExplorationOfTrap.append(id_well);

        wc_curr->Begin_decsription->slotSetItemToolTipText(well->wellDescription);

        wc_curr->elementRisk->slotSetRiskItemText(well->negativeResultProbability);

        wc_curr->slotChangeName(well->wellName);

        wc_curr->setWellFieldID(id_well);

        refreshWellListOnProjectAfterChaingeWellID(wgt->oldID, id_well);

        refreshWellListOnProject();
    }
    else
    {
        Well *well = currentProject
                ->wellsInProject.value(id_well);

        if (!well) return;

        well->negativeResultProbability = wgt->lned_NegResProb->text().toDouble();

        well->wellDescription = wgt->te_Description->toPlainText();

        well->wellName = wgt->lned_wellName->text();

        wc_curr->Begin_decsription->slotSetItemToolTipText(well->wellDescription);

        wc_curr->elementRisk->slotSetRiskItemText(well->negativeResultProbability);

        wc_curr->slotChangeName(well->wellName);

        refreshWellListOnProject();
    }
}

void MainWnd::refreshWellListOnProject()
{
    ProjectRootCollection *prc = projectsTreeWgt->retProjectItemByProjectID(prjMgr->currentProjectNumber);

    while (prc->projectPart_Wells_ReadOnly_List->childCount() > 0)
    {
        QTreeWidgetItem *item = //prc->projectPart_Wells_ReadOnly_List->removeLastElement();
                prc->projectPart_Wells_ReadOnly_List->child(0);

        prc->projectPart_Wells_ReadOnly_List->removeChild(item);
    }

    Project *project = prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber);

    for (int i_struct = 0; i_struct < project->structuresInProject.keys().size(); ++i_struct)
    {
        int id = project->structuresInProject.keys().at(i_struct);

        Structure *structure = project->structuresInProject.value(id);

        StructureCollection *sc = prc->projectPart_Structures->retStructureCollectionElementByStructureID(id);

        QVector<int> wellsID_onStructure = QVector<int>();

        for (int i_trap_on_struct = 0;
             i_trap_on_struct < structure
             ->trapsNumbersFromCommonMap_onStructure.size();
             ++i_trap_on_struct)
        {
            int id = structure->trapsNumbersFromCommonMap_onStructure.at(i_trap_on_struct);

            Trap *trap = project->trapsInProjectCommonMap.value(id);

            for (int i_well_for_trap = 0;
                 i_well_for_trap < trap->IDofWellsRecommendedForExplorationOfTrap.size();
                 ++i_well_for_trap)
            {
                int id_well_current = trap->IDofWellsRecommendedForExplorationOfTrap.at(i_well_for_trap);

                if (!wellsID_onStructure.contains(id_well_current))
                {
                    wellsID_onStructure.append(id_well_current);
                }
            }
        }

        if (wellsID_onStructure.isEmpty()) continue; // оставить текущую структуру в покое

        while (sc->structurePart_Wells_ReadOnly->childCount() > 0)
        {
            QTreeWidgetItem *item = sc->structurePart_Wells_ReadOnly->child(0);

            sc->structurePart_Wells_ReadOnly->removeChild(item);
        }

        for (int i_id = 0; i_id < wellsID_onStructure.size(); ++i_id)
        {
            Well *well = project->wellsInProject.value(
                        wellsID_onStructure.at(i_id)
                        );

            WellCollection *wc3 = new WellCollection(well->wellID,
                                                     prjMgr->currentProjectNumber,
                                                     well->wellName,
                                                     projectsTreeWgt
                                                     );

            wc3->Begin_decsription->slotSetItemToolTipText(well->wellDescription);

            wc3->elementRisk->slotSetRiskItemText(well->negativeResultProbability);

            sc->structurePart_Wells_ReadOnly->addSubElement(wc3);
        }
    }

    for (int i_well = 0; i_well < project->wellsInProject.keys().size(); ++i_well)
    {
        int id = project->wellsInProject.keys().at(i_well);

        Well *well = project->wellsInProject.value(id);

        WellCollection *wc = new WellCollection(well->wellID,
                                                prjMgr->currentProjectNumber,
                                                well->wellName,
                                                projectsTreeWgt);

        wc->Begin_decsription->slotSetItemToolTipText(well->wellDescription);

        wc->elementRisk->slotSetRiskItemText(well->negativeResultProbability);

        prc->projectPart_Wells_ReadOnly_List->addSubElement(wc);
    }
}

void MainWnd::refreshWellListOnProjectAfterChaingeWellID(int oldIDwell, int newIDwell)
{
    Q_UNUSED(oldIDwell);
    Q_UNUSED(newIDwell);

    ProjectRootCollection *prc = projectsTreeWgt->retProjectItemByProjectID(prjMgr->currentProjectNumber);

    Project *project = prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber);

//    Well *wellOld = project->wellsInProject.value(oldIDwell);

    StructureCollection *sc = qobject_cast<StructureCollection* >(
                static_cast<BaseItem* >(
                    projectsTreeWgt->currentItem()->parent()->parent()->parent()->parent()
                    )
                );

    if (!sc)
    {
        QString txt = trUtf8("0: MainWnd::refreshWellListOnProjectAfterChaingeWellID mainwnd.cpp\n1: MainWnd::synchronizeDataAfterChangingInWellInfoWidget mainwnd.cpp\n2: MainWnd::slotSomethingChangedInCurrentWidget mainwnd.cpp\n3: MainWnd::qt_static_metacall moc_mainwnd.cpp\n4: QMetaObject::activate(QObject *, QMetaObject const *, int, void * *)\n5: BaseWellPropertiesWidget::signalSomethingChanged moc_specialwidget.cpp\n6: BaseWellPropertiesWidget::slotDialogOfWellIdChangingToExec specialwidget.cpp\n...: <Более>	");
        QMessageBox::critical(this,
                              trUtf8("Критический сбой!"),
                              trUtf8("Стек вызовов:\n") + txt);
        return;
    }

    Structure *structure = project->structuresInProject.value(sc->ID_structure->ID);

    bool wellMustBeDeletedFromStructure = true;

    for (int i_trapID = 0; i_trapID < structure->trapsNumbersFromCommonMap_onStructure.size(); ++i_trapID)
    {
        int id_trap = structure->trapsNumbersFromCommonMap_onStructure.at(i_trapID);

        if (project->trapsInProjectCommonMap.value(id_trap)
                ->IDofWellsRecommendedForExplorationOfTrap.contains(oldIDwell))
        {
            wellMustBeDeletedFromStructure = false;

            break;
        }
    }

    if (wellMustBeDeletedFromStructure)
    {
        project->wellsInProject.remove(oldIDwell);

        prc->projectPart_Wells_ReadOnly_List->removeChild(
                    prc->projectPart_Wells_ReadOnly_List->retWellCollectionElementByWellID(oldIDwell)
                    );
    }

    //    StructureCollection *sc = prc->projectPart_Structures->retStructureCollectionElementByStructureID();
}

void MainWnd::aboutToCloseProject_CheckForUnSavedChanges()
{
//    if (prjMgr->arrayOfProjects.isEmpty()) return;

//    Project *project = prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber);

//    if (project->isSavedProjectWithoutUnsavedChanges())
//    {

//    }
//    else
//    {
//        QMessageBox msg;
//        msg.setWindowTitle(trUtf8("Несохраненные изменения"));
//        QString text = trUtf8("Сохранить изменения в проекте ")
//                + project->getProjectName() + QString("?");
//        msg.setText(text);
//        msg.addButton(QMessageBox::Ok);
//        msg.addButton(QMessageBox::No);
//        msg.addButton(QMessageBox::Cancel);

//        int result = msg.exec();

//        if (result == QMessageBox::Ok)
//        {
//            prjMgr->currentProjectNumber = project->getThisProjectNumb();

//            prjMgr->slotSaveCurrentProject();
//        }
//        else if (result == QMessageBox::No)
//        {

//        }
//        else return; // Cancel
//    }

//    // сама процедура закрытия

//    ProjectRootCollection *prc = projectsTreeWgt->retProjectItemByProjectID(prjMgr->currentProjectNumber);

//    projectsTreeWgt->removeItemWidget(prc, 0);

//    prjMgr->slotCLoseCurrentProject();
}

void MainWnd::closeEvent(QCloseEvent *evnt)
{
    if (prjMgr->arrayOfProjects.isEmpty())
    {
        //
    }
    else
    {
        for (int i_prj = 0; i_prj < prjMgr->arrayOfProjects.values().size(); ++i_prj)
        {
            Project *project = prjMgr->arrayOfProjects.values().at(i_prj);
            if (project->isSavedProjectWithoutUnsavedChanges()) continue;
            else
            {
                QMessageBox msg;
                msg.setWindowTitle(trUtf8("Несохраненные изменения"));
                QString text = trUtf8("Сохранить изменения в проекте ")
                        + project->getProjectName() + QString("?");
                msg.setText(text);
                msg.addButton(QMessageBox::Ok);
                msg.addButton(QMessageBox::No);
                msg.addButton(QMessageBox::Cancel);

                int result = msg.exec();

                if (result == QMessageBox::Ok)
                {
                    prjMgr->currentProjectNumber = project->getThisProjectNumb();

                    prjMgr->slotSaveCurrentProject();

                    continue;
                }
                else if (result == QMessageBox::No)
                {
                    continue;
                }
                else if (result == QMessageBox::Cancel)
                {
                    evnt->ignore();

                    return;
                }
            }
        }
    }

    evnt->accept();
}

void MainWnd::slotAboutToQuit_CheckForUnSavedChanges()
{
    if (prjMgr->arrayOfProjects.isEmpty()) qApp->exit(0);

    for (int i_prj = 0; i_prj < prjMgr->arrayOfProjects.values().size(); ++i_prj)
    {
        Project *project = prjMgr->arrayOfProjects.values().at(i_prj);
        if (project->isSavedProjectWithoutUnsavedChanges()) continue;
        else
        {
            QMessageBox msg;
            msg.setWindowTitle(trUtf8("Несохраненные изменения"));
            QString text = trUtf8("Сохранить изменения в проекте ")
                    + project->getProjectName() + QString("?");
            msg.setText(text);
            msg.addButton(QMessageBox::Ok);
            msg.addButton(QMessageBox::No);
            msg.addButton(QMessageBox::Cancel);

            int result = msg.exec();

            if (result == QMessageBox::Ok)
            {
                prjMgr->currentProjectNumber = project->getThisProjectNumb();

                prjMgr->slotSaveCurrentProject();

                continue;
            }
            else if (result == QMessageBox::No)
            {
                //prjMgr->currentProjectNumber = project->getThisProjectNumb();

                continue;
            }
            else return;
        }
    }

    qApp->exit(0);
}

void MainWnd::slotForCloseCurrentProject_CheckForUnSavedChanges()
{
    if (!prjMgr->arrayOfProjects.isEmpty())
    {
        Project *prjCurr = prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber);

        if (!prjCurr->isSavedProjectWithoutUnsavedChanges())
        {
            QMessageBox msg;
            msg.setWindowTitle(trUtf8("Несохраненные изменения"));
            QString text = trUtf8("Сохранить изменения в проекте ")
                    + prjCurr->getProjectName() + QString("?");
            msg.setText(text);
            msg.addButton(QMessageBox::Ok);
            msg.addButton(QMessageBox::No);
            msg.addButton(QMessageBox::Cancel);

            int result = msg.exec();

            if (result == QMessageBox::Ok)
            {
                //prjMgr->currentProjectNumber = project->getThisProjectNumb();

                prjMgr->slotSaveCurrentProject();



                //continue;
            }
            else if (result == QMessageBox::No)
            {
                //continue;
            }
            else if (result == QMessageBox::Cancel)
            {
                return;
            }
        }

        // сама процедура закрытия

        ProjectRootCollection *prc = projectsTreeWgt->retProjectItemByProjectID(prjMgr->currentProjectNumber);

        //QTreeWidgetItem *item = static_cast<QTreeWidgetItem* >(prc);

        //projectsTreeWgt->removeItemWidget(item, 0);

        //item->setHidden(true);

        prc->QTreeWidgetItem::setHidden(true);

        delete prc;

        prjMgr->slotCLoseCurrentProject();

        if (prjMgr->arrayOfProjects.count() == 0)
        {
            stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeEmpty);
        }
    }
}

void MainWnd::slotForCloseAllProjects_CheckForUnSavedChanges()
{
    if (!prjMgr->arrayOfProjects.isEmpty())
    {
        int countOfProjects = prjMgr->arrayOfProjects.count();

        for (int i_prj = 0; i_prj < countOfProjects; ++i_prj)
        {
            Project *prjCurr = prjMgr->arrayOfProjects.values().at(0);

            if (!prjCurr->isSavedProjectWithoutUnsavedChanges())
            {
                QMessageBox msg;
                msg.setWindowTitle(trUtf8("Несохраненные изменения"));
                QString text = trUtf8("Сохранить изменения в проекте ")
                        + prjCurr->getProjectName() + QString("?");
                msg.setText(text);
                msg.addButton(QMessageBox::Ok);
                msg.addButton(QMessageBox::No);
                msg.addButton(QMessageBox::Cancel);

                int result = msg.exec();

                if (result == QMessageBox::Ok)
                {
                    prjMgr->currentProjectNumber = prjCurr->getThisProjectNumb();

                    prjMgr->slotSaveCurrentProject();



                    continue;
                }
                else if (result == QMessageBox::No)
                {
                    continue;
                }
                else if (result == QMessageBox::Cancel)
                {
                    return;
                }
            }

            // сама процедура закрытия

            ProjectRootCollection *prc = projectsTreeWgt->retProjectItemByProjectID(prjMgr->currentProjectNumber);

            prc->QTreeWidgetItem::setHidden(true);

            delete prc;

            prjMgr->slotCLoseCurrentProject();

            if (prjMgr->arrayOfProjects.count() == 0)
            {
                stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeEmpty);
            }
        }
    }
}

void MainWnd::slotCreateElementsForEmptyProject(int projID)
{
    projectsTreeWgt->slotCreateEmptyProjectItems(projID);

    Project *prj = new Project;

    prj->setThisProjectNumb(projID);

    prjMgr->arrayOfProjects.insert(projID, prj);
}

void MainWnd::slotSomethingChangedInCurrentWidget()
{
    if (prjMgr->arrayOfProjects.isEmpty()) return;

    prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber)->setUnSavedChangesExist();

    if (sender()->metaObject()->className() ==
            stackOfWidgets->widget((int) SpecialWidget::typeProjectProperties)->metaObject()->className())
    {
        synchronizeDataAfterChangingInProjectInfoWidget(sender());
    }
    else if (sender()->metaObject()->className() ==
             stackOfWidgets->widget((int) SpecialWidget::typeStratumProperties)->metaObject()->className())
    {
        synchronizeDataAfterChangingInStratumInfoWidget(sender());
    }
    else if (sender()->metaObject()->className() ==
             stackOfWidgets->widget((int) SpecialWidget::typeStructureProperties)->metaObject()->className())
    {
        synchronizeDataAfterChangingInStructureInfoWidget(sender());
    }
    else if (sender()->metaObject()->className() ==
             stackOfWidgets->widget((int) SpecialWidget::typeTrapProperties)->metaObject()->className())
    {
        synchronizeDataAfterChangingInTrapInfoWidget(sender());
    }
    else if (sender()->metaObject()->className() ==
             stackOfWidgets->widget((int) SpecialWidget::typeWellProperties)->metaObject()->className())
    {
        synchronizeDataAfterChangingInWellInfoWidget(sender());
    }

    return;


}

void MainWnd::slotChangeWindowTitle(QString str)
{
    QString adding = QString();
    if (!str.isEmpty()) adding = QString(" - ") + str;
    setWindowTitle(baseWindowTitle + adding);
//    prjMgr->arrayOfProjects.value(prjMgr->currentProjectNumber)
//            ->setProjectName(str);
}

void MainWnd::slotAddStratumElementInCurrentProjectAtProjectsTreeWgt(int currentProject,
                                                                     bool isInProjectStratumsPartition,
                                                                     BaseItem *itemOfAdd)
{
    if (itemOfAdd->projectID != currentProject)
    {
        QMessageBox::critical(this,
                              trUtf8("СБОЙ!"),
                              trUtf8("Несоответствие идентификаторов проекта!"));
        return;
    }

    prjMgr->currentProjectNumber = currentProject;

    if (!isInProjectStratumsPartition)
    {
        QMessageBox::critical(this,
                              trUtf8("СБОЙ!"),
                              trUtf8("Попытка создания элемента пласта в несоответствующем месте дерева проектов."));
        return;
    }

    QList<int> currPrjStratumsKeys =
            prjMgr->arrayOfProjects.value(currentProject)->stratumsNamesByNumber.keys();

    int stratKeysMaxVal = 0;

    for (int i = 0; i < currPrjStratumsKeys.count(); ++i)
    {
        if (stratKeysMaxVal < currPrjStratumsKeys.at(i))
            stratKeysMaxVal = currPrjStratumsKeys.at(i);
    }

    stratKeysMaxVal++;

    Stratum *stratumNew = new Stratum;

    stratumNew->numberOfStratumInCommonMapOfProject = stratKeysMaxVal;

    prjMgr->arrayOfProjects.value(currentProject)->stratumsNamesByNumber.insert(
                stratKeysMaxVal, stratumNew
                );

    StratumField *newStratumItem = new StratumField(stratKeysMaxVal, currentProject);//(stratKeysMaxVal);

    newStratumItem->slotChangeName(stratumNew->nameOfStratum);

    ProjectRootCollection *currPrjItem = projectsTreeWgt->retProjectItemByProjectID(currentProject);

    currPrjItem->projectPart_Stratums->addSubElement(newStratumItem);

    stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeEmpty);
}

void MainWnd::slotAddWellElementInCurrentProjectAtProjectsTreeWgt(int projectID,
                                                                  bool isInWellsOfProject,
                                                                  bool isInWellsOfTrap,
                                                                  BaseItem *itemOfAdd)
{
    if (itemOfAdd->projectID != projectID)
    {
        QMessageBox::critical(this,
                              trUtf8("СБОЙ!"),
                              trUtf8("Несоответствие идентификаторов проекта!"));
        return;
    }

    prjMgr->currentProjectNumber = projectID;

    if (!isInWellsOfProject && !isInWellsOfTrap)
    {
        QMessageBox::critical(this,
                              trUtf8("СБОЙ!"),
                              trUtf8("Попытка создания элемента скважины в несоответствующем месте дерева проектов."));
        return;
    }

    Well *newWell = new Well;

    //ProjectRootCollection *currPrjItem = projectsTreeWgt->retProjectItemByProjectID(projectID);

    QList<int> currPrjWellsKeys =
            prjMgr->arrayOfProjects.value(projectID)->wellsInProject.keys();

    int valWellID = 0;

    if (isInWellsOfTrap)
    {
        for (int i = 0; i < currPrjWellsKeys.count(); ++i)
        {
            if (valWellID < currPrjWellsKeys.at(i))
                valWellID = currPrjWellsKeys.at(i);
        }

        valWellID++;
    }
    else
    {
        valWellID = Well::emptyWellID_start;

        for (int i = 0; i < currPrjWellsKeys.count(); ++i)
        {
            if (valWellID > currPrjWellsKeys.at(i))
                valWellID = currPrjWellsKeys.at(i);
        }
        valWellID--;
    }

    newWell->wellID = valWellID;
    QString nmw_str = trUtf8("Новая скважина");
    newWell->wellName = nmw_str;

    WellCollection *newWColl = new WellCollection(
                valWellID, projectID
                );
    newWColl->setWellFieldID(valWellID);
    newWColl->slotChangeName(nmw_str);

    prjMgr->arrayOfProjects.value(projectID)->wellsInProject.insert(newWell->wellID, newWell);

    if (isInWellsOfTrap)
    {
        TrapCollection *currTrpc = qobject_cast<TrapCollection* >(
                    static_cast<BaseItem* >(itemOfAdd->QTreeWidgetItem::parent()->parent())
                    );

        Trap *trap = prjMgr->arrayOfProjects.value(projectID)
                ->trapsInProjectCommonMap.value(currTrpc->ID_trap->ID);

        trap->IDofWellsRecommendedForExplorationOfTrap.append(newWell->wellID);

        currTrpc->trapPart_Wells->addSubElement(newWColl);
    }

    refreshWellListOnProject();

    stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeEmpty);
}

void MainWnd::slotAddStructureElementInCurrentProjectAtProjectsTreeWgt(int currentProject, BaseItem *itemOfAdd)
{
    if (itemOfAdd->projectID != currentProject)
    {
        QMessageBox::critical(this,
                              trUtf8("СБОЙ!"),
                              trUtf8("Несоответствие идентификаторов проекта!"));
        return;
    }

    prjMgr->currentProjectNumber = currentProject;

    QList<int> currPrjStructuresKeys = prjMgr->arrayOfProjects.value(currentProject)->structuresInProject.keys();

    int structKeysMaxVal = 0;

    for (int i = 0; i < currPrjStructuresKeys.count(); ++i)
    {
        if (structKeysMaxVal < currPrjStructuresKeys.at(i)) structKeysMaxVal = currPrjStructuresKeys.at(i);
    }

    structKeysMaxVal++;

    Structure *newStructure = new Structure;

    prjMgr->arrayOfProjects.value(currentProject)->structuresInProject.insert(structKeysMaxVal, newStructure);

    StructureCollection *newStructureItem = new StructureCollection(structKeysMaxVal, currentProject,
                                                                    newStructure->getNameStruct());

    ProjectRootCollection *currPrjItem = projectsTreeWgt->retProjectItemByProjectID(currentProject);

    currPrjItem->projectPart_Structures->addSubElement(newStructureItem);

//    projectsTreeWgt->clearSelection();

    stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeEmpty);
}

void MainWnd::slotAddTrapElementInCurrentProjectAtProjectsTreeWgt(int currentProject,
                                                                  int currentStructure,
                                                                  BaseItem *itemOfAdd)
{
    if (itemOfAdd->projectID != currentProject)
    {
        QMessageBox::critical(this,
                              trUtf8("СБОЙ!"),
                              trUtf8("Несоответствие идентификаторов проекта!"));
        return;
    }

    prjMgr->currentProjectNumber = currentProject;

    QList<int> currPrjTrapsKeys = prjMgr->arrayOfProjects.value(currentProject)->trapsInProjectCommonMap.keys();

    int trapKeysMaxVal = 0;

    for (int i = 0; i < currPrjTrapsKeys.count(); ++i)
    {
        if (trapKeysMaxVal < currPrjTrapsKeys.at(i)) trapKeysMaxVal = currPrjTrapsKeys.at(i);
    }

    trapKeysMaxVal++;

    Trap *newTrap = new Trap;

    newTrap->baseData->numberInTrapsMap = trapKeysMaxVal;

    prjMgr->arrayOfProjects.value(currentProject)->trapsInProjectCommonMap.insert(trapKeysMaxVal, newTrap);

    TrapCollection *newTrapCollItem = new TrapCollection(trapKeysMaxVal, currentProject,
                                                         newTrap->nameTrap);

    ProjectRootCollection *currPrjItem = projectsTreeWgt->retProjectItemByProjectID(currentProject);

    StructureCollection *sc =
            currPrjItem->projectPart_Structures->retStructureCollectionElementByStructureID(currentStructure);

    prjMgr->arrayOfProjects.value(currentProject)
            ->structuresInProject.value(sc->ID_structure->ID)
            ->trapsNumbersFromCommonMap_onStructure.append(trapKeysMaxVal);

    sc->structurePart_Traps->addSubElement(newTrapCollItem);

    stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeEmpty);
}

void MainWnd::slotSelectProjectIDThroughClickOnPartition(int projectID)
{
    prjMgr->currentProjectNumber = projectID;

    Project *crPrj = prjMgr->arrayOfProjects.value(projectID);

    stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeProjectProperties);

    slotChangeWindowTitle(prjMgr->arrayOfProjects.value(projectID)->getProjectName());

    setConnectedSlotsOfChangingsInBaseWidgets(false, SpecialWidget::typeProjectProperties);

    BaseProjectPropertiesWidget *wgt = qobject_cast<BaseProjectPropertiesWidget* >(
                stackOfWidgets->widget((int) SpecialWidget::typeProjectProperties)
                );

//    this->slot

    wgt->lned_PrjID->setText(QString::number(projectID));

    wgt->lned_PrjName->setText(crPrj->getProjectName());

    wgt->te_Description->setPlainText(crPrj->getDescription());

    setConnectedSlotsOfChangingsInBaseWidgets(true, SpecialWidget::typeProjectProperties);
}

void MainWnd::slotShowEmptyWidget()
{
    stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeEmpty);
}

void MainWnd::slotShowSelectedStratumInfo(int projectID, int stratumID)
{
    prjMgr->currentProjectNumber = projectID;

    if (stratumID < 0) return;

    Stratum *stratum = prjMgr->arrayOfProjects.value(projectID)
            ->stratumsNamesByNumber.value(stratumID);

    stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeStratumProperties);

    setConnectedSlotsOfChangingsInBaseWidgets(false, SpecialWidget::typeStratumProperties);

    BaseStratumPropertiesWidget *wgt = qobject_cast<BaseStratumPropertiesWidget* >(
                stackOfWidgets->widget((int) SpecialWidget::typeStratumProperties)
                );

    wgt->lned_stratumID->setText(QString::number(stratumID));
    wgt->lned_stratumName->setText(
                stratum->nameOfStratum
                );
    wgt->te_DescriptionStratum->setPlainText(stratum->description);

    if (!qobject_cast<ProjectRootCollection* >(
                static_cast<BaseItem* >(projectsTreeWgt->currentItem()->parent()->parent())
                ))
    {
        wgt->lned_stratumName->setReadOnly(true);
        wgt->te_DescriptionStratum->setReadOnly(true);
    }
    else
    {
        wgt->lned_stratumName->setReadOnly(false);
        wgt->te_DescriptionStratum->setReadOnly(false);
    }

    setConnectedSlotsOfChangingsInBaseWidgets(true, SpecialWidget::typeStratumProperties);
}

void MainWnd::slotShowSelectedStructureInfo(int projectID, int structureID)
{
    prjMgr->currentProjectNumber = projectID;

    Structure *structure = prjMgr->arrayOfProjects.value(projectID)
            ->structuresInProject.value(structureID);

    stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeStructureProperties);

    setConnectedSlotsOfChangingsInBaseWidgets(false, SpecialWidget::typeStructureProperties);

    BaseStructurePropertiesWidget *wgt = qobject_cast<BaseStructurePropertiesWidget* >(
                stackOfWidgets->widget((int) SpecialWidget::typeStructureProperties)
                );

    wgt->lned_structID->setText(QString::number(structureID));
    wgt->lned_structName->setText(
                structure->getNameStruct()
                );
    wgt->te_DescriptionStruct->setPlainText(structure->getDescription());

    setConnectedSlotsOfChangingsInBaseWidgets(true, SpecialWidget::typeStructureProperties);
}

void MainWnd::slotShowSelectedWellInfo(int projectID, int wellID)
{
    prjMgr->currentProjectNumber = projectID;

    Well *well = prjMgr->arrayOfProjects.value(projectID)
            ->wellsInProject.value(wellID);

    stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeWellProperties);

    setConnectedSlotsOfChangingsInBaseWidgets(false, SpecialWidget::typeWellProperties);

    BaseWellPropertiesWidget *wgt = qobject_cast<BaseWellPropertiesWidget* >(
                stackOfWidgets->widget((int) SpecialWidget::typeWellProperties)
                );

    wgt->lned_NegResProb->setText(QString::number(well->negativeResultProbability));
    wgt->lned_wellName->setText(well->wellName);
    wgt->lned_ID->setText(QString::number(wellID));
    wgt->cmb_ID->clear();

    WellsPartition *wpcur_parent =
            qobject_cast<WellsPartition* >(static_cast<BaseItem* >(projectsTreeWgt->currentItem()->parent()));

//    TrapCollection *tc_cur =
//            qobject_cast<TrapCollection* >(static_cast<BaseItem* >(
//                                               projectsTreeWgt->currentItem()->parent()->parent()));

    StructureCollection *sc_curr;

    if (wpcur_parent->text(0) == trUtf8("Скважины на структуре"))
    {
        wgt->setElementsAreReadOnly(true);

//        sc_curr = qobject_cast<StructureCollection* >(static_cast<BaseItem* >(
//                                                          projectsTreeWgt->currentItem()->parent()
//                                                          ->parent()
//                                                          ));
        sc_curr = NULL; // все равно кнопка вызова смены ID через ComboBox не активна

    }
    else if (wpcur_parent->text(0) == trUtf8("Скважины проекта (список)"))
    {
        wgt->setElementsAreReadOnly(true);

        sc_curr = NULL;
    }
    else
    {
        wgt->setElementsAreReadOnly(false);

        sc_curr = qobject_cast<StructureCollection* >(static_cast<BaseItem* >(
                                                   projectsTreeWgt->currentItem()->parent()->parent()
                                                        ->parent()->parent()));
    }

    if (sc_curr)
    {
        QList<int> keysIdWellOnCurrStructure = QList<int>();

        Project *project = prjMgr->arrayOfProjects.value(projectID);

        keysIdWellOnCurrStructure = project->retWellsIDsOnStructure(sc_curr->ID_structure->ID).toList();

        int indexItemInCmbForSelect = 0;

        for (int i = 0; i < keysIdWellOnCurrStructure.count(); ++i)
        {
            wgt->cmb_ID->addItem(
                        QString::number(
                            keysIdWellOnCurrStructure.at(i)
                            )
                        + trUtf8(" (id), имя: ") // 12 знаков
                        + prjMgr->arrayOfProjects.value(projectID)
                        ->wellsInProject.value(
                            keysIdWellOnCurrStructure.at(i)
                            )
                        ->wellName
                        );

            if (keysIdWellOnCurrStructure.at(i) == wellID)
            {
                indexItemInCmbForSelect = wgt->cmb_ID->count()-1;
                //wgt->oldID = wellID;
            }
        }

        wgt->cmb_ID->setCurrentIndex(indexItemInCmbForSelect);
    }

    wgt->te_Description->setPlainText(well->wellDescription);

    setConnectedSlotsOfChangingsInBaseWidgets(true, SpecialWidget::typeWellProperties);
}

void MainWnd::slotShowSelectedTrapInfo(int projectID, int trapID, TrapCollection *tc)
{
    prjMgr->currentProjectNumber = projectID;

    Trap *trap = prjMgr->arrayOfProjects.value(projectID)
            ->trapsInProjectCommonMap.value(trapID);

    stackOfWidgets->setCurrentIndex((int) SpecialWidget::typeTrapProperties);

    setConnectedSlotsOfChangingsInBaseWidgets(false, SpecialWidget::typeTrapProperties);

    BaseTrapPropertiesWidget *wgt = qobject_cast<BaseTrapPropertiesWidget* >(
                stackOfWidgets->widget((int) SpecialWidget::typeTrapProperties)
                );

    wgt->lineEdit_IDtrap->setText(QString::number(prjMgr->arrayOfProjects.value(projectID)
                                                  ->trapsInProjectCommonMap.key(trap)));

    wgt->lineEdit_StructName->setText(
                tc->retStructureCollectionElementByTrapCollItem()
                ->name
                );

    wgt->lineEdit_TrapName->setText(
                trap->nameTrap
                );

    wgt->cmb_StratumIndex->clear();
    int indexForStratumOfCurrentTrap = -1;
    wgt->cmb_StratumIndex->insertItem(0, QString());

    for (int sti = 0; sti < prjMgr->arrayOfProjects.value(projectID)
         ->stratumsNamesByNumber.keys().count(); ++sti)
    {
        int index = sti + 1; // учитывая вставку первой пустой строки

        int stratID = trap->numberOfStratumInCommonMapOfProject;

        int stratIDat_sti = prjMgr->arrayOfProjects.value(projectID)
                ->stratumsNamesByNumber.keys().at(sti);

        wgt->cmb_StratumIndex->insertItem(index,
                                          prjMgr->arrayOfProjects.value(projectID)
                                                   ->stratumsNamesByNumber.value(stratIDat_sti)
                                          ->nameOfStratum
                                          );

        if (stratID == stratIDat_sti)
        {
            indexForStratumOfCurrentTrap = index;
        }
    }

    wgt->cmb_StratumIndex->setCurrentIndex(indexForStratumOfCurrentTrap);

    wgt->cmb_AllocationType->clear();
    for (int i = 0; i < trap->allocationType->getMapOfAllocateTypes().keys().size(); ++i)
    {
        wgt->cmb_AllocationType->insertItem(trap->allocationType->getMapOfAllocateTypes()
                                            .keys().at(i),
                                            trap->allocationType->getAllocTypeStringByKey(
                                                trap->allocationType->getMapOfAllocateTypes()
                                                .keys().at(i)
                                                ));
    }
    wgt->cmb_AllocationType->setCurrentIndex(trap->allocationType->getAllocTypeInt_Key());

    wgt->lineEdit_AreaAvg->setText(QString::number(trap->baseData->Area.average));
    wgt->lineEdit_AreaMax->setText(QString::number(trap->baseData->Area.maximum));
    wgt->lineEdit_AreaMin->setText(QString::number(trap->baseData->Area.minimum));

    wgt->lineEdit_EffThickAvg->setText(QString::number(trap->baseData->EffectiveThick.average));
    wgt->lineEdit_EffThickMax->setText(QString::number(trap->baseData->EffectiveThick.maximum));
    wgt->lineEdit_EffThickMin->setText(QString::number(trap->baseData->EffectiveThick.minimum));

    wgt->lineEdit_PorosityCoeffAvg->setText(QString::number(trap->baseData->PorosityCoeff.average));
    wgt->lineEdit_PorosityCoeffMax->setText(QString::number(trap->baseData->PorosityCoeff.maximum));
    wgt->lineEdit_PorosityCoeffMin->setText(QString::number(trap->baseData->PorosityCoeff.minimum));

    wgt->lineEdit_OilSaturationCoeffAvg->setText(QString::number(trap->baseData->OilSaturationCoeff.average));
    wgt->lineEdit_OilSaturationCoeffMax->setText(QString::number(trap->baseData->OilSaturationCoeff.maximum));
    wgt->lineEdit_OilSaturationCoeffMin->setText(QString::number(trap->baseData->OilSaturationCoeff.minimum));

    wgt->lineEdit_OilDensityAvg->setText(QString::number(trap->baseData->DensityOfOil.average));
    wgt->lineEdit_OilDensityMax->setText(QString::number(trap->baseData->DensityOfOil.maximum));
    wgt->lineEdit_OilDensityMin->setText(QString::number(trap->baseData->DensityOfOil.minimum));

    wgt->lineEdit_OilShrinkingAvg->setText(QString::number(trap->baseData->ShrinkingCoeff.average));
    wgt->lineEdit_OilShrinkingMax->setText(QString::number(trap->baseData->ShrinkingCoeff.maximum));
    wgt->lineEdit_OilShrinkingMin->setText(QString::number(trap->baseData->ShrinkingCoeff.minimum));

    wgt->lineEdit_KIN->setText(QString::number(trap->baseData->determinedKIN));

    wgt->lineEdit_ProbaboilityOfDeposite->setText(QString::number(trap->baseData->probabilityOfDepositExisting));

    setConnectedSlotsOfChangingsInBaseWidgets(true, SpecialWidget::typeTrapProperties);
}

void MainWnd::slotFillProjectItemsFromItsData(int projectID)
{
    projectsTreeWgt->slotCreateEmptyProjectItems(projectID);

    ProjectRootCollection *prc = projectsTreeWgt->retProjectItemByProjectID(projectID);

    Project *project = prjMgr->arrayOfProjects.value(projectID);

    prc->slotChangeName(project->getProjectName());

    prjMgr->currentProjectNumber = projectID;

    // слои

    for (int i_stratum = 0; i_stratum < project->stratumsNamesByNumber.keys().size(); ++i_stratum)
    {
        int id_stratum = project->stratumsNamesByNumber.keys().at(i_stratum);

        Stratum *stratum = project->stratumsNamesByNumber.value(id_stratum);

        StratumField *newStratumItem = new StratumField(id_stratum, projectID,
                                                        stratum->nameOfStratum,
                                                        projectsTreeWgt
                                                        );

        newStratumItem->slotChangeID(id_stratum);

        newStratumItem->projectID = projectID;

        newStratumItem->slotChangeName(stratum->nameOfStratum);

        prc->projectPart_Stratums->addSubElement(newStratumItem);
    }

    // структуры, в структурах - ловушки, в ловушках - скважины

    for (int i_structure = 0; i_structure < project->structuresInProject.keys().size(); ++i_structure)
    {
        int id_structure = project->structuresInProject.keys().at(i_structure);

        Structure *structure = project->structuresInProject.value(id_structure);

        StructureCollection *newStructureItem = new StructureCollection(id_structure,
                                                                        projectID,
                                                                        structure->getNameStruct(),
                                                                        projectsTreeWgt);

        newStructureItem->slotChangeName(structure->getNameStruct());
        newStructureItem->Begin_decsription->slotSetItemToolTipText(structure->getDescription());
        newStructureItem->ID_structure->slotChangeID(id_structure);
        newStructureItem->projectID = projectID;

        prc->projectPart_Structures->addSubElement(newStructureItem);

        // для построения списка слоев на структуре

        QVector<StratumField* > vecOfStratumFieldsOnStruct = QVector<StratumField* >();

        for (int i_trap = 0; i_trap < structure->trapsNumbersFromCommonMap_onStructure.size(); ++i_trap)
        {
            int id_trap = structure->trapsNumbersFromCommonMap_onStructure.at(i_trap);

            Trap *trap = project->trapsInProjectCommonMap.value(id_trap);

            TrapCollection *newTrapItem = new TrapCollection(id_trap,
                                                             projectID,
                                                             trap->nameTrap,
                                                             projectsTreeWgt);

            newTrapItem->projectID = projectID;
            newTrapItem->slotChangeName(trap->nameTrap);
            newTrapItem->ID_trap->slotChangeID(id_trap);
            newTrapItem->Begin_decsription->slotSetItemToolTipText(trap->description);
            newTrapItem->StratumOfTrap->slotChangeName(trap->nameOfStratum);
            int id_stratum = 0;
            for (int i_stratum = 0; i_stratum < project->stratumsNamesByNumber.keys().size(); ++i_stratum)
            {
                int id_curr = project->stratumsNamesByNumber.keys().at(i_stratum);
                Stratum *stratum = project->stratumsNamesByNumber.value(id_curr);

                if (trap->nameOfStratum == stratum->nameOfStratum)
                {
                    id_stratum = id_curr;
                    break;
                }
            }
            newTrapItem->StratumOfTrap->slotChangeID(id_stratum);

            StratumField *stratumFld = new StratumField(newTrapItem->StratumOfTrap->ID,
                                                        projectID,
                                                        newTrapItem->StratumOfTrap->nameOfStratum,
                                                        projectsTreeWgt);

            stratumFld->slotChangeID(newTrapItem->StratumOfTrap->ID);

            stratumFld->projectID = projectID;

            stratumFld->slotChangeName(newTrapItem->StratumOfTrap->nameOfStratum);

            if (!vecOfStratumFieldsOnStruct.contains(stratumFld))
                vecOfStratumFieldsOnStruct.append(stratumFld);

            newStructureItem->structurePart_Traps->addSubElement(newTrapItem);

            for (int i_well = 0; i_well < trap->IDofWellsRecommendedForExplorationOfTrap.size(); ++i_well)
            {
                int id_well = trap->IDofWellsRecommendedForExplorationOfTrap.at(i_well);

                Well *well = project->wellsInProject.value(id_well);

                WellCollection *newWellItem = new WellCollection(id_well,
                                                                 projectID,
                                                                 well->wellName,
                                                                 projectsTreeWgt);

                newWellItem->projectID = projectID;
                newWellItem->slotChangeName(well->wellName);
                newWellItem->ID_well->slotChangeID(id_well);
                newWellItem->Begin_decsription->slotSetItemToolTipText(well->wellDescription);
                newWellItem->elementRisk->slotSetRiskItemText(well->negativeResultProbability);

                newTrapItem->trapPart_Wells->addSubElement(newWellItem);
            }
        }

        for (int i_sf = 0; i_sf < vecOfStratumFieldsOnStruct.size(); ++i_sf)
        {
            newStructureItem->structurePart_Stratums_ReadOnly->addSubElement(
                        vecOfStratumFieldsOnStruct.at(i_sf)
                        );
        }
    }

    // списки скважин по структурам и проекту

    refreshWellListOnProject();
}
