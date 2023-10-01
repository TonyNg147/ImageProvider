#include "ImagePool.h"
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <QImage>
struct ImageHeader{
    uint32_t width;
    uint32_t height;
    uint32_t scanLineHeight;
    uint32_t format;
    uint32_t imageNumbers;
};
class ImagePoolPrivate{
    friend class ImagePool;
    public:
        ImagePoolPrivate(const QString& filePath){
            fd = open(filePath.toStdString().c_str(), O_RDONLY);
            if (fd < 0){
                std::string error = "Open file " + filePath.toStdString() + " failed with error " + strerror(errno);
                throw std::runtime_error(error.c_str());
            }
            struct stat sb;
            if (fstat(fd, &sb) == -1){
                std::string error = "Read infor of " + filePath.toStdString() + " failed with error " + strerror(errno);
                throw std::runtime_error(error.c_str());
            }
            if (sb.st_size & (1<<13) != 0 || sb.st_size < 8196){
                close(fd);
               throw std::runtime_error("File input invalid - not enough length"); 
            }
            int bytes = read(fd, &header, sizeof(header));
            if (bytes != sizeof(header)){
                close(fd);
                throw std::runtime_error("Read data file error");
            }
            imageSize = header.width * header.scanLineHeight; 
            imageCount = header.imageNumbers; 
            totalImagesSize = sb.st_size - sizeof(header);
            int offset = sizeof(header);
            qWarning()<<"total image size "<<totalImagesSize<<" fd "<<fd<<" "<<offset;
            dataPtr = (uchar*)mmap(nullptr, totalImagesSize, PROT_READ, MAP_SHARED , fd, 8192);
            if (dataPtr == MAP_FAILED){
                close(fd);
                std::string error = std::string("Mapping failed with error ") + strerror(errno);
                throw std::runtime_error(error.c_str());  
            }
        }
        ~ImagePoolPrivate(){
            if (munmap(dataPtr, totalImagesSize) == -1){
                close(fd);
                std::string error = std::string("UnMapping failed with error ") + strerror(errno);
                throw std::runtime_error(error.c_str());          
            }
            close(fd);
        }
    private:
        int fd;
        ImageHeader header;
        uint32_t    imageSize;
        uchar* dataPtr = nullptr;
        uint32_t totalImagesSize;
        uint32_t imageCount;
};
ImagePool::ImagePool(const QString& filePath)
    :QQuickImageProvider(QQuickImageProvider::Image)
{
    m_priv = new ImagePoolPrivate(filePath);
}
QImage ImagePool::requestImage(const QString &id, QSize *size, const QSize &requestedSize){
    bool isValid = false;
    uint32_t index = id.toUInt(&isValid);
    if (!isValid){
        throw std::runtime_error("Index passed in invalid. Index must be an integer");
    }
    if (index < 0 || index > m_priv->imageCount) return QImage();
    return QImage(
        &m_priv->dataPtr[index * m_priv->imageSize]  ,
        m_priv->header.width,
        m_priv->header.height,
        m_priv->header.scanLineHeight,
        static_cast<QImage::Format>(m_priv->header.format)
    );
}