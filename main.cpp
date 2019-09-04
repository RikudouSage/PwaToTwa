#include <QCoreApplication>
#include <iostream>

#include <QDebug>

#include "inputoutput.h"
#include "githelper.h"
#include "websiteparser.h"
#include "androidprojectmodifier.h"

int main(int argc, char *argv[])
{
    InputOutput io;
    try {
        QCoreApplication app(argc, argv);

        QString url;
        QString packageName;
        QString outputDirectory;

        if(app.arguments().count() < 4) {
            outputDirectory = io.ask("Please input the output path");
        } else {
            outputDirectory = app.arguments().at(3);
        }

        if (app.arguments().count() < 3) {
            url = io.ask("Please input the PWA url");
        } else {
            url = app.arguments().at(2);
        }

        if (app.arguments().count() < 2) {
            packageName = io.ask("Please input the app package name");
        } else {
            packageName = app.arguments().at(1);
        }

        GitHelper gitHelper;

        gitHelper.clone(outputDirectory);
        gitHelper.checkout(outputDirectory);
        gitHelper.reinitGitDirectory(outputDirectory);

        WebsiteParser parser(url);

        auto icons = parser.getImages();
        auto basicData = parser.getData();
        basicData.insert("package", packageName);

        AndroidProjectModifier androidProject(outputDirectory);
        androidProject.addSupportLibrary();
        androidProject.setBasicData(basicData);
        androidProject.addImages(icons);

        gitHelper.initialCommit(outputDirectory);

        io.writeln("Successfully created, you can now open the project in Android Studio and build the apk");

        return 0;
    } catch (QString e) {
        io.writeln(e);
        return 1;
    }
}
