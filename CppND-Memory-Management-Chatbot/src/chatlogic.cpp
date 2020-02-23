#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"


ChatLogic::ChatLogic()
{
    //// STUDENT CODE
    ////
  
    std::cout << "ChatLogic constructor" << std::endl;

    // create instance of chatbot
    // _chatBot = new ChatBot("../images/chatbot.png");

    // add pointer to chatlogic so that chatbot answers can be passed on to the GUI
    // _chatBot->SetChatLogicHandle(this);  // for these 2 lines of code, keep it or not, both work;
  
    ////
    //// EOF STUDENT CODE
}

ChatLogic::~ChatLogic()
{
    //// STUDENT CODE
    ////
  
    std::cout << "ChatLogic destructor" << std::endl;  

    // delete chatbot instance
    // _chatBot is "new" allocated and here deleted with ChatLogic, how come it says "not owned"? 
    // if it's not owned by CHatLogic, owned by who?, by Node, see task 5.
    /** Ligang: see task 5, here _chatBot is just a ref, non-owning!
    if (_chatBot) {
        std::cout << "In ChatLogic::~ChatLogic(), _chatBot is not NULL." << std::endl;
        delete _chatBot;
    }
    */

  
    /** Ligang: changed to use unique_ptr which automatically deallocate memeory when _nodes go out of the scope. 
    // delete all nodes
    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        delete *it;
    }
    */

    /** Ligang: changed to use unique_ptr, the GraphEdge objects on heap memeory are owned by their 
                parent Node, not this ChatLogic object. The _edge vector itself is on stack, and is 
                owned by this ChatLogic object.
    // delete all edges
    for (auto it = std::begin(_edges); it != std::end(_edges); ++it)
    {
        delete *it;
    }
    */

    ////
    //// EOF STUDENT CODE
}

template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element)
{
    // find all occurences for current node
    auto token = tokens.begin();
    while (true)
    {
        token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) { return pair.first == tokenID;; });
        if (token != tokens.end())
        {
            element.AddToken(token->second); // add new keyword to edge
            token++;                         // increment iterator to next element
        }
        else
        {
            break; // quit infinite while-loop
        }
    }
}

