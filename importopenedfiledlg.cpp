#include "importopenedfiledlg.h"
#include "ui_importopenedfiledlg.h"

ImportOpenedFileDlg::ImportOpenedFileDlg(QWidget *parent, bool isKinDetermined) :
    QDialog(parent),
    ui(new Ui::ImportOpenedFileDlg)
{
    isAccepted = false;
    isKinDetermined__ = isKinDetermined;

    ui->setupUi(this);

    //informativeColumnsMap.clear();

    splittingOfSeparators = false;
    joiningTheGroupOfSepatorsAtOne = false;

    separator_Tab = false;
    separator_Space = false;
    separator_Comma = false;
    separator_DotWithComma = false;
    separator_Other = false;
//    separator_Tab_OUT = false;
//    separator_Space_OUT = false;
//    separator_Comma_OUT = false;
//    separator_DotWithComma_OUT = false;
//    separator_Other_OUT = false;

    headersPresent = false;

    ui->btnSaveTo->setVisible(false);
    ui->buttonBoxCancel->setVisible(false);
    ui->tblView_ImportingData->setSelectionBehavior(QAbstractItemView::SelectColumns);

//    informativeColumnsMap.insert("NameStruct", -1);
//    informativeColumnsMap.insert("StratumIndex", -1);
//    informativeColumnsMap.insert("Probability", -1);
//    informativeColumnsMap.insert("Average", -1);
//    informativeColumnsMap.insert("Minimum", -1);
//    informativeColumnsMap.insert("Maximum", -1);
//    informativeColumnsMap.insert("KIN", -1);
//    informativeColumnsMap.insert("AllocateType", -7);
//    informativeColumnsMap.insert("Wells", -7);
    //informativeColumnsMap.insert("NegResProbability", -7);

    informativeColumnsMap.insert("NameTrap", -1);

    informativeColumnsMap.insert("AreaMin", -1);
    informativeColumnsMap.insert("AreaAvg", -1);
    informativeColumnsMap.insert("AreaMax", -1);

    informativeColumnsMap.insert("EffThickMin", -1);
    informativeColumnsMap.insert("EffThickAvg", -1);
    informativeColumnsMap.insert("EffThickMax", -1);

    informativeColumnsMap.insert("PorosityMin", -1);
    informativeColumnsMap.insert("PorosityAvg", -1);
    informativeColumnsMap.insert("PorosityMax", -1);

    informativeColumnsMap.insert("OilSaturationMin", -1);
    informativeColumnsMap.insert("OilSaturationAvg", -1);
    informativeColumnsMap.insert("OilSaturationMax", -1);

    informativeColumnsMap.insert("DensityMin", -1);
    informativeColumnsMap.insert("DensityAvg", -1);
    informativeColumnsMap.insert("DensityMax", -1);

    informativeColumnsMap.insert("ShrinkingMin", -1);
    informativeColumnsMap.insert("ShrinkingAvg", -1);
    informativeColumnsMap.insert("ShrinkingMax", -1);

    informativeColumnsMap.insert("AllocType", -7);

    if (isKinDetermined__)
    {
        informativeColumnsMap.insert("KinDetermined", -1);
    }
    else
    {
        informativeColumnsMap.insert("KinMin", -1);
        informativeColumnsMap.insert("KinAvg", -1);
        informativeColumnsMap.insert("KinMax", -1);
    }

    informativeColumnsMap.insert("DepositProbability", -1);

    ui->btn_KIN_Max->setEnabled(false);
    ui->btn_KIN_Max->setVisible(false);
    ui->btn_KIN_Min->setEnabled(false);
    ui->btn_KIN_Min->setVisible(false);
}

ImportOpenedFileDlg::~ImportOpenedFileDlg()
{
    delete ui;
}

QStandardItemModel *ImportOpenedFileDlg::retResultTableModel()
{
    return tblResultModel;
}

