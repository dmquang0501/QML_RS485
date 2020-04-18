#ifndef SENSORMODEL_H
#define SENSORMODEL_H

#include <QAbstractListModel>

class SensorList;

class SensorModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(SensorList *list READ list WRITE setList)

public:
	explicit SensorModel(QObject *parent = nullptr);
	SensorList *list() const;
	void setList(SensorList *list);

	enum{
		DoneRole = Qt::UserRole,
		DescriptionRole,
		Data1Role,
		Data2Role
	};

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	// Editable:SensorList.data1
	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;

	virtual QHash<int, QByteArray> roleNames() const override;

public slots:


private:
	SensorList *mList;
};

#endif // SENSORMODEL_H
