@echo off
color 0a
title 删删更清爽 
del *.bak /s
del *.ddk /s
del *.edk /s
del *.lst /s
del *.lnp /s
del *.mpf /s
del *.mpj /s
del *.obj /s
del *.omf /s
::del *.opt /s  ::不允许删除JLINK的设置
del *.plg /s
del *.rpt /s
del *.tmp /s
del *.__i /s
del *.crf /s
del *.o /s
del *.d /s
del *.axf /s
del *.tra /s
del *.dep /s           
del JLinkLog.txt /s

del *.iex /s
del *.htm /s
del *.sct /s
del *.map /s
echo 【说明】:
echo         工程很多垃圾文件占用挺大的空间，都是编译生成的，经过清理后，下次打开
echo  重新编译工程即可！ —— 侯霄
echo                         2014-08-21 08:07
echo. & pause 
