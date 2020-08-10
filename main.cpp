#include <QCoreApplication>

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

            io.writeError("Usage: " + binaryName + " androidPackageName pwaUrl outputPath [--manifest path-to-manifest]");
            io.writeError("Example: " + binaryName + " com.vendor.pwa https://pwa.vendor.com ./my-cool-pwa");
            io.writeError("Example: " + binaryName + " com.vendor.pwa https://pwa.vendor.com ./my-cool-pwa --manifest relative/path/to/manifest");
            return app.arguments().contains("--help") ? 0 : 1;
        }

        QString manifestPath;

        if (app.arguments().contains("--manifest")) {
            auto index = app.arguments().indexOf("--manifest") + 1;
            if (app.arguments().count() <= index) {
                qCritical() << "You must set a value for --manifest";
                return 1;
            }
            manifestPath = app.arguments().at(index);
        }

        auto url = app.arguments().at(2);
        auto packageName = app.arguments().at(1);
        auto outputDirectory = app.arguments().at(3);

        GitHelper gitHelper;

        gitHelper.clone(outputDirectory);
        gitHelper.checkout(outputDirectory);
        gitHelper.reinitGitDirectory(outputDirectory);

        WebsiteParser parser(url, manifestPath);

        auto basicData = parser.getData();
        basicData.insert("package", packageName);
        auto icons = parser.getImages();

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
