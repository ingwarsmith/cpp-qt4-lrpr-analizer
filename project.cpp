#include "project.h"

Project::Project(QObject *parent) : QObject(parent)//: QTreeWidget(parent)
{
    Q_UNUSED(parent);

    m_unsavedChangesExist = false;

    structuresInProject = QMap<int, Structure* >();
    stratumsNamesByNumber = QMap<int, Stratum* >();
    wellsInProject = QMap<int, Well* >();
    trapsInProjectCommonMap = QMap<int, Trap* >();
    realizationsInProject = QMap<int, Realization* >();

//    commonParentPointer = parent; // родителем для всех виджетов проекта служит Объект главного окна (MainWnd)

    m_project_header_file = new QFile(this);
}

Project::~Project()
{
    delete m_project_header_file;
}

int Project::getThisProjectNumb()
{
    return m_thisProjectNumber;
}

void Project::setThisProjectNumb(int nmb)
{
    m_thisProjectNumber = nmb;
}

QString Project::getDescription()
{
    return m_Description;
}

void Project::setDescription(QString newDescription)
{
    m_Description = newDescription;
}

QString Project::getProjectName()
{
    return m_projectName;
}

void Project::setProjectName(QString newName)
{
    m_projectName = newName;
}

QString Project::getStringFileNameOfProject()
{
    return m_stringFileName;
}

bool Project::isSavedProjectWithoutUnsavedChanges()
{
    return !m_unsavedChangesExist;
}

void Project::setUnSavedChangesExist()
{
    m_unsavedChangesExist = true;
}

QVector<int> Project::retWellsIDsOnStructure(int structureID)
{
    QVector<int> vec4return = QVector<int>();

    Structure *structure = structuresInProject.value(structureID);

    for (int i_idTrap = 0;
         i_idTrap < structure->trapsNumbersFromCommonMap_onStructure.size();
         ++i_idTrap)
    {
        int trapID = structure->trapsNumbersFromCommonMap_onStructure.at(i_idTrap);

        Trap *trap = trapsInProjectCommonMap.value(trapID);

        for (int i_idWell = 0;
             i_idWell < trap->IDofWellsRecommendedForExplorationOfTrap.size();
             ++i_idWell)
        {
            int idWell = trap->IDofWellsRecommendedForExplorationOfTrap.at(i_idWell);

            if (vec4return.isEmpty())
            {
                vec4return.append(idWell);
            }
            else if (!vec4return.contains(idWell)) vec4return.append(idWell);
        }
    }

    return vec4return;
}

void Project::loadStratums()
{
    QDir dirOfProjectData;
    QFileInfo fi_prjHeader(*m_project_header_file);

    dirOfProjectData.setPath(fi_prjHeader.absoluteDir().path());

    dirOfProjectData.cd(fi_prjHeader.baseName() + QString(".projectData"));

    dirOfProjectData.cd(QString("00_stratums.dat"));

    int stratumFilesCount = dirOfProjectData.entryInfoList().count();

    for (int i_stratumFile = 0; i_stratumFile < stratumFilesCount; ++i_stratumFile)
    {
        QFileInfo fi_sf = dirOfProjectData.entryInfoList().at(i_stratumFile);

        if (fi_sf.fileName() == QString(".") || fi_sf.fileName() == QString("..")) continue;

        QFile f_stratum(fi_sf.absoluteFilePath());

        if (f_stratum.open(QIODevice::ReadOnly))
        {
            QDataStream instr(&f_stratum);
            instr.setVersion(QDataStream::Qt_4_8);

            Stratum *stratum = new Stratum;

            instr >> stratum->numberOfStratumInCommonMapOfProject; //id
            instr >> stratum->nameOfStratum;
            instr >> stratum->description;

            stratumsNamesByNumber.insert(stratum->numberOfStratumInCommonMapOfProject,
                                         stratum);

            f_stratum.close();
        }
        else
        {
            QMessageBox::critical(NULL,
                                  trUtf8("Критческая ошибка!"),
                                  f_stratum.errorString());

            return;
        }
    }
}

