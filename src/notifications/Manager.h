#pragma once

#include <QImage>
#include <QObject>
#include <QString>

#if defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_HAIKU)
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusInterface>
#endif

struct roomEventId
{
        QString roomId;
        QString eventId;
};

inline bool
operator==(const roomEventId &a, const roomEventId &b)
{
        return a.roomId == b.roomId && a.eventId == b.eventId;
}

class NotificationsManager : public QObject
{
        Q_OBJECT
public:
        NotificationsManager(QObject *parent = nullptr);

        void postNotification(const QString &roomId,
                              const QString &eventId,
                              const QString &roomName,
                              const QString &senderName,
                              const QString &text,
                              const QImage &icon,
                              const bool &isEmoteMsg = false);

signals:
        void notificationClicked(const QString roomId, const QString eventId);
        void sendNotificationReply(const QString roomId, const QString eventId, const QString body);

public slots:
        void removeNotification(const QString &roomId, const QString &eventId);

#if defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_HAIKU)
public:
        void closeNotifications(QString roomId);

private:
        QDBusInterface dbus;
        void closeNotification(uint id);

        // notification ID to (room ID, event ID)
        QMap<uint, roomEventId> notificationIds;
#endif

        // these slots are platform specific (D-Bus only)
        // but Qt slot declarations can not be inside an ifdef!
private slots:
        void actionInvoked(uint id, QString action);
        void notificationClosed(uint id, uint reason);
        void notificationReplied(uint id, QString reply);
};

#if defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_HAIKU)
QDBusArgument &
operator<<(QDBusArgument &arg, const QImage &image);
const QDBusArgument &
operator>>(const QDBusArgument &arg, QImage &);
#endif
