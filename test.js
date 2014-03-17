//var logger = WScript.CreateObject("PumpkinLog.Server").createLogger("test.js", "window://test" + (new Date()).getTime());
var logger = WScript.CreateObject("PumpkinLog.Server").createLogger("test.js");
logger.info("Logger 1 הצהü");


logger.info("OK, finishing now");

WScript.echo("OK");