void Project::loadTraps()
{
    QDir dirOfProjectData;
    QFileInfo fi_prjHeader(*m_project_header_file);

    dirOfProjectData = fi_prjHeader.absoluteDir();

    dirOfProjectData.cd(fi_prjHeader.baseName() + QString(".projectData"));

    dirOfProjectData.cd(QString("01_traps.dat"));

    int trapFilesCount = dirOfProjectData.entryInfoList().count();

    for (int i_trapFile = 0; i_trapFile < trapFilesCount; ++i_trapFile)
    {
        QFileInfo fi_tf = dirOfProjectData.entryInfoList().at(i_trapFile);

        QFile f_trap(fi_tf.absoluteFilePath());

        if (f_trap.open(QIODevice::ReadOnly))
        {
            QDataStream instr(&f_trap);
            instr.setVersion(QDataStream::Qt_4_8);

            Trap *trap = new Trap;

            int id_trap = 0;
            instr >> id_trap;
            instr >> trap->nameTrap;
            instr >> trap->nameOfStratum;
            instr >> trap->commentForTrap;
            int allocTypeKey = -1;
            instr >> allocTypeKey;
            trap->allocationType->setAllocTypeInt_Key(allocTypeKey);
            instr >> trap->description;
            instr >> trap->baseData->Area.minimum;
            instr >> trap->baseData->Area.average;
            instr >> trap->baseData->Area.maximum;
            instr >> trap->baseData->DensityOfOil.minimum;
            instr >> trap->baseData->DensityOfOil.average;
            instr >> trap->baseData->DensityOfOil.maximum;
            instr >> trap->baseData->determinedKIN;
            instr >> trap->baseData->EffectiveThick.minimum;
            instr >> trap->baseData->EffectiveThick.average;
            instr >> trap->baseData->EffectiveThick.maximum;
            instr >> trap->baseData->KIN.minimum;
            instr >> trap->baseData->KIN.average;
            instr >> trap->baseData->KIN.maximum;
            instr >> trap->baseData->numberOfAllocationType;
            instr >> trap->baseData->numberOfWells;
            instr >> trap->baseData->OilSaturationCoeff.minimum;
            instr >> trap->baseData->OilSaturationCoeff.average;
            instr >> trap->baseData->OilSaturationCoeff.maximum;
            instr >> trap->baseData->PorosityCoeff.minimum;
            instr >> trap->baseData->PorosityCoeff.average;
            instr >> trap->baseData->PorosityCoeff.maximum;
            instr >> trap->baseData->probabilityOfDepositExisting;
            instr >> trap->baseData->ShrinkingCoeff.minimum;
            instr >> trap->baseData->ShrinkingCoeff.average;
            instr >> trap->baseData->ShrinkingCoeff.maximum;
            int wellsIDsCount = 0;
            instr >> wellsIDsCount;
            for (int i_well = 0; i_well < wellsIDsCount; ++i_well)
            {
                int id_current = 0;
                instr >> id_current;
                trap->IDofWellsRecommendedForExplorationOfTrap.append(id_current);
            }

            trapsInProjectCommonMap.insert(id_trap, trap);

            f_trap.close();
        }
    }
}

void Project::loadStructures()
{
    QDir dirOfProjectData;
    QFileInfo fi_prjHeader(*m_project_header_file);

    dirOfProjectData = fi_prjHeader.absoluteDir();

    dirOfProjectData.cd(fi_prjHeader.baseName() + QString(".projectData"));

    dirOfProjectData.cd(QString("02_structures.dat"));

    int structureFilesCount = dirOfProjectData.entryInfoList().count();

    for (int i_structureFile = 0; i_structureFile < structureFilesCount; ++i_structureFile)
    {
        QFileInfo fi_sf = dirOfProjectData.entryInfoList().at(i_structureFile);

        QFile f_structure(fi_sf.absoluteFilePath());

        if (f_structure.open(QIODevice::ReadOnly))
        {
            QDataStream instr(&f_structure);
            instr.setVersion(QDataStream::Qt_4_8);

            Structure *structure = new Structure(this);

            int id_structure = 0;
            instr >> id_structure;
            QString name, description;
            instr >> name;
            structure->setNameStruct(name);
            instr >> description;
            structure->setDescription(description);
            int trapsOnStructureCount = 0;
            instr >> trapsOnStructureCount;
            for (int i_trap = 0; i_trap < trapsOnStructureCount; ++i_trap)
            {
                int trapId = 0;
                instr >> trapId;
                structure->trapsNumbersFromCommonMap_onStructure.append(trapId);
            }

            structuresInProject.insert(id_structure, structure);

            f_structure.close();
        }
    }
}

