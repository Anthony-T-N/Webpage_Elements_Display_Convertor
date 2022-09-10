#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>

int generate_HTML_file(std::vector<std::string> test)
{
    std::ofstream output_file;
    output_file.open("report.html");
    std::vector<std::string> temp_report_c = {};
    // Output file stream allows you to write contents to a file.
    std::ofstream output_file;
    if (std::filesystem::exists("List.csv") == false)
    {
        std::cout << "[!] Creating/opening List.csv;" << "\n";
        output_file.open("test.html", std::ios::app);
        std::cout << "[+] Opened List.csv successfully;" << "\n";
        output_file << line << "\n";
        temp_report_c.push_back(line);
        output_file.close();
        return 0;
    }
}

std::vector<std::string> read_file()
{
    std::ifstream input_file;
    input_file.open("List.csv");

    std::vector<std::string> input_file_line_vec;
    std::string input_file_line;

    while (std::getline(input_file, input_file_line))
    {
        std::cout << input_file_line << "\n";
        input_file_line_vec.push_back(input_file_line);
    }
    return input_file_line_vec;
}

int main()
{
    generate_HTML_file(read_file());

    // 1) Read text file with URLS
    // 2) Iterate through each URL to collect page elements.
    // 3) Generate HTML page with sorted elements.
}
