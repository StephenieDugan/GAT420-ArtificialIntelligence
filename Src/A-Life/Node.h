#include <iostream>

using namespace std;

// Enum defining different types of predicates (Less, Greater, Equal, Not_Equal)
enum ePredicate
{
    Less,
    Greater,
    Equal,
    Not_Equal
};

// Function to convert enum predicate to string representation
inline string getString(ePredicate predicate)
{
    switch (predicate)
    {
    case ePredicate::Less: return "<";
    case ePredicate::Greater: return ">";
    case ePredicate::Equal: return "==";
    case ePredicate::Not_Equal: return "!=";
    default: return "";
    }
}

// Structure for a node in the decision tree
struct node_t
{
    // Constructor
    node_t(const string& info) : info(info) {}

    virtual node_t* execute() = 0; // Make pure virtual

    string info;
    node_t* trueNode{ nullptr };    // Pointer to true branch
    node_t* falseNode{ nullptr };   // Pointer to false branch
};

template <typename T>
struct decision_t : public node_t
{
    // Constructor
    decision_t(const string& info, T& parameter, ePredicate predicate, const T& value) :
        node_t{ info },
        parameter{ parameter },
        predicate{ predicate },
        value{ value }
    {}

    node_t* execute() 
    {
        bool result = false;
        switch (predicate)
        {
        case ePredicate::Less: result = parameter < value; break;
        case ePredicate::Greater: result = parameter > value; break;
        case ePredicate::Equal: result = parameter == value; break;
        case ePredicate::Not_Equal: result = parameter != value; break;
        }

        cout << "Decision: " << info << " : " << parameter << " " << getString(predicate) << " " << value << " (" << (result ? "true" : "false") << ")\n";

        // Return true or false branch node based on result
        return result ? trueNode : falseNode;
    }

    T& parameter; // Reference to the parameter used for comparison
    T value;      // Value for comparison
    ePredicate predicate; // Predicate used for comparison
};

struct action_t : public node_t
{
    // Constructor
    action_t(const string& info) : node_t{ info } {}

    virtual node_t* execute() override
    {
        cout << "Action: " << info << endl;

        // Action node is a leaf node (no children)
        return nullptr;
    }
};

// Traverse root using recursion
void traverse(node_t* node)
{
    node_t* nextNode = node->execute();
    if (nextNode)
    {
        // Call traverse with the next node, uses recursion to descend tree until the leaf (action)
        traverse(nextNode);
    }
}