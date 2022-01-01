.ONESHELL:
create_env:
	virtualenv $(VENV)
	$(VENV)/bin/pip install -r requirements.txt

.PHONY: clean 
clean:
	rm -f info.txt report.tex errors.csv report.pdf

.PHONY: run
run: clean
	g++ -o exec algo.cpp
	./exec 
	. $(VENV)/bin/activate
	python3 csv2latex.py