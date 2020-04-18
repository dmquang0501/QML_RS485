#include<iostream>
using namespace std;
#include<fstream>

#include "sensorlist.h"

SensorList::SensorList(QObject *parent) : QObject(parent){
	SensorItem a;
	a.done = true;
	a.m_ID = 255;
	a.description = "Sensor id " + QString::number(a.m_ID);
	a.m_data1 = 0;
	a.m_data2 = 0;
	mItems.push_back(a);

//	ifstream file;
//	file.open("/home/quang/v3sModbus/listsensor.txt");
//	int n = 0;
//	file >> n;
//	qDebug() << QString("n = %1").arg(n);
//	for(int i = 0; i < n; i++){
//		SensorItem m;
//		mItems.push_back(m);
//		file >> mItems[i].m_ID;
//		mItems[i].done = false;
//		mItems[i].description = "Sensor id " + QString::number(mItems[i].m_ID);
//	}
//	file.close();

//	file.open("/home/quang/v3sModbus/listsensor1.txt");
//	for(int i = 0; i < n; i++){
//		file >> mItems[i].m_data1;
//	}
//	file.close();

//	file.open("/home/quang/v3sModbus/listsensor2.txt");
//	for(int i = 0; i < n; i++){
//		file >> mItems[i].m_data2;
//	}
//	file.close();

	Baud = 9600;
	m_timer = 1;
	m_index = 0;
	for(int i = 0; i < 20; i++){
		array[i] = 0;
	}
	m_resul = 0;
	m_setdata = 0;
}

QVector<SensorItem> SensorList::items() const
{
	return mItems;
}

bool SensorList::setItemAt(int index, const SensorItem &item)
{
	if(index < 0 || index >= mItems.size())
		return false;

	const SensorItem &oldItem = mItems.at(index);
	if(item.done == oldItem.done && item.description == oldItem.description && item.m_data1 == oldItem.m_data1 && item.m_data2 == oldItem.m_data2)
		return false;

	mItems[index] = item;
	return true;
}

int SensorList::index(){
	QString("******* Index return = %1").arg(m_index);
	return m_index;
}

void SensorList::setIndex(int value)
{
	if(m_index != value){
		m_index = value;
	}
	emit indexChanged();
}

void SensorList::appendItem(){
	emit preItemAppended();

	SensorItem item;
	item.done = false;
	item.description = "Sensor new";
	item.m_data1 = 0;
	item.m_data2 = 0;
	mItems.append(item);

	emit postItemAppended();
}

void SensorList::removeCompletedItems(){
	for(int i = 0; i < mItems.size(); ){
		if(mItems.at(i).done){
			emit preItemRemoved();

			mItems.removeAt(i);

			emit postItemRemoved();
		} else {
			++i;
		}
	}

	ofstream file1("/home/quang/v3sModbus/listsensor.txt");
	file1 << mItems.size()<<endl;
	for(int i = 0; i < mItems.size(); i++){
		file1 << mItems[i].m_ID<<endl;
	}
	file1.close();

	ofstream file("/home/quang/v3sModbus/listsensor1.txt");
	for(int i = 0; i < mItems.size(); i++){
		file << mItems[i].m_data1<<endl;
	}
	file.close();

	ofstream file2("/home/quang/v3sModbus/listsensor2.txt");
	for(int i = 0; i < mItems.size(); i++){
		file2 << mItems[i].m_data2<<endl;
	}
	file2.close();
}

void SensorList::myConnect() {
	client = new QModbusRtuSerialMaster();

	client->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
								   "/dev/ttyUSB0");
	client->setConnectionParameter(QModbusDevice::SerialParityParameter,
								   QSerialPort::NoParity);
	client->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
								   QString::number(Baud));
	client->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
								   QSerialPort::Data8);
	client->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
								   QSerialPort::OneStop);

	if (!client->connectDevice())
		qDebug() << "cannot connect ";
	else
		qDebug() << "connect ";

	qDebug() << "error: " << client->errorString();
	qDebug() << "state: " << client->state();
}

