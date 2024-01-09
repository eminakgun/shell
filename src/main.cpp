#include "shell.hpp"
#include "commands.hpp"

int main(int argc, char *argv[]) {
    Shell shell;
    bool utest = false;

    if (argc == 2) {
        utest = static_cast<string>(argv[1]) == "utest";
    }
    
    if (utest) {
        // test outward copy
        shell.execute_command("cd test_dir");
        shell.execute_command("cp test_file.txt ../outer_test_file.txt");
        shell.execute_command("cd ..");
        
        // test cd to outside of file system
        shell.execute_command("cd ..");

        // copy file from outside of filesystem
        shell.execute_command("cp ../sample_file.txt sample_file.txt");

        // duplicate file in the same folder
        shell.execute_command("cp some_file.txt some_new_file.txt");

        shell.execute_command("mkdir some_dir");

        // Inward copy
        shell.execute_command("cp some_file.txt some_dir/another_file.txt");

        // outward hierarchical path copy
        shell.execute_command("cp ../test/test_cp.txt test_cp_2.txt");

        // link and cat linked file
        shell.execute_command("link some_file.txt link_file.txt");
        shell.execute_command("cat link_file.txt");
        
        // mkdir, ls .... etc
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