void Project::loadWells()
{
    QDir dirOfProjectData;
    QFileInfo fi_prjHeader(*m_project_header_file);

    dirOfProjectData = fi_prjHeader.absoluteDir();

    dirOfProjectData.cd(fi_prjHeader.baseName() + QString(".projectData"));

    dirOfProjectData.cd(QString("03_wells.dat"));

    int wellFilesCount = dirOfProjectData.entryInfoList().count();

    for (int i_wellFile = 0; i_wellFile < wellFilesCount; ++i_wellFile)
    {
        QFileInfo fi_wf = dirOfProjectData.entryInfoList().at(i_wellFile);

        QFile f_well(fi_wf.absoluteFilePath());

        if (f_well.open(QIODevice::ReadOnly))
        {
            QDataStream instr(&f_well);
            instr.setVersion(QDataStream::Qt_4_8);

            Well *well = new Well;

            int wellId = 0;
            instr >> wellId;
            well->wellID = wellId;
            instr >> well->wellName;
            instr >> well->wellDescription;
            instr >> well->negativeResultProbability;

            wellsInProject.insert(wellId, well);

            f_well.close();
        }
    }
}

void Project::loadRealizations()
{
}

void Project::saveStratums()
{
    QDir dirOfProjectData;
    QFileInfo fi_prjHeader(*m_project_header_file);

    dirOfProjectData = fi_prjHeader.absoluteDir();

    dirOfProjectData.mkdir(fi_prjHeader.baseName() + QString(".projectData"));
    dirOfProjectData.cd(fi_prjHeader.baseName() + QString(".projectData"));

    if (!dirOfProjectData.mkdir(QString("00_stratums.dat")))
    {
        dirOfProjectData.cd(QString("00_stratums.dat"));

        QFileInfoList flist4delete = dirOfProjectData.entryInfoList();

        for (int i = 0; i < flist4delete.size(); ++i)
        {
            QFile fl4del(flist4delete.at(i).absoluteFilePath());
            fl4del.remove();
        }
    }
    else
    {
        dirOfProjectData.cd(QString("00_stratums.dat"));
    }

    for (int i_stratum_id = 0; i_stratum_id < stratumsNamesByNumber.keys().size(); ++i_stratum_id)
    {
        int id = stratumsNamesByNumber.keys().at(i_stratum_id);
        Stratum *stratum = stratumsNamesByNumber.value(id);

        QFileInfo fi_stratum4save(
                    dirOfProjectData.absolutePath() + QString(QDir::separator())
                    + QString("stratum.") + QString::number(i_stratum_id)
                    );

        QFile f_stratum(fi_stratum4save.absoluteFilePath());

        if (f_stratum.open(QIODevice::WriteOnly))
        {
            QDataStream outstr(&f_stratum);
            outstr.setVersion(QDataStream::Qt_4_8);

            outstr << stratumsNamesByNumber.key(stratum); // id
            outstr << stratum->nameOfStratum;
            outstr << stratum->description;

            f_stratum.close();
        }
    }
}

