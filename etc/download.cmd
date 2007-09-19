setMode -bscan
#setCable -p usb21 -server ensigam01
setCable -p usb21
identify
assignfile -p 3 -file implementation/download.bit
program -p 3
quit
