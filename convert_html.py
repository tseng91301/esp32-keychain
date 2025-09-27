import os

INPUT = "data/index.html"
OUTPUT = "src/index_html.h"

def to_c_string(s: str):
    return s.replace("\\", "\\\\").replace("\"", "\\\"").replace("\n", "\\n\"\n\"")

if os.path.exists(INPUT):
    with open(INPUT, "r", encoding="utf-8") as f:
        html_content = f.read()

    c_str = to_c_string(html_content)
    with open(OUTPUT, "w", encoding="utf-8") as f:
        f.write("// This code is auto generated\n")
        f.write("const char index_html[] PROGMEM = \n")
        f.write("\"" + c_str + "\";\n")

    print(f"[HTML->C] {INPUT} -> {OUTPUT}")
else:
    print(f"[HTML->C] 找不到 {INPUT}，跳過轉換")
