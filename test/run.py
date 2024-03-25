import os
import subprocess
import shutil

with open("code_name.txt") as code_file:
    content = code_file.readlines()
    for code_name in content:
        code_name = code_name.strip("\n")
        code_name = code_name.replace("/","\\")
        asm_file = f"{code_name}"
        folder_name = code_name.split("\\")[0]

        # Create a new folder for each code_name
        try:
            os.mkdir(folder_name)
        except FileExistsError:
            print(f"Folder '{folder_name}' already exists. Files will be overwritten.")
        
        file = code_name.split("\\")[-1] # remove the complete directory
        filename = file.split(".")[0] # remove the extension
        output_file = f"{filename}.report"
        with open(output_file, "w") as output:
            result = subprocess.run(["./rva.exe", asm_file], stdout=output, stderr=subprocess.STDOUT, text=True)

        # Move the generated files to the respective folder
        report_file = f"{filename}.report"        
        shutil.move(report_file, os.path.join(folder_name, report_file))