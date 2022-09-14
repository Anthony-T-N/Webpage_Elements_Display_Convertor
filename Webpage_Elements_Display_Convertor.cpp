#define CURL_STATICLIB
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>

int generate_HTML_file(std::vector<std::string> input_file_line_vec)
{
    std::ifstream input_file;
    input_file.open("elements_webpage_template.html");
    std::ofstream output_file;
    if (std::filesystem::exists("test.html") == true)
    {
        std::cout << "[!] Creating/opening List.csv;" << "\n";
        output_file.open("test.html");
        std::cout << "[+] Opened List.csv successfully;" << "\n";
        std::string input_file_line;
        while (std::getline(input_file, input_file_line))
        {
            if (input_file_line.find("<tbody>") != std::string::npos)
            {
                std::cout << "<tbody> found" << "\n";
                for (int i = 0; i <= input_file_line_vec.size() - 1; i++)
                {
                    //std::string test = "<tr><td style=\"width:50 % \"><h3>Title 1</h3></td><td>&nbsp;</td><td style=\"width : 50 % \"><h3>Title 2</h3></td></tr><tr><td><p>" + input_file_line_vec[i] + "</p><p>&nbsp;</p><p>&nbsp;</p></td><td>&nbsp;</td><td><p>" + input_file_line_vec[i] + "</p><p>&nbsp;</p><p>&nbsp;</p></td></tr>";
                    //std::cout << input_file_line_vec[i] << "\n";
                    //std::cout << test << "\n";
                    output_file << "<tr><td style=\"width:50 % \"><h3>Title 1</h3></td><td>&nbsp;</td><td style=\"width : 50 % \"><h3>Title 2</h3></td></tr><tr><td><p>" + input_file_line_vec[i] + "</p><p>&nbsp;</p><p>&nbsp;</p></td><td>&nbsp;</td><td><p>" + input_file_line_vec[i] + "</p><p>&nbsp;</p><p>&nbsp;</p></td></tr>" << "\n";
                }
            }
            else
            {
                output_file << input_file_line << "\n";
            }
            std::cout << input_file_line << "\n";

        }
        input_file.close();
        output_file.close();
        return 0;
    }
    return 0;
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
    input_file.close();
    return input_file_line_vec;
}

int main()
{
    //generate_HTML_file(read_file());

    CURL* curl;

    curl = curl_easy_init();
    curl_easy_cleanup(curl);

    return 0;

    // 1) Read text file with URLS
    // 2) Iterate through each URL to collect page elements.
    // 3) Generate HTML page with sorted elements.
}
