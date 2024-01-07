#include "shell.hpp"
#include "commands.hpp"

int main() {
    Shell shell;
    
    shell.execute_command("cd ..");
    shell.execute_command("cp some_file.txt some_new_file.txt");
    shell.execute_command("cp some_file.txt some_dir/some_new_file.txt");
    shell.execute_command("cp ../test/test_cp.txt cp_file2.txt");
    shell.execute_command("link some_file.txt link");
    shell.execute_command("cat link");
    shell.execute_command("cd some_dir/recurse_dir");
    shell.execute_command("ls");
    shell.execute_command("mkdir another_dir");
    shell.execute_command("ls -R");
    shell.execute_command("cd another_dir");
    shell.execute_command("ls");
    shell.execute_command("cd ..");
    shell.execute_command("cd ..");
    shell.execute_command("ls -R");
    //shell.execute_command("cd ../.."); // TODO does not work
    
    
    //shell.execute_command("exit");

    // Run the interactive shell
    shell.interactive();

    

    return 0;
}