void SensorList::myReadData(int n){
	QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, n, 2);
	for(int i = 0; i < mItems.size(); ){
		if(mItems.at(i).done){
			for(int j = 0 ; j < 2; j++){
				if (auto *reply = client->sendReadRequest(readUnit, mItems[i].m_ID)) {
					if (!reply->isFinished()){
						m_index = i;
						qDebug() << QString("m_index = %1").arg(m_index);
						//      connect the finished signal of the request to your read slot
						connect(reply, &QModbusReply::finished, this, [=]{

							QModbusReply *replyy = qobject_cast<QModbusReply *>(sender());
							const QModbusDataUnit result = replyy->result();
							setData1(result.value(0));
							setData2(result.value(1));
							for (int j = 0; j < 2; j++){
								qDebug() << QString("The value of %1 is %2").arg(j).arg(result.value(j));
							}
						});
					}
				} else
					qDebug() << "request error";
			}
			break;
		} else {
			++i;
		}
	}
}


void SensorList::myWriteData(){
	QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, 100, 1);

	for(int i = 0; i < mItems.size(); ){
		if(mItems.at(i).done){
			writeUnit.setValue(0, mItems[i].m_newID);
			for(int j = 0; j < 3; j++){
				client->sendWriteRequest(writeUnit, mItems[i].m_ID);
			}
			mItems[i].m_ID = mItems[i].m_newID;
			break;
		} else {
			++i;
		}
	}

	ofstream file1("/home/quang/v3sModbus/listsensor.txt");
	file1 << mItems.size()<<endl;
	for(int i = 0; i < mItems.size(); i++){
		file1 << mItems[i].m_ID<<endl;
	}
	file1.close();
}

void SensorList::setBaudray(int n)
{
	Baud = n;
}

void SensorList::setTimer(int n)
{
	m_timer = n;
}

void SensorList::setSlave_id(int n){
	mItems[mItems.size()-1].m_ID = n;

	ofstream file1("/home/quang/v3sModbus/listsensor.txt");
	file1 << mItems.size()<<endl;
	for(int i = 0; i < mItems.size(); i++){
		file1 << mItems[i].m_ID<<endl;
	}
	file1.close();
	mItems[mItems.size()-1].description = "Sensor id " + QString::number(n);

	ofstream file("/home/quang/v3sModbus/listsensor1.txt");
	for(int i = 0; i < mItems.size(); i++){
		file << mItems[i].m_data1<<endl;
	}
	file.close();

	ofstream file2("/home/quang/v3sModbus/listsensor2.txt");
	for(int i = 0; i < mItems.size(); i++){
		file2 << mItems[i].m_data2<<endl;
	}
	file2.close();
}

void SensorList::setnew_id(int n){
	for(int i = 0; i < mItems.size(); ){
		if(mItems.at(i).done){
			mItems[i].m_newID = n;
			break;
		} else {
			++i;
		}
	}
}

void SensorList::endConnection() {
	client->disconnectDevice();
	serialTimer->stop();
	delete serialTimer;
	serialTimer = nullptr;
	qDebug() << QString("endConnect");
}

int SensorList::data1() const {
	return mItems[m_index].m_data1;
}

void SensorList::setData1(int value) {
	emit predata1();

	if(mItems[m_index].m_data1 != value) {
		mItems[m_index].m_data1 = value;
		mItems[m_index].description = "Sensor id " + QString::number(mItems[m_index].m_ID);
	}

	ofstream file1("/home/quang/v3sModbus/listsensor1.txt");
	for(int i = 0; i < mItems.size(); i++){
		file1 << mItems[i].m_data1<<endl;
	}
	file1.close();

	emit dataChanged1();
}

int SensorList::data2() const {
	return mItems[m_index].m_data2;
}

