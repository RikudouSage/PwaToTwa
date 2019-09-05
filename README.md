# Convert PWA to Android APK / TWA

This tool creates an Android Studio project from your
Progressive Web Application (PWA).

The usage is fairly simple, it uses official Google example
as a base and replaces values and images fetched from
your PWA site.

After running this tool you should be able to open the
newly created project in Android Studio and make any
modifications you want.

Note that this tool is in a really early state and thus
may fail anytime. I tested it with Angular powered
apps and it worked well but results with other frameworks
may vary. If it doesn't work for you, create an
[issue](https://github.com/RikudouSage/PwaToTwa/issues/new).

> The PWA needs to specify that your app can access the
content, read more [here](https://developers.google.com/digital-asset-links/v1/getting-started)

## Installation

You can either download the compiled static binary from
[releases](https://github.com/RikudouSage/PwaToTwa/releases/latest)
or compile it yourself.

## Usage

This tool takes three parameters:

1. The android package name (e.g. `com.example.my_pwa`)
2. The PWA url (e.g. `https://my_pwa.example.com`)
3. The output directory (e.g. `./my-cool-pwa`)

If the manifest path is not in html source code or could
not be detected for any other reason, you can add the
relative path to your manifest using the --manifest option.

Example:

`pwa-to-twa com.example.my_pwa https://my_pwa.example.com ./my-cool-pwa`

Example with --manifest option:

`pwa-to-twa com.example.my_pwa https://my_pwa.example.com ./my-cool-pwa --manifest /manifest.json`

> Note: Currently the splash image is not replaced from the default
value, it will be in a future version

## Dependencies

This tool needs these cli tools to be able to work:

- convert (from ImageMagick)

## Building from source

You need the Qt framework installed (tested with 5.10.1).

Change into the cloned directory and:

- `qmake`
- `make`

The binary file `pwa-to-twa` will be created, to test
that everything works run `./pwa-to-twa`, you should see
something like this:

```
Wrong input, usage: pwa-to-twa androidPackageName pwaUrl outputPath [--manifest path-to-manifest]
Example: pwa-to-twa com.vendor.pwa https://pwa.vendor.com ./my-cool-pwa
Example: pwa-to-twa com.vendor.pwa https://pwa.vendor.com ./my-cool-pwa --manifest relative/path/to/manifest
```

### Building statically

You need to have docker installed.

Change into the cloned directory and:

- `docker run --rm -it -v $(pwd):/app fffaraz/qt:static /bin/bash -c "cd /app && qmake && make"`

This command will download the necessary image and
statically compile the app to binary file `pwa-to-twa`.
(Binary created using the same command is always
available in releases).

