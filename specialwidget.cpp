#include "specialwidget.h"

SpecialWidget::SpecialWidget(QWidget *parent) : QWidget(parent)
{
    QPalette plt = this->palette();
    plt.setColor(QPalette::Background, Qt::lightGray);
    setPalette(plt);
}

void SpecialWidget::setEnabledAndVisibleButtonsOfCreatingMode(bool yes)
{
    btnCreateCanceling->setEnabled(yes);
    btnCreateCanceling->setVisible(yes);

    btnCreateIsOk->setEnabled(yes);
    btnCreateIsOk->setVisible(yes);
}

void SpecialWidget::initMainLayout()
{
    _lt_main = new QVBoxLayout;
}

void SpecialWidget::initLayoutForCreateMode()
{
    _ltEnd_ButtonsForCreateMode = new QHBoxLayout;
    btnCreateCanceling = new QPushButton(trUtf8("Создать ???"));
    btnCreateIsOk = new QPushButton(trUtf8("Отмена"));
    _ltEnd_ButtonsForCreateMode->addWidget(btnCreateCanceling);
    _ltEnd_ButtonsForCreateMode->addWidget(btnCreateIsOk);
}

void SpecialWidget::addLayoutForCreateModeInMainLayout()
{
    _lt_main->addLayout(_ltEnd_ButtonsForCreateMode);
}

