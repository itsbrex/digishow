/*
    Copyright 2021 Robin Zhang & Labs

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

 */

#include "digishow_environment.h"
#include "digishow_slot.h"
#include "digishow_interface.h"
#include "dgs_midi_interface.h"
#include "dgs_rioc_interface.h"
#include "dgs_modbus_interface.h"
#include "dgs_dmx_interface.h"
#include "dgs_audioin_interface.h"
#include "dgs_screen_interface.h"
#include "rioc_aladdin2560_def.h"

DigishowEnvironment::DigishowEnvironment(QObject *parent) : QObject(parent)
{

}

bool DigishowEnvironment::needLogs()
{
    return g_needLogCtl;
}

void DigishowEnvironment::setNeedLogs(bool need)
{
    g_needLogCtl = need;
    emit needLogsChanged();
}

QString DigishowEnvironment::appDataPath(const QString &filename)
{
    QString path = QDir::home().filePath(g_appname);
    QDir dir(path);
    if (!dir.exists()) dir.mkpath(".");

    if (filename.isEmpty())
        return path;
    else
        return dir.filePath(filename);
}

QString DigishowEnvironment::appExecPath(const QString &filename)
{
    QString path = QCoreApplication::applicationDirPath();
    QDir dir(path);

    if (filename.isEmpty())
        return path;
    else
        return dir.filePath(filename);
}

QString DigishowEnvironment::appFilePath(const QString &filename)
{
    QString path = g_app->filepath();
    if (path.isEmpty()) return QString();

    QFileInfo fileinfo(path);
    QDir dir = fileinfo.dir();

    if (filename.isEmpty())
        return path;
    else
        return dir.filePath(filename);
}

void DigishowEnvironment::setAppOptions(const QVariantMap &options)
{
    AppUtilities::saveJsonToFile(options, QDir(appDataPath()).filePath("options.json"));
}

QVariantMap DigishowEnvironment::getAppOptions()
{
    QVariantMap options = AppUtilities::loadJsonFromFile(QDir(appDataPath()).filePath("options.json"));

    // service options
    QString fileServiceOptions = QDir(appDataPath()).filePath("service.json");
    if (QFile::exists(fileServiceOptions)) {
        QVariantMap serviceOptions = AppUtilities::loadJsonFromFile(fileServiceOptions);
        QMapIterator<QString, QVariant> i(serviceOptions);
        while (i.hasNext()) {
            i.next();
            options[i.key()] = i.value();
        }
    }

    return options;
}

int DigishowEnvironment::inputValueAt(int slotIndex)
{
    int val = -1;
    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot != nullptr) val = slot->getEndpointInValue();

    return (val != -1 ? val : 0);
}

int DigishowEnvironment::inputValueOf(const QString &slotTitle)
{
    int val = -1;
    DigishowSlot *slot = g_app->slotTitled(slotTitle);
    if (slot != nullptr) val = slot->getEndpointInValue();

    return (val != -1 ? val : 0);
}

int DigishowEnvironment::outputValueAt(int slotIndex)
{
    int val = -1;
    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot != nullptr) val = slot->getEndpointOutValue();

    return (val != -1 ? val : 0);
}

int DigishowEnvironment::outputValueOf(const QString &slotTitle)
{
    int val = -1;
    DigishowSlot *slot = g_app->slotTitled(slotTitle);
    if (slot != nullptr) val = slot->getEndpointOutValue();

    return (val != -1 ? val : 0);
}

bool DigishowEnvironment::setOutputValueAt(int slotIndex, int value)
{
    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot == nullptr) return false;

    slot->setEndpointOutValue(value);
    return true;
}

bool DigishowEnvironment::setOutputValueOf(const QString &slotTitle, int value)
{
    DigishowSlot *slot = g_app->slotTitled(slotTitle);
    if (slot == nullptr) return false;

    slot->setEndpointOutValue(value);
    return true;
}

QVariantMap DigishowEnvironment::getSlotConfiguration(int slotIndex)
{
    QVariantMap data;

    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot != nullptr) {
        data["slotOptions"] = slot->getSlotOptions();
        data["slotInfo"   ] = slot->getSlotInfo();

        DigishowInterface *interfaceIn = slot->sourceInterface();
        if (interfaceIn != nullptr) {
            int epInIndex = slot->sourceEndpointIndex();
            data["epInOptions"] = interfaceIn->getEndpointOptionsAt(epInIndex);
            data["epInInfo"   ] = interfaceIn->getEndpointInfoAt(epInIndex);
        }

        DigishowInterface *interfaceOut = slot->destinationInterface();
        if (interfaceOut != nullptr) {
            int epOutIndex = slot->destinationEndpointIndex();
            data["epOutOptions"] = interfaceOut->getEndpointOptionsAt(epOutIndex);
            data["epOutInfo"   ] = interfaceOut->getEndpointInfoAt(epOutIndex);
        }
    }

    return data;
}