void Project::saveTraps()
{
    QDir dirOfProjectData;
    QFileInfo fi_prjHeader(*m_project_header_file);

    dirOfProjectData = fi_prjHeader.absoluteDir();

    dirOfProjectData.mkdir(fi_prjHeader.baseName() + QString(".projectData"));
    dirOfProjectData.cd(fi_prjHeader.baseName() + QString(".projectData"));

    if (!dirOfProjectData.mkdir(QString("01_traps.dat")))
    {
        dirOfProjectData.cd(QString("01_traps.dat"));

        QFileInfoList flist4delete = dirOfProjectData.entryInfoList();

        for (int i = 0; i < flist4delete.size(); ++i)
        {
            QFile fl4del(flist4delete.at(i).absoluteFilePath());
            fl4del.remove();
        }
    }
    else
    {
        dirOfProjectData.cd(QString("01_traps.dat"));
    }

    for (int i_trap_id = 0; i_trap_id < trapsInProjectCommonMap.keys().size(); ++i_trap_id)
    {
        int id = trapsInProjectCommonMap.keys().at(i_trap_id);
        Trap *trap = trapsInProjectCommonMap.value(id);

        QFileInfo fi_trap4save(
                    dirOfProjectData.absolutePath() + QString(QDir::separator())
                    + QString("trap.") + QString::number(i_trap_id)
                    );

        QFile f_trap(fi_trap4save.absoluteFilePath());

        if (f_trap.open(QIODevice::WriteOnly))
        {
            QDataStream outstr(&f_trap);
            outstr.setVersion(QDataStream::Qt_4_8);

            outstr << trapsInProjectCommonMap.key(trap); //id
            outstr << trap->nameTrap;
            outstr << trap->nameOfStratum;
            outstr << trap->commentForTrap;
            outstr << trap->allocationType->getAllocTypeInt_Key();
            outstr << trap->description;
            outstr << trap->baseData->Area.minimum;
            outstr << trap->baseData->Area.average;
            outstr << trap->baseData->Area.maximum;
            outstr << trap->baseData->DensityOfOil.minimum;
            outstr << trap->baseData->DensityOfOil.average;
            outstr << trap->baseData->DensityOfOil.maximum;
            outstr << trap->baseData->determinedKIN;
            outstr << trap->baseData->EffectiveThick.minimum;
            outstr << trap->baseData->EffectiveThick.average;
            outstr << trap->baseData->EffectiveThick.maximum;
            outstr << trap->baseData->KIN.minimum;
            outstr << trap->baseData->KIN.average;
            outstr << trap->baseData->KIN.maximum;
            outstr << trap->baseData->numberOfAllocationType;
            outstr << trap->baseData->numberOfWells;
            outstr << trap->baseData->OilSaturationCoeff.minimum;
            outstr << trap->baseData->OilSaturationCoeff.average;
            outstr << trap->baseData->OilSaturationCoeff.maximum;
            outstr << trap->baseData->PorosityCoeff.minimum;
            outstr << trap->baseData->PorosityCoeff.average;
            outstr << trap->baseData->PorosityCoeff.maximum;
            outstr << trap->baseData->probabilityOfDepositExisting;
            outstr << trap->baseData->ShrinkingCoeff.minimum;
            outstr << trap->baseData->ShrinkingCoeff.average;
            outstr << trap->baseData->ShrinkingCoeff.maximum;
            outstr << trap->IDofWellsRecommendedForExplorationOfTrap.size();
            for (int i_well = 0; i_well < trap->IDofWellsRecommendedForExplorationOfTrap.size(); ++i_well)
            {
                outstr << trap->IDofWellsRecommendedForExplorationOfTrap.at(i_well);
            }

            f_trap.close();
        }
    }
}

void Project::saveStructures()
{
    QDir dirOfProjectData;
    QFileInfo fi_prjHeader(*m_project_header_file);

    dirOfProjectData = fi_prjHeader.absoluteDir();

    dirOfProjectData.mkdir(fi_prjHeader.baseName() + QString(".projectData"));
    dirOfProjectData.cd(fi_prjHeader.baseName() + QString(".projectData"));

    if (!dirOfProjectData.mkdir(QString("02_structures.dat")))
    {
        dirOfProjectData.cd(QString("02_structures.dat"));

        QFileInfoList flist4delete = dirOfProjectData.entryInfoList();

        for (int i = 0; i < flist4delete.size(); ++i)
        {
            QFile fl4del(flist4delete.at(i).absoluteFilePath());
            fl4del.remove();
        }
    }
    else
    {
        dirOfProjectData.cd(QString("02_structures.dat"));
    }

    for (int i_structure_id = 0; i_structure_id < structuresInProject.keys().size(); ++i_structure_id)
    {
        int id = structuresInProject.keys().at(i_structure_id);
        Structure *structure = structuresInProject.value(id);

        QFileInfo fi_structure4save(
                    dirOfProjectData.absolutePath() + QString(QDir::separator())
                    + QString("structure.") + QString::number(i_structure_id)
                    );

        QFile f_stucture(fi_structure4save.absoluteFilePath());

        if (f_stucture.open(QIODevice::WriteOnly))
        {
            QDataStream outstr(&f_stucture);
            outstr.setVersion(QDataStream::Qt_4_8);

            outstr << structuresInProject.key(structure); // id
            outstr << structure->getNameStruct();
            outstr << structure->getDescription();
            outstr << structure->trapsNumbersFromCommonMap_onStructure.size();
            for (int i_trapsOnStr = 0;
                 i_trapsOnStr < structure->trapsNumbersFromCommonMap_onStructure.size();
                 ++i_trapsOnStr)
            {
                outstr << structure->trapsNumbersFromCommonMap_onStructure.at(i_trapsOnStr);
            }

            f_stucture.close();
        }
    }
}

