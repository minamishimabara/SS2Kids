10 T:TEMPERATURE()
20 H:HUMIDITY()
30 A:ANALOG()
40 LCD "DATA SENDING"
50 HTTP "http://sensortag.azure-mobile.net/tables/ss2ito","tmp",T,"hum",H,"lux",A
60 LCD "PHOTO DATA SENDING"
70 CAMERA "http://sensortag.azure-mobile.net/tables/ss2saito"
80 LCD "T:",T," H:",H," L:",A
90 SLEEP 5000
100 GOTO 10