QVariantMap DigishowEnvironment::getSlotRuntimeData(int slotIndex)
{
    QVariantMap data;

    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot != nullptr) {

        data["epInValue"     ] = slot->getEndpointInValue();
        data["epOutValue"    ] = slot->getEndpointOutValue();
        data["epOutPreValue" ] = slot->getEndpointOutValue(true);
        data["epInBusy"      ] = slot->isEndpointInBusy();
        data["epOutBusy"     ] = slot->isEndpointOutBusy();
        data["slotEnabled"   ] = slot->isEnabled();
        data["slotLinked"    ] = slot->isLinked();
        data["errTraffic"    ] = slot->hasTrafficError();
        data["errInExp"      ] = slot->hasInputExpressionError();
        data["errOutExp"     ] = slot->hasOutputExpressionError();

        // data["epInRange"     ] = slot->getEndpointInRange();
        // data["epOutRange"    ] = slot->getEndpointOutRange();
    }

    return data;
}


QVariantMap DigishowEnvironment::getInterfaceConfiguration(int interfaceIndex)
{
    QVariantMap data;

    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface != nullptr) {
        data["interfaceOptions"] = interface->getInterfaceOptions();
        data["interfaceInfo"   ] = interface->getInterfaceInfo();

        QVariantList listOptions, listInfo;
        for (int n=0 ; n<interface->endpointCount() ; n++) {
            listOptions.append(interface->getEndpointOptionsAt(n));
            listInfo.append(interface->getEndpointInfoAt(n));
        }
        data["epOptionsList"] = listOptions;
        data["epInfoList"]    = listInfo;
    }

    return data;
}

int DigishowEnvironment::getSourceInterfaceIndex(int slotIndex)
{
    int interfaceIndex = -1;

    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot != nullptr) {
        DigishowInterface *interface = slot->sourceInterface();
        if (interface != nullptr) {
            int interfaceCount = g_app->interfaceCount();
            for (int n=0 ; n<interfaceCount ; n++) {
                if (g_app->interfaceAt(n) == interface) {
                    interfaceIndex = n;
                    break;
                }
            }
        }
    }

    return interfaceIndex;
}

int DigishowEnvironment::getSourceEndpointIndex(int slotIndex)
{
    int endpointIndex = -1;

    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot != nullptr) endpointIndex = slot->sourceEndpointIndex();

    return endpointIndex;
}

int DigishowEnvironment::getDestinationInterfaceIndex(int slotIndex)
{
    int interfaceIndex = -1;

    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot != nullptr) {
        DigishowInterface *interface = slot->destinationInterface();
        if (interface != nullptr) {
            int interfaceCount = g_app->interfaceCount();
            for (int n=0 ; n<interfaceCount ; n++) {
                if (g_app->interfaceAt(n) == interface) {
                    interfaceIndex = n;
                    break;
                }
            }
        }
    }

    return interfaceIndex;
}

int DigishowEnvironment::getDestinationEndpointIndex(int slotIndex)
{
    int endpointIndex = -1;

    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot != nullptr) endpointIndex = slot->destinationEndpointIndex();

    return endpointIndex;
}

void DigishowEnvironment::clearSourceEndpoint(int slotIndex)
{
    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot == nullptr) return;

    slot->setSource(nullptr, -1);
    slot->setSlotOption("source", "");
}

void DigishowEnvironment::clearDestinationEndpoint(int slotIndex)
{
    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot == nullptr) return;

    slot->setDestination(nullptr, -1);
    slot->setSlotOption("destination", "");
}

int DigishowEnvironment::updateSourceEndpoint(int slotIndex, int interfaceIndex, const QVariantMap &endpointOptions)
{
    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot == nullptr) return -1;

    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return -1;

    int endpointIndex = makeEndpoint(interfaceIndex, endpointOptions);
    if (endpointIndex != -1) {
        slot->setSource(interface, endpointIndex);
        slot->setSlotOption(
                    "source",
                    interface->interfaceOptions()->value("name").toString() + "/" +
                    interface->endpointOptionsList()->at(endpointIndex).value("name").toString());
        if (!slot->isEnabled() && g_app->isRunning()) slot->setEnabled(true);
    }
    return endpointIndex;
}