void ImportOpenedFileDlg::txtStreamReadingAfterInputFileOpen(QTextStream *stream)
{
    ui->chk_AnotherSeparator->setChecked(false);
    //ui->chk_AnotherSeparator_Output->setChecked(false);
    ui->chk_CommaSeparator->setChecked(false);
    //ui->chk_CommaSeparator_Output->setChecked(false);
    ui->chk_DotWithCommaSeparator->setChecked(false);
    //ui->chk_DotWithCommaSeparator_Output->setChecked(false);
    ui->chk_JoinGroupSeparatorsAtOne->setChecked(false);
    ui->chk_SpaceSeparator->setChecked(false);
    //ui->chk_SpaceSeparator_Output->setChecked(false);
    ui->chk_SplitSeparators->setChecked(false);
    ui->chk_TabSeparator->setChecked(false);
    //ui->chk_TabSeparator_Output->setChecked(false);
    ui->lineEdit_AnotherSeparator->clear();
    //ui->lineEdit_AnotherSeparator_Output->clear();
    //ui->btnSaveTo->setEnabled(false);

    strlist.clear();

    while(!stream->atEnd())
    {
        strlist.push_back(stream->readLine());
    }

    tblModel = new QStandardItemModel(strlist.length(), 1, this);
    tblModelWithHeaders = new QStandardItemModel(strlist.length(), 1, this);
    tblShadowModel = new QStandardItemModel(strlist.length(), 1, this);

    for (int iRowOneRec = 0; iRowOneRec < strlist.length(); ++iRowOneRec)
    {
        QStandardItem *item3 = new QStandardItem(strlist.at(iRowOneRec));
        tblShadowModel->setItem(iRowOneRec, item3);
    }

    ui->tblView_ImportingData->setModel(tblShadowModel);

    ui->tblView_ImportingData->setEditTriggers(QTableView::NoEditTriggers);
    //ui->tblView_ImportingData->setSelectionBehavior(QTableView::SelectRows);

    ColumnsWidthChanging();

    ui->grpBox_step1->setEnabled(true);
    ui->grpBox_step2->setEnabled(true);
    ui->grpBox_step3->setEnabled(true);

//    if (isKinDetermined__)
//    {
//        ui->btn_KIN_Min->setEnabled(false);
//        ui->btn_KIN_Max->setEnabled(false);
//    }

    ui->buttonBoxCancel->setVisible(true);
}

