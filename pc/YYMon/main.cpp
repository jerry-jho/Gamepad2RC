#include <QtWidgets>
#include <QtCore>
#include "YGamePadConfig.hpp"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("MySoft");
    QApplication::setOrganizationDomain("mysoft.com");
    QApplication::setApplicationName("YYTest");
    QFile f(":qdarkstyle/dark/style.qss");

    if (f.exists()) {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }


    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    if (gamepads.isEmpty()) {
        qDebug() << "Did not find any connected gamepads";
        return -1;
    }
    auto m_gamepad = new QGamepad(gamepads[0]);
    qDebug()<<m_gamepad->name();
    YGamePad gamepad(m_gamepad);
    //gamepad.setMap("BA", "MyKey");

    QObject::connect(&gamepad, &YGamePad::onAction, &app, [](QString name, double value){
        qDebug() << name << value;
    });

    auto Keys = QStringList() << "Left" << "Right" << "Forward" << "Backward";


    YGamePadConfig ui(&gamepad, &Keys);
    ui.show();
    return app.exec();
}
