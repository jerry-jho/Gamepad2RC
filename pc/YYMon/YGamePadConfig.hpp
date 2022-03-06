#pragma once


#include <QtCore>
#include <QtWidgets>
#include <QDebug>

#include "YGamePad.hpp"

class YGamePadConfig : public QDialog {
  Q_OBJECT
public:
  explicit YGamePadConfig(YGamePad * gamepad, QStringList * list, QWidget * parent = nullptr) : 
    QDialog(parent), m_gamepad(gamepad), m_list(list), m_configing(false) {

      m_signalMapper = new QSignalMapper(this);

      auto mainLay = new QVBoxLayout();

      QHBoxLayout * tlay = new QHBoxLayout();
      tlay->addWidget(new QLabel("Current Joystick"));

      auto edtGamePadName = new QLineEdit();
      edtGamePadName->setText(gamepad->name());
      edtGamePadName->setReadOnly(true);
      tlay->addWidget(edtGamePadName);

      mainLay->addLayout(tlay);

      auto lay = new QGridLayout();
      for (int i=0;i<m_list->size();i++) {
        auto item = m_list->at(i);
        auto edt = new QLineEdit();
        edt->setReadOnly(true);
        edt->setText(m_gamepad->mapped(item));
        m_edtMap[item] = edt;
        auto sld = new QProgressBar();
        sld->setRange(0, 100);
        m_sliderMap[item] = sld;
        auto btn = new QPushButton("Config...");
        m_btnMap[item] = btn;
        connect(btn, SIGNAL(clicked(bool)), m_signalMapper, SLOT(map()));
        m_signalMapper->setMapping(btn, item);

        lay->addWidget(new QLabel(item), i, 0);
        lay->addWidget(edt, i, 1);
        lay->addWidget(sld, i, 2);
        lay->addWidget(btn, i, 3);

      }
      auto btnGroup = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
      connect(btnGroup, SIGNAL(accepted()), this, SLOT(accept()));
      connect(btnGroup, SIGNAL(rejected()), this, SLOT(reject()));

      connect(m_signalMapper, &QSignalMapper::mappedString,
              this, &YGamePadConfig::clicked);
      connect(m_gamepad, &YGamePad::onRawAction,
              this, &YGamePadConfig::gamepadValue);
      mainLay->addLayout(lay);
      mainLay->addStretch();
      mainLay->addWidget(btnGroup);
      this->setLayout(mainLay);
      this->resize(850,300);
    }

private:
  YGamePad * m_gamepad;
  QStringList * m_list;
  QMap<QString, QProgressBar *> m_sliderMap;
  QMap<QString, QLineEdit *> m_edtMap;
  QMap<QString, QPushButton *> m_btnMap;
  QSignalMapper *m_signalMapper;
  bool m_configing;

public slots:
  void clicked(const QString &item) {
    if (m_configing) {
      m_btnMap[item]->setText("Config...");
      for (auto aitem : * m_list) {
        m_btnMap[aitem]->setEnabled(true);
      }
      m_configing = false;
      m_gamepad->setMap(m_edtMap[item]->text(), item);
      return;
    }
    for (auto aitem : * m_list) {
      if (aitem != item) {
        m_btnMap[aitem]->setEnabled(false);
      } else {
        m_btnMap[aitem]->setText("Done");
      }
    }
    m_configing = true;
  }

  void gamepadValue(QString buttonName, double value) {
    if (!m_configing) {
      qDebug()<<buttonName<<value;
      for (auto aitem : * m_list) {
        if (m_edtMap[aitem]->text() == buttonName) {
          m_sliderMap[aitem]->setValue(value * 100);
        }
      }
      return;
    }
    if (value < 0.0001) return;
    QString item;
    for (auto aitem : * m_list) {
      if (m_btnMap[aitem]->isEnabled()) {
        item = aitem;
        break;
      }
    }
    m_edtMap[item]->setText(buttonName);
    m_sliderMap[item]->setValue(value * 100);
  }
};

