import os

# 原始資料夾
SOURCE_DIR = "pages"
# 輸出資料夾
OUTPUT_DIR = "src/pages"

# 確保輸出資料夾存在
os.makedirs(OUTPUT_DIR, exist_ok=True)

for root, dirs, files in os.walk(SOURCE_DIR):
    for filename in files:
        # 原始檔案路徑
        src_path = os.path.join(root, filename)
        # 將檔名中的 "." 改成 "_"，再加上 ".h"
        base_name = filename.replace(".", "_")
        header_name = f"{base_name}.h"
        dst_path = os.path.join(OUTPUT_DIR, header_name)

        # 讀取原始檔案內容
        with open(src_path, "r", encoding="utf-8") as f:
            content = f.read()

        # 產生變數名稱 (去除副檔名)
        var_name = base_name

        # 組合 .h 檔內容
        header_content = f'const char {var_name}[] PROGMEM = R"rawliteral(\n{content}\n)rawliteral";\n'

        # 寫出 .h 檔
        with open(dst_path, "w", encoding="utf-8") as f:
            f.write(header_content)

        print(f"✅ Converted: {filename} → {header_name}")

print("\nAll files converted successfully!")