int DigishowEnvironment::updateDestinationEndpoint(int slotIndex, int interfaceIndex, const QVariantMap &endpointOptions)
{
    DigishowSlot *slot = g_app->slotAt(slotIndex);
    if (slot == nullptr) return -1;

    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return -1;

    int endpointIndex = makeEndpoint(interfaceIndex, endpointOptions);
    if (endpointIndex != -1) {
        slot->setDestination(interface, endpointIndex);
        slot->setSlotOption(
                    "destination",
                    interface->interfaceOptions()->value("name").toString() + "/" +
                    interface->endpointOptionsList()->at(endpointIndex).value("name").toString());
        if (!slot->isEnabled() && g_app->isRunning()) slot->setEnabled(true);
    }
    return endpointIndex;
}

bool DigishowEnvironment::restartInterface(int interfaceIndex)
{
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return false;

    interface->closeInterface();
    return (interface->openInterface() == ERR_NONE);
}

int DigishowEnvironment::makeInterface(const QVariantMap &interfaceOptions)
{
    QList<DigishowInterface*> interfaceList = g_app->interfaceList();

    // look for the matched interface that already exists
    int interfaceCount = g_app->interfaceCount();
    for (int i=0 ; i<interfaceCount ; i++) {

        // find interface
        QStringList keys = interfaceOptions.keys();
        bool matched = true;
        for (int n=0 ; n<keys.length() ; n++) {
            QString key = keys[n];
            if (key != "name" && key != "outputInterval" && key != "frequency") // ignore these options
            if (interfaceOptions[key] != interfaceList[i]->interfaceOptions()->value(key)) {
                matched = false;
                break;
            }
        }

        // interface is found
        if (matched) {

            // update these options
            for (int n=0 ; n<keys.length() ; n++) {
                QString key = keys[n];
                if (key == "outputInterval" || key == "frequency") {
                    interfaceList[i]->setInterfaceOption(key, interfaceOptions[key]);
                }
            }

            return i;
        }
    }

    // no matched interface found, now need make a new one
    QString interfaceType = interfaceOptions.value("type").toString();
    if (interfaceType.isEmpty()) return -1;

    int interfaceIndex = g_app->newInterface(interfaceType);
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);

    QString interfaceName = interface->interfaceOptions()->value("name").toString();
    interface->setInterfaceOptions(interfaceOptions);
    interface->setInterfaceOption("name", interfaceName);
    interface->updateMetadata();

    return interfaceIndex;
}

int DigishowEnvironment::findInterfaceWithName(const QString &interfaceName)
{
    for (int i=0 ; i<g_app->interfaceCount() ; i++) {
        if (g_app->interfaceAt(i)->interfaceOptions()->value("name") == interfaceName) {
            return i;
        }
    }
    return -1;
}

QString DigishowEnvironment::getInterfaceName(int interfaceIndex)
{
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return "";

    return interface->interfaceOptions()->value("name").toString();
}

QVariantMap DigishowEnvironment::getInterfaceOptions(int interfaceIndex)
{
    QVariantMap interfaceOptions;

    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return interfaceOptions;

    interfaceOptions = interface->getInterfaceOptions();
    interfaceOptions.remove("endpoints");
    interfaceOptions.remove("media");

    return interfaceOptions;
}

int DigishowEnvironment::makeEndpoint(int interfaceIndex, const QVariantMap &endpointOptions)
{
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return -1;

    // look for the matched endpoint that already exists in the interface
    int endpointCount = interface->endpointCount();
    for (int i=0 ; i<endpointCount ; i++) {

        // find endpoint
        QStringList keys = endpointOptions.keys();
        bool matched = true;
        for (int n=0 ; n<keys.length() ; n++) {
            QString key = keys[n];
            if (key != "name" && key != "initial" && key != "range" && !key.startsWith("opt")) // ignore these options
            if (endpointOptions[key] != interface->endpointOptionsList()->at(i).value(key)) {

                matched = false;
                break;
            }
        }

        // endpoint is found
        if (matched) {

            // update these options
            for (int n=0 ; n<keys.length() ; n++) {
                QString key = keys[n];
                if (key == "initial" || key == "range" || key.startsWith("opt")) {
                    interface->setEndpointOption(i, key, endpointOptions[key]);
                }
            }

            // enable the endpoint
            interface->setEndpointOption(i, "enabled", true);

            interface->updateMetadata();
            return i;
        }
    }

    // no matched endpoint found, now need make a new one
    QString endpointType = endpointOptions.value("type").toString();
    if (endpointType.isEmpty()) return -1;

    // assign a name to the new endpoint
    QString endpointName = "";
    for (int n=1 ; ; n++) {
        QString name = endpointType + QString::number(n);
        bool nameExisted = false;
        for (int i=0 ; i<endpointCount ; i++) {
            if (interface->endpointOptionsList()->at(i).value("name") == name) {
                nameExisted = true;
                break;
            }
        }
        if (!nameExisted) {
            endpointName = name;
            break;
        }
    }

    int endpointIndex = -1;
    interface->addEndpoint(endpointOptions, &endpointIndex);
    interface->setEndpointOption(endpointIndex, "name", endpointName);
    interface->setEndpointOption(endpointIndex, "enabled", true);
    interface->updateMetadata();

    return endpointIndex;
}

