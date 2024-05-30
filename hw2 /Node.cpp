#include "Node.h"
#include <iostream>

// Constructor
Node::Node(const std::string &text, int position)
    : text(text), position(position)
{
}

// Accessors
const std::string &Node::getText() const
{
    return text;
}

int Node::getPosition() const
{
    return position;
}

const std::vector<std::shared_ptr<Node>> &Node::getChildren() const
{
    return children;
}

// Mutators
void Node::setText(const std::string &newText)
{
    text = newText;
}

void Node::setPosition(int newPosition)
{
    position = newPosition;
}

void Node::addChild(const std::shared_ptr<Node> &child)
{
    children.push_back(child);
}

void Node::clearChildren()
{
    children.clear();
}

// Tree-related functionalities
std::string Node::printTree(const std::string &depthIndicator) const
{
    std::string str = depthIndicator + text + (position >= 0 ? "[" + std::to_string(position) + "]" : "") + "\n";
    for (const auto &child : children)
    {
        str += child->printTree(depthIndicator + "  ");
    }
    return str;
}
