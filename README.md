PumpkinLog
==========

PumpkinLog is a logserver for windows.

It is implemented using COM, so it can be used from various languages like C++,
JavaScript, VBScript etc.

Multiple "buckets" for logging are supported, so you can setup a logger to log
to e.g. a window and a file at the same time.

Multiple loggers can be created, where a certain logger is identified by its 
name.

A certain logger can be used from multiple locations, even from different 
processes.

The core runs as a singleton with the scope of a user-account.

Installing
----------

Currently there is no installer, you have to download a zip from the [Releases](https://github.com/IUnknown68/PumpkinLog/releases) -
or build it from source - and register the logger manually. To do so open a 
command prompt **as administrator**, navigate to the folder where you unzipped 
PumpkinLog and type:
```
PumpkinLog.exe /RegServer
```
To unregister type
```
PumpkinLog.exe /UnregServer
```

Since PumpkinLog is for developers, I guess that's acceptable for now.

**Note:** The logserver itself is 32bit, but the wrapper libraries (see below)
exist both in 32 and 64bit.

Usage
-----

You create (or get, in case it exists already) your logger by creating a
`PumpkinLog.Server` object and calling `createLogger` on it.

`createLogger` takes 1-n arguments:
- the name of the logger
- URI(s) identifying buckets

### From JavaScript, Windows Scripting Host:
A logger named "MyLogger", that logs to a window named "Default":
```JS
var logger = WScript.CreateObject("PumpkinLog.Server").
    createLogger("MyLogger", "window://Default/");
logger.log("Hello", "World");
```
A logger named "MyLogger", that logs to a window named "Default" and a file:
```JS
var logger = WScript.CreateObject("PumpkinLog.Server").
    createLogger("MyLogger", "window://Default/", "file://C:\\logfile.txt");
logger.log("Hello", "World");
```
When the logger exists already all URI arguments are ignored.

If you want to do this in IE or the WebBrowser-Control you have to do create a
`new ActiveXObject("PumpkinLog.Server")` instead of calling
`WScript.CreateObject("PumpkinLog.Server")`. Which will most probably fail
because of security restrictions. Just wanted to mention it.

The only URIs currently available are `window://` and `file://`, but the plan
is to add more, at least `http://` for logging on a remote server via http.

As you might have guessed already you use the `logger` object like the console
in your browser. It has the methods:
- log
- info
- debug
- warn
- error

All these methods take any number of arguments, they will joined together by
spaces. 

It also supports simple printf-like string substitution patterns as specified in
the [CommonJS-specs](http://wiki.commonjs.org/wiki/Console).

### C++
Of course you can use plain COM to do the same steps in C++, I guess you know
how. In case you don't like to deal with COM you can use the wrapper library
included. The wrapper also has the advantage, that nothing bad will happen on
systems where the logger is not installed, the calls to the logger will simply
be ignored.

So you have to:
- include `libPumpkinLog.h` in your project
- link against `libPumpkinLog.lib` (32 and 64bit available)

```C++
using namespace PumpkinLog;

Logger logger;

LPCWSTR buckets[] = {
  L"window://Default",
  L"file://C:\\logfile.txt"
};
HRESULT hr = logger.Init(L"MyLogger", buckets, _countof(buckets));
// error handling here

logger << L"Hello" << L"World" << Logger::log;
```
The way of passing multiple arguments here is the `<<` operator. Note that you
have to terminate each log line with one of the terminators. They determine the
log type (log, info, debug, warn, error) and trigger the actual call to the
underlying `Logger` object. Otherwise it works exactly like in JavaScript.

So to format a debug message you would do:
```C++
// from somewhere
int itemCount;
LPCWSTR itemType;
logger << L"Roger, I have %i items of type %s" << itemCount << itemType << Logger::log;
```
You can log everything that can be converted to a `VARIANT`.

Also there are special types for logging `LRESULT` (Win32-errors) and `HRESULT`
types.

Special classes exist for this: `CLRESULT` and `CHRESULT`. Use them like this:
```C++
// from somewhere
HRESULT hr;
logger << L"Result:" << CHRESULT(hr) << Logger::log;
```

*to be continued...*
