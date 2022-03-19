#include <QtWidgets>
#include <QtCore>
#include "YGamePadConfig.hpp"
#include "YGamePadManager.hpp"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("MySoft");
    QApplication::setOrganizationDomain("mysoft.com");
    QApplication::setApplicationName("YY Cars Monitor");

    QTranslator translator;
    translator.load(":yymon.qm");
    app.installTranslator(&translator);

    QFile f(":qdarkstyle/dark/style.qss");
    if (f.exists()) {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
    qInfo() << "Starting GamePad Manager ...\n";
    YGamePadManager manager;
    if (manager.exec() == QDialog::Rejected) {
      qInfo() << "No GamePad Selected, exit\n";
      return -1;
    }
    int deviceID = manager.getDeviceID();
    if (deviceID < 0) return -2;
    auto m_gamepad = new QGamepad(deviceID);

    qDebug()<<m_gamepad->name();
    YGamePad gamepad(m_gamepad);
//     //gamepad.setMap("BA", "MyKey");

// //    QObject::connect(&gamepad, &YGamePad::onAction, &app, [](QString name, double value){
// //        qDebug() << name << value;
// //    });

     auto Keys = QStringList() << "Left" << "Right" << "Forward" << "Backward";


     YGamePadConfig ui(&gamepad, &Keys);
     ui.show();
     return app.exec();
     //return 0;
}
