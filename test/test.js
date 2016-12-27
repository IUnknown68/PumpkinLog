function UserInput( myPrompt ) {
  WScript.StdOut.Write(myPrompt + " ");
  return WScript.StdIn.ReadLine();
}

var console = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", ["window://default/", "file://U:\\Users\\Hans\\Documents\\Visual Studio 2012\\Projects\\PumpkinLog\\test.log"]);
//var console = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", "window://default/", "file://U:\\Users\\Hans\\Documents\\Visual Studio 2012\\Projects\\PumpkinLog\\test.log");
//var console = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", "http://3.arne.internal.salsitasoft.com/");
//var console = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", "file://C:/Users/Arne/Documents/Visual Studio 2012/Projects/PumpkinLog/testlog.txt");
//var console = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", "file://C:/Users/Arne/Documents/Visual Studio 2012/Projects/PumpkinLog/testlog.rtf");
//var console = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", "config://C:/Users/Arne/Documents/Visual Studio 2012/Projects/PumpkinLog/logconfig.xml");

//console.info("Wait for input");
//var s = UserInput('Enter something');
//console.info("DONE:", s);

var ob = {a: "hello"};
console.info("Just a text");
console.info("A text: %s and a number %i", "Hello", 123);
console.warn("A text: %s and a number %i, but no arguments");
console.warn("A text: %s and a number %i, but only one argument", "Hello");
console.warn("A text: %s and a number %i, but more arguments", "Hello", 123, "hello", 456);
console.info("Starting with % then a text: %s and a number %i", "Hello", 123);
console.info("A text: %s and a percentage number %i%", "Hello", 123);
console.info("An object %o", undefined);

console.warn("Expected text, but have number: '%s'", 123);
console.warn("Expected number, but have text: '%i'", "Hello");

var ll = 1;
console.loglow(ll, ["Lowlevel logging", ll]);
