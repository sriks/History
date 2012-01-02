#ifndef HISTORYENGINE_H
#define HISTORYENGINE_H
#include <QObject>
#include <QUrl>
#include <QVariant>

class HistoryInfo:public QObject
{
    Q_OBJECT
public:
    HistoryInfo(QObject* parent):
                QObject(parent){}
    Q_INVOKABLE QString title() {
        return mTitle;
    }
    Q_INVOKABLE QString description() {
        return mDescription;
    }
    Q_INVOKABLE QUrl link() {
        return mLink;
    }
    QString eventDate() {
        // TODO: convert to correct date format with QDate
        return mEventDate;
    }
    QString mTitle;
    QString mDescription;
    QUrl mLink;
    QString mEventDate;
};


class RSSManager;
class RSSParser;
class HistoryEnginePrivate;
class HistoryEngine : public QObject
{
    Q_OBJECT
public:
    explicit HistoryEngine(QObject *parent = 0);
    ~HistoryEngine();
    Q_INVOKABLE QVariant historyInfo() const;
    Q_INVOKABLE QVariant favorites();
    Q_INVOKABLE QObject* favorite(int favIndex);
    Q_INVOKABLE bool saveAsFavorite();
    Q_INVOKABLE bool deleteFavorite(int index);
    Q_INVOKABLE bool deleteAllFavorites();
    Q_INVOKABLE int favoritesCount();
    QStringList favoriteTitles();
signals:
    void updateReady(QVariant);
    void favoriteSelected(QVariant );
    void error(QString errorDescription,QUrl sourceUrl);

public slots:
    void start();
    void update();
    bool share(int index);

private slots:
    void handleUpdateAvailable(QUrl sourceUrl, int updateditems);

private:
    HistoryInfo* parseInfo(RSSParser* parser);
    QString fileNameForKey(const QString& key);
    QStringList favFileList();

private:
    HistoryEnginePrivate* d;
};

#endif // HISTORYENGINE_H
