
PumpkinLogps.dll: dlldata.obj PumpkinLog_p.obj PumpkinLog_i.obj
	link /dll /out:PumpkinLogps.dll /def:PumpkinLogps.def /entry:DllMain dlldata.obj PumpkinLog_p.obj PumpkinLog_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del PumpkinLogps.dll
	@del PumpkinLogps.lib
	@del PumpkinLogps.exp
	@del dlldata.obj
	@del PumpkinLog_p.obj
	@del PumpkinLog_i.obj
