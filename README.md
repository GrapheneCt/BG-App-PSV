# BG Application Sample for PS Vita

This sample demonstrates how background applications for PS Vita can be created. 

Use [NoSharedFb plugin](https://github.com/GrapheneCt/NoSharedFb) to bypass framebuffer problems when resuming from LiveArea.

SceBgAppUtil is used to manage BG application state from main application and SceNotificationUtil is used in BG application to send custom notifications.

Both main and BG applications can run with games in the background (and also foreground for BG application).

Notifications from BG application can be enabled/disabled in Settings->Notifications. BG application can be restared from main application once it finished working (after the 20sec notification).

BG - main application pair is connected by Vita's OS like this:

![alt text](https://github.com/GrapheneCt/BG-App-PSV/blob/master/img.jpg?raw=true)

# How to Build

Use vitasdk to build application. After building, boot params for both executables must be set manually. Refer to the precompiled version for the values.