void ChatLogic::LoadAnswerGraphFromFile(std::string filename)
{
    // load file with answer graph elements
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file)
    {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr))
        {
            // extract all tokens from current line
            tokenlist tokens;
            while (lineStr.size() > 0)
            {
                // extract next token
                int posTokenFront = lineStr.find("<");
                int posTokenBack = lineStr.find(">");
                if (posTokenFront < 0 || posTokenBack < 0)
                    break; // quit loop if no complete token has been found
                std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

                // extract token type and info
                int posTokenInfo = tokenStr.find(":");
                if (posTokenInfo != std::string::npos)
                {
                    std::string tokenType = tokenStr.substr(0, posTokenInfo);
                    std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

                    // add token to vector
                    tokens.push_back(std::make_pair(tokenType, tokenInfo));
                }

                // remove token from current line
                lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
            }

            // process tokens for current line
            auto type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "TYPE"; });
            if (type != tokens.end())
            {
                // check for id
                auto idToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "ID"; });
                if (idToken != tokens.end())
                {
                    // extract id from token
                    int id = std::stoi(idToken->second);

                    // node-based processing
                    if (type->second == "NODE")
                    {
                        //// STUDENT CODE
                        ////

                        // check if node with this ID exists already
                        // Ligang: 
                        // auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](GraphNode *node) { return node->GetID() == id; });
                        // if it's "std::unique_ptr<GraphNode> node", i.e. not reference, compile error! Can not pass-by-value for unique_ptr;
                        auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](std::unique_ptr<GraphNode>& node) { return node->GetID() == id; });

                        // create new element if ID does not yet exist
                        if (newNode == _nodes.end())
                        {
                            _nodes.emplace_back(new GraphNode(id));  // difference from push_back()?
                            newNode = _nodes.end() - 1; // get iterator to last element

                            // add all answers to current node
                            AddAllTokensToElement("ANSWER", tokens, **newNode);
                        }

                        ////
                        //// EOF STUDENT CODE
                    }

                    // edge-based processing
                    if (type->second == "EDGE")
                    {
                        //// STUDENT CODE
                        ////

                        // find tokens for incoming (parent) and outgoing (child) node
                        auto parentToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "PARENT"; });
                        auto childToken  = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "CHILD"; });

                        if (parentToken != tokens.end() && childToken != tokens.end())  // found
                        {
                            // get iterator on incoming and outgoing node via ID search
                            // auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](GraphNode *node) { return node->GetID() == std::stoi(parentToken->second); });
                            // auto childNode  = std::find_if(_nodes.begin(), _nodes.end(), [&childToken ](GraphNode *node) { return node->GetID() == std::stoi(childToken->second ); });
                            
                            // Ligang: Need to use ref for unique_ptr, not pass-by-value, can not copy, can use std::move semantics; 
                            auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](std::unique_ptr<GraphNode>& node) { return node->GetID() == std::stoi(parentToken->second); });
                            auto childNode  = std::find_if(_nodes.begin(), _nodes.end(), [&childToken ](std::unique_ptr<GraphNode>& node) { return node->GetID() == std::stoi(childToken->second ); });

                            // create new edge
                            /**
                            GraphEdge *edge = new GraphEdge(id);
                            edge->SetChildNode(*childNode);
                            edge->SetParentNode(*parentNode);
                            _edges.push_back(edge);
                            */
                          
                            std::unique_ptr<GraphEdge> edge(new GraphEdge(id));
                            edge->SetChildNode((*childNode).get());  // use raw pointer as non-owning ref.
                            edge->SetParentNode((*parentNode).get());
                            _edges.push_back(edge.get());

                          
                            // find all keywords for current node
                            AddAllTokensToElement("KEYWORD", tokens, *edge);

                            // store reference in child node and parent node
                            (*childNode)->AddEdgeToParentNode(edge.get());
                            (*parentNode)->AddEdgeToChildNode(std::move(edge));  // need to use move semantics for unique_ptr.
                            // (*parentNode)->AddEdgeToChildNode(edge);
                        }

                        ////
                        //// EOF STUDENT CODE
                    }
                }
                else
                {
                    std::cout << "Error: ID missing. Line is ignored!" << std::endl;
                }
            }
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }

    //// STUDENT CODE
    ////

    // identify root node
    GraphNode *rootNode = nullptr;
    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        // search for nodes which have no incoming edges
        if ((*it)->GetNumberOfParents() == 0)
        {

            if (rootNode == nullptr)
            {
                // rootNode = *it; // assign current node to root
              
                // Ligang: assign the raw pointer of current node to root, "*it" is an std::unique_ptr; 
                // rootNode = *it.get();  // compile error, need (). 
                rootNode = (*it).get();
            }
            else
            {
                std::cout << "ERROR : Multiple root nodes detected" << std::endl;
            }
        }
    }
  
    //Ligang: Task 5
    // std::unique_ptr<ChatBot> pChatBot(new ChatBot("../images/chatbot.png"));
    // _chatBot = pChatBot.get();  // in the video the insturctor says "replace" and remain, so not add a new instance!

    ChatBot chatBot("../images/chatbot.png");
    //if (_chatBot != nullptr)
    //    delete _chatBot; 
    _chatBot = &chatBot; 
    // add chatbot to graph root node
    _chatBot->SetChatLogicHandle(this);  //do not forget, copied from constructor.
    _chatBot->SetRootNode(rootNode);
   
    // Ligang: move constructor, not copy constructor to construct the ChatBot instance.
    rootNode->MoveChatbotHere(std::move(chatBot));  
    
    ////
    //// EOF STUDENT CODE
}


void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog)
{
    _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot)
{
    _chatBot = chatbot;
}

void ChatLogic::SendMessageToChatbot(std::string message)
{
    _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message)
{
    _panelDialog->PrintChatbotResponse(message);
}

wxBitmap *ChatLogic::GetImageFromChatbot()
{
    return _chatBot->GetImageHandle();
}
