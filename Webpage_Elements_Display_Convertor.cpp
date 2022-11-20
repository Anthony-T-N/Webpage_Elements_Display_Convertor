#define CURL_STATICLIB
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>
#include <map>

std::vector<std::string> html_extractor(const char* target_url);

#pragma warning(disable:4996)

std::string get_current_date()
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    // Error C4996 'localtime': This function or variable may be unsafe.Consider using localtime_s instead.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    return buffer;
}

int generate_HTML_file(std::vector<std::string> input_file_line_vec)
{
    std::vector<std::string> extracted_strings;
    std::ifstream input_file;
    std::cout << "\n" << "[!] Creating/opening elements_webpage_template.html;" << "\n";
    input_file.open("elements_webpage_template.html");
    std::cout << "[+] Opened elements_webpage_template.html successfully;" << "\n";
    std::ofstream output_file;
    std::cout << "\n" << "[!] Creating/opening elements_list_output.html;" << "\n";
    output_file.open("elements_list_output-(" + get_current_date() + ").html");
    std::cout << "[+] Opened elements_list_output.html successfully;" << "\n\n";
    std::string input_file_line;
    int count = 0;
    while (std::getline(input_file, input_file_line))
    {
        if (input_file_line.find("<div class=\"row\">") != std::string::npos)
        {
            std::cout << "<div class=\"row\"> found" << "\n";
            output_file << input_file_line << "\n";
            for (int i = 0; i <= input_file_line_vec.size() - 1; i++)
            {
                std::cout << "[" << count << "|" << input_file_line_vec.size() << "] ";
                count++;
                extracted_strings = html_extractor(input_file_line_vec[i].c_str());
                if (extracted_strings[0] == "N/A")
                {
                    continue;
                }
                std::string color;
                if (i % 2 == 0)
                {
                    color = "#34495E";
                }
                else
                {
                    color = "#273746";
                }
                
                // 

                // #ba000a for completed urls.

                output_file << "<div class=\"column\" style=\"background-color:" + color + ";display:flex;\">" << "\n"
                    << "<a href=" + input_file_line_vec[i] + ">" + "<img style=\"margin-right:10px;display:block;height:445px;\" src=\"" + extracted_strings[1] + "\"></a>" << "\n"
                    << "<div class=\"text\"><h3 style=\"color:white;\"><a href=" + input_file_line_vec[i] + ">" + extracted_strings[0] + "</a></h2><p style=\"color:white;\">" + extracted_strings[2] + "</p>" << "\n" 
                    << "</div>" << "\n" << "</div>" << "\n\n";
            }
        }
        else if (input_file_line.find("<p>Footer</p>") != std::string::npos)
        {
            std::cout << "<p>Footer</p> found" << "\n";
            output_file << "Total Count: " << count << "\n";
        }
        else
        {
            output_file << input_file_line << "\n";
        }
    }
    input_file.close();
    output_file.close();
    return 0;
}

/* Obsolete Function: 
std::vector<std::string> read_file()
{
    std::ifstream input_file;
    input_file.open("List.csv");

    std::vector<std::string> input_file_line_vec;
    std::string input_file_line;

    std::cout << "URL List:" << "\n";
    while (std::getline(input_file, input_file_line))
    {
        input_file_line = input_file_line.substr(input_file_line.find(',') + 1);
        std::cout << input_file_line << "\n";
        input_file_line_vec.push_back(input_file_line);
    }
    input_file.close();
    return input_file_line_vec;
}
*/

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::vector<std::string> html_extractor(const char* target_url)
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
    }

    try
    {
        std::string target_tag = stored_webpage.substr(stored_webpage.find("<h1"), stored_webpage.find("</h1>") - stored_webpage.find("<h1"));
        target_tag = target_tag.substr(target_tag.find("<strong>") + 8, target_tag.find("</strong>") - target_tag.find("<strong>") - 8);
        std::cout << "Target_Tag: " << target_tag << "\n";

        std::string target_image = stored_webpage.substr(stored_webpage.find("lazyload"));
        target_image = target_image.substr(target_image.find("data-src=\"") + 10, target_image.find("\" alt=") - target_image.find("data-src=\"") - 10);
        std::cout << "Target_Image: " << target_image << "\n";

        std::string target_desc = stored_webpage.substr(stored_webpage.find("<p itemprop=\"description\">"));
        target_desc = target_desc.substr(target_desc.find("<p itemprop=\"description\">") + 26, target_desc.find("</p>") - target_desc.find("<p itemprop=\"description\">") - 22);
        //std::cout << "Target_Desc: " << target_desc << "\n\n";

        std::cout << "\n";

        std::vector<std::string> extracted_strings = { target_tag , target_image, target_desc };
        return extracted_strings;
    }
    catch (const std::out_of_range& out_of_range)
    {
        std::cerr << "\n" << "\033[4;31m" << "Out_of_range Error : " << out_of_range.what() << "\033[0m" << "\n\n";
        std::vector<std::string> extracted_strings = { "N/A" , "", "" };
        return extracted_strings;
    }
}

