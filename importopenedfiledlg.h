#ifndef IMPORTOPENEDFILEDLG_H
#define IMPORTOPENEDFILEDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QMap>
#include <QVector>
#include <QRegExp>

namespace Ui {
class ImportOpenedFileDlg;
}

class ImportOpenedFileDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImportOpenedFileDlg(QWidget *parent = 0/*, QTextStream *stream = 0*/,
                                 bool isKinDetermined = true);
    ~ImportOpenedFileDlg();
    QStandardItemModel *retResultTableModel();
    bool    isAccepted;

signals:

    void ExportTableModelWithParams(QStandardItemModel*, QMap<QString, int>);
    void ExportData(QMap< QString, QVector<QVariant> >);

private slots:

    //void on_chk_HeaderPresent_stateChanged(int state);

    void on_chk_SplitSeparators_stateChanged(int state);

    void on_chk_TabSeparator_stateChanged(int state);

    void on_chk_DotWithCommaSeparator_stateChanged(int state);

    void on_chk_SpaceSeparator_stateChanged(int state);

    void on_chk_CommaSeparator_stateChanged(int state);

    void on_chk_AnotherSeparator_stateChanged(int state);

    //void on_btn_step1_Successed_clicked();

    //void on_btn_step2_Successed_clicked();

    //void on_btn_step3_Successed_clicked();

    //void on_btn_UN_column_clicked();

    //void on_btn_NameLayer_column_clicked();

    //void on_btn_NameField_column_clicked();

    //void on_btn_NameResources_column_clicked();

    //void on_buttonBoxOK_accepted();

    void on_btnOpenFile_clicked();

    void on_buttonBoxCancel_accepted();

    //void on_chk_TabSeparator_Output_stateChanged(int state);

    //void on_chk_SpaceSeparator_Output_stateChanged(int state);

    //void on_chk_DotWithCommaSeparator_Output_stateChanged(int state);

    //void on_chk_CommaSeparator_Output_stateChanged(int state);

    //void on_chk_AnotherSeparator_Output_stateChanged(int state);

    //void on_btnSaveTo_clicked();

    void on_chk_JoinGroupSeparatorsAtOne_stateChanged(int state);

    void on_chk_HeadersPresent_stateChanged(int state);

    void AnyButtonForInformativeColumnsPressed(QPushButton *sender);

    void on_btn_TrapName_clicked();

    void on_btn_AreaMin_clicked();

    void on_btn_AreaAvg_clicked();

    void on_btn_AreaMax_clicked();

    void on_btn_EffectiveThickMin_clicked();

    void on_btn_EffectiveThickAvg_clicked();

    void on_btn_EffectiveThickMax_clicked();

    void on_btn_PorosityMin_clicked();

    void on_btn_PorosityAverage_clicked();

    void on_btn_PorosityMax_clicked();

    void on_btn_OilSatMin_clicked();

    void on_btn_OilSatAverage_clicked();

    void on_btn_OilSatMax_clicked();

    void on_btn_DensityOfOilMin_clicked();

    void on_btn_DensityOfOilAverage_clicked();

    void on_btn_DensityOfOilMax_clicked();

    void on_btn_ShrinkingMin_clicked();

    void on_btn_ShrinkingAverage_clicked();

    void on_btn_ShrinkingMax_clicked();

    void on_btn_KIN_Min_clicked();

    void on_btn_KIN_Avg_clicked();

    void on_btn_KIN_Max_clicked();

    void on_btn_AllocType_clicked();

    void on_btn_ProbabilityOfDepositExisting_clicked();

private:

    Ui::ImportOpenedFileDlg *ui;

    bool                    isKinDetermined__;

    QStandardItemModel      *tblModel;
    QStandardItemModel      *tblModelWithHeaders;
    QStandardItemModel      *tblShadowModel;
    QStandardItemModel      *tblResultModel;

    QStringList     strlist;

    QVector<QString>  vectorOfSeparators,
                      vectorOfSeparatorsOut;
    bool            splittingOfSeparators;
    bool            joiningTheGroupOfSepatorsAtOne;
    bool            headersPresent;
    bool            separator_Tab, separator_Space,
                    separator_Comma, separator_DotWithComma, separator_Other;//,
                    //separator_Tab_OUT, separator_Space_OUT,
                    //separator_Comma_OUT, separator_DotWithComma_OUT, separator_Other_OUT;
    QString         otherSeparatorString;
    QFile           infile;//,
                    //outfile;

    void            AdjustSeparation(int stateLast);

    void            ColumnsWidthChanging();

    void            txtStreamReadingAfterInputFileOpen(QTextStream *stream);

    QMap< QString, int >    informativeColumnsMap;
    QMap< QString, QVector<QVariant> > dataMap;
};

#endif // IMPORTOPENEDFILEDLG_H
