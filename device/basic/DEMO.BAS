10 H:HUMIDITY()
20 T:TEMPERATURE()
30 L:ANALOG ()
40 PRINT "TMP:",T," HUM:",H," LUX:",L
45 LED 14 ON
50 HTTP "http://sensortag.azure-mobile.net/tables/ss2ito","tmp",T,"hum",H,"lux",L
55 LED 14 OFF
60 SLEEP 1000
70 GOTO 10