void ImportOpenedFileDlg::AdjustSeparation(int stateLast)
{
    ColumnsWidthChanging();

    vectorOfSeparators.clear();

    if (separator_Tab)
    {
        vectorOfSeparators.push_back("\t");
    }
    if (separator_Space)
    {
        vectorOfSeparators.push_back(" ");
    }
    if (separator_Comma)
    {
        vectorOfSeparators.push_back(",");
    }
    if (separator_DotWithComma)
    {
        vectorOfSeparators.push_back(";");
    }
    if (separator_Other)
    {
        vectorOfSeparators.push_back(ui->lineEdit_AnotherSeparator->text());
    }

    QVector <QStringList > fromShadowTblModel;//fromTblModel,
                            //fromTblModelWithHeaders;

    for (int iRowsTShM = 0; iRowsTShM < tblShadowModel->rowCount(); ++iRowsTShM)
    {
        QStringList lstTM;
        for (int iColumns = 0; iColumns < tblShadowModel->columnCount(); ++iColumns)
        {
            QModelIndex idx = tblShadowModel->index(iRowsTShM, iColumns);
            QString idxTxt = idx.data().toString();
            lstTM.push_back(idxTxt);
        }
        fromShadowTblModel.push_back(lstTM);
    }

    for (int iVec = 0; iVec < fromShadowTblModel.size(); ++iVec)
    {
        if (joiningTheGroupOfSepatorsAtOne)
        {
            QString rxstr;

            rxstr.append("[");
            for (int iSep = 0; iSep < vectorOfSeparators.size(); iSep++)
            {
                if (vectorOfSeparators.at(iSep) == " ")
                    rxstr.append("\s");
                else
                    rxstr.append(vectorOfSeparators.at(iSep));
            }
            rxstr.append("]{");
            //QString strnumvh = QString::number(vectorOfSeparators.size());
            QString strnumvh = QString::number(
                        rxstr.length()-3
                        );
            rxstr.append(strnumvh);
            rxstr.append(",");
            rxstr.append(strnumvh);
            rxstr.append("}");

            QRegExp rx(rxstr);

            for (int iString = 0; iString < fromShadowTblModel.at(iVec).size(); ++iString)
            {
                for (int iSep = 0; iSep < vectorOfSeparators.size(); iSep++)
                {
                    QString sepNew("@%&%$$@");
                    fromShadowTblModel[iVec][iString].replace(
                                rx, sepNew);
                }
            }
        }
        else
        {
            for (int iString = 0; iString < fromShadowTblModel.at(iVec).size(); ++iString)
            {
                for (int iSep = 0; iSep < vectorOfSeparators.size(); iSep++)
                {
                    QString sepThis(vectorOfSeparators.at(iSep));
                    QString sepNew("@%&%$$@");
                    fromShadowTblModel[iVec][iString].replace(
                                sepThis, sepNew);
                }
            }
        }

        if (splittingOfSeparators)
        {
            QStringList newStrListTM;

            for (int iString = 0; iString < fromShadowTblModel.at(iVec).length(); ++iString)
            {
                newStrListTM = fromShadowTblModel.at(iVec)
                        .at(iString).split("@%&%$$@", QString::SkipEmptyParts);
            }

            fromShadowTblModel[iVec] = newStrListTM;
        }
        else
        {
            QStringList newStrListTM;

            for (int iString = 0; iString < fromShadowTblModel.at(iVec).length(); ++iString)
            {
                newStrListTM = fromShadowTblModel.at(iVec)
                        .at(iString).split("@%&%$$@");
            }

            fromShadowTblModel[iVec] = newStrListTM;
        }
    }

    int fromShadowTblModelMaxRowLenght = 0;

    for (int iVec2 = 0; iVec2 < fromShadowTblModel.size(); ++iVec2)
    {
        if (fromShadowTblModel.at(iVec2).length() > fromShadowTblModelMaxRowLenght)
            fromShadowTblModelMaxRowLenght = fromShadowTblModel.at(iVec2).length();
    }

    // для таблицы без заголовков

    tblModel = new QStandardItemModel(fromShadowTblModel.size(), fromShadowTblModelMaxRowLenght, this);

    for (int iRowsTM = 0; iRowsTM < fromShadowTblModel.size(); ++iRowsTM)
    {
        for (int iColumns = 0; iColumns < fromShadowTblModelMaxRowLenght; ++iColumns)
        {
            int rowLength = fromShadowTblModel.at(iRowsTM).length();

            if (! (iColumns >= rowLength) )
            {
                QStandardItem *item = new QStandardItem(
                            fromShadowTblModel.at(iRowsTM).at(iColumns)
                            );
                tblModel->setItem(iRowsTM, iColumns, item);
            }
        }
    }

    // для таблицы с заголовками

    tblModelWithHeaders = new QStandardItemModel(fromShadowTblModel.size(), fromShadowTblModelMaxRowLenght, this);

    for (int iRowsTM = 0; iRowsTM < fromShadowTblModel.size(); ++iRowsTM)
    {
        for (int iColumns = 0; iColumns < fromShadowTblModelMaxRowLenght; ++iColumns)
        {
            int rowLength = fromShadowTblModel.at(iRowsTM).length();

            if (! (iColumns >= rowLength) )
            {
                QStandardItem *item = new QStandardItem(
                            fromShadowTblModel.at(iRowsTM).at(iColumns)
                            );
                tblModelWithHeaders->setItem(iRowsTM, iColumns, item);
            }
        }
    }

    tblModelWithHeaders->removeRow(0);

    for (int iHeaderSection = 0; iHeaderSection < fromShadowTblModelMaxRowLenght; ++iHeaderSection)
    {
        //tblModelWithHeaders->setHorizontalHeaderItem(iHeaderSection, tblModel->item(0, iHeaderSection));

        tblModelWithHeaders->setHeaderData(iHeaderSection, Qt::Horizontal, tblModel->index(0, iHeaderSection).data());
    }

//    for (int iRowsTM = 1; iRowsTM < fromShadowTblModel.size(); ++iRowsTM)
//    {
//        for (int iColumns = 0; iColumns < fromShadowTblModelMaxRowLenght; ++iColumns)
//        {
//            int rowLength = fromShadowTblModel.at(iRowsTM).length();

//            if (! (iColumns >= rowLength) )
//            {
//                QStandardItem *item = new QStandardItem(
//                            fromShadowTblModel.at(iRowsTM).at(iColumns)
//                            );
//                tblModelWithHeaders->setItem(iRowsTM, iColumns, item);
//            }
//        }
//    }

    // подготовка к возможному шагу 2

    ui->tblView_ImportingData->setModel(tblModel);
}

