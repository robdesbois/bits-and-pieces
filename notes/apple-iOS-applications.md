# Listing 2-1  Bundle structure of an iOS application #
    [https://developer.apple.com/library/mac/documentation/corefoundation/conceptual/cfbundles/BundleTypes/ BundleTypes.html]
    MyApp.app
        MyApp
        MyAppIcon.png
        MySearchIcon.png
        Info.plist
        Default.png
        MainWindow.nib
        Settings.bundle
        MySettingsIcon.png
        iTunesArtwork
        en.lproj
            MyImage.png
        fr.lproj
            MyImage.png

# Table 2-2  Contents of a typical iOS application bundle #
    [https://developer.apple.com/library/mac/documentation/corefoundation/conceptual/cfbundles/BundleTypes/BundleTypes.html]
    MyApp             (Required) The executable file containing your application’s code. The name of this file is the same as
                      your application name minus the .app extension.
    Icons             (Required/Recommended) Application icons are used at specific times to represent the application. For
                      example, different sizes of the application icon are displayed in the Home screen, in search results, and in the Settings application. Not all of the icons are required but most are recommended. For information about application icons, see “Application Icon and Launch Images.”
    Info.plist        (Required) This file contains configuration information for the application, such as its bundle ID,
                      version number, and display name. See “The Information Property List File” for further information.
    Images            (Recommended) One or more images that show the initial interface of your application in a specific
                      orientation. The system uses one of the provided launch images as a temporary background until your application loads its window and user interface. If your application does not provide any launch images, a black background is displayed while the application launches. For information about application icons, see “Application Icon and Launch Images.”
    MainWindow.nib    (Recommended) The application’s main nib file contains the default interface objects to load at
                      application launch time. Typically, this nib file contains the application’s main window object and an instance of the application delegate object. Other interface objects are then either loaded from additional nib files or created programmatically by the application. (The name of the main nib file can be changed by assigning a different value to the NSMainNibFile key in the Info.plist file. See “The Information Property List File” for further information.)
    Settings.bundle   The Settings bundle is a special type of plug-in that contains any application-specific preferences that
                      you want to add to the Settings application. This bundle contains property lists and other resource files to configure and display your preferences.
    Resource files    Non-localized resources are placed at the top level directory and localized resources are placed in
                      language-specific subdirectories of the application bundle. Resources consist of nib files, images, sound files, configuration files, strings files, and any other custom data files you need for your application. For more information about resources, see “Resources in an iOS Application.”