void Project::saveWells()
{
    QDir dirOfProjectData;
    QFileInfo fi_prjHeader(*m_project_header_file);

    dirOfProjectData = fi_prjHeader.absoluteDir();

    dirOfProjectData.mkdir(fi_prjHeader.baseName() + QString(".projectData"));
    dirOfProjectData.cd(fi_prjHeader.baseName() + QString(".projectData"));

    if (!dirOfProjectData.mkdir(QString("03_wells.dat")))
    {
        dirOfProjectData.cd(QString("03_wells.dat"));

        QFileInfoList flist4delete = dirOfProjectData.entryInfoList();

        for (int i = 0; i < flist4delete.size(); ++i)
        {
            QFile fl4del(flist4delete.at(i).absoluteFilePath());
            fl4del.remove();
        }
    }
    else
    {
        dirOfProjectData.cd(QString("03_wells.dat"));
    }

    for (int i_well_id = 0; i_well_id < wellsInProject.keys().size(); ++i_well_id)
    {
        int id = wellsInProject.keys().at(i_well_id);
        Well *well = wellsInProject.value(id);

        QFileInfo fi_well4save(
                    dirOfProjectData.absolutePath() + QString(QDir::separator())
                    + QString("well.") + QString::number(i_well_id)
                    );

        QFile f_well(fi_well4save.absoluteFilePath());

        if (f_well.open(QIODevice::WriteOnly))
        {
            QDataStream outstr(&f_well);
            outstr.setVersion(QDataStream::Qt_4_8);

            outstr << wellsInProject.key(well); //id
            outstr << well->wellName;
            outstr << well->wellDescription;
            outstr << well->negativeResultProbability;

            f_well.close();
        }
    }
}

void Project::saveRealizations()
{
}

void Project::slotLoad()
{
    QString prjHeaderFileName = QFileDialog::getOpenFileName(NULL,//this,
                                                       trUtf8("Открыть файл-заголовок и данные проекта"),
                                                       QString(),
                                                       QString("*.lrprhdr")
                                                       );

    if (prjHeaderFileName.isEmpty())
        return;

    m_stringFileName = prjHeaderFileName;

    m_project_header_file->setFileName(m_stringFileName);

    if (m_project_header_file->open(QIODevice::ReadOnly))
    {
        QDataStream instream(m_project_header_file);
        instream.setVersion(QDataStream::Qt_4_8);

        int stratumsNumber;
        int trapsNumber;
        int structuresNumber;
        int wellsNumber;
        int realizationsNumber;

        QString begHeader = QString("LocalResourcesProbabilityandRisksAnalizerProject");
        QByteArray ba = begHeader.toAscii();
        int rawBeginningHeader = ba.count();

        const int rawAllInHeader = rawBeginningHeader;

        char arr_str[rawAllInHeader];

        for (int ich = 0; ich < rawAllInHeader; ++ich)
        {
            arr_str[ich] = (char) 0xaa;
        }

        char *data_str = arr_str;

        instream.readRawData(data_str, rawAllInHeader);

        QByteArray baReadHeaderBeginning((const char*) data_str, rawBeginningHeader);

        if (baReadHeaderBeginning != ba)
        {
            QMessageBox::critical(NULL, //this,
                                  trUtf8("Ошибка в заголовке файла!"),
                                  trUtf8("Неверное содержимое элемента заголовка файла - описателя проекта."));
            m_project_header_file->close();
            return;
        }

        instream.skipRawData(7);

        instream >> stratumsNumber;
        instream >> trapsNumber;
        instream >> structuresNumber;
        instream >> wellsNumber;
        instream >> realizationsNumber;

        instream >> m_projectName;

        instream >> m_Description;

        quint32 endMarker;

        instream >> endMarker;

        if (endMarker != ((quint32) 0xa0b7ccdf))
        {
            QMessageBox::information(NULL, //this,
                                  trUtf8("Ошибка в заголовке файла!"),
                                  trUtf8("Неверный маркер окончания файла - описателя проекта.")
                                     + trUtf8("\nДанная ошибка должна исчезнуть после пересохранения проекта."));
        }

        m_project_header_file->close();

        if (stratumsNumber > 0) loadStratums();
        if (trapsNumber > 0) loadTraps();
        if (structuresNumber > 0) loadStructures();
        if (wellsNumber > 0) loadWells();
        if (realizationsNumber > 0) loadRealizations();
    }
}

