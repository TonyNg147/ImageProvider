#ifndef IMAGE_POOL_H
#define IMAGE_POOL_H
#include <QQuickImageProvider>
class ImagePoolPrivate;
class ImagePool: public QQuickImageProvider {
    Q_OBJECT
    public:
        ImagePool(const QString& filePath);
        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    private:
        ImagePoolPrivate* m_priv;
};
#endif 