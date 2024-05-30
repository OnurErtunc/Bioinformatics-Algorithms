#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>

class Node
{
public:
    Node(const std::string &text, int position);

    // Accessors
    const std::string &getText() const;
    int getPosition() const;
    const std::vector<std::shared_ptr<Node>> &getChildren() const;

    // Mutators
    void setText(const std::string &text);
    void setPosition(int position);
    void addChild(const std::shared_ptr<Node> &child);
    void clearChildren();

    // Tree-related functionalities
    std::string printTree(const std::string &depthIndicator = "") const;

private:
    std::string text;                            // Portion of the string represented by this node
    std::vector<std::shared_ptr<Node>> children; // Children in the suffix tree
    int position;                                // The position in the original string where this substring starts
};

#endif // NODE_H