void ImportOpenedFileDlg::ColumnsWidthChanging()
{
    bool checkedSomethingSeparator =
            separator_Tab | separator_Space | separator_Comma
            | separator_DotWithComma | separator_Other;

    if (!checkedSomethingSeparator)
    {
        ui->tblView_ImportingData->setColumnWidth(0, 620);
    }
    else
    {
        int columnsNumb = tblModel->columnCount();
        for (int iCol = 0; iCol < columnsNumb; ++iCol)
        {
            ui->tblView_ImportingData->setColumnWidth(iCol, 90);
        }
    }
}

void ImportOpenedFileDlg::on_chk_SplitSeparators_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        splittingOfSeparators = true;

        AdjustSeparation(state);
    }
    else if (state == Qt::Unchecked)
    {
        splittingOfSeparators = false;

        AdjustSeparation(state);
    }
}

void ImportOpenedFileDlg::on_chk_JoinGroupSeparatorsAtOne_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        joiningTheGroupOfSepatorsAtOne = true;

        AdjustSeparation(state);
    }
    else if (state == Qt::Unchecked)
    {
        joiningTheGroupOfSepatorsAtOne = false;

        AdjustSeparation(state);
    }
}

void ImportOpenedFileDlg::on_chk_TabSeparator_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        separator_Tab = true;

        AdjustSeparation(state);
    }
    else if (state == Qt::Unchecked)
    {
        separator_Tab = false;

        AdjustSeparation(state);
    }
}

void ImportOpenedFileDlg::on_chk_DotWithCommaSeparator_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        separator_DotWithComma = true;

        AdjustSeparation(state);
    }
    else if (state == Qt::Unchecked)
    {
        separator_DotWithComma = false;

        AdjustSeparation(state);
    }
}

void ImportOpenedFileDlg::on_chk_SpaceSeparator_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        separator_Space = true;

        AdjustSeparation(state);
    }
    else if (state == Qt::Unchecked)
    {
        separator_Space = false;

        AdjustSeparation(state);
    }
}

void ImportOpenedFileDlg::on_chk_CommaSeparator_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        separator_Comma = true;

        AdjustSeparation(state);
    }
    else if (state == Qt::Unchecked)
    {
        separator_Comma = false;

        AdjustSeparation(state);
    }
}

void ImportOpenedFileDlg::on_chk_AnotherSeparator_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        if (ui->lineEdit_AnotherSeparator->text().isEmpty())
        {
            QMessageBox::information(this,
                                     QObject::trUtf8("Укажите сепаратор"),
                                     QObject::trUtf8(
                                         "Необходимо вначале указать символ или набор символов, играющих роль сепаратора."
                                         ));

            disconnect(ui->chk_AnotherSeparator,
                       SIGNAL(stateChanged(int)),
                       this,
                       SLOT(on_chk_AnotherSeparator_stateChanged(int)));

            ui->chk_AnotherSeparator->setCheckState(Qt::Unchecked); // disconnect - чтобы stateChanged не привел
                                                                    // к ненужному вызову AdjustSeparation()

            connect(ui->chk_AnotherSeparator,
                       SIGNAL(stateChanged(int)),
                       this,
                       SLOT(on_chk_AnotherSeparator_stateChanged(int)));
        }
        else
        {
            separator_Other = true;

            AdjustSeparation(state);
        }
    }
    else if (state == Qt::Unchecked)
    {
        separator_Other = false;

        ui->lineEdit_AnotherSeparator->clear();

        AdjustSeparation(state);
    }
}

void ImportOpenedFileDlg::on_btnOpenFile_clicked()
{
    QString fileNameToOpen = QFileDialog::getOpenFileName(
                this, QObject::trUtf8("Открыть файл ввода (текстовый файл с разделителями)..."), "",
                QObject::trUtf8("Все файлы (*.*)")
                );

    if (fileNameToOpen.isEmpty())
    {
        return;
    }

    infile.setFileName(fileNameToOpen);

    if (infile.open(QIODevice::ReadOnly))
    {
        QTextStream intxtstream(&infile);

        txtStreamReadingAfterInputFileOpen(&intxtstream);

        infile.close();
    }

}

void ImportOpenedFileDlg::on_buttonBoxCancel_accepted()
{
//    tblResultModel = new QStandardItemModel(ui->tblView_ImportingData->model()->rowCount(), informativeColumnsMap.count(), this);

//    for (int iRow = 0; iRow < ui->tblView_ImportingData->model()->rowCount(); ++iRow)
//    {
////        QStandardItem *item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("NameStruct")).data().toString());

////        tblResultModel->setItem(iRow, 0, item); //NameStruct

//        QStandardItem *item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("NameTrap")).data().toString());

//        tblResultModel->setItem(iRow, 0, item); //NameTrap

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("AreaMin")).data().toString());

