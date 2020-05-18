/* === This file is part of Calamares - <https://github.com/calamares> ===
 *
 *   Copyright 2020, Adriaan de Groot <groot@kde.org>
 *
 *   Calamares is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Calamares is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Calamares. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRACKING_CONFIG_H
#define TRACKING_CONFIG_H

#include <QObject>
#include <QString>
#include <QVariantMap>

/** @brief Base class for configuring a specific kind of tracking.
 *
 * All tracking types have a policy URL, which is used to explain what
 * kind of tracking is involved, what data is sent, etc. The content
 * of that URL is the responsibility of the distro.
 *
 * A tracking type is disabled by default: if it isn't specifically
 * enabled (for configuration) in the config file, it will always be disabled.
 * If it is enabled (for configuration) in the config file, it still
 * defaults to disabled, but the user can choose to enable it.
 */
class TrackingStyleConfig : public QObject
{
    Q_OBJECT

    Q_PROPERTY( TrackingState trackingStatus READ tracking WRITE setTracking NOTIFY trackingChanged FINAL )
    Q_PROPERTY( bool isEnabled READ isEnabled NOTIFY trackingChanged FINAL )
    Q_PROPERTY( bool isConfigurable READ isConfigurable NOTIFY trackingChanged FINAL )
    Q_PROPERTY( QString policy READ policy NOTIFY policyChanged FINAL )

public:
    TrackingStyleConfig( QObject* parent );
    virtual ~TrackingStyleConfig();

    void setConfigurationMap( const QVariantMap& );

    enum TrackingState
    {
        DisabledByConfig,
        DisabledByUser,
        EnabledByUser
    };
    Q_ENUM( TrackingState );

public Q_SLOTS:
    TrackingState tracking() const { return m_state; }
    /// @brief Has the user specifically enabled this kind of tracking?
    bool isEnabled() const { return m_state == EnabledByUser; }
    /// @brief Is this tracking enabled for configuration?
    bool isConfigurable() const { return m_state != DisabledByConfig; }
    /** @brief Sets the tracking state
     *
     * Unless the tracking is enabled for configuration, it always
     * remains disabled.
     */
    void setTracking( TrackingState );
    /** @brief Sets the tracking state
     *
     * Use @c true for @c EnabledByUser, @c false for DisabledByUser,
     * but keep in mind that if the tracking is not enabled for
     * configuration, it will always remain disabled.
     */
    void setTracking( bool );

    /// @brief URL for the policy explaining this tracking
    QString policy() const { return m_policy; }

signals:
    void trackingChanged();
    void policyChanged( QString );

private:
    TrackingState m_state = DisabledByConfig;
    QString m_policy;  // URL
};

class Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString generalPolicy READ generalPolicy NOTIFY generalPolicyChanged FINAL )
    Q_PROPERTY( TrackingStyleConfig* installTracking READ installTracking FINAL )

public:
    Config( QObject* parent = nullptr );
    void setConfigurationMap( const QVariantMap& );

public Q_SLOTS:
    QString generalPolicy() const;
    TrackingStyleConfig* installTracking() const { return m_installTracking; }

signals:
    void generalPolicyChanged( QString );

private:
    QString m_generalPolicy;

    TrackingStyleConfig* m_installTracking;
};

#endif