int DigishowEnvironment::findEndpointWithName(int interfaceIndex, const QString &endpointName)
{
    // confirm interface exists
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return -1;

    // get endpoint list
    QList<QVariantMap> *endpointList = interface->endpointOptionsList();

    // look for the name-matched endpoint item in the list
    for (int n=0 ; n<endpointList->length() ; n++) {
        QVariantMap endpoint = endpointList->at(n);
        if (endpoint.value("name") == endpointName) {
            return n;
        }
    }

    return -1;
}

QString DigishowEnvironment::getEndpointName(int interfaceIndex, int endpointIndex)
{
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return "";

    QList<QVariantMap> *endpointOptionsList = interface->endpointOptionsList();
    if (endpointIndex<0 || endpointIndex>=endpointOptionsList->length()) return "";

    return endpointOptionsList->at(endpointIndex).value("name").toString();
}

QVariantMap DigishowEnvironment::getEndpointOptions(int interfaceIndex, int endpointIndex)
{
    QVariantMap endpointOptions;

    // confirm interface exists
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return endpointOptions;

    // get the specified endpoint item in the list
    endpointOptions = interface->getEndpointOptionsAt(endpointIndex);

    return endpointOptions;
}

int DigishowEnvironment::makeMedia(int interfaceIndex, const QString &mediaUrl, const QString &mediaType)
{
    int mediaIndex = -1;

    // confirm interface exists
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return -1;

    // confirm media url and type are valid
    if (mediaUrl.isEmpty() || mediaUrl == "file://" || mediaType.isEmpty()) return -1;

    // get media list
    QVariantList mediaList = interface->interfaceOptions()->value("media").toList();

    // look for the url-matched media item in the list
    for (int n=0 ; n<mediaList.length() ; n++) {
        QVariantMap media = mediaList[n].toMap();
        if (media.value("url") == mediaUrl && media.value("type") == mediaType) {
            mediaIndex = n;
            break;
        }
    }

    // no matched media found, now need make a new one
    if (mediaIndex == -1) {

        QString mediaName = "";

        // assign a name to the new media
        for (int n=1 ; ; n++) {
            QString name = mediaType + QString::number(n);
            bool nameExisted = false;
            for (int i=0 ; i<mediaList.length() ; i++) {
                if (mediaList[i].toMap().value("name") == name) {
                    nameExisted = true;
                    break;
                }
            }
            if (!nameExisted) {
                mediaName = name;
                break;
            }
        }

        // add the new one to media list
        QVariantMap mediaOptions;
        mediaOptions["name"] = mediaName;
        mediaOptions["url" ] = mediaUrl;
        mediaOptions["type"] = mediaType;
        mediaList.append(mediaOptions);
        mediaIndex = mediaList.length() - 1;
        interface->setInterfaceOption("media", mediaList);

        // load the new media into the interface object
        if (interface->isInterfaceOpened()) {
            interface->loadMedia(mediaOptions);
        }
    }

    return mediaIndex;
}

int DigishowEnvironment::findMediaWithName(int interfaceIndex, const QString &mediaName)
{
    // confirm interface exists
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return -1;

    // get media list
    QVariantList mediaList = interface->interfaceOptions()->value("media").toList();

    // look for the name-matched media item in the list
    for (int n=0 ; n<mediaList.length() ; n++) {
        QVariantMap media = mediaList[n].toMap();
        if (media.value("name") == mediaName) {
            return n;
        }
    }

    return -1;
}

QString DigishowEnvironment::getMediaName(int interfaceIndex, int mediaIndex)
{
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return "";

    QVariantList mediaList = interface->interfaceOptions()->value("media").toList();
    if (mediaIndex < 0 && mediaIndex >= mediaList.length()) return "";

    return mediaList.at(mediaIndex).toMap().value("name").toString();
}

QVariantMap DigishowEnvironment::getMediaOptions(int interfaceIndex, int mediaIndex)
{
    QVariantMap mediaOptions;

    // confirm interface exists
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return mediaOptions;

    // get media list
    QVariantList mediaList = interface->interfaceOptions()->value("media").toList();

    // get the specified media item in the list
    if (mediaIndex >= 0 && mediaIndex < mediaList.length()) {
        mediaOptions = mediaList[mediaIndex].toMap();
    }

    return mediaOptions;
}

