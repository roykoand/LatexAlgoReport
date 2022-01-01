from typing import List, Dict
import os
import math
import jinja2
import pandas as pd
import matplotlib.pyplot as plt


def csv2plot(graph_filename: str, csv_path: str, algo_name: str) -> List[str]:
	df = pd.read_csv(csv_path)
	with plt.style.context(['science']):
		fig, ax = plt.subplots()
		ax.set_title(algo_name)
		ax.plot(df["X"], df["Error"])
		ax.autoscale(tight=True)
		pparam = dict(xlabel='x', ylabel='Relative error')
		ax.set(**pparam)
		fig.savefig(graph_filename, dpi=300)

def info_parser(file_path: str) -> Dict[float, int]:
	df = pd.read_csv(file_path, header=None)
	tuples_info = df.to_records(index=False)
	return dict(tuples_info)


latex_jinja_env = jinja2.Environment(
	block_start_string = '\BLOCK{',
	block_end_string = '}',
	variable_start_string = '\VAR{',
	variable_end_string = '}',
	comment_start_string = '\#{',
	comment_end_string = '}',
	line_statement_prefix = '%%',
	line_comment_prefix = '%#',
	trim_blocks = True,
	autoescape = False,
	loader = jinja2.FileSystemLoader(os.path.abspath('.'))
)

latex_jinja_env.filters["log2"] = lambda x: round(abs(math.log2(abs(x))), 3)

template = latex_jinja_env.get_template('report_template.tex')

ERRORS_CSV_PATH = "errors.csv"
INFO_PATH = "info.txt"
GRAPH_PATH = "relative_error.png"
LATEX_TEMPLATE_PATH = "report.tex"
ALGO_NAME="Fast Square Inverse Root"

csv2plot(GRAPH_PATH, ERRORS_CSV_PATH, ALGO_NAME)
info = info_parser(INFO_PATH)

with open(LATEX_TEMPLATE_PATH, 'w+', encoding='utf-8') as latex_report:
    latex_report.write(template.render(
		algo_name=ALGO_NAME,
        name='Andrii Roiko', 
		a = info["a"],
        b = info["b"],
        min_rel=info["MinRelError"], 
        x_min_rel=info["ArgMinRel"],
        max_rel=info["MaxRelError"],
        x_max_rel=info["ArgMaxRel"],
		algo_time=info["Time"],
		filename=GRAPH_PATH))
		
os.system("pdflatex " + "report.tex")   