void SensorList::setData2(int value) {
	//	emit predata2();
	if(mItems[m_index].m_data2 != value) {
		mItems[m_index].m_data2 = value;
	}
	ofstream file1("/home/quang/v3sModbus/listsensor2.txt");
	for(int i = 0; i < mItems.size(); i++){
		file1 << mItems[i].m_data2<<endl;
	}
	file1.close();

	//	emit dataChanged2();
}


void SensorList::returndata()
{
//	for(int i = 0; i < 20; i++){
//		array[i] = 0;
//	}
//	m_resul = 0;
//	m_index = 0;

	ofstream file("/home/quang/v3sModbus/listsensor1.txt");
	for(int i = 0; i < mItems.size(); i++){
		file << mItems[i].m_data1<<endl;
	}
	file.close();

	ofstream file2("/home/quang/v3sModbus/listsensor2.txt");
	for(int i = 0; i < mItems.size(); i++){
		file2 << mItems[i].m_data2<<endl;
	}
	file2.close();
}

void SensorList::showdata()
{
	ifstream file;
	file.open("/home/quang/v3sModbus/listsensor.txt");
	int n = 0;
	file >> n;
	file.close();

	file.open("/home/quang/v3sModbus/listsensor1.txt");
	for(int i = 0; i < n; i++){
		file >> mItems[i].m_data1;
	}
	file.close();

	file.open("/home/quang/v3sModbus/listsensor2.txt");
	for(int i = 0; i < n; i++){
		file >> mItems[i].m_data2;
	}
	file.close();
}

void SensorList::upp()
{
	m_setdata++;
	qDebug() << QString("******* Index return = %1").arg(m_setdata);
}

void SensorList::down()
{
	if(m_setdata == 0){
		m_setdata = 0;
	} else{
		m_setdata--;
	}
	qDebug() << QString("******* Index return = %1").arg(m_setdata);

}

void SensorList::setautoread(){
	serialTimer = new QTimer();
	connect(serialTimer, &QTimer::timeout, this, &SensorList::query);
	serialTimer->start(m_timer*1000);
	qDebug() << "started reading";
}

void SensorList::query(){
	QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, 0, 2);
	int a = 0;
	for(int i = 0; i < mItems.size(); i++){
		if(mItems.at(i).done){
			array[a++] = i;
			qDebug() << QString("a = %1***************").arg(a);
			if (QModbusReply *reply = client->sendReadRequest(readUnit, mItems[i].m_ID)) {
				if (!reply->isFinished()){
					connect(reply, &QModbusReply::finished, this, [=](){
						qDebug() << QString("ID = %1***************").arg(mItems[i].m_ID);
						QModbusReply *replyy = qobject_cast<QModbusReply *>(sender());
						const QModbusDataUnit resultt = replyy->result();
						qDebug() << QString("Slave ID %1************************************").arg(mItems[i].m_ID);
						for (int k = 0; k < 2; k++){
							qDebug() << QString("The value of %1 is %2").arg(k).arg(resultt.value(k));
						}
						if(m_resul > a){
							m_resul = 0;
						}
						for(int k = m_resul; k < 20; k++){
							if(array[0] == 0){
								emit predata2();
								m_index = array[k];
								mItems[m_index].m_data1 = resultt.value(0);
								mItems[m_index].m_data2 = resultt.value(1);
								qDebug() << QString("index = %1").arg(m_index);
								emit dataChanged2();
								m_resul++;
								break;
							}
							if(array[k] != 0){
								emit predata2();
								m_index = array[k];
								mItems[m_index].m_data1 = resultt.value(0);
								mItems[m_index].m_data2 = resultt.value(1);
								qDebug() << QString("index = %1").arg(m_index);
								emit dataChanged2();
								m_resul++;
								break;
							}else {
								m_resul = 0;
								break;
							}
						}
					});
				}
			}
		}
	}
}