//        tblResultModel->setItem(iRow, 1, item); //AreaMin

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("AreaAvg")).data().toString());

//        tblResultModel->setItem(iRow, 2, item); //AreaAvg

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("AreaMax")).data().toString());

//        tblResultModel->setItem(iRow, 3, item); //AreaMax

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("EffThickMin")).data().toString());

//        tblResultModel->setItem(iRow, 4, item); //EffThickMin

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("EffThickAvg")).data().toString());

//        tblResultModel->setItem(iRow, 5, item); //EffThickAvg

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("EffThickMax")).data().toString());

//        tblResultModel->setItem(iRow, 6, item); //EffThickMax

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("PorosityMin")).data().toString());

//        tblResultModel->setItem(iRow, 7, item); //PorosityMin

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("PorosityAvg")).data().toString());

//        tblResultModel->setItem(iRow, 8, item); //PorosityAvg

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("PorosityMax")).data().toString());

//        tblResultModel->setItem(iRow, 9, item); //PorosityMax

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("OilSaturationMin")).data().toString());

//        tblResultModel->setItem(iRow, 10, item); //OilSaturationMin

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("OilSaturationAvg")).data().toString());

//        tblResultModel->setItem(iRow, 11, item); //OilSaturationAvg

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("OilSaturationMax")).data().toString());

//        tblResultModel->setItem(iRow, 12, item); //OilSaturationMax

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("DensityMin")).data().toString());

//        tblResultModel->setItem(iRow, 13, item); //DensityMin

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("DensityAvg")).data().toString());

//        tblResultModel->setItem(iRow, 14, item); //DensityAvg

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("DensityMax")).data().toString());

//        tblResultModel->setItem(iRow, 15, item); //DensityMax

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("ShrinkingMin")).data().toString());

//        tblResultModel->setItem(iRow, 16, item); //ShrinkingMin

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("ShrinkingAvg")).data().toString());

//        tblResultModel->setItem(iRow, 17, item); //ShrinkingAvg

//        item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("ShrinkingMax")).data().toString());

//        tblResultModel->setItem(iRow, 18, item); //ShrinkingMax

//        if (isKinDetermined__)
//        {
//            item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("KinDetermined")).data().toString());

//            tblResultModel->setItem(iRow, 19, item); //KinDetermined

//            if (informativeColumnsMap.value("AllocType") != -7)
//            {
//                item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("AllocType")).data().toString());

//                tblResultModel->setItem(iRow, 20, item); //AllocType
//            }
//            else
//            {
//                item = new QStandardItem(QString::number(0));  // ~ "Двухтреугольное"

//                tblResultModel->setItem(iRow, 20, item); //AllocType
//            }
//        }
//        else
//        {
//            item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("KinMin")).data().toString());

//            tblResultModel->setItem(iRow, 19, item); //KinMin

//            item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("KinAvg")).data().toString());

//            tblResultModel->setItem(iRow, 20, item); //KinAvg

//            item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("KinMax")).data().toString());

//            tblResultModel->setItem(iRow, 21, item); //KinMax

//            if (informativeColumnsMap.value("AllocType") != -7)
//            {
//                item = new QStandardItem(ui->tblView_ImportingData->model()->index(iRow, informativeColumnsMap.value("AllocType")).data().toString());

//                tblResultModel->setItem(iRow, 22, item); //AllocType
//            }
//            else
//            {
//                item = new QStandardItem(QString::number(0));  // ~ "Двухтреугольное"

//                tblResultModel->setItem(iRow, 23, item); //AllocType
//            }
//        }
//    }

//    emit ExportTableModelWithParams(tblResultModel, informativeColumnsMap);

    isAccepted = true;

    emit ExportData(dataMap);

    this->accept();
}

void ImportOpenedFileDlg::on_chk_HeadersPresent_stateChanged(int state)
{
    if (state == Qt::Checked)
    {
        headersPresent = true;

        ui->tblView_ImportingData->setModel(tblModelWithHeaders);
    }
    else if (state == Qt::Unchecked)
    {
        headersPresent = false;

        ui->tblView_ImportingData->setModel(tblModel);
    }
}

