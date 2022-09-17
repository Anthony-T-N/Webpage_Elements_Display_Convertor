#define CURL_STATICLIB
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>

int generate_HTML_file(std::vector<std::string> input_file_line_vec)
{
    /*
    const char* target_url = "";
    //generate_HTML_file(read_file());
    html_extractor(target_url);
    */

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
        input_file_line = input_file_line.substr(input_file_line.find(',') + 1);
        std::cout << input_file_line << "\n";
        input_file_line_vec.push_back(input_file_line);
    }
    input_file.close();
    return input_file_line_vec;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void html_extractor(const char* target_url)
{
    std::cout << target_url << "\n";

    CURL* curl;
    CURLcode res;
    std::string stored_webpage;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, target_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stored_webpage);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        std::cout << stored_webpage << std::endl;
    }
    std::string target_tag = stored_webpage.substr(stored_webpage.find("<h1"), stored_webpage.find("</h1>") - stored_webpage.find("<h1"));
    std::cout << "Target_Tag: " << target_tag << "\n";
    /*
    for (int i = 0; i <= stored_webpage.size() - 1; i++)
    {
        "<h1 class = "title-name h1_bold_none"><strong>Hataraku Maou - sama!!< / strong>< / h1>"
        if (stored_webpage[i] == 'c')
        {
            std::cout << "note" << std::endl;
        }
    }
    */
}

int main()
{
    generate_HTML_file(read_file());
    
    return 0;

    // 1) Read text file with URLS
    // 2) Iterate through each URL to collect page elements.
    // 3) Generate HTML page with sorted elements.
}
