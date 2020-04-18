#ifndef SENSORLIST_H
#define SENSORLIST_H

#include <QObject>
#include <QDebug>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusReply>
#include <QModbusRtuSerialMaster>
#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>

struct SensorItem{
	bool done;
	QString description;
	QString content;
	int m_Addressdata;
	int m_ID;
	int m_newID;
	int m_data1;
	int m_data2;
	QTimer *serialTimer;
};

class SensorList : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)

public:
	explicit SensorList(QObject *parent = nullptr);

	QVector<SensorItem> items()const;

	bool setItemAt(int index, const SensorItem &item);

	int index();
	void setIndex(int value);
	int data1() const;
	void setData1(int value);
	int data2() const;
	void setData2(int value);



signals:
	 void preItemAppended();
	 void postItemAppended();

	 void preItemRemoved();
	 void postItemRemoved();

	 void predata1();
	 void dataChanged1();

	 void predata2();
	 void dataChanged2();

	 void indexChanged();


public slots:
	void appendItem();
	void removeCompletedItems();
	void myConnect();
	void myReadData(int n);
	void myWriteData();
	void setBaudray(int n);
	void setTimer(int n);
	void setSlave_id(int n);
	void setnew_id(int n);
	void endConnection();
	void setautoread();
	void query();
	void returndata();
	void showdata();
	void upp();
	void down();

private:
	QVector<SensorItem> mItems;
	QModbusRtuSerialMaster *client;
	QTimer *serialTimer;
	int Baud;
	int m_timer;
	int m_index;
	int array[20];
	int m_resul;
	int m_setdata;
};



#endif // SENSORLIST_H
