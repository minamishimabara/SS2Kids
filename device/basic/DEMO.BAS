10 H:HUMIDITY()
20 T:TEMPERATURE()
30 L:ANALOG()
40 PRINT "TMP:",T," HUM:",H," LUX:",L
50 http "tables/ss2ito","tmp",T,"hum",H,"lux",L
60 SLEEP 2000
70 GOTO 10

