#include <QCoreApplication>

#include "inputoutput.h"
#include "githelper.h"
#include "websiteparser.h"
#include "androidprojectmodifier.h"

int main(int argc, char *argv[])
{
    InputOutput io;
    try {
        QCoreApplication app(argc, argv);

        if(app.arguments().count() < 4) {
            qCritical() << "Wrong input, usage: pwa-to-twa [androidPackageName] [pwaUrl] [outputPath]";
            qCritical() << "Example: pwa-to-twa com.vendor.pwa https://pwa.vendor.com ./my-cool-pwa";
            return 1;
        }

        auto url = app.arguments().at(2);
        auto packageName = app.arguments().at(1);
        auto outputDirectory = app.arguments().at(3);

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
