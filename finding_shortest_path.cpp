#include <iostream> 
#include<string>
#include<sstream>
#include<list>
#include<vector>
#include<cstring>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <iterator>
using namespace std; 

class Graph
{

private:
    std::unordered_map<int, std::unordered_set<int>> adjacencyList;

public:
    std::unordered_set<int> & getChildren(int vertex);
    std::unordered_map<int, std::unordered_set<int>>& getList();
    void includeEdge(int vertex1, int vertex2);
    void includeArc(int tail, int head);

};


void Graph::includeEdge(int tailNode, int headNode)
{
    includeArc(tailNode, headNode);
    includeArc(headNode, tailNode);
}

void Graph::includeArc(int tailNode, int headNode)
{
    adjacencyList[tailNode].insert(headNode);
}

std::unordered_set<int>& Graph::getChildren(int vertex)
{
    return adjacencyList[vertex];
    
}

std::unordered_map<int, int*>makeTree( Graph &graph, int originNode)
{
    std::queue<int> que;
    std::unordered_map<int, int*> ancestorMap = {{ originNode, nullptr }};
    que.push(originNode);
    while (!que.empty())
    {
        int presentNode = que.front();
        que.pop();
        int* ancestorNode = new int{presentNode};
        for (int descendantNode : graph.getChildren(presentNode))
        {
            if (ancestorMap.find(descendantNode) == ancestorMap.end())
            {
                ancestorMap[descendantNode] = ancestorNode;
                que.push(descendantNode);
            }
        }
    }

    return ancestorMap;
}

std::unordered_map<int, std::unordered_set<int>>& Graph::getList()
{
    return adjacencyList;
}

std::vector<int> findPath(int endNode,std::unordered_map<int, int*> &shortestPath)
{
    int presentNode = endNode;
    std::vector<int> path;
    int* nextNode = shortestPath[presentNode];
    while (true){
        path.push_back(presentNode);
        nextNode = shortestPath[presentNode];
        if (nextNode == nullptr)
        {
            break;
        }

        presentNode = *nextNode;
    } 
    std::reverse(path.begin(), path.end());
    return path;
}