void Project::slotImportFromTextFileWithDelimeters()
{
}

void Project::slotSave()
{
    m_project_header_file->setFileName(m_stringFileName);

    if (m_project_header_file->open(QIODevice::WriteOnly))
    {
        QDataStream outstream(m_project_header_file);
        outstream.setVersion(QDataStream::Qt_4_8);

        int stratumsNumber = stratumsNamesByNumber.size();
        int trapsNumber = trapsInProjectCommonMap.size();
        int structuresNumber = structuresInProject.size();
        int wellsNumber = wellsInProject.size();
        int realizationsNumber = realizationsInProject.size();

        QString begHeader = QString("LocalResourcesProbabilityandRisksAnalizerProject");
        QByteArray ba = begHeader.toAscii();
        int rawBeginningHeader = ba.length();
        outstream.writeRawData(ba.constData(), rawBeginningHeader);
        int rawDataOneLenghtInBytes = 7;
        ba.resize(rawDataOneLenghtInBytes);
        for (int i = 0; i < ba.size(); ++i)
        {
            char rndCharVal = (char) rand() % 256;
            ba[i] = rndCharVal;
        }
        int outRes1 = outstream.writeRawData(ba, rawDataOneLenghtInBytes);
        if (outRes1 != rawDataOneLenghtInBytes)
        {
            QMessageBox::critical(NULL, //this,
                                  trUtf8("СБОЙ!"),
                                  trUtf8("Ошибка записи файла-заголовка проекта"));
            m_project_header_file->close();
            return;
        }

        outstream << stratumsNumber;
        outstream << trapsNumber;
        outstream << structuresNumber;
        outstream << wellsNumber;
        outstream << realizationsNumber;

        outstream << m_projectName;

        outstream << m_Description;

        quint32 endMarker = (quint32)0xa0b7ccdf;

        outstream << endMarker;

        m_project_header_file->close();

        if (stratumsNumber > 0) saveStratums();
        if (trapsNumber > 0) saveTraps();
        if (structuresNumber > 0) saveStructures();
        if (wellsNumber > 0) saveWells();
        if (realizationsNumber > 0) saveRealizations();

        m_unsavedChangesExist = false;
    }
}

void Project::slotSaveAs()
{
    m_stringFileName = QFileDialog::getSaveFileName(NULL, //this,
                                                       trUtf8("Сохранить файл-заголовок и данные проекта"),
                                                       QString(),
                                                       QString("*.lrprhdr")
                                                       );

    QStringList nm_ext = m_stringFileName.split(QString(".lrprhdr"));

    if (nm_ext.size() < 2 && nm_ext.size() > 0)
        m_stringFileName.append(QString(".lrprhdr"));

    slotSave();
}

void Project::slotClose()
{
    //Project::~Project();
}

void Project::slotChangeProjectName(QString newProjectName)
{
    m_projectName = newProjectName;
}
