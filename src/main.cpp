/*************************************************************************
 * Copyright (C) 2021  Zineddine Saibi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *************************************************************************/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <cstring>

#ifndef ZTIMER_VERSION
    #error "The ZTIMER_VERSION macro isn't defined. Please define it and proceed"
#endif

#define HELP_MESSAGE                                                                   \
    "DeepTags " ZTIMER_VERSION " (" ZTIMER_WEBSITE ")\n"                           \
    "Copyright (C) 2021 Zineddine SAIBI <saibi.zineddine@yahoo.com>.\n"                \
    "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>\n" \
    "This is free software: you are free to change and redistribute it.\n"

int main(int argc, char* argv[]) {

    if (argc == 2) {
        if (std::strcmp(argv[1], "-v") == 0 || std::strcmp(argv[1], "--version") == 0) {
            std::printf("%s\n", ZTIMER_VERSION);
            return 0;
        } else if (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0) {
            std::printf("%s\n", HELP_MESSAGE);
            return 0;
        } else {
            std::printf("ERROR. Try to run ZTimer without any argument.\n");
            return 1;
        }
    }

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    app.setApplicationName("ZTimer");
    app.setApplicationDisplayName("ZTimer");
    app.setApplicationVersion(ZTIMER_VERSION);
    app.setOrganizationName("ZTimer");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
