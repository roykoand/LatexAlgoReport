.ONESHELL:
create_env:
	virtualenv $(VENV)
	$(VENV)/bin/pip install -r requirements.txt

.PHONY: clean 
clean:
	rm -f info.txt
	rm -f errors.csv
	rm -f report.pdf
	rm -f report.tex

.PHONY: run
run: clean
	touch info.csv
	g++ -o exec algo.cpp
	./exec 
	. $(VENV)/bin/activate
	python3 csv2latex.py