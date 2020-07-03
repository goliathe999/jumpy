@echo off
echo gfx
deltree ./bank1/*.*
.\bmp2tile\bmp2tile.exe .\gfx\titre.png -removedupes -mirror -savetiles .\bank1\titrebg.psgcompr -palsms -savepalette .\bank1\titrepalette.bin -savetilemap  .\bank1\titremap.bin -exit
.\bmp2tile\bmp2tile.exe .\gfx\SegaMasterSystemTitleScreen.png -mirror -removedupes -savetiles .\bank1\backgroundtiles.psgcompr -palsms -savepalette .\bank1\backgroundpalette.bin -savetilemap  .\bank1\backgroundtilemap.bin -exit
.\bmp2tile\bmp2tile.exe .\gfx\spyvsspyspritesheet.png -nomirror -noremovedupes -savetiles .\bank1\spritetiles.psgcompr -palsms -savepalette .\bank1\spritepalette.bin -exit
.\bmp2tile\bmp2tile.exe .\gfx\font.png -removedupes -mirror -savetiles .\bank1\font.psgcompr -palsms -savepalette .\bank1\fontpalette.bin -savetilemap  .\bank1\fontmap.bin -exit
#.\bmp2tile\bmp2tile.exe .\gfx\font.png -nomirror -noremovedupes -savetiles .\bank1\font.psgcompr -palsms -savepalette .\bank1\fontpalette.bin -exit

.\bmp2tile\bmp2tile.exe .\gfx\spyvsspysprite2.png -mirror -removedupes -savetiles .\bank1\spritetiles2.psgcompr -palsms -savepalette .\bank1\spritepalette2.bin -exit

echo folder2c
folder2c bank1 bank1
echo copy
copy .\bank1.c .\source\bank1.c
copy .\bank1.h .\source\header\bank1.h 
echo sdcc -c -mz80
sdcc -c -mz80 --peep-file peep-rules.txt source/main.c
sdcc -c -mz80 --peep-file peep-rules.txt source/bank1.c
sdcc -o sms.ihx -mz80 --no-std-crt0 --data-loc 0xC000 rel/crt0_sms.rel main.rel bank1.rel SMSlib.lib
echo ihx2sms
ihx2sms sms.ihx out/sms.sms

pause
del *.sym
del *.ihx
del *.lk
del *.noi
del *.lst
del *.asm
del *.map
del *.rel