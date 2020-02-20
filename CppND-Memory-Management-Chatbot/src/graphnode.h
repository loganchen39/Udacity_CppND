#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <string>
#include "chatbot.h"

#include <memory>


// forward declarations
class GraphEdge;

class GraphNode
{
private:
    //// STUDENT CODE
    ////

    // data handles (owned)
    // Ligang: edges to subsequent nodes
    // std::vector<GraphEdge *> _childEdges;  // edges to subsequent nodes
    std::vector<std::unique_ptr<GraphEdge>> _childEdges;  

    // data handles (not owned)
    // Ligang: edges to preceding nodes, use raw pointer as ref to unique_ptr;
    std::vector<GraphEdge *> _parentEdges;  
    
    // ChatBot *_chatBot;  // changed to unique_ptr for ownership declaration. 
    std::unique_ptr<ChatBot> _chatBot; 
    // std::unique_ptr<ChatBot> _uniquePtrChatBot;  // the insturctor says "replace", so not another instance.

    ////
    //// EOF STUDENT CODE

    // proprietary members
    int _id;
    std::vector<std::string> _answers;

public:
    // constructor / destructor
    GraphNode(int id);
    ~GraphNode();

    // getter / setter
    int GetID() { return _id; }
    int GetNumberOfChildEdges() { return _childEdges.size(); }
    GraphEdge *GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() { return _answers; }
    int GetNumberOfParents() { return _parentEdges.size(); }

    // proprietary functions
    void AddToken(std::string token); // add answers to list
    void AddEdgeToParentNode(GraphEdge *edge);
    // void AddEdgeToChildNode(GraphEdge *edge);
    void AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge);

    //// STUDENT CODE
    ////

    // void MoveChatbotHere(ChatBot *chatbot); 
    void MoveChatbotHere(std::unique_ptr<ChatBot> pChatbot);

    ////
    //// EOF STUDENT CODE

    void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */