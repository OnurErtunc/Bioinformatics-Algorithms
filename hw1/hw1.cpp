#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <map>

std::vector<int> brute_force_string_match(const std::string &t, const std::string &p)
{
    std::vector<int> matches;
    int n = static_cast<int>(t.length());
    int m = static_cast<int>(p.length());

    for (int i = 0; i <= n - m; ++i)
    {
        bool match = true;
        for (int j = 0; j < m; ++j)
        {
            if (t[i + j] != p[j])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            matches.push_back(i + 1); // Add 1 to convert from 0-based to 1-based index
        }
    }
    // If no matches were found, return an empty vector
    return matches;
}

// d is the number of characters in the input alphabet
#define d 256

std::vector<int> rabin_karp_search(const std::string &pat, const std::string &txt, int q)
{
    std::vector<int> found_indices;
    int m = pat.size();
    int n = txt.size();
    int i, j;
    int p = 0; // hash value for pattern
    int t = 0; // hash value for txt
    int h = 1;

    // The value of h would be "pow(d, m-1)%q"
    for (i = 0; i < m - 1; i++)
    {
        h = (h * d) % q;
    }

    // Calculate the hash value of pattern and first window of text
    for (i = 0; i < m; i++)
    {
        p = (d * p + pat[i]) % q;
        t = (d * t + txt[i]) % q;
    }

    // Slide the pattern over text one by one
    for (i = 0; i <= n - m; i++)
    {
        // Check the hash values of current window of text and pattern
        if (p == t)
        {
            // Check for characters one by one
            for (j = 0; j < m; j++)
            {
                if (txt[i + j] != pat[j])
                    break;
            }
            // if p == t and pat[0...m-1] = txt[i, i+1, ...i+m-1]
            if (j == m)
            {
                found_indices.push_back(i + 1); // Collect the index where the pattern is found
            }
        }

        // Calculate hash value for next window of text: Remove leading digit, add trailing digit
        if (i < n - m)
        {
            t = (d * (t - txt[i] * h) + txt[i + m]) % q;
            // Convert negative value of t to positive
            if (t < 0)
                t = (t + q);
        }
    }
    return found_indices;
}

std::string read_file(const std::string &filename)
{
    std::ifstream ifs(filename);
    if (!ifs)
    {
        throw std::runtime_error("Could not open file: " + filename);
    }
    return std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
}

std::string read_file(const std::string &filename);

int main(int argc, char *argv[])
{
    std::string text_file;
    std::string pattern_file;
    std::string method;

    // Parse command line arguments
    for (int i = 1; i < argc; i += 2)
    {
        std::string arg = argv[i];
        if (arg == "-i")
        {
            text_file = argv[i + 1];
        }
        else if (arg == "-p")
        {
            pattern_file = argv[i + 1];
        }
        else if (arg == "-m")
        {
            method = argv[i + 1];
        }
    }

    // Read the text and patterns from the files
    std::string text = read_file(text_file);
    std::ifstream patterns_stream(pattern_file);
    std::string pattern;
    std::vector<std::vector<int> > all_pattern_indices;

    // Start the clock to measure the time taken
    std::clock_t start = std::clock();

    if (method == "BF")
    {
        while (std::getline(patterns_stream, pattern))
        {
            std::vector<int> matches = brute_force_string_match(text, pattern);
            all_pattern_indices.push_back(matches);
        }
    }
    else if (method == "RK")
    {
        const int q = 101; // A prime number for hashing
        while (std::getline(patterns_stream, pattern))
        {
            std::vector<int> matches = rabin_karp_search(pattern, text, q);
            all_pattern_indices.push_back(matches);
        }
    }
    else
    {
        std::cerr << "Unknown method: " << method << std::endl;
        return 1;
    }

    // Stop the clock and calculate the time taken
    std::clock_t finish = std::clock();
    double elapsed = static_cast<double>(finish - start) / static_cast<double> CLOCKS_PER_SEC;

    // Output the results
    for (size_t i = 0; i < all_pattern_indices.size(); ++i)
    {
        if (!all_pattern_indices[i].empty())
        {
            std::cout << "match[";
            for (size_t j = 0; j < all_pattern_indices[i].size(); ++j)
            {
                if (j > 0)
                    std::cout << ", ";
                std::cout << all_pattern_indices[i][j];
            }
            std::cout << "]" << std::endl;
        }
        else
        {
            std::cout << "match[0]" << std::endl;
        }
    }

    // Print the total wall-clock time taken
    std::cout << "time: " << elapsed << "s" << std::endl;

    return 0;
}