LDFLAGS=-lcxxtools
CC=g++
all: wjson wjsond wjsonc wjsoncc wjsondd wjsoncc2 wjsond2 wjsonweb

json: wjson.o
jsonc: wjsonc.o
jsoncc: wjsoncc.o
jsond: wjsond.o
jsoncc2: wjsoncc2.o
jsond2: wjsond2.o
wjsonweb: LDFLAGS = -lcxxtools -ltntnet
wjsonweb: wjsonweb.o json/person.o json/storePerson.o webmain.o jquery.o

webmain.cpp: webmain.ecpp
	ecppc -p -o $@ $<

json/person.cpp: json/person.ecpp
	ecppc -p -o $@ $<

json/storePerson.cpp: json/storePerson.ecpp
	ecppc -p -o $@ $<

jquery.cpp: jquery-3.3.1.min.js
	ecppc -b -n jquery.js -o $@ $<
