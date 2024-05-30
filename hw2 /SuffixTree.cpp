#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "Node.h"
#include <iostream>
#include <fstream>

class SuffixTree
{
private:
    static constexpr char WORD_TERMINATION = '$'; // Unique termination character
    static constexpr int POSITION_UNDEFINED = -1;
    std::shared_ptr<Node> root; // The root of the suffix tree
    std::string fullText;       // The full text for which the suffix tree is built

public:
    // Constructor to initialize the suffix tree with a text
    SuffixTree(const std::string &text) : fullText(text)
    {
        // Initialize the root of the suffix tree
        root = std::make_shared<Node>("", POSITION_UNDEFINED);

        // Add all suffixes to the suffix tree
        for (size_t i = 0; i < text.length(); ++i)
        {
            addSuffix(text.substr(i) + WORD_TERMINATION, i);
        }
    }

    // Add a new suffix starting at the given position to the suffix tree
    void SuffixTree::addSuffix(const std::string &suffix, int position)
    {
        std::shared_ptr<Node> currentNode = root;
        for (char ch : suffix)
        {
            bool found = false;
            for (auto &child : currentNode->getChildren())
            {
                if (child->getText().front() == ch)
                {
                    currentNode = child;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                // Child with the current character doesn't exist, add new node
                auto newNode = std::make_shared<Node>(std::string(1, ch), position);
                currentNode->addChild(newNode); // Use the addChild method
                break;                          // Suffix added, no further action needed
            }
        }
    }

    // Function to print the suffix tree for debugging purposes
    void printTree(const std::shared_ptr<Node> &node, const std::string &indent = "") const
    {
        if (!node)
            return;
        std::cout << indent << node->getText();
        if (node->getPosition() != POSITION_UNDEFINED)
        {
            std::cout << " [" << node->getPosition() << "]";
        }
        std::cout << std::endl;
        for (const auto &child : node->getChildren())
        {
            printTree(child, indent + "  ");
        }
    }

    // Public API to print the tree
    void printTree() const
    {
        printTree(root);
    }
};

std::string readFasta(const std::string &filePath)
{
    std::ifstream file(filePath);
    std::string line, content;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '>')
            continue; // Skip headers and empty lines
        content += line;
    }
    return content;
}

int main(int argc, char *argv[])
{
    std::string textFilePath;
    std::string patternFilePath;

    // Parse command-line arguments for text and pattern file paths
    for (int i = 1; i < argc; i += 2)
    {
        if (std::string(argv[i]) == "-t")
        {
            textFilePath = argv[i + 1];
        }
        else if (std::string(argv[i]) == "-p")
        {
            patternFilePath = argv[i + 1];
        }
    }

    if (textFilePath.empty() || patternFilePath.empty())
    {
        std::cerr << "Usage: " << argv[0] << " -t <text.fasta> -p <patterns.fasta>" << std::endl;
        return 1;
    }

    // Read the text and patterns from the specified FASTA files
    std::string text = readFasta(textFilePath);
    // Since the current SuffixTree implementation does not directly support pattern searches,
    // we are only reading and constructing the suffix tree for the text here.
    // Pattern reading and searching would require additional implementation.

    if (text.empty())
    {
        std::cerr << "Failed to read text from " << textFilePath << std::endl;
        return 1;
    }

    // Initialize the suffix tree with the text
    SuffixTree suffixTree(text);
    // Demonstrates suffix tree construction, no direct support for pattern searching in the provided code
    std::cout << "Suffix Tree for text: " << std::endl;
    suffixTree.printTree();

    // Placeholder for pattern searching logic
    // Actual pattern search functionality to be implemented within the SuffixTree class

    return 0;
}
