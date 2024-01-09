
#include "shell.hpp"
#include "commands.hpp"

using namespace shell;

int main(int argc, char *argv[]) {
    Shell shell_app;
    bool utest = false;

    if (argc == 2) {
        utest = static_cast<string>(argv[1]) == "utest";
    }
    
    if (utest) {
        // test outward copy
        shell_app.execute_command("cd test_dir");
        shell_app.execute_command("cp test_file.txt ../outer_test_file.txt");
        shell_app.execute_command("cd ..");
        
        // test cd to outside of file system
        shell_app.execute_command("cd ..");

        // copy file from outside of filesystem
        shell_app.execute_command("cp ../sample_file.txt sample_file.txt");

        // duplicate file in the same folder
        shell_app.execute_command("cp some_file.txt some_new_file.txt");

        shell_app.execute_command("mkdir some_dir");

        // Inward copy
        shell_app.execute_command("cp some_file.txt some_dir/another_file.txt");

        // outward hierarchical path copy
        shell_app.execute_command("cp ../test/test_cp.txt test_cp_2.txt");

        // link and cat linked file
        shell_app.execute_command("link some_file.txt link_file.txt");
        shell_app.execute_command("cat link_file.txt");
        
        // mkdir, ls .... etc
        shell_app.execute_command("cd some_dir");
        shell_app.execute_command("mkdir recurse_dir");
        shell_app.execute_command("cd ..");
        shell_app.execute_command("cd some_dir/recurse_dir");
        shell_app.execute_command("ls");
        shell_app.execute_command("mkdir another_dir");
        shell_app.execute_command("ls -R");
        shell_app.execute_command("cd another_dir");
        shell_app.execute_command("ls");
        shell_app.execute_command("cd ..");
        shell_app.execute_command("mkdir another_dir"); // removes another_dir
        shell_app.execute_command("cd ../..");
        shell_app.execute_command("ls -R");

        // TODO Test size limit
    }

    // Run the interactive shell_app
    shell_app.interactive();

    return 0;
}