# BG Application Sample for PS Vita

This sample demonstrates how background applications for PS Vita can be created.

Both main and BG applications can run with games in the background (and also foreground for BG application). 

Use [NoSharedFb plugin](https://github.com/GrapheneCt/NoSharedFb) to bypass framebuffer problems when resuming from LiveArea.

BG - main application pair is connected by Vita's OS like this:

![alt text](https://github.com/GrapheneCt/BG-App-PSV/blob/master/img.jpg?raw=true)

# How to Build

Use vitasdk to build application. After building, boot params for both executables have to be set manually. Refer to the precompiled version for the values.