int DigishowEnvironment::makeSlot(const QVariantMap &slotOptions,
                         const QVariantMap &sourceInterfaceOptions, const QVariantMap &sourceEndpointOptions,
                         const QVariantMap &destinationInterfaceOptions, const QVariantMap &destinationEndpointOptions,
                         const QString &mediaUrl, const QString &mediaType) {

    // create a new slot
    int newSlotIndex = g_app->newSlot();
    DigishowSlot *slotNew = g_app->slotAt(newSlotIndex);
    slotNew->setSlotOptions(slotOptions);

    // make source interface and endpoint
    if (!sourceInterfaceOptions.isEmpty() && !sourceEndpointOptions.isEmpty()) {
        int sourceInterfaceIndex = makeInterface(sourceInterfaceOptions);
        int sourceEndpointIndex = makeEndpoint(sourceInterfaceIndex, sourceEndpointOptions);
        if (sourceEndpointIndex != -1) {
            QString sourceInterfaceName = getInterfaceName(sourceInterfaceIndex);
            QString sourceEndpointName = getEndpointName(sourceInterfaceIndex, sourceEndpointIndex);
            slotNew->setSlotOption("source", sourceInterfaceName + "/" + sourceEndpointName);
            slotNew->setSource(g_app->interfaceAt(sourceInterfaceIndex), sourceEndpointIndex);
        }
    }

    // make destination interface and endpoint
    if (!destinationInterfaceOptions.isEmpty() && !destinationEndpointOptions.isEmpty()) {
        int destinationInterfaceIndex = makeInterface(destinationInterfaceOptions);
        int destinationEndpointIndex = makeEndpoint(destinationInterfaceIndex, destinationEndpointOptions);
        if (destinationEndpointIndex != -1) {
            QString destinationInterfaceName = getInterfaceName(destinationInterfaceIndex);
            QString destinationEndpointName = getEndpointName(destinationInterfaceIndex, destinationEndpointIndex);
            slotNew->setSlotOption("destination", destinationInterfaceName + "/" + destinationEndpointName);
            slotNew->setDestination(g_app->interfaceAt(destinationInterfaceIndex), destinationEndpointIndex);

            // make media (optional)
            if (!mediaUrl.isEmpty() && !mediaType.isEmpty()) {
                int mediaIndex = makeMedia(destinationInterfaceIndex, mediaUrl, mediaType);
                if (mediaIndex != -1) {
                    QString mediaName = getMediaName(destinationInterfaceIndex, mediaIndex);
                    DigishowInterface *interface = g_app->interfaceAt(destinationInterfaceIndex);
                    interface->setEndpointOption(destinationEndpointIndex, "media", mediaName);
                }
            }
        }
    }

    return newSlotIndex;
}

bool DigishowEnvironment::startInterfaceDataInputDetection(int interfaceIndex)
{
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return false;
    if (interface->needReceiveRawData()) return true;

    //connect(interface, SIGNAL(rawDataReceived(QVaribleMap)), this, SLOT(onRawDataReceived(QVaribleMap)));
    connect(interface, &DigishowInterface::rawDataReceived, this, &DigishowEnvironment::onRawDataReceived);
    interface->setNeedReceiveRawData(true);
    return true;
}

bool DigishowEnvironment::stopInterfaceDataInputDetection(int interfaceIndex)
{
    DigishowInterface *interface = g_app->interfaceAt(interfaceIndex);
    if (interface == nullptr) return false;
    if (!interface->needReceiveRawData()) return true;

    interface->setNeedReceiveRawData(false);
    //disconnect(interface, SIGNAL(rawDataReceived(QVaribleMap)), this, nullptr);
    disconnect(interface, &DigishowInterface::rawDataReceived, this, nullptr);
    return true;
}

void DigishowEnvironment::stopInterfaceDataInputDetectionAll()
{
    for (int n=0 ; n<g_app->interfaceCount() ; n++) stopInterfaceDataInputDetection(n);
}

void DigishowEnvironment::onRawDataReceived(const QVariantMap &rawData)
{
    QObject* obj = sender();
    if (obj == nullptr) return;

    DigishowInterface* interface = qobject_cast<DigishowInterface*>(obj);

    for (int i=0 ; i<g_app->interfaceCount() ; i++) {
        if (g_app->interfaceAt(i) == interface) {

            //qDebug() << "interfaceDataInputDetected" << i << rawData;
            emit interfaceDataInputDetected(i, rawData);
            //stopInterfaceDataInputDetection(i);
            break;
        }
    }
}

QVariantMap DigishowEnvironment::listOnline()
{
    QVariantMap info;
    info["midi"   ] = DgsMidiInterface   ::listOnline();
    info["dmx"    ] = DgsDmxInterface    ::listOnline();
    info["rioc"   ] = DgsRiocInterface   ::listOnline();
    info["modbus" ] = DgsModbusInterface ::listOnline();
    info["audioin"] = DgsAudioinInterface::listOnline();
    info["screen" ] = DgsScreenInterface ::listOnline();
    return info;
}

