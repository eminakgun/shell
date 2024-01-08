#include "shell.hpp"
#include "commands.hpp"

int main(int argc, char *argv[]) {
    Shell shell;
    bool utest;

    if (argc == 2) {
        utest = static_cast<string>(argv[1]) == "utest";
    }
    
    if (utest) {
        shell.execute_command("cd ..");
        shell.execute_command("cp ../sample_file.txt sample_file.txt");
        shell.execute_command("cp some_file.txt some_new_file.txt");
        shell.execute_command("mkdir some_dir");
        shell.execute_command("cp some_file.txt some_dir/another_file.txt");
        shell.execute_command("cp ../test/test_cp.txt test_cp_2.txt");
        shell.execute_command("link some_file.txt link_file.txt");
        shell.execute_command("cat link_file.txt");
        shell.execute_command("cd some_dir");
        shell.execute_command("mkdir recurse_dir");
        shell.execute_command("cd ..");
        shell.execute_command("cd some_dir/recurse_dir");
        shell.execute_command("ls");
        shell.execute_command("mkdir another_dir");
        shell.execute_command("ls -R");
        shell.execute_command("cd another_dir");
        shell.execute_command("ls");
        shell.execute_command("cd ..");
        shell.execute_command("mkdir another_dir"); // removes another_dir
        shell.execute_command("cd ../..");
        shell.execute_command("ls -R"); 
    }

    // Run the interactive shell
    shell.interactive();

    return 0;
}