void ImportOpenedFileDlg::AnyButtonForInformativeColumnsPressed(QPushButton *sender)
{
    if (ui->tblView_ImportingData->selectionModel()->currentIndex().isValid())
    {
        QVector<QVariant> vectorDataForCurrentColumn;
        QString descriptor = sender->objectName();

        if (!isKinDetermined__ && (sender->objectName() == QString("KinDetermined")))
        {
            descriptor = QString("KinAvg");
        }

        for (int i = 0; i < ui->tblView_ImportingData->model()->rowCount(); ++i)
        {
            int col = ui->tblView_ImportingData->selectionModel()->currentIndex().column();

            QVariant varData = ui->tblView_ImportingData
                    ->model()->index(i, col).data();

            vectorDataForCurrentColumn.append(varData);
        }

        dataMap.remove(descriptor);
        dataMap.insert(descriptor, vectorDataForCurrentColumn);

//        if (/*sender->text() == QObject::trUtf8("Имя ловушки")*/
//                sender->objectName() == QString("NameTrap")
//                )
//        {
////            QMessageBox::information(this, "SUCCESS!", sender->text());
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("NameTrap");
//            informativeColumnsMap.insert("NameTrap", column);
//        }
//        if (/*sender->text().contains(trUtf8("распределения"))*/
//                 sender->objectName() == QString("AllocType")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("AllocType");
//            informativeColumnsMap.insert("AllocType", column);
//        }
//        if (/*sender->text() == QObject::trUtf8("Площадь мин., км2")*/
//                 sender->objectName() == QString("AreaMin")
//                 )
//        {
////            QMessageBox::information(this, "SUCCESS!", sender->text());
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("AreaMin");
//            informativeColumnsMap.insert("AreaMin", column);
//        }
//        if (/*sender->text() == QObject::trUtf8("Площадь сред., км2")*/
//                 sender->objectName() == QString("AreaAvg")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("AreaAvg");
//            informativeColumnsMap.insert("AreaAvg", column);
//        }
//        if (/*sender->text() == QObject::trUtf8("Площадь макс., км2")*/
//                 sender->objectName() == QString("AreaMax")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("AreaMax");
//            informativeColumnsMap.insert("AreaMax", column);
//        }
//        if (/*sender->text().contains(trUtf8("насыщенная толщина\nмин., м"))*/
//                 sender->objectName() == QString("EffThickMin")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("EffThickMin");
//            informativeColumnsMap.insert("EffThickMin", column);
//        }
//        if (/*sender->text().contains(trUtf8("насыщенная толщина\nсред., м"))*/
//                 sender->objectName() == QString("EffThickAvg")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("EffThickAvg");
//            informativeColumnsMap.insert("EffThickAvg", column);
//        }
//        if (/*sender->text().contains(trUtf8("насыщенная толщина\nмакс., м"))*/
//                 sender->objectName() == QString("EffThickMax")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("EffThickMax");
//            informativeColumnsMap.insert("EffThickMax", column);
//        }
//        if (/*sender->text().contains(trUtf8("Пористости коэф.\nмин"))*/
//                 sender->objectName() == QString("PorosityMin")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("PorosityMin");
//            informativeColumnsMap.insert("PorosityMin", column);
//        }
//        if (/*sender->text().contains(trUtf8("Пористости коэф.\nсред"))*/
//                 sender->objectName() == QString("PorosityAvg")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("PorosityAvg");
//            informativeColumnsMap.insert("PorosityAvg", column);
//        }
//        if (/*sender->text().contains(trUtf8("Пористости коэф.\nмакс"))*/
//                 sender->objectName() == QString("PorosityMax")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("PorosityMax");
//            informativeColumnsMap.insert("PorosityMax", column);
//        }
//        if (/*sender->text().contains(trUtf8("Нефтенасыщ. коэф.\nмин"))*/
//                 sender->objectName() == QString("OilSaturationMin")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("OilSaturationMin");
//            informativeColumnsMap.insert("OilSaturationMin", column);
//        }
//        if (/*sender->text().contains(trUtf8("Нефтенасыщ. коэф.\nсред"))*/
//                 sender->objectName() == QString("OilSaturationAvg")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("OilSaturationAvg");
//            informativeColumnsMap.insert("OilSaturationAvg", column);
//        }
//        if (/*sender->text().contains(trUtf8("Нефтенасыщ. коэф.\nмакс"))*/
//                 sender->objectName() == QString("OilSaturationMax")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("OilSaturationMax");
//            informativeColumnsMap.insert("OilSaturationMax", column);
//        }
//        if (/*sender->text().contains(trUtf8("мин., г/см3"))*/
//                 sender->objectName() == QString("DensityMin")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("DensityMin");
//            informativeColumnsMap.insert("DensityMin", column);
//        }
//        if (/*sender->text().contains(trUtf8("сред., г/см3"))*/
//                 sender->objectName() == QString("DensityAvg")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("DensityAvg");
//            informativeColumnsMap.insert("DensityAvg", column);
//        }
//        if (/*sender->text().contains(trUtf8("макс., г/см3"))*/
//                 sender->objectName() == QString("DensityMax")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("DensityMax");
//            informativeColumnsMap.insert("DensityMax", column);
//        }
//        if (/*sender->text().contains(trUtf8("Пересчетный коэф.\nмин"))*/
//                 sender->objectName() == QString("ShrinkingMin")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("ShrinkingMin");
//            informativeColumnsMap.insert("ShrinkingMin", column);
//        }
//        if (/*sender->text().contains(trUtf8("Пересчетный коэф.\nсред"))*/
//                 sender->objectName() == QString("ShrinkingAvg"))
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("ShrinkingAvg");
//            informativeColumnsMap.insert("ShrinkingAvg", column);
//        }
//        if (/*sender->text().contains(trUtf8("Пересчетный коэф.\nмакс"))*/
//                 sender->objectName() == QString("ShrinkingMax"))
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("ShrinkingMax");
//            informativeColumnsMap.insert("ShrinkingMax", column);
//        }
//        if (/*sender->text().contains(trUtf8("КИН мин"))*/
//                 sender->objectName() == QString("KinMin")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("KinMin");
//            informativeColumnsMap.insert("KinMin", column);
//        }
//        if (/*sender->text().contains(trUtf8("или детермин"))*/
//                 sender->objectName() == QString("KinDetermined")
//                 )
//        {
//            if (isKinDetermined__)
//            {
//                int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//                informativeColumnsMap.remove("KinDetermined");
//                informativeColumnsMap.insert("KinDetermined", column);
//            }
//            else
//            {
//                int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//                informativeColumnsMap.remove("KinAvg");
//                informativeColumnsMap.insert("KinAvg", column);
//            }
//        }
//        if (/*sender->text().contains(trUtf8("КИН макс"))*/
//                 sender->objectName() == QString("KinMax")
//                 )
//        {
//            int column = ui->tblView_ImportingData->selectionModel()->currentIndex().column();
//            informativeColumnsMap.remove("KinMax");
//            informativeColumnsMap.insert("KinMax", column);
//        }
    }
    else
    {
        QMessageBox::information(this, "", QObject::trUtf8("Нужно выбрать столбец таблицы!"));
    }
}