QString DigishowEnvironment::getLightControlName(int control, bool shortName)
{
    switch(control) {
        case CONTROL_LIGHT_R:    return shortName ? tr("R")   : tr("Red");
        case CONTROL_LIGHT_G:    return shortName ? tr("G")   : tr("Green");
        case CONTROL_LIGHT_B:    return shortName ? tr("B")   : tr("Blue");
        case CONTROL_LIGHT_W:    return shortName ? tr("W")   : tr("White");
        case CONTROL_LIGHT_HUE:  return shortName ? tr("Hue") : tr("Hue");
        case CONTROL_LIGHT_SAT:  return shortName ? tr("Sat") : tr("Saturation");
        case CONTROL_LIGHT_BRI:  return shortName ? tr("Bri") : tr("Brightness");
        case CONTROL_LIGHT_CT:   return shortName ? tr("CT")  : tr("C. Temp");
    }
    return "";
}

QString DigishowEnvironment::getMediaControlName(int control, bool forScreen)
{
    switch(control) {
        case CONTROL_MEDIA_START:    return forScreen ? tr("Show")     : tr("Play");
        case CONTROL_MEDIA_STOP:     return forScreen ? tr("Hide")     : tr("Stop");
        case CONTROL_MEDIA_STOP_ALL: return forScreen ? tr("Hide All") : tr("Stop All");

        case CONTROL_MEDIA_OPACITY:  return tr("Opacity");
        case CONTROL_MEDIA_SCALE:    return tr("Scale");
        case CONTROL_MEDIA_ROTATION: return tr("Rotation");
        case CONTROL_MEDIA_XOFFSET:  return tr("X Offset");
        case CONTROL_MEDIA_YOFFSET:  return tr("Y Offset");
        case CONTROL_MEDIA_VOLUME:   return tr("Volume");
        case CONTROL_MEDIA_SPEED:    return tr("Speed");
        case CONTROL_MEDIA_POSITION: return tr("Position");
    }
    return "";
}

QString DigishowEnvironment::getMidiControlName(int control)
{
    // undefined midi cc
    if (control == 3 || control == 9 ||
        (control >= 14 && control <= 15) ||
        (control >= 20 && control <= 31) ||
        (control >= 85 && control <= 90) ||
        (control >= 102 && control <= 119)) return "";

    // common midi cc
    switch(control) {
        case 0:  return tr("Bank");
        case 1:  return tr("Modulation");
        case 4:  return tr("Foot Pedal");
        case 6:  return tr("Data Entry");
        case 7:  return tr("Volume ");
        case 10: return tr("Pan");
        case 11: return tr("Expression");
        case 32: return tr("Bank (LSB)");
        case 64: return tr("Sustain");
        case 65: return tr("Portamento");
        case 71: return tr("Resonance");
        case 74: return tr("Cutoff");
        case 91: return tr("Reverb");
        case 93: return tr("Chorus");
    }

    // other midi cc
    return "-";
}

