function UserInput( myPrompt ) {
  WScript.StdOut.Write(myPrompt + " ");
  return WScript.StdIn.ReadLine();
}

var logger1 = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", "window://default/");
//var logger1 = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", "window://default/", "file://U:\\Users\\Hans\\Documents\\Visual Studio 2010\\Projects\\PumpkinLog\\test.log");
//var logger1 = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", "http://3.arne.internal.salsitasoft.com/");
//var logger1 = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", "file://C:/Users/Arne/Documents/Visual Studio 2010/Projects/PumpkinLog/testlog.txt");
//var logger1 = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", "file://C:/Users/Arne/Documents/Visual Studio 2010/Projects/PumpkinLog/testlog.rtf");
//var logger1 = WScript.CreateObject("PumpkinLog.Server").createLogger("logger1", "config://C:/Users/Arne/Documents/Visual Studio 2010/Projects/PumpkinLog/logconfig.xml");

logger1.info("Wait for input");
var s = UserInput('Enter something');
logger1.info("DONE:", s);

