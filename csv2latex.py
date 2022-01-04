from typing import List, Dict
import os
import math
import subprocess

import jinja2
import pandas as pd
import matplotlib.pyplot as plt


def csv2plot(
    error_type: str, graph_filename: str, csv_path: str, algo_name: str
) -> List[str]:
    df = pd.read_csv(csv_path)
    with plt.style.context(["science"]):
        fig, ax = plt.subplots()
        ax.set_title(
            "Relative errors" if error_type == "RelError" else "Absolute errors"
        )
        ax.plot(df["X"], df[error_type])
        ax.autoscale(tight=True)
        pparam = dict(
            xlabel="$x$",
            ylabel="$\delta(y(x), \hat{y}(x))$"
            if error_type == "RelError"
            else "$\Delta(y(x), \hat{y}(x))$",
        )
        ax.set(**pparam)
        fig.savefig(graph_filename, dpi=300)


def info_parser(file_path: str) -> Dict[float, str]:
    df = pd.read_csv(file_path, header=None)
    tuples_info = df.to_records(index=False)
    return dict(
        (col, float(value)) if col not in ["AlgoName", "Author"] else (col, value)
        for col, value in tuples_info
    )


latex_jinja_env = jinja2.Environment(
    block_start_string="\BLOCK{",
    block_end_string="}",
    variable_start_string="\VAR{",
    variable_end_string="}",
    comment_start_string="\#{",
    comment_end_string="}",
    line_statement_prefix="%%",
    line_comment_prefix="%#",
    trim_blocks=True,
    autoescape=False,
    loader=jinja2.FileSystemLoader(os.path.abspath(".")),
)

# Round to three decimals
latex_jinja_env.filters["log2"] = (
    lambda x: round(abs(math.log2(abs(x))), 3) if x != 0 else "\infty"
)

template = latex_jinja_env.get_template("report_template.tex")

ERRORS_CSV_PATH = "errors.csv"
INFO_PATH = "info.txt"
GRAPH_REL_NAME = "relative_error.png"
GRAPH_ABS_NAME = "absolute_error.png"
LATEX_TEMPLATE_PATH = "report.tex"
info = info_parser(INFO_PATH)


def template_variables(info: Dict[float, str]) -> Dict[float, str]:
    result_variables = {
        "algo_name": info["AlgoName"],
        "name": info["Author"],
        "a": info["a"],
        "b": info["b"],
        "algo_time": info["Time"],
        "step": info["Step"],
    }

    if "MinRelError" in info:
        csv2plot("RelError", GRAPH_REL_NAME, ERRORS_CSV_PATH, info["AlgoName"])
        result_variables.update(
            {
                "min_rel": info["MinRelError"],
                "x_min_rel": info["ArgMinRel"],
                "max_rel": info["MaxRelError"],
                "x_max_rel": info["ArgMaxRel"],
                "rel_graph": GRAPH_REL_NAME,
            }
        )

    if "MinAbsError" in info:
        csv2plot("AbsError", GRAPH_ABS_NAME, ERRORS_CSV_PATH, info["AlgoName"])
        result_variables.update(
            {
                "min_abs": info["MinAbsError"],
                "x_min_abs": info["ArgMinAbs"],
                "max_abs": info["MaxAbsError"],
                "x_max_abs": info["ArgMaxAbs"],
                "abs_graph": GRAPH_ABS_NAME,
            }
        )
    return result_variables


with open(LATEX_TEMPLATE_PATH, "w+", encoding="utf-8") as latex_report:
    latex_report.write(template.render(template_variables(info)))

pdflatex = subprocess.call("pdflatex " + "report.tex > /dev/null", shell=True)
if pdflatex != 0:
    print("Oops! Dear friend, we have problems with pdflatex or your .tex template.")
else:
    print("Congratulations! Your report.pdf has been created")