QString DigishowEnvironment::getMidiNoteName(int noteNumber, bool longName)
{
    static QString notes[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    int octave = noteNumber / 12;   // 0 ~
    int semitone = noteNumber % 12; // 0 ~ 11
    QString note = notes[semitone] + QString::number(octave-2);

    QString name;
    if (longName) {
        switch (noteNumber) {
        case 60: name += tr("Mid C"       ); break;
        case 36: name += tr("Kick"        ); break;
        case 37: name += tr("Side Stick"  ); break;
        case 38: name += tr("Snare 1"     ); break;
        case 39: name += tr("Clap"        ); break;
        case 40: name += tr("Snare 2"     ); break;
        case 41: name += tr("Low Tom 2"   ); break;
        case 42: name += tr("Closed Hat"  ); break;
        case 43: name += tr("Low Tom 1"   ); break;
        case 44: name += tr("Pedal Hat"   ); break;
        case 45: name += tr("Mid Tom 2"   ); break;
        case 46: name += tr("Open Hat"    ); break;
        case 47: name += tr("Mid Tom 1"   ); break;
        case 48: name += tr("High Tom 2"  ); break;
        case 49: name += tr("Crash"       ); break;
        case 50: name += tr("High Tom 1"  ); break;
        case 51: name += tr("Ride"        ); break;
        }
    }

    return note + (name.isEmpty() ? "" : " ( " + name +" )") ;
}

QString DigishowEnvironment::getSpectrumBandName(int band, bool longName)
{
    QString name;
    switch (band) {
    case  1: name = "20 - 25 Hz";      break;
    case  2: name = "25 - 32 Hz";      break;
    case  3: name = "32 - 40 Hz";      break;
    case  4: name = "40 - 50 Hz";      break;
    case  5: name = "50 - 63 Hz";      break;
    case  6: name = "63 - 80 Hz";      break;
    case  7: name = "80 - 100 Hz";     break;
    case  8: name = "100 - 125 Hz";    break;
    case  9: name = "125 - 160 Hz";    break;
    case 10: name = "160 - 200 Hz";    break;
    case 11: name = "200 - 250 Hz";    break;
    case 12: name = "250 - 315 Hz";    break;
    case 13: name = "315 - 400 Hz";    break;
    case 14: name = "400 - 500 Hz";    break;
    case 15: name = "500 - 630 Hz";    break;
    case 16: name = "630 - 800 Hz";    break;
    case 17: name = "800 - 1K Hz";     break;
    case 18: name = "1K - 1.25K Hz";   break;
    case 19: name = "1.25K - 1.6K Hz"; break;
    case 20: name = "1.6K - 2K Hz";    break;
    case 21: name = "2K - 2.5K Hz";    break;
    case 22: name = "2.5K - 5K Hz";    break;
    case 23: name = "5K - 10K Hz";     break;
    case 24: name = "10K - 20K Hz";    break;
    }

    QString description;
    if (longName) {
        switch (band) {
        case  1: description = tr("Sub-bass perception"     ); break;
        case  2: description = tr("Ultra-low bass"          ); break;
        case  3: description = tr("Subwoofer main"          ); break;
        case  4: description = tr("Bass fundamental 1"      ); break;
        case  5: description = tr("Bass fundamental 2"      ); break;
        case  6: description = tr("Bass playing range 1"    ); break;
        case  7: description = tr("Bass playing range 2"    ); break;
        case  8: description = tr("Kick drum main"          ); break;
        case  9: description = tr("Low vocals"              ); break;
        case 10: description = tr("Male bass voice"         ); break;
        case 11: description = tr("Male tenor"              ); break;
        case 12: description = tr("Middle C range (261.6Hz)"); break;
        case 13: description = tr("Female alto"             ); break;
        case 14: description = tr("Violin G string"         ); break;
        case 15: description = tr("Vocal main 1"            ); break;
        case 16: description = tr("Vocal main 2"            ); break;
        case 17: description = tr("Vocal main 3"            ); break;
        case 18: description = tr("Vocal harmonics 1"       ); break;
        case 19: description = tr("Vocal harmonics 2"       ); break;
        case 20: description = tr("Presence range"          ); break;
        case 21: description = tr("High detail 1"           ); break;
        case 22: description = tr("High detail 2"           ); break;
        case 23: description = tr("Brilliance"              ); break;
        case 24: description = tr("Air band"                ); break;
        }
    }

    return name + (longName ? "  " + description : "");
}

QString DigishowEnvironment::getRiocPinName(int mode, int pinNumber)
{
    if (mode == INTERFACE_RIOC_ARDUINO_UNO) {

        if (pinNumber>=14 && pinNumber<=21)
            return "A" + QString::number(pinNumber-14);
        else
            return "D" + QString::number(pinNumber);

    } else if (mode == INTERFACE_RIOC_ARDUINO_MEGA) {

        if (pinNumber>=54 && pinNumber<=69)
            return "A" + QString::number(pinNumber-54);
        else
            return "D" + QString::number(pinNumber);

    } else if (mode == INTERFACE_RIOC_ALADDIN) {

        for (int n=0 ; n<16 ; n++)
            if (pinNumber==PIN_UD_DI[n]) return "DI" + QString::number(n);

        for (int n=0 ; n<8 ; n++)
            if (pinNumber==PIN_UD_DO[n]) return "DO" + QString::number(n);

        for (int n=0 ; n<4 ; n++)
            if (pinNumber==PIN_UD_ADC[n]) return "ADC" + QString::number(n);

    } else if (mode == INTERFACE_RIOC_PLC1) {

        for (int n=0 ; n<8 ; n++)
            if (pinNumber==PIN_PLC1_X[n]) return "X" + QString::number(n);
        for (int n=0 ; n<8 ; n++)
            if (pinNumber==PIN_PLC1_X[n+8]) return "X" + QString::number(n+10);

        for (int n=0 ; n<8 ; n++)
            if (pinNumber==PIN_PLC1_Y[n]) return "Y" + QString::number(n);
        for (int n=0 ; n<4 ; n++)
            if (pinNumber==PIN_PLC1_Y[n+8]) return "Y" + QString::number(n+10);

        switch (pinNumber) {
        case PIN_PLC1_LED_RUN: return "LED RUN";
        case PIN_PLC1_LED_ERR: return "LED ERR";
        case PIN_PLC1_SWITCH : return "SW1";
        }

    } else if (mode == INTERFACE_RIOC_PLC2) {

        for (int n=0 ; n<8 ; n++)
            if (pinNumber==PIN_PLC2_X[n]) return "X" + QString::number(n);
        for (int n=0 ; n<7 ; n++)
            if (pinNumber==PIN_PLC2_X[n+8]) return "X" + QString::number(n+10);

        for (int n=0 ; n<8 ; n++)
            if (pinNumber==PIN_PLC2_Y[n]) return "Y" + QString::number(n);
        for (int n=0 ; n<2 ; n++)
            if (pinNumber==PIN_PLC2_Y[n+8]) return "Y" + QString::number(n+10);

        switch (pinNumber) {
        case PIN_PLC2_LED_RUN: return "LED RUN";
        case PIN_PLC2_LED_ERR: return "LED ERR";
        case PIN_PLC2_SWITCH : return "SW1";
        }
    }

    return QString::number(pinNumber);
}

QList<int> DigishowEnvironment::getRiocPinList(int mode, int types)
{
    QList<int> list;
    int len;

    switch (mode) {
    case INTERFACE_RIOC_ALADDIN:
        if (types & PIN_TYPE_DI ) { len = sizeof(PIN_UD_DI )/sizeof(int); for (int n=0 ; n<len; n++) list << PIN_UD_DI[n]; }
        if (types & PIN_TYPE_DO ) { len = sizeof(PIN_UD_DO )/sizeof(int); for (int n=0 ; n<len; n++) list << PIN_UD_DO[n]; }
        if (types & PIN_TYPE_AI ) { len = sizeof(PIN_UD_ADC)/sizeof(int); for (int n=0 ; n<len; n++) list << PIN_UD_ADC[n]; }
        break;
    case INTERFACE_RIOC_PLC1:
        if (types & PIN_TYPE_DI ) { len = sizeof(PIN_PLC1_X)/sizeof(int); for (int n=0 ; n<len; n++) list << PIN_PLC1_X[n]; }
        if (types & PIN_TYPE_DO ) { len = sizeof(PIN_PLC1_Y)/sizeof(int); for (int n=0 ; n<len; n++) list << PIN_PLC1_Y[n]; }
        if (types & PIN_TYPE_LED) { list << PIN_PLC1_LED_RUN << PIN_PLC1_LED_ERR; }
        if (types & PIN_TYPE_SW ) { list << PIN_PLC1_SWITCH; }
        break;
    case INTERFACE_RIOC_PLC2:
        if (types & PIN_TYPE_DI ) { len = sizeof(PIN_PLC2_X)/sizeof(int); for (int n=0 ; n<len; n++) list << PIN_PLC2_X[n]; }
        if (types & PIN_TYPE_DO ) { len = sizeof(PIN_PLC2_Y)/sizeof(int); for (int n=0 ; n<len; n++) list << PIN_PLC2_Y[n]; }
        if (types & PIN_TYPE_LED) { list << PIN_PLC2_LED_RUN << PIN_PLC2_LED_ERR; }
        if (types & PIN_TYPE_SW ) { list << PIN_PLC2_SWITCH; }
        break;
    }

    return list;
}

QString DigishowEnvironment::getPipeModeName(int mode)
{
    switch (mode) {
    case INTERFACE_PIPE_REMOTE: return tr("Remote");
    case INTERFACE_PIPE_CLOUD:  return tr("Cloud");
    default:                    return tr("Pipe");
    }
}

QString DigishowEnvironment::getMediaType(const QString &mediaUrl)
{
    if (!mediaUrl.startsWith("file://") &&
        !mediaUrl.startsWith("http://") &&
        !mediaUrl.startsWith("https://")) return "";

    if (mediaUrl.length()<10) return "";

    QString mediaUrl1 = mediaUrl.toLower();

    if (mediaUrl1.endsWith(".mp4") || mediaUrl1.endsWith(".mov") || mediaUrl1.endsWith(".avi") || mediaUrl1.endsWith(".flv") || mediaUrl1.endsWith(".m4v") || mediaUrl1.endsWith(".wmv") || mediaUrl1.endsWith(".mkv"))
        return "video";
    else if (mediaUrl1.endsWith(".jpg") || mediaUrl1.endsWith(".jpeg") || mediaUrl1.endsWith(".png") || mediaUrl1.endsWith(".bmp") || mediaUrl1.endsWith(".svg"))
        return "image";
    else if (mediaUrl1.endsWith(".ini"))
        return "ini";
    else
        return "web";
}


