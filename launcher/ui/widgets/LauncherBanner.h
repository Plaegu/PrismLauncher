// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <QPixmap>
#include <QString>
#include <QWidget>

/**
 * Large hero image shown on the "Play" page of the launcher-style layout.
 * Paints an image scaled to cover the whole widget (or a gradient when there
 * is no image), darkens the bottom, and draws a title + subtitle over it.
 */
class LauncherBanner : public QWidget {
    Q_OBJECT

   public:
    explicit LauncherBanner(QWidget* parent = nullptr);

    void setImage(const QPixmap& image);
    void setTitle(const QString& title);
    void setSubtitle(const QString& subtitle);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

   protected:
    void paintEvent(QPaintEvent* event) override;

   private:
    QPixmap m_image;
    QPixmap m_scaled;
    QSize m_scaledFor;
    QString m_title;
    QString m_subtitle;
};
