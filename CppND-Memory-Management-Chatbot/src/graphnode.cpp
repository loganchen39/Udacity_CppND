#include <iostream>

#include "graphedge.h"
#include "graphnode.h"

GraphNode::GraphNode(int id)
{
    _id = id;
}

GraphNode::~GraphNode()
{
    //// STUDENT CODE
    ////

    // Ligang: _chatBot is owned by one certain node with unique_ptr for automatic deallocation.
    // delete _chatBot; 

    ////
    //// EOF STUDENT CODE
}

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
{
    _parentEdges.push_back(edge);
}

/** Ligang: 
void GraphNode::AddEdgeToChildNode(GraphEdge *edge)
{
    _childEdges.push_back(edge);
}
*/

void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
{
    // compile failed. push_back should be a COPY operator?! then need to use move!
    // _childEdges.push_back(edge);  
 
    _childEdges.push_back(std::move(edge));  // it works! 
}


//// STUDENT CODE
////
/** Ligang: 
void GraphNode::MoveChatbotHere(ChatBot *chatbot)
{
    _chatBot = chatbot;
    _chatBot->SetCurrentNode(this);
}
*/

/** 1st submission
void GraphNode::MoveChatbotHere(std::unique_ptr<ChatBot> chatbot)
{
    // transfer ownership, which means responsibility (for releasing memory and resources).
    _chatBot = std::move(chatbot);  
    _chatBot->SetCurrentNode(this);
}
*/

void GraphNode::MoveChatbotHere(ChatBot chatbot)
{
    // std::cout << "Entering function GraphNode::MoveChatbotHere() ..." << std::endl;
  
    // transfer ownership, which means responsibility (for releasing memory and resources).
    ChatBot tmp_chatbot = std::move(chatbot);
    _chatBot = std::move(tmp_chatbot);
    // _chatBot = std::move(chatbot);  // Ligang: ?
    // _chatBot = std::make_unique<ChatBot>(std::move(chatbot));  // Ligang: ?
  
    _chatBot.SetCurrentNode(this);
  
    // std::cout << "Leaving function GraphNode::MoveChatbotHere() ..." << std::endl; 
}

/** Ligang: 
void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(_chatBot);
    _chatBot = nullptr; // invalidate pointer at source
}
*/

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    // See task 5 video, transfer ownership / responsibility of ChatBot instance among nodes.
    // newNode->MoveChatbotHere(std::move(*_chatBot));
    newNode->MoveChatbotHere(_chatBot);
  
    //_chatBot = nullptr; // invalidate pointer at source, do not do like this.
}

////
//// EOF STUDENT CODE

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    //// STUDENT CODE
    ////

    // Ligang: 
    // return _childEdges[index];
    return _childEdges[index].get();

    ////
    //// EOF STUDENT CODE
}