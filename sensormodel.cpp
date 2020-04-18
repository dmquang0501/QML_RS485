#include "sensormodel.h"

#include "sensorlist.h"

SensorModel::SensorModel(QObject *parent)
	: QAbstractListModel(parent)
	,mList(nullptr)
{
}

int SensorModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid() || !mList)
		return 0;

	return mList->items().size();
}

QVariant SensorModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || !mList)
		return QVariant();

	const SensorItem item = mList->items().at(index.row());
	switch (role) {
	case DoneRole:
		return  QVariant(item.done);
	case DescriptionRole:
		return QVariant(item.description);
	case Data1Role:
		return QVariant(item.m_data1);
	case Data2Role:
		return QVariant(item.m_data2);
	}

	return QVariant();
}

bool SensorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(!mList){
		return  false;
	}
	SensorItem item = mList->items().at(index.row());
	switch (role) {
	case DoneRole:
		item.done = value.toBool();
		break;
	case DescriptionRole:
		item.description = value.toString();
		break;
	case Data1Role:
		item.m_data1 = value.toInt();
		break;
	case Data2Role:
		item.m_data2 = value.toInt();
		break;
	}

	if (mList->setItemAt(index.row(), item)) {
		emit dataChanged(index, index, QVector<int>() << role);
		return true;
	}
	return false;
}

Qt::ItemFlags SensorModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return Qt::ItemIsEditable;
}

QHash<int, QByteArray> SensorModel::roleNames() const
{
	QHash<int, QByteArray> names;
	names[DoneRole] = "done";
	names[DescriptionRole] = "description";
	names[Data1Role] = "data1";
	names[Data2Role] = "data2";
	return names;
}


SensorList *SensorModel::list() const{
	return mList;
}

void SensorModel:: setList(SensorList *list){
	beginResetModel();

	if(mList) mList ->disconnect(this);

	mList = list;

	if (mList){
		connect(mList, &SensorList::predata1, this, [=](){
			for(int i = 0; i < mList->items().size(); ){
				if(mList->items().at(i).done){
					const int index = i;
					beginRemoveRows(QModelIndex(), index,index);
					endRemoveRows();
					beginInsertRows(QModelIndex(), index, index);
					break;
				} else {
					++i;
				}
			}
		});
		connect(mList, &SensorList::dataChanged1, this, [=](){
			endInsertRows();
		});

		connect(mList, &SensorList::predata2, this, [=](){
			beginRemoveRows(QModelIndex(), mList->index(),mList->index());
			endRemoveRows();
			beginInsertRows(QModelIndex(), mList->index(),mList->index());
		});
		connect(mList, &SensorList::dataChanged2, this, [=](){
			endInsertRows();
		});

		connect(mList, &SensorList::preItemAppended, this, [=](){
			const int index = mList->items().size();
			beginInsertRows(QModelIndex(), index, index);
		});
		connect(mList, &SensorList::postItemAppended, this, [=](){
			endInsertRows();
		});

		connect(mList, &SensorList::preItemRemoved, this, [=](){
			for(int i = 0; i < mList->items().size(); ){
				if(mList->items().at(i).done){
					const int index = i;
					beginRemoveRows(QModelIndex(), index,index);
					break;
				} else {
					++i;
				}
			}
		});
		connect(mList, &SensorList::postItemRemoved, this, [=](){
			endRemoveRows();
		});
	}

	endResetModel();
}
