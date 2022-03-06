#pragma once

#include <QtCore>
#include <QtGamepad>
#include <QDebug>

class YGamePad : public QObject {
  Q_OBJECT
  

public:
  explicit YGamePad(QGamepad * _m, QObject *parent = nullptr) : QObject(parent), m_gamepad(_m) {

    connect(m_gamepad, &QGamepad::axisLeftXChanged, this, &YGamePad::onALXChanged);

    connect(m_gamepad, &QGamepad::axisLeftYChanged, this, &YGamePad::onALYChanged);

    connect(m_gamepad, &QGamepad::axisRightXChanged, this, &YGamePad::onARXChanged);

    connect(m_gamepad, &QGamepad::axisRightYChanged, this, &YGamePad::onARYChanged);

    connect(m_gamepad, &QGamepad::buttonAChanged, this, &YGamePad::onBAChanged);

    connect(m_gamepad, &QGamepad::buttonBChanged, this, &YGamePad::onBBChanged);

    connect(m_gamepad, &QGamepad::buttonXChanged, this, &YGamePad::onBXChanged);

    connect(m_gamepad, &QGamepad::buttonYChanged, this, &YGamePad::onBYChanged);

    connect(m_gamepad, &QGamepad::buttonL1Changed, this, &YGamePad::onL1Changed);

    connect(m_gamepad, &QGamepad::buttonR1Changed, this, &YGamePad::onR1Changed);

    connect(m_gamepad, &QGamepad::buttonL2Changed, this, &YGamePad::onAL2Changed);

    connect(m_gamepad, &QGamepad::buttonR2Changed, this, &YGamePad::onAR2Changed);

    connect(m_gamepad, &QGamepad::buttonL3Changed, this, &YGamePad::onL3Changed);

    connect(m_gamepad, &QGamepad::buttonR3Changed, this, &YGamePad::onR3Changed);

    connect(m_gamepad, &QGamepad::buttonSelectChanged, this, &YGamePad::onSelectChanged);

    connect(m_gamepad, &QGamepad::buttonStartChanged, this, &YGamePad::onStartChanged);


    connect(m_gamepad, &QGamepad::buttonUpChanged, this, &YGamePad::onBUChanged);

    connect(m_gamepad, &QGamepad::buttonDownChanged, this, &YGamePad::onBDChanged);

    connect(m_gamepad, &QGamepad::buttonLeftChanged, this, &YGamePad::onBLChanged);

    connect(m_gamepad, &QGamepad::buttonRightChanged, this, &YGamePad::onBRChanged);

    m_name = m_gamepad->name();


  }

  QString name() {
    return m_name;
  }

  void setMap(QString buttonName, QString mapName) {
    QSettings settings;
    settings.setValue(QString("keymap/") + m_name + "/" + buttonName, mapName);
    settings.sync();
  }

  QString getMap(QString buttonName) {
    QSettings settings;
    return settings.value(QString("keymap/") + m_name + "/" + buttonName, buttonName).toString();    
  }

  QString mapped(QString mapName) {
    QSettings settings;
    settings.beginGroup(QString("keymap/") + m_name);
    for (auto key : settings.childKeys()) {
      //qDebug() << "check" << key << "for" << mapName;
      if (settings.value(key).toString() == mapName) return key;
    }
    return "";
  }


  ~YGamePad() {

  }

signals:
  void onAction(QString mappedName, double value);
  void onRawAction(QString buttonName, double value);

private slots:
  void onALXChanged(double value) {
    Action("ALX", value);
  }
  void onALYChanged(double value) {
    Action("ALY", value);
  }
  void onARXChanged(double value) {
    Action("ARX", value);
  }
  void onARYChanged(double value) {
    Action("ARY", value);
  }
  void onBAChanged(bool value) {
    Action("BA", value);
  }
  void onBBChanged(bool value) {
    Action("BB", value);
  }
  void onBXChanged(bool value) {
    Action("BX", value);
  }
  void onBYChanged(bool value) {
    Action("BY", value);
  }
  void onL1Changed(bool value) {
    Action("BL1", value);
  }
  void onR1Changed(bool value) {
    Action("BR1", value);
  }
  void onAL2Changed(double value) {
    Action("AL2", value);
  }
  void onAR2Changed(double value) {
    Action("AR2", value);
  }
  void onL3Changed(bool value) {
    Action("BL1", value);
  }
  void onR3Changed(bool value) {
    Action("BR1", value);
  }
  void onStartChanged(bool value) {
    Action("BST", value);
  }
  void onSelectChanged(bool value) {
    Action("BSL", value);
  }

  void onBUChanged(bool value) {
    Action("BU", value);
  }
  void onBDChanged(bool value) {
    Action("BD", value);
  }
  void onBLChanged(bool value) {
    Action("BL", value);
  }
  void onBRChanged(bool value) {
    Action("BR", value);
  }
private:

  void Action(QString name, bool value) {
    double dv = value ? 1 : 0;
    emit  onRawAction(name, dv);
    emit  onAction(getMap(name), dv);
  }

  void Action(QString name, double value) {
    QString aname;
    if (value >= 0.0) {
      aname = name + "+";
      emit  onRawAction(aname, value);
      emit  onAction(getMap(aname), value);
    }
    if (value <= 0.0) {
      aname = name + "-";
      value = -1 * value;
      emit  onRawAction(aname, value);
      emit  onAction(getMap(aname), value);
    }
  }

  QGamepad *m_gamepad;
  QString m_name;

};