std::vector<std::string> url_detection(std::map<int, std::string> path_list_map, std::string user_input)
{
    std::ifstream input_file;
    input_file.open(path_list_map[std::stoi(user_input)]);
    std::cout << "[!] Selection: " << path_list_map[std::stoi(user_input)] << "\n\n";

    std::ifstream url_detection_logic_file;
    url_detection_logic_file.open("url_detection_logic.txt");

    std::vector<std::string> input_file_line_vec;
    std::string input_file_line;
    std::string url_detection_logic;

    while (std::getline(url_detection_logic_file, input_file_line))
    {
        url_detection_logic = input_file_line;
        std::cout << "[!] URL Detection Logic: " << url_detection_logic << "\n";
    }
    url_detection_logic_file.close();

    int i = 0;
    while (std::getline(input_file, input_file_line))
    {
        if (input_file_line.find(url_detection_logic) != std::string::npos)
        {
            input_file_line = input_file_line.substr(input_file_line.find(url_detection_logic));
            input_file_line = input_file_line.substr(input_file_line.find(url_detection_logic), input_file_line.find("\""));
            std::cout << i << ") " << input_file_line << "\n";
            input_file_line_vec.push_back(input_file_line);
            i++;
        }
        else
        {
            std::cout << "[-]" << input_file_line.substr(0, 100) << "\n";
        }
    }
    input_file.close();

    std::cout << "url_list" << "\n";
    for (int i = 0; i < input_file_line_vec.size() - 1; i++)
    {
        std::cout << input_file_line_vec[i] << "\n";
    }
    return input_file_line_vec;
}

int main()
{
    std::cout << "=======================================" << "\n";
    std::cout << "- Webpage_Elements_Display_Convertor console application" << "\n";
    std::cout << "- Console Application Version: 1.0" << "\n";
    std::cout << "- Created By: Anthony-T-N." << "\n";
    std::cout << "- Current location of executable: " << std::filesystem::current_path() << "\n";
    std::cout << "=======================================" << "\n\n";

    if (std::filesystem::exists("elements_webpage_template.html") != 1)
    {
        std::cout << "[-] elements_webpage_template.html" << "\n";
        std::cout << "[!] END" << "\n";
        std::cout << "[!] Exiting..." << "\n\n";
        std::cin.get();
        return 0;
    }
    if (std::filesystem::exists("url_detection_logic.txt") != 1)
    {
        std::cout << "[-] url_detection_logic.txt" << "\n";
        std::cout << "[!] END" << "\n";
        std::cout << "[!] Exiting..." << "\n\n";
        std::cin.get();
        return 0;
    }

    std::map<int, std::string> path_list_map;
    std::vector<std::string> url_list;
    std::string path = std::filesystem::current_path().generic_string();
    int iteration = 0;
    std::cout << "[!] Current text files:" << "\n";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if ((entry.path().generic_string().substr(entry.path().generic_string().find_last_of("//") + 1)).find(".txt") != std::string::npos ||
            (entry.path().generic_string().substr(entry.path().generic_string().find_last_of("//") + 1)).find(".csv") != std::string::npos ||
            (entry.path().generic_string().substr(entry.path().generic_string().find_last_of("//") + 1)).find(".html") != std::string::npos)
        {
            std::cout << "[" << iteration << "] " << entry.path().generic_string().substr(entry.path().generic_string().find_last_of("//") + 1) << "\n";
            path_list_map.insert(std::make_pair(iteration, entry.path().generic_string().substr(entry.path().generic_string().find_last_of("//") + 1)));
            iteration++;
        }
    }
    /*
    for (auto const& [key, val] : path_list_map)
    {
        std::cout << key << ": " << val << std::endl;
    }
    */
    std::string user_input;
    while (true)
    {
        std::cout << "\n" << "[=] Make Selection: " << "[0-" << iteration - 1 << "]" << "\n";
        std::cout << "> ";
        std::getline(std::cin, user_input);

        if (user_input.find_first_not_of("0123456789") != std::string::npos || user_input.empty())
        {
            std::cout << "[-] Invalid input - Please try again" << "\n";
        }
        else if (std::stoi(user_input) > iteration - 1)
        {
            std::cout << "[-] Invalid input - Please try again" << "\n";
        }
        else
        {
            break;
        }
    }
 
    generate_HTML_file(url_detection(path_list_map, user_input));

    std::cout << "[!] END" << "\n";
    std::cout << "[!] Exiting..." << "\n\n";
    std::cin.get();
    return 0;
}