void ImportOpenedFileDlg::on_btn_TrapName_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("NameTrap"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_AreaMin_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("AreaMin"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_AreaAvg_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("AreaAvg"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_AreaMax_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("AreaMax"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_EffectiveThickMin_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("EffectiveThickMin"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_EffectiveThickAvg_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("EffectiveThickAvg"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_EffectiveThickMax_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("EffectiveThickMax"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_PorosityMin_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("PorosityMin"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_PorosityAverage_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("PorosityAvg"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_PorosityMax_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("PorosityMax"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_OilSatMin_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("OilSaturationMin"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_OilSatAverage_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("OilSaturationAvg"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_OilSatMax_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("OilSaturationMax"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_DensityOfOilMin_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("DensityMin"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_DensityOfOilAverage_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("DensityAvg"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_DensityOfOilMax_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("DensityMax"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_ShrinkingMin_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("ShrinkingMin"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_ShrinkingAverage_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("ShrinkingAvg"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_ShrinkingMax_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("ShrinkingMax"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_KIN_Min_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("KinMin"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_KIN_Avg_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("KinDetermined"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_KIN_Max_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("KinMax"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_AllocType_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("AllocType"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}

void ImportOpenedFileDlg::on_btn_ProbabilityOfDepositExisting_clicked()
{
    QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());

    senderBtn->setObjectName(QString("DepositProbability"));

    AnyButtonForInformativeColumnsPressed(senderBtn);
}
