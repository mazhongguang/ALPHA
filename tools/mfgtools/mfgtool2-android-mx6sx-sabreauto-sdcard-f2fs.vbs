Set wshShell = CreateObject("WScript.shell")
wshShell.run "mfgtool2.exe -c ""linux"" -l ""SDCard-Android"" -s ""sxuboot=sabreauto"" -s ""folder=sabreauto"" -s ""board=sabreauto"" -s ""soc=66sx"" -s ""sxdtb=sabreauto"" -s ""mmc=2"" -s ""data_type=-f2fs"""
Set wshShell = Nothing
