#VST Interceptor

This is a very simpl VST plugin wrapper that lets you replace the VstId of a plugin with your own. Useful for some plugins that don't set the Id correctly.

I built this mainly so I could use [Vadim Taranov's VST plugins](https://pvdhp7.wordpress.com/) in Ableton.

Usage is simple: Just build the file (or download the binary from the Release folder), and rename the file in the following way:

    PluginName.Intercept.1234567890.dll

Where PluginName is the name of a matching "PluginName.dll" VST Plugin that you want to wrap. The 10-digit number is your new VstId. **Note: It must contain exactly 10 digits, pad the number with leading zeros if necessary!**

The code is also useful in case you want to intercept more operations in the a plugin. Feel free to use as you see fit.

The code is public domain, do with it what you please, but don't holde me responsible if you make a mess!