int main() 
{ 
    Graph graph;
    int V=0;
    int E = 0;

    while (!std::cin.eof()) {
        int eCount = 0;
        int bbCount = 0;
        int beCount = 0;
        int sCount = 0;
        int bceCount = 0;
        int bcsCount = 0;
        int flag = 0;
       
    	std::string line;
    	std::getline(std::cin,line);
    	char cstr[line.size() + 1];
    	strcpy(cstr, line.c_str());	
    	
    	for (int i = 0; cstr[i]; i++){
    		if(cstr[i]=='?'|| cstr[i]=='[' || cstr[i]==']' || cstr[i]=='"'){
        			    sCount=sCount+1;
        	}
        			
        	else if(cstr[i]=='!'|| cstr[i]=='@'|| cstr[i]=='#'|| cstr[i]=='$'|| cstr[i]=='%'){
        			    sCount=sCount+1;
        	}
        	else if(cstr[i]=='^'|| cstr[i]=='&'|| cstr[i]=='*'|| cstr[i]=='('|| cstr[i]==')'){
        	     sCount=sCount+1;
        	}
    	}
    	if(sCount>0){
    		    std::cerr<<"Error: special char not allowed\n";
    		    continue;
    	}
        
        if(cstr[0]=='V'){
            int count = 0; 
            for (int i = 0; cstr[i]; i++)
        		if (cstr[i] != ' ') 
        			cstr[count++] = cstr[i];  
        	cstr[count] = '\0'; 
            std::istringstream input1(cstr);
            while (!input1.eof()) {
                char v;
                input1>>v;
                int n1;
                if(v=='V'){
                    input1>>n1;
                    V = n1;
                    std::unordered_map<int, std::unordered_set<int>> &adjList = graph.getList();
                    adjList.clear();
                }
                
            }	
        }
        
    	else if(cstr[0]=='E'){
    	    E = 1;
    	    if(V==0){
    	        std::cerr<<"Error: V not entered\n";
    	        E = 0;
    	        continue;
    	    }
    	    
    	    int count = 0; 
            for (int i = 0; cstr[i]; i++)
        		if (cstr[i] != ' ') 
        			cstr[count++] = cstr[i];  
        	cstr[count] = '\0';
    	    
    	    for (int i = 0; cstr[i]; i++){ 
    		    if (cstr[i] == '{') {
    			    bcsCount = bcsCount+1;
    		    }
    			else if(cstr[i]=='}'){
    			    bceCount = bceCount+1;
    			}
    			else if(cstr[i]=='<'){
    			    bbCount = bbCount+1;
    			    
    			}
    			else if(cstr[i]=='>'){
    			    beCount = beCount+1;
    			}
    	    }
    			
    		if(bcsCount!=bceCount){
    		    std::cerr<<"Error: Braces error\n";
    		    continue;
    		}
    		
    		if(bcsCount==0 && bceCount==0){
    		    std::cerr<<"Error: No braces present\n";
    		    continue;
    		}
    		
    		if(bbCount!=beCount){
    		    std::cerr<<"Error: Braces error\n";
    		    continue;
    		}
    		
    		if(bbCount==0&&beCount==0){
    		    std::cerr<<"Error: No braces present\n";
    		    continue;
    		}
    	    
    	    std::istringstream input(cstr);
    	    
    	    while (!input.eof()) {
    	        char brac;
    	        char separator;
    	        unsigned num;
    	        input >> brac;
    	        if (input.fail()) {
                    break;
                }
                if(brac=='<'){
                    int a;
                    input >> a;
                    if (input.fail()) {
                    std::cerr << "Error: Invalid Input\n";
                    break;
                    }
                    
                    if (input.eof()) break;
                    input >> separator;

                    if (input.fail() || separator != ',') {
                    std::cerr << "Error: Invalid Input \n";
                    break;
                    }
                    int b;
                    input>>b;
                    if (input.fail()) {
                    std::cerr << "Error: Invalid Input\n";
                    break;
                    
                    }
                    
                    if (input.eof()) break;
                    if(a>V-1 || b>V-1){
                        std::cerr<<"Error: Invalid vertice number\n";
                        E = 0;
                        break;
                    }
                    else{           
                        graph.includeEdge(a, b);
                        continue;
                    }
                }    

    	    }
    	}
    
    	else if(cstr[0]=='s'){
    	    if(V==0){
    	        std::cerr<<"Error: No vertice\n";
    	        continue;
    	    }
    	    if(E==0){
    	        std::cerr<<"Error: No edges\n";
    	        continue;
    	    }
    	    
    	    int cCount = 0;
    	    for(int i=line.size()-1; i >= 0; i-- ){
                if(line[i]==' '&& line[i]==line[i-1]){
                    line.erase( line.begin() + i );
                }
            }
        	
        	char cstr[line.size() + 1];
        	strcpy(cstr, line.c_str());	
            int count = 0; 
            
    	    for (int i = 0; cstr[i]; i++)
        		if (cstr[i] != ' ') 
        			cstr[count++] = cstr[i];
        		else{
        		    cstr[count++]=',';
        		}
        	cstr[count] = '\0';
        	for(int i=0; cstr[i];i++)
        	    if(cstr[i]==',')
        	        cCount= cCount +1;
        	
        	if (cCount != 2){
        	    std::cerr<<"Error: Invalid Input\n";
        	    continue;
        	}
    
            std::istringstream input2(cstr);
            while (!input2.eof()) {
                char s;
                input2>>s;
                int a;
                int b;
                if(s==','){
                        input2>>a;
                        input2>>s;
                        input2>>b;
                        if(a> (V-1) || b>(V-1) ){
                            std::cerr<<"Error: Invalid Input\n";
                            break;
                        }
                        std::unordered_map<int, int*> shortestPath = makeTree(graph, a);
                        std::vector<int> path = findPath(b, shortestPath);
                
                int pCount = 0;
                for (std::vector<int>::const_iterator i = path.begin(); i != path.end(); ++i){
                    pCount = pCount+1;
                }
                      
                 
                if(pCount==1){
                    std::cerr<<"Error: No path exist\n";
                    break;
                }        
                for (std::vector<int>::const_iterator i = path.begin(); i != path.end(); ++i){
                        if(i==path.end()-1){
                            std::cout<<*i;
                        }
                        else std::cout << *i << '-';
                }
                std::cout << "\n";
                        break;
                }
                    
            }	
        }
        else if(std::cin.eof()){
            return 0;
        }
    	else{
    	    std::cerr<<"Error: Invalid command"<<"\n";
    	    continue;
    	}
    }
}