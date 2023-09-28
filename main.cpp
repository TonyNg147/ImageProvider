#include <QGuiApplication>
#include <QCommandLineParser>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ImagePool.h"
#include <QImage>
#include <QQmlApplicationEngine>
struct ImageHeader{
    uint32_t width;
    uint32_t height;
    uint32_t scanLineHeight;
    uint32_t format;
    uint32_t imageNumbers;
};
int main(int argc, char** argv){
    QGuiApplication app(argc, argv);
    // QCommandLineParser parser;
    // parser.setApplicationDescription("Test helper");
    // parser.addHelpOption();
    // parser.addVersionOption();
    // parser.addOptions({
    //     {{"f","from"}         , "Start index of the image"  ,"index"},
    //     {{"t","to"}           , "End index of the image"    ,"index"},
    // });
    // parser.process(app);

    // do{
    //     QString from, to, source, dest;
    //     from = parser.value("from");
    //     to   = parser.value("to");
    //     bool isValid = false;
    //     uint32_t _from, _to;
    //     _from = from.toUInt(&isValid);
    //     if (!isValid){
    //         qCritical()<<"From index must be an integer";
    //         parser.showHelp();
    //         break;
    //     }
    //     _to = to.toUInt(&isValid);
    //     if (!isValid){
    //         qCritical()<<"To index must be an integer";
    //         parser.showHelp();
    //         break;
    //     }
    //     if (parser.positionalArguments().size() < 2){
    //         qCritical()<<"Have to provide the source and the destination path";
    //         parser.showHelp();
    //         break;
    //     }
    //     source = parser.positionalArguments()[0];
    //     dest   = parser.positionalArguments()[1];
    //     int fd =-1, i=0, dst, byteWrite, imageSize;
    //     QString filePath;
    //     ImageHeader header;
    //     for (i=_from; i < _to ; ++i){
    //         filePath = source + QString::asprintf("/landscape%u.png", i);
    //         QImage image(filePath.toStdString().c_str());
    //         if (image.isNull()){
    //             std::string error = "Image path is invalid " + filePath.toStdString();
    //             throw std::runtime_error(error.c_str());
    //         }
    //         if (i == _from){
    //             header.width = image.width();
    //             header.height = image.height();
    //             header.scanLineHeight = image.bytesPerLine();
    //             header.format = image.format();
    //             header.imageNumbers = (_from - _to + 1);
    //             imageSize  = header.height * header.scanLineHeight;
    //             qWarning()<<"Dest is "<<dest;
    //             dst = open(dest.toStdString().c_str(), O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR);
    //             if (dst < 0){
    //                 close(dst);
    //                 throw std::runtime_error("Cannot open desination file. Please provide the correct one");
    //             }
    //             byteWrite = write(dst, &header, sizeof(header));
    //             if (byteWrite != sizeof(header)){
    //                 close(dst);
    //                 throw std::runtime_error("Write data to destination file failed");
    //             }
    //         }
    //         byteWrite = write(dst, image.bits(), imageSize);
    //         if (byteWrite != imageSize){
    //             close(dst);
    //             throw std::runtime_error("Write data to destination file failed");
    //         }
    //         qWarning()<<"byte write "<<byteWrite;
    //     }
    //     uint32_t byteAlign = ((header.imageNumbers * imageSize) + (1 << 13 -1) & (~(1 << 13 - 1))) - header.imageNumbers * imageSize;
    //     byteWrite = write(dst, &byteAlign, byteAlign);
    //     if (byteWrite != byteAlign){
    //         close(dst);
    //         throw std::runtime_error("Write align byte to destination file failed");
    //     }
    //     close(dst);
    // }
    // while(0);
    QQmlApplicationEngine engine;
    engine.addImageProvider("Landscape", new ImagePool(argv[1]));
    engine.load(QUrl("qrc:/main.qml"));
}