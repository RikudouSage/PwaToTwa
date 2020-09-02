#include <QCoreApplication>
#include <QScopedPointer>

#include "inputoutput.h"
#include "githelper.h"
#include "websiteparser.h"
#include "androidprojectmodifier.h"
#include "coloredstring.h"

int main(int argc, char *argv[])
{
    InputOutput io;
    try {
        QCoreApplication app(argc, argv);

        if (app.arguments().count() < 4 || app.arguments().contains("--help")) {
            const auto binaryName = app.arguments().at(0);

            io.writeError("Usage: " + binaryName + " androidPackageName pwaUrl outputPath [--manifest path-to-manifest] [--local-manifest path-to-manifest]");
            io.writeError("Example: " + binaryName + " com.vendor.pwa https://pwa.vendor.com ./my-cool-pwa");
            io.writeError("Example: " + binaryName + " com.vendor.pwa https://pwa.vendor.com ./my-cool-pwa --manifest relative/path/to/manifest");
            io.writeError("Example: " + binaryName + " com.vendor.pwa https://pwa.vendor.com ./my-cool-pwa --local-manifest ./manifest.json");
            return app.arguments().contains("--help") ? 0 : 1;
        }

        QString manifestPath;
        QFile *localManifestFile = nullptr;

        if (app.arguments().contains("--manifest")) {
            auto index = app.arguments().indexOf("--manifest") + 1;
            if (app.arguments().count() <= index) {
                throw QString("You must set a value for --manifest");
            }
            manifestPath = app.arguments().at(index);
        } else if (app.arguments().contains("--local-manifest")) {
            auto index = app.arguments().indexOf("--local-manifest") + 1;
            if (app.arguments().count() <= index) {
                throw QString("You must set a value for --local-manifest");
            }
            localManifestFile = new QFile(app.arguments().at(index), &app);
            if (!localManifestFile->exists()) {
                throw QString("The specified local manifest file does not exist");
            }
        }

        auto url = app.arguments().at(2);
        auto packageName = app.arguments().at(1);
        auto outputDirectory = app.arguments().at(3);

        GitHelper gitHelper;

        gitHelper.clone(outputDirectory);
        gitHelper.checkout(outputDirectory);
        gitHelper.reinitGitDirectory(outputDirectory);

        QScopedPointer<WebsiteParser> parser(
                    localManifestFile == nullptr
                    ? new WebsiteParser(url, manifestPath)
                    : new WebsiteParser(url, *localManifestFile)
        );

        auto basicData = parser->getData();
        basicData.insert("package", packageName);
        auto icons = parser->getImages();

        AndroidProjectModifier androidProject(outputDirectory);
        androidProject.addSupportLibrary();
        androidProject.setBasicData(basicData);
        androidProject.addImages(icons);

        gitHelper.initialCommit(outputDirectory);

        io.writeln("Successfully created, you can now open the project in Android Studio and build the apk");
        io.writeln("");
        io.writeln(ColoredString("Don't forget to include the Digital Asset Link on your website at <green>" + url + "/.well-known/assetlinks.json</>:"));
        io.writeln(ColoredString(
                       "[{ \n"
                       "    \"relation\": [\"delegate_permission/common.handle_all_urls\"], \n"
                       "    \"target\":   {\"namespace\": \"android_app\", \"package_name\": \"<green>" + packageName + "</>\", \n"
                       "                 \"sha256_cert_fingerprints\": [\"<green>hash_of_app_certificate</>\"]} \n"
                       "}]"
        ));

        return 0;
    } catch (QString e) {
        io.writeln(e);
        return 1;
    }
}