BaseTrapPropertiesWidget::BaseTrapPropertiesWidget(QWidget *parent) : SpecialWidget(parent)
{
    initMainLayout();
    initLayoutForCreateMode();
    this->setLayout(_lt_main);

    _lt1_ParamsStructName = new QHBoxLayout; _lt_main->addLayout(_lt1_ParamsStructName);
    lblStructName = new QLabel(trUtf8("Название структуры:  ")); _lt1_ParamsStructName->addWidget(lblStructName);
    lineEdit_StructName = new QLineEdit; _lt1_ParamsStructName->addWidget(lineEdit_StructName);
    lineEdit_StructName->setReadOnly(true);

    _lt2_ParamsTrapName = new QHBoxLayout; _lt_main->addLayout(_lt2_ParamsTrapName);
    lblTrapName = new QLabel(trUtf8("Название ловушки:  ")); _lt2_ParamsTrapName->addWidget(lblTrapName);
    lineEdit_TrapName = new QLineEdit; _lt2_ParamsTrapName->addWidget(lineEdit_TrapName);    
    connect(lineEdit_TrapName,
            SIGNAL(editingFinished()),
            this,
            SIGNAL(signalSomethingChanged()));

    _lt3_ParamsStratumIndex = new QHBoxLayout; _lt_main->addLayout(_lt3_ParamsStratumIndex);
    lblStratumIndex = new QLabel(trUtf8("Индекс пласта:  ")); _lt3_ParamsStratumIndex->addWidget(lblStratumIndex);
    cmb_StratumIndex = new QComboBox; _lt3_ParamsStratumIndex->addWidget(cmb_StratumIndex);
    lblTrapID = new QLabel(trUtf8("ID ловушки: ")); _lt3_ParamsStratumIndex->addWidget(lblTrapID);
    lineEdit_IDtrap = new QLineEdit; _lt3_ParamsStratumIndex->addWidget(lineEdit_IDtrap);
    cmb_StratumIndex->setEditable(false);
    lineEdit_IDtrap->setReadOnly(true);
    connect(cmb_StratumIndex,
            SIGNAL(currentIndexChanged(int)),
            this,
            SIGNAL(signalSomethingChanged()));
    connect(cmb_StratumIndex,
            SIGNAL(editTextChanged(QString)),
            this,
            SIGNAL(signalSomethingChanged()));

    _lt4_AllocationType = new QHBoxLayout; _lt_main->addLayout(_lt4_AllocationType);
    lblAlloctionType = new QLabel(trUtf8("Тип распределения:  ")); _lt4_AllocationType->addWidget(lblAlloctionType);
    cmb_AllocationType = new QComboBox; _lt4_AllocationType->addWidget(cmb_AllocationType);

    cmb_AllocationType->addItems(QStringList(trUtf8("для теста")));
    connect(cmb_AllocationType,
            SIGNAL(currentIndexChanged(int)),
            this,
            SIGNAL(signalSomethingChanged()));

    lblVariativeEstimationParameters = new QLabel(trUtf8("Параметры оценки по объемной формуле (вариативные): "));
    _lt_main->addWidget(lblVariativeEstimationParameters);

    _gridLt_variativeEstimationParams = new QGridLayout; _lt_main->addLayout(_gridLt_variativeEstimationParams);
    _gridLt_variativeEstimationParams->addWidget(new QWidget, 0, 0);
    _gridLt_variativeEstimationParams->addWidget(new QLabel(trUtf8("Площадь,\nкм^2")), 0, 1, 1, 1, Qt::AlignCenter);
    _gridLt_variativeEstimationParams->addWidget(new QLabel(trUtf8("Эффективная\nнефтенасыщенная\nтолщина, м")), 0, 2, 1, 1, Qt::AlignCenter);
    _gridLt_variativeEstimationParams->addWidget(new QLabel(trUtf8("Кп")), 0, 3, 1, 1, Qt::AlignCenter);
    _gridLt_variativeEstimationParams->addWidget(new QLabel(trUtf8("Кн")), 0, 4, 1, 1, Qt::AlignCenter);
    _gridLt_variativeEstimationParams->addWidget(new QLabel(trUtf8("Плотность,\nг/см^3")), 0, 5, 1, 1, Qt::AlignCenter);
    _gridLt_variativeEstimationParams->addWidget(new QLabel(trUtf8("Усадка нефти")), 0, 6, 1, 1, Qt::AlignCenter);
    _gridLt_variativeEstimationParams->addWidget(new QLabel(trUtf8("Минимум  ->")), 1, 0, 1, 1, Qt::AlignCenter);
    _gridLt_variativeEstimationParams->addWidget(new QLabel(trUtf8("Среднее  ->")), 2, 0, 1, 1, Qt::AlignCenter);
    _gridLt_variativeEstimationParams->addWidget(new QLabel(trUtf8("Максимум ->")), 3, 0, 1, 1, Qt::AlignCenter);

    lineEdit_AreaMin = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_AreaMin);
    connect(lineEdit_AreaMin, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_AreaMin, 1, 1);

    lineEdit_AreaAvg = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_AreaAvg);
    connect(lineEdit_AreaAvg, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_AreaAvg, 2, 1);

    lineEdit_AreaMax = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_AreaMax);
    connect(lineEdit_AreaMax, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_AreaMax, 3, 1);

    lineEdit_EffThickMin = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_EffThickMin);
    connect(lineEdit_EffThickMin, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_EffThickMin, 1, 2);

    lineEdit_EffThickAvg = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_EffThickAvg);
    connect(lineEdit_EffThickAvg, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_EffThickAvg, 2, 2);

    lineEdit_EffThickMax = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_EffThickMax);
    connect(lineEdit_EffThickMax, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_EffThickMax, 3, 2);

    lineEdit_PorosityCoeffMin = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_PorosityCoeffMin);
    connect(lineEdit_PorosityCoeffMin, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_PorosityCoeffMin, 1, 3);

    lineEdit_PorosityCoeffAvg = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_PorosityCoeffAvg);
    connect(lineEdit_PorosityCoeffAvg, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_PorosityCoeffAvg, 2, 3);

    lineEdit_PorosityCoeffMax = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_PorosityCoeffMax);
    connect(lineEdit_PorosityCoeffMax, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_PorosityCoeffMax, 3, 3);

    lineEdit_OilSaturationCoeffMin = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_OilSaturationCoeffMin);
    connect(lineEdit_OilSaturationCoeffMin, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_OilSaturationCoeffMin, 1, 4);

    lineEdit_OilSaturationCoeffAvg = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_OilSaturationCoeffAvg);
    connect(lineEdit_OilSaturationCoeffAvg, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_OilSaturationCoeffAvg, 2, 4);

    lineEdit_OilSaturationCoeffMax = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_OilSaturationCoeffMax);
    connect(lineEdit_OilSaturationCoeffMax, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_OilSaturationCoeffMax, 3, 4);

    lineEdit_OilDensityMin = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_OilDensityMin);
    connect(lineEdit_OilDensityMin, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_OilDensityMin, 1, 5);

    lineEdit_OilDensityAvg = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_OilDensityAvg);
    connect(lineEdit_OilDensityAvg, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_OilDensityAvg, 2, 5);

    lineEdit_OilDensityMax = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_OilDensityMax);
    connect(lineEdit_OilDensityMax, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_OilDensityMax, 3, 5);

    lineEdit_OilShrinkingMin = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_OilShrinkingMin);
    connect(lineEdit_OilShrinkingMin, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_OilShrinkingMin, 1, 6);

    lineEdit_OilShrinkingAvg = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_OilShrinkingAvg);
    connect(lineEdit_OilShrinkingAvg, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_OilShrinkingAvg, 2, 6);

    lineEdit_OilShrinkingMax = new QLineEdit(this); //listTextFieldsEstimateParamsVariations.append(lineEdit_OilShrinkingMax);
    connect(lineEdit_OilShrinkingMax, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    _gridLt_variativeEstimationParams->addWidget(lineEdit_OilShrinkingMax, 3, 6);

    _lt5_KINdetermined = new QHBoxLayout; _lt_main->addLayout(_lt5_KINdetermined);
    lblKIN = new QLabel(trUtf8("КИН (детерминированный):  ")); _lt5_KINdetermined->addWidget(lblKIN);
    lineEdit_KIN = new QLineEdit; _lt5_KINdetermined->addWidget(lineEdit_KIN);
    connect(lineEdit_KIN,
            SIGNAL(editingFinished()),
            this,
            SIGNAL(signalSomethingChanged()));

    _lt_Probabilities = new QHBoxLayout; _lt_main->addLayout(_lt_Probabilities);
    lblProbabilityDeposit = new QLabel(trUtf8("Вероятность наличия залежи: ")); _lt_Probabilities->addWidget(lblProbabilityDeposit);
    lineEdit_ProbaboilityOfDeposite = new QLineEdit; _lt_Probabilities->addWidget(lineEdit_ProbaboilityOfDeposite);
    connect(lineEdit_ProbaboilityOfDeposite,
            SIGNAL(editingFinished()),
            this,
            SIGNAL(signalSomethingChanged()));

    te_Comment = new QTextEdit;
    te_Comment->setMaximumHeight(50);
    te_Comment->setPlainText(trUtf8("Здесь можно ввести комментарий к ловушке..."));
    connect(te_Comment,
            SIGNAL(textChanged()),
            this,
            SIGNAL(signalSomethingChanged()));

    te_Output = new QTextEdit;
    connect(te_Output,
            SIGNAL(textChanged()),
            this,
            SIGNAL(signalSomethingChanged()));

    _lt_main->addWidget(te_Comment, 2);
    _lt_main->addWidget(te_Output, 7);

//    refreshComment = new QAction(trUtf8("ОБНОВИТЬ КОММЕНТАРИЙ"), this);
//    connect(refreshComment,
//            SIGNAL(triggered(bool)),
//            this,
//            SIGNAL(signalToRefreshComment()));
//    QMenu *customMenu = te_Comment->createStandardContextMenu();
//    customMenu->insertAction(customMenu->actions().at(0), refreshComment);
//    te_Comment->setContextMenuPolicy(Qt::ActionsContextMenu);
//    te_Comment->addActions(customMenu->actions());

    te_Output->setReadOnly(true);

    //addLayoutForCreateModeInMainLayout();
}

