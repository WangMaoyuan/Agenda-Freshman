cc := g++-4.8 -std=c++0x
INCLUDE := -I./include

bin/Main: build/User.o build/Date.o build/Meeting.o build/Storage.o build/AgendaService.o build/AgendaUI.o build/Agenda.o
	@mkdir -p bin
	$(cc) $(INCLUDE) $^ -o $@

build/%.o: src/%.cpp
	@mkdir -p build
	$(cc) $(INCLUDE) -c -o $@ $<

clean:
	@rm -rf bin
	@rm -rf build
	
