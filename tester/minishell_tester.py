import subprocess
import difflib

def run_test(command):
    try:
        shell_output = subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT, text=True)
        return shell_output.strip()
    except subprocess.CalledProcessError as e:
        return e.output.strip()

def main(input_file, output_diff_file):
    with open(input_file, 'r') as file:
        commands = file.readlines()
    with open(output_diff_file, 'w') as diff_file:
        diff_file.write("")
    for idx, command in enumerate(commands, start=1):
        command = command.strip()
        shell_output = run_test(command)
        minishell_output = run_test(f'./minishell {command}')

        if shell_output != minishell_output:
            diff = difflib.unified_diff(shell_output.splitlines(), minishell_output.splitlines())
            diff_text = '\n'.join(diff)
            with open(output_diff_file, 'a') as diff_file:
                diff_file.write(f"Test {idx} - Command: {command}\n")
                diff_file.write(diff_text)
                diff_file.write("\n\n")
            print(f"KO - Test {idx} - Diff saved to {output_diff_file}")
        else:
            print(f"OK - Test {idx}")

if __name__ == "__main__":
    input_file_path = "tests.txt"
    output_diff_file_path = "diff_output.txt"
    main(input_file_path, output_diff_file_path)