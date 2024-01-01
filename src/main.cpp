#include "shell.hpp"
#include "commands.hpp"

int main() {
    Shell shell;

    CpCommand* cp = new CpCommand();
    shell.add_command("cp", cp);

    CpCommand cp_test("test/test_cp.txt", "cp_file.txt");
    //shell.execute_command(cp_test);
    //shell.execute_command("cp test/test_cp.txt cp_file2.txt");
    shell.execute_command("exit");

    /*
    // Run the interactive shell
    while (true) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        shell.execute_command(input);
    }
    */

    return 0;
}