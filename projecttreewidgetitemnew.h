#ifndef PROJECTTREEWIDGETITEMNEW_H
#define PROJECTTREEWIDGETITEMNEW_H

#include <QObject>
#include <QTreeWidgetItem>

class ProjectTreeWidgetItemNew : public QTreeWidgetItem
{
public:
    ProjectTreeWidgetItemNew();
};

class CollectionItem : ProjectTreeWidgetItemNew
{
public:
    CollectionItem(QObject *parentDataObj);

};

#endif // PROJECTTREEWIDGETITEMNEW_H
