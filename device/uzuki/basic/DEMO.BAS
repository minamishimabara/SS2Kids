10 T:TEMPERATURE()
20 H:HUMIDITY()
30 L:ILLUMI()
40 I:ILLUMIIR()
50 U:UVINDEX()
60 PRINT "TMP:",T," HUM:",H," LUX:",L," IR:",I," UV:",U
65 LED 14 ON
70 HTTP "http://sensortag.azure-mobile.net/tables/ss2ito","tmp",T,"hum",H,"lux",L,"ir",I,"uv",U
75 LED 14 OFF
80 SLEEP 1000
90 GOTO 10
