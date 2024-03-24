#include <print>

#include <QCoreApplication>
#include <QCanBus>
#include <QList>
#include <QString>

namespace Can{
    void listPlugins(){
        auto canBus = QCanBus::instance();
        if(!canBus){
            std::println("ERR {}() canBus==nullptr", __FUNCTION__);
            return;
        }
        auto plugins = canBus->plugins();
        std::println("plugins:");
        for(const auto& plugin: plugins){
            std::println("  {}", plugin.toLocal8Bit().constData());
        }
    }

    void listDevices(){
        auto canBus = QCanBus::instance();
        if(!canBus){
            std::println("ERR {}() canBus==nullptr", __FUNCTION__);
            return;
        }
        QString errStr;
        const QList<QCanBusDeviceInfo> devices = canBus->availableDevices(QStringLiteral("peakcan"), &errStr);
        if(!errStr.isEmpty()){
            std::println("ERR {}() errStr: {}", __FUNCTION__, errStr.toLatin1().constData());
        }
        std::println("devices:");
        for(auto dev: devices){
            std::println("  alias             : {}", dev.alias().toLatin1().constData());
            std::println("  channel           : {}", dev.channel());
            std::println("  name              : {}", dev.name().toLatin1().constData());
            std::println("  plugin            : {}", dev.plugin().toLatin1().constData());
            std::println("  serial            : {}", dev.serialNumber().toLatin1().constData());
            std::println("  descr             : {}", dev.description().toLatin1().constData());
            std::println("  flex data rates   : {}", dev.hasFlexibleDataRate());
            std::println("  virtual           : {}", dev.isVirtual());
        }
    }
}

int main(int argc, char**argv){
    std::println("{}()", __FUNCTION__);
    QCoreApplication app(argc, argv);

    Can::listPlugins();
    Can::listDevices();

    QString errStr;
    auto device = QCanBus::instance()->createDevice(
          QStringLiteral("peakcan")
        , QStringLiteral("usb0")
        , &errStr
    );
    if(!device){
        qDebug() << errStr;
        return 1;
    }

    device->connect(device, &QCanBusDevice::stateChanged, [](QCanBusDevice::CanBusDeviceState state){
        std::println("stateChanged: {}", (int)state);
    });
    device->connect(device, &QCanBusDevice::framesReceived, [device](){
        //std::println("framesReceived");
        auto frames = device->readAllFrames();
        for(auto& frame: frames){
            unsigned id = frame.frameId();
            std::println("frame: id={:4X} payload={} bytes", id, frame.payload().size());
        }
    });
    if(!device->connectDevice()){
        std::println("connect failed");
    }

    app.exec();
    return 0;
}