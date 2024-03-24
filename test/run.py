import os
import subprocess
import shutil

with open("code_name.txt") as code_file:
    content = code_file.readlines()
    for code_name in content:
        code_name = code_name.strip("\n")
        asm_file = f"{code_name}.asm"
        folder_name = code_name

        # Create a new folder for each code_name
        try:
            os.mkdir(folder_name)
        except FileExistsError:
            print(f"Folder '{folder_name}' already exists. Files will be overwritten.")

        output_file = f"{code_name}.report"
        with open(output_file, "w") as output:
            result = subprocess.run(["./rva.exe", asm_file], stdout=output, stderr=subprocess.STDOUT, text=True)

        # Move the generated files to the respective folder
        report_file = f"{code_name}.report"
        mc_file = f"{code_name}.mc"
        trace_file = f"{code_name}.trace"

        shutil.move(report_file, os.path.join(folder_name, report_file))
        shutil.move(mc_file, os.path.join(folder_name, mc_file))
        shutil.move(trace_file, os.path.join(folder_name, trace_file))