BaseStratumPropertiesWidget::BaseStratumPropertiesWidget(QWidget *parent) : SpecialWidget(parent)
{
    initMainLayout();
    initLayoutForCreateMode();
    this->setLayout(_lt_main);

    _lt1_ParamsStratumName = new QHBoxLayout;
    _lt_main->addLayout(_lt1_ParamsStratumName);

    lblSid = new QLabel(trUtf8("Уникальный идентификатор пласта: "));
    lblSname = new QLabel(trUtf8("Индекс пласта: "));
    lblDesc = new QLabel(trUtf8("Описание пласта"));

    lned_stratumID = new QLineEdit; lned_stratumID->setReadOnly(true);
    lned_stratumName = new QLineEdit;
    te_DescriptionStratum = new QTextEdit; //te_DescriptionStratum->setReadOnly(true);

    _lt1_ParamsStratumName->addWidget(lblSid);
    _lt1_ParamsStratumName->addWidget(lned_stratumID);
    _lt1_ParamsStratumName->addWidget(lblSname);
    _lt1_ParamsStratumName->addWidget(lned_stratumName);

    _lt_main->addWidget(lblDesc);
    _lt_main->addWidget(te_DescriptionStratum);

    connect(lned_stratumName, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    connect(te_DescriptionStratum, SIGNAL(textChanged()), this, SIGNAL(signalSomethingChanged()));
}

BaseWellPropertiesWidget::BaseWellPropertiesWidget(QWidget *parent) : SpecialWidget(parent)
{
    ID_valueChanged = false;
    oldID = -1;

    initMainLayout();
    initLayoutForCreateMode();
    this->setLayout(_lt_main);

    _lt1_ParamsWellName = new QHBoxLayout;
    _lt2_ParamsWellID = new QHBoxLayout;
    _lt3_ParamsNegResProb = new QHBoxLayout;
    _lt_WellIDCmbForDialog = new QHBoxLayout;
    _lt_main->addLayout(_lt1_ParamsWellName);
    _lt_main->addLayout(_lt2_ParamsWellID);
    _lt_main->addLayout(_lt3_ParamsNegResProb);

    lblWellName = new QLabel(trUtf8("Название скважины: "));
    lblID = new QLabel(trUtf8("ID скважины: "));
    lblNegResProb = new QLabel(trUtf8("Вероятность отрицательного результата (технические риски): "));

    lned_wellName = new QLineEdit;
    lned_ID = new QLineEdit;
    lned_ID->setReadOnly(true);
    btn_dialogForChangeExecuting = new QPushButton(trUtf8("Диалог смены ID"));
    cmb_ID = new QComboBox;
    cmb_ID->setEditable(false);
    btn_changeID = new QPushButton(trUtf8("Сменить ID"));
    btn_CancellingOfChange = new QPushButton(trUtf8("Отмена"));
    lned_NegResProb = new QLineEdit;
    te_Description = new QTextEdit;

    _lt1_ParamsWellName->addWidget(lblWellName);
    _lt1_ParamsWellName->addWidget(lned_wellName);

    _lt2_ParamsWellID->addWidget(lblID);
    _lt2_ParamsWellID->addWidget(lned_ID);
    _lt2_ParamsWellID->addWidget(btn_dialogForChangeExecuting);

    _lt3_ParamsNegResProb->addWidget(lblNegResProb);
    _lt3_ParamsNegResProb->addWidget(lned_NegResProb);

    dlg.setLayout(_lt_WellIDCmbForDialog);

    dlg.setWindowTitle(trUtf8("Смена ID скважины"));

    connect(btn_changeID, SIGNAL(clicked()), &dlg, SLOT(accept()));
    connect(btn_CancellingOfChange, SIGNAL(clicked()), &dlg, SLOT(reject()));

    _lt_WellIDCmbForDialog->addWidget(cmb_ID);
    _lt_WellIDCmbForDialog->addWidget(btn_changeID);
    _lt_WellIDCmbForDialog->addWidget(btn_CancellingOfChange);

    _lt_main->addWidget(te_Description);

    connect(btn_dialogForChangeExecuting, SIGNAL(clicked()), this, SLOT(slotDialogOfWellIdChangingToExec()));
    connect(lned_wellName, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    connect(lned_NegResProb, SIGNAL(editingFinished()), this, SIGNAL(signalSomethingChanged()));
    connect(te_Description, SIGNAL(textChanged()), this, SIGNAL(signalSomethingChanged()));
    setCmbSignalsDisconnected(false);
}

void BaseWellPropertiesWidget::setElementsAreReadOnly(bool ro)
{
    lned_NegResProb->setReadOnly(ro);
    lned_wellName->setReadOnly(ro);
    te_Description->setReadOnly(ro);
    cmb_ID->setDisabled(ro);
    lned_ID->setReadOnly(ro);
    btn_dialogForChangeExecuting->setDisabled(ro);
}

void BaseWellPropertiesWidget::setCmbSignalsDisconnected(bool disconnected)
{
    if (!disconnected)
    {
        connect(cmb_ID, SIGNAL(currentIndexChanged(int)), this, SIGNAL(signalSomethingChanged()));
    }
    else
    {
        disconnect(cmb_ID, SIGNAL(currentIndexChanged(int)), this, SIGNAL(signalSomethingChanged()));
    }
}

int BaseWellPropertiesWidget::retWellIDByComboBox()
{
    if (dlg.exec() == QDialog::Accepted)
    {
        return cmb_ID->currentText().split(trUtf8(" (id), имя: ")).at(0).toInt();
    }

    return 0;
}

void BaseWellPropertiesWidget::slotDialogOfWellIdChangingToExec()
{
    int res = retWellIDByComboBox();

    if (res != 0)
    {
//        lned_ID->setText(QString::number(res));

        emit signalSomethingChanged();
    }
}

BaseStructurePropertiesWidget::BaseStructurePropertiesWidget(QWidget *parent) : SpecialWidget(parent)
{
    initMainLayout();
    initLayoutForCreateMode();
    this->setLayout(_lt_main);

    _lt1_ParamsStructName = new QHBoxLayout; _lt_main->addLayout(_lt1_ParamsStructName);

    lblStructName = new QLabel(trUtf8("Название структуры: ")); _lt1_ParamsStructName->addWidget(lblStructName);
    lned_structName = new QLineEdit; _lt1_ParamsStructName->addWidget(lned_structName);

    lblStructID = new QLabel(trUtf8("Уникальный идентификатор структуры: ")); _lt1_ParamsStructName->addWidget(lblStructID);
    lned_structID = new QLineEdit; _lt1_ParamsStructName->addWidget(lned_structID);

    lned_structID->setReadOnly(true);

    lblDescription = new QLabel(trUtf8("Описание структуры"));
    _lt_main->addWidget(lblDescription);

    te_DescriptionStruct = new QTextEdit;
    _lt_main->addWidget(te_DescriptionStruct);

    connect(lned_structName,
            SIGNAL(editingFinished()),
            this,
            SIGNAL(signalSomethingChanged()));

    connect(te_DescriptionStruct,
            SIGNAL(textChanged()),
            this,
            SIGNAL(signalSomethingChanged()));
}


BaseProjectPropertiesWidget::BaseProjectPropertiesWidget(QWidget *parent) : SpecialWidget(parent)
{
    initMainLayout();
    initLayoutForCreateMode();
    this->setLayout(_lt_main);

    _lt1_ParamsProjectName = new QHBoxLayout; _lt_main->addLayout(_lt1_ParamsProjectName);
    lblPrjName = new QLabel(trUtf8("Название проекта: ")); _lt1_ParamsProjectName->addWidget(lblPrjName);
    lned_PrjName = new QLineEdit; _lt1_ParamsProjectName->addWidget(lned_PrjName);
    lned_PrjName->setObjectName(QString("namePrj"));
    connect(lned_PrjName,
            SIGNAL(editingFinished()),
            this,
            SIGNAL(signalSomethingChanged()));

    _lt2_ParamsProjectID = new QHBoxLayout; _lt_main->addLayout(_lt2_ParamsProjectID);
    lblPrjID = new QLabel(trUtf8("Уникальный идентификатор проекта: ")); _lt2_ParamsProjectID->addWidget(lblPrjID);
    lned_PrjID = new QLineEdit; _lt2_ParamsProjectID->addWidget(lned_PrjID);
    lned_PrjID->setObjectName(QString("IDprj"));
    lned_PrjID->setReadOnly(true);

    _lt3_ParamsProjectDescription = new QHBoxLayout; _lt_main->addLayout(_lt3_ParamsProjectDescription);
    lblDescription = new QLabel(trUtf8("Описание проекта")); _lt3_ParamsProjectDescription->addWidget(lblDescription);

    te_Description = new QTextEdit; _lt_main->addWidget(te_Description, 7);
    te_Description->setObjectName("descriptionPrj");
    connect(te_Description,
            SIGNAL(textChanged()),
            this,
            SIGNAL(signalSomethingChanged()));
}
