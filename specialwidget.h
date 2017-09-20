#ifndef SPECIALWIDGET_H
#define SPECIALWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
//#include <QTabWidget>
#include <QPushButton>
//#include <QMdiArea>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QMessageBox>
#include <QAction>
#include <QMenu>

#include "projecttreewidgetitem.h" /// !!!!!!!!!!

class SpecialWidget : public QWidget
{
    Q_OBJECT

public:

    enum SpecialWidgetType
    {
        typeEmpty = 0,
        typeProjectProperties = 1,
        typeStructureProperties = 2,
        typeStratumProperties = 3,
        typeTrapProperties = 4,
        typeWellProperties = 5
    };

    explicit SpecialWidget(QWidget *parent = 0);
    void     setEnabledAndVisibleButtonsOfCreatingMode(bool yes);
    void     initMainLayout();
    void     initLayoutForCreateMode();
    void     addLayoutForCreateModeInMainLayout();

    QVBoxLayout *_lt_main;
    QHBoxLayout *_ltEnd_ButtonsForCreateMode;

    QPushButton *btnCreateIsOk;
    QPushButton *btnCreateCanceling;

private:

signals:
    void signalSomethingChanged();

public slots:
};

class BaseProjectPropertiesWidget : public SpecialWidget
{
    Q_OBJECT

public:
    explicit BaseProjectPropertiesWidget(QWidget *parent = 0);
//    void    setElements()

private:
    QHBoxLayout *_lt1_ParamsProjectName;
    QHBoxLayout *_lt2_ParamsProjectID;
    QHBoxLayout *_lt3_ParamsProjectDescription;
    QLabel      *lblPrjName;
    QLabel      *lblPrjID;
    QLabel      *lblDescription;

public:
    QLineEdit   *lned_PrjID;
    QLineEdit   *lned_PrjName;
    QTextEdit   *te_Description;

signals:
    void signalSomethingChanged();
};

class BaseStructurePropertiesWidget : public SpecialWidget
{
    Q_OBJECT

public:
    explicit BaseStructurePropertiesWidget(QWidget *parent = 0);

private:    
    QHBoxLayout *_lt1_ParamsStructName;
    QHBoxLayout *_lt2_ParamsStructNumberInProject;
    QHBoxLayout *_lt3_ParamsStructDescription;
    QLabel      *lblStructName;
    QLabel      *lblStructID;
    QLabel      *lblDescription;

public:
    QLineEdit   *lned_structName;
    QLineEdit   *lned_structID;
    QTextEdit   *te_DescriptionStruct;

signals:
    void signalSomethingChanged();
};

class BaseStratumPropertiesWidget : public SpecialWidget
{
    Q_OBJECT

public:
    explicit BaseStratumPropertiesWidget(QWidget *parent = 0);

private:
    QHBoxLayout *_lt1_ParamsStratumName;
    QLabel      *lblSname, *lblSid, *lblDesc;

public:
    QLineEdit   *lned_stratumName;
    QLineEdit   *lned_stratumID;
    QTextEdit   *te_DescriptionStratum;

signals:
    void signalSomethingChanged();
};

class BaseWellPropertiesWidget : public SpecialWidget
{
    Q_OBJECT

public:
    explicit BaseWellPropertiesWidget(QWidget *parent = 0);

private:
    QHBoxLayout *_lt1_ParamsWellName;
    QHBoxLayout *_lt2_ParamsWellID;
    QHBoxLayout *_lt3_ParamsNegResProb;

    QHBoxLayout *_lt_WellIDCmbForDialog;

    QLabel      *lblWellName;
    QLabel      *lblID;
    QLabel      *lblNegResProb;

    QDialog     dlg;

public:
    QLineEdit   *lned_wellName;
    QLineEdit   *lned_ID;
    QPushButton *btn_dialogForChangeExecuting;
    QComboBox   *cmb_ID;
    QPushButton *btn_changeID;
    QPushButton *btn_CancellingOfChange;
    QLineEdit   *lned_NegResProb;
    QTextEdit   *te_Description;

    bool        ID_valueChanged;

    int         oldID;

    QString     oldValueTxtForID;

    void        setElementsAreReadOnly(bool ro);

    void        setCmbSignalsDisconnected(bool disconnected);

    int         retWellIDByComboBox();

signals:
    void signalSomethingChanged();

private slots:
    void slotDialogOfWellIdChangingToExec();
};

class BaseTrapPropertiesWidget : public SpecialWidget
{
    Q_OBJECT

public:
    explicit BaseTrapPropertiesWidget(QWidget *parent = 0);

private:
    QHBoxLayout *_lt1_ParamsStructName;
    QHBoxLayout *_lt2_ParamsTrapName;
    QHBoxLayout *_lt3_ParamsStratumIndex;
    QHBoxLayout *_lt4_AllocationType;

    QHBoxLayout *_lt5_KINdetermined;

    QHBoxLayout *_lt6_Comment;
    QHBoxLayout *_ltSimpleOutput;
    QHBoxLayout *_lt_Probabilities;

    QLabel      *lblStructName;

    QLabel      *lblTrapName;

    QLabel      *lblTrapID;

    QLabel      *lblStratumIndex;

    QLabel      *lblAlloctionType;

    QLabel      *lblVariativeEstimationParameters;
    QGridLayout *_gridLt_variativeEstimationParams;

    QLabel      *lblKIN;

    QLabel      *lblProbabilityDeposit;

//    QAction     *refreshComment;

public:
    QLineEdit   *lineEdit_StructName;
    QLineEdit   *lineEdit_TrapName;
    QLineEdit   *lineEdit_IDtrap;
    QComboBox   *cmb_StratumIndex;

    QComboBox   *cmb_AllocationType;

    QLineEdit   *lineEdit_AreaAvg;
    QLineEdit   *lineEdit_AreaMin;
    QLineEdit   *lineEdit_AreaMax;

    QLineEdit   *lineEdit_EffThickAvg;
    QLineEdit   *lineEdit_EffThickMin;
    QLineEdit   *lineEdit_EffThickMax;

    QLineEdit   *lineEdit_PorosityCoeffAvg;
    QLineEdit   *lineEdit_PorosityCoeffMin;
    QLineEdit   *lineEdit_PorosityCoeffMax;

    QLineEdit   *lineEdit_OilSaturationCoeffAvg;
    QLineEdit   *lineEdit_OilSaturationCoeffMin;
    QLineEdit   *lineEdit_OilSaturationCoeffMax;

    QLineEdit   *lineEdit_OilDensityAvg;
    QLineEdit   *lineEdit_OilDensityMin;
    QLineEdit   *lineEdit_OilDensityMax;

    QLineEdit   *lineEdit_OilShrinkingAvg;
    QLineEdit   *lineEdit_OilShrinkingMin;
    QLineEdit   *lineEdit_OilShrinkingMax;

    QLineEdit   *lineEdit_KIN;

    QLineEdit   *lineEdit_ProbaboilityOfDeposite;

    QTextEdit   *te_Comment, *te_Output;

signals:
//    void        signalToRefreshComment();
    void        signalSomethingChanged();
};

#endif // SPECIALWIDGET_H
