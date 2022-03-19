#pragma once


#include <QtCore>
#include <QtWidgets>
#include <QDebug>

#include <QtGamepad>

class YGamePadManager : public QDialog {
  Q_OBJECT
public:
  explicit YGamePadManager(QWidget * parent = nullptr) : 
    QDialog(parent) {
      //printf("here\n");
      auto mainLay = new QVBoxLayout;
      QString L1 = tr("Select a Joystick from the following list");
      QString L2 = tr("If the list is empty, make sure you have pluged a Joystick in your computer");
      mainLay->addWidget(new QLabel(L1 + "\n" + L2));
      mainLay->addWidget(&m_label);
      mainLay->addWidget(&m_list);
      auto btnGroup = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
      btnGroup->button(QDialogButtonBox::Ok)->setText(tr("OK"));
      btnGroup->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
      connect(btnGroup, SIGNAL(accepted()), this, SLOT(accept()));
      connect(btnGroup, SIGNAL(rejected()), this, SLOT(reject()));
      connect(btnGroup, SIGNAL(helpRequested()), this, SLOT(onUpdateGamepad()));
      mainLay->addWidget(btnGroup);
      setLayout(mainLay);
      m_label.setText(QString(tr("0 Joystick(s) connected")));
      m_manager = QGamepadManager::instance();
      connect(m_manager, SIGNAL(connectedGamepadsChanged()), this, SLOT(onUpdateGamepadLater()));

      connect(&m_timer, SIGNAL(timeout()), this, SLOT(onUpdateGamepad()));
      //onUpdateGamepad();

    }
  int getDeviceID() {
    if (m_list.count()) {
      return m_list.currentRow();
    }
    return -1;
  }
  ~YGamePadManager() {}
private:
  QGamepadManager * m_manager;
  QListWidget m_list;
  QLabel m_label;
  QTimer m_timer;
private slots:
  void onUpdateGamepadLater() {
    m_timer.setSingleShot(true);
    m_timer.start(500);
  }
  void onUpdateGamepad() {
    m_list.clear();
    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    m_label.setText(QString(tr("%1 Joystick(s) connected")).arg(gamepads.size()));
    // if (gamepads.isEmpty()) {
    //      QMessageBox::critical(nullptr, "Error", "Did not find any connected JoySticks\n\nPress OK, plug in a JoyStick and Rerun");
    //      return;
    //  }
    //printf("%d\n", gamepads.size());
    for (int i = 0;i < gamepads.size(); i++) {
      int deviceID = gamepads.at(i);
      qDebug()<<"Device ID"<<deviceID;
      auto deviceName = QGamepadManager::instance()->gamepadName(deviceID);
      qDebug()<<"Device Name"<<deviceName;
      //printf("Device: %d\n", deviceID);
      m_list.addItem(deviceName);
    }
    if (gamepads.size()) {
      m_list.setCurrentRow(0);
    }
  }
};
