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
#include <memory>
#include <bits/stdc++.h>
#include <thread>
#include <fstream>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <pthread.h>
#include <time.h>
using namespace std; 
std::string line;
std::vector<std::vector<int>> ES;
int V;
int E;

class Graph
{

private:
    std::unordered_map<int, std::unordered_set<int>> adjacencyList;

public:
    std::unordered_set<int> & getChildren(int vertex);
    std::unordered_map<int, std::unordered_set<int>>& getList();
    void inE(std::vector< std::vector<int>> &ES, int src, int dest);
    void clearE(std::vector< std::vector<int>>& ES);
    void includeEdge(int vertex1, int vertex2);
    void includeArc(int tail, int head);
};

void Graph::includeEdge(int tailNode, int headNode)
{
    includeArc(tailNode, headNode);
    includeArc(headNode, tailNode);
}
void Graph::inE(std::vector< std::vector<int>> &ES, int src, int dest)
{
	ES[src].push_back(dest);
	ES[dest].push_back(src);
}

void Graph::clearE(std::vector< std::vector<int>>&ES) 
{
	for (unsigned i = 0; i < ES.size(); i++) {
		ES[i] = std::vector<int>();
	}
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
#define handle_error(txts) \
       do { perror(txts); exit(EXIT_FAILURE); } while (0)

#define handle_error_en(en, txts) \
       do { errno = en; perror(txts); exit(EXIT_FAILURE); } while (0)

//static void clk(char *txts, clockid_t C1)
//{
//    struct timespec tspc;
//    printf("%s", txts);
//    if (clock_gettime(C1, &tspc) == -1)
//        handle_error("clock_gettime");
//    printf("%4ld.%03ld\n", tspc.tv_sec, tspc.tv_nsec);
//    cout<<endl;
//}

void* vCov(void* args){	
	std::vector< std::vector<int>> ES1;
	ES1 = ES;
	
	int V1;
	V1 = V;

	int k = 0;
	int j;int i;int h;int f;int s1;
	std::vector<int> vCo;
	while(k <= V1){
		unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
		std::vector<std::vector<Minisat::Lit>> mtLit(V);
		for (i = 0; i < V1; i++){
			for(j = 0; j < k; j++){
				Minisat::Lit xi = Minisat::mkLit(solver->newVar());
				mtLit[i].push_back(xi);			
			}		
		}
		for(i = 0; i < k; i++){
			Minisat::vec<Minisat::Lit> Xi;
			for(j = 0;j < V1; j++){
				Xi.push(mtLit[j][i]);			
			}		
			solver->addClause(Xi);
		}
		for(h = 0; h < V1; h++){
			for(i = 0; i < k; i++){
				for(j = i+1;j < k; j++){
					solver->addClause(~mtLit[h][i],~mtLit[h][j]);
				}
			}		
		}
		for(h = 0; h < k; h++){
			for(i = 0; i < V1; i++){
				for(j = i+1; j < V1; j++){
					solver->addClause(~mtLit[i][h],~mtLit[j][h]);
				}
			}		
		}
		for(s1 = 0; s1 < V1; ++s1){
			for(auto s2:ES1[s1]){
				Minisat::vec<Minisat::Lit> A;
				if(s2 < s1)
					continue;
				for(i = 0; i < k; i++){
					A.push(mtLit[s1][i]);
					A.push(mtLit[s2][i]);
				}
				solver->addClause(A);
			}
		}
		bool r = solver->solve();
		if(r == 1){
			for(i = 0; i < V1; i++){
				for(j = 0; j < k; j++){
					f = Minisat::toInt(solver->modelValue(mtLit[i][j]));
					if(f == 0){
						vCo.push_back(i);
					}
				}
			}
			break;
			
		}else{
			vCo.clear();
			solver.reset(new Minisat::Solver());
		}
		
		k= k+1;
	}
	cout<<"CNF-SAT-VC: ";
	sort(vCo.begin(), vCo.end());
	for(unsigned j = 0; j < vCo.size(); j++){
		std::cout << vCo[j];
		if(j+1 != vCo.size()){
			std::cout<<",";
		}	
	}
	
	std::cout<<std::endl;
	//std::cout<<"Optimal size"<<vCo.size()<<std::endl;
	//clockid_t C1;
	//unsigned int t;
	//t = pthread_getcpuclockid(pthread_self(), &C1);
	//if (t != 0)
	 	//handle_error_en(t, "pthread_getcpuclockid");
	 //clk("CNF-SAT-VC Time:", C1);
	 //cout<<endl;

	return NULL;
}

void* ApproxVC1(void* args){
	std::vector< std::vector<int>> ES1;
	ES1 = ES;
	
	std::vector<int> vCo1;
	unsigned int maxdeg;
	maxdeg = 0;
	int MVer;
	unsigned int count = 0;
	
	while (count < ES1.size()){
		
		count = 0;
		for(unsigned int i = 0; i<ES1.size() ; i++){
			if(ES1[i].size()>maxdeg){
				maxdeg = ES1[i].size();
				MVer = i;		
			}
		}
		
		for (unsigned int i = 0; i<ES1[MVer].size();i++){
			unsigned int p = ES1[MVer][i];
			for(unsigned int j=0;j<ES1[p].size();j++){
				if(ES1[p][j]==MVer){
					
					ES1[p].erase(ES1[p].begin()+j);
				}			
			}
		}
		ES1[MVer].clear();
		vCo1.push_back(MVer);
		for (unsigned int i =0; i<ES1.size();i++){
			if(ES1[i].size() == 0){
				count = count +1;					
			}
		}
		maxdeg = 0;
		

	}	
	std::cout<<"APPROX-VC-1: ";
	sort(vCo1.begin(), vCo1.end());
	for(unsigned int j = 0; j < vCo1.size(); j++){
		std::cout << vCo1[j];
		if(j+1 != vCo1.size()){
			std::cout<<",";
		}	
	}
	std::cout<<std::endl;
	//std::cout<<"A1 size"<<vCo1.size()<<std::endl;
 	//clockid_t C1;
	//unsigned int t;
        //t = pthread_getcpuclockid(pthread_self(), &C1);
	//if (t != 0)
	//handle_error_en(t, "pthread_getcpuclockid");
	//clk("Approx VC 1 Time:", C1);
	//cout<<endl;
	return NULL;			
}

void* ApproxVC2(void* args){
	std::vector< std::vector<int>> ES2;
	ES2 = ES;
	
	std::vector<int> vCo2;
	
	unsigned int count = 0;
	int v;
	int u;
	int flag;
	while (count < ES2.size()){
		
		count = 0;
		flag = 0;
		for(unsigned int i = 0; i < ES2.size(); i++){
			for(unsigned int j = 0; j < ES2[i].size(); j++){
				u = i;
				v = ES2[i][j];
				flag = 1;
				break;
			}
			if(flag == 1)
				break;
		}		
		//std::cout<<"u"<<u<<endl;
		//std::cout<<"v"<<v<<endl;

		for(unsigned int i = 0; i < ES2.size(); i++){
			for(unsigned int j = 0; j < ES2[i].size(); j++){
				if(ES2[i][j] == u || ES2[i][j] == v ){
					ES2[i].erase(ES2[i].begin()+j);	
					j--;			
				}
			}
		}
		


		ES2[u].clear();
		ES2[v].clear();

		vCo2.push_back(u);
		vCo2.push_back(v);

		for (unsigned int i =0; i<ES2.size();i++){
			if(ES2[i].size() == 0){
				count = count +1;					
			}
		}
		

	}	
	std::cout<<"APPROX-VC-2: ";
	sort(vCo2.begin(), vCo2.end());
	for(unsigned int j = 0; j < vCo2.size(); j++){
		std::cout << vCo2[j];
		if(j+1 != vCo2.size()){
			std::cout<<",";
		}	
	}
	std::cout<<std::endl;
	//std::cout<<"A-2 size"<<vCo2.size()<<std::endl;

 	//clockid_t C1;
	//unsigned int t;
        //t = pthread_getcpuclockid(pthread_self(), &C1);
	//if (t != 0)
		//handle_error_en(t, "pthread_getcpuclockid");
	//clk("Approx2 VC Time:", C1);
	//cout<<endl;
	return NULL;						
}





void* parsing_func(void* args)
{ 
    Graph graph;
        int bbCount = 0;
        int beCount = 0;
        int sCount = 0;
        int bceCount = 0;
        int bcsCount = 0;       
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
    		    std::cerr<<"Error: special char not allowed\n"<< std::endl;
    		    return NULL;
    	}
        //std::cout<<"line input" << line<<endl;
        if(cstr[0]=='V'){
		//std::cout<<"in V"<< endl;
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
		    graph.clearE(ES);
                    std::unordered_map<int, std::unordered_set<int>> &adjList = graph.getList();
                    adjList.clear();
		    graph.clearE(ES);
                }
                
            }	
        }
        
    	else if(cstr[0]=='E'){
	     
	    for (int i = 0; i < V; i++) {
					ES.push_back(std::vector<int>());
				}

    	    E = 1;
    	    if(V==0){
    	        std::cerr<<"Error: V not entered\n"<< std::endl;
    	        E = 0;
    	        return NULL;
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
    		    std::cerr<<"Error: Braces error\n"<< std::endl;
    		    return NULL;
    		}
    		
    		if(bcsCount==0 && bceCount==0){
    		    std::cerr<<"Error: No braces present\n"<< std::endl;
       		    return NULL;
    		}
    		
    		if(bbCount!=beCount){
    		    std::cerr<<"Error: Braces error\n"<< std::endl;
    		    return NULL;
    		}
    		
    		if(bbCount==0&&beCount==0){
    		    std::cerr<<"Error: No braces present\n"<< std::endl;
    		    return NULL;
    		}
    	    
    	    std::istringstream input(cstr);
    	    
    	    while (!input.eof()) {
    	        char brac;
    	        char separator;
    	        input >> brac;
    	        if (input.fail()) {
                    break;
                }
                if(brac=='<'){
                    int a;
                    input >> a;
                    if (input.fail()) {
                    	std::cerr << "Error: Invalid Input\n"<< std::endl;
                        break;
                    }
                    
                    if (input.eof()) break;
                    input >> separator;

                    if (input.fail() || separator != ',') {
                    	std::cerr << "Error: Invalid Input \n"<< std::endl;
                    	break;
                    }
                    int b;
                    input>>b;
                    if (input.fail()) {
                    	std::cerr << "Error: Invalid Input\n"<< std::endl;
                    	break;
                    
                    }
                    
                    if (input.eof()) break;
                    if(a>V-1 || b>V-1){
                        std::cerr<<"Error: Invalid vertice number\n"<< std::endl;
                        E = 0;
                        break;
                    }
                    else{           
                        graph.includeEdge(a, b);
			graph.inE(ES, a, b);
                        continue;
                    }
                }    

    	    }
	
	if (E==1){
		pthread_t t2,t3,t4;
		pthread_create(&t2,NULL,&vCov,NULL);
		pthread_join(t2,NULL);
		
		pthread_create(&t3,NULL,&ApproxVC1,NULL);
		pthread_join(t3,NULL);

		pthread_create(&t4,NULL,&ApproxVC2,NULL);
                pthread_join(t4,NULL);
	}
    	}
    	else if(cstr[0]=='s'){
    	    if(V==0){
    	        std::cerr<<"Error: No vertice\n"<< std::endl;
		return NULL;
    	    }
    	    if(E==0){
    	        std::cerr<<"Error: No edges\n"<< std::endl;
		return NULL;
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
        	    std::cerr<<"Error: Invalid Input\n"<< std::endl;
		    return NULL;
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
                            std::cerr<<"Error: Invalid Input\n"<< std::endl;
                            break;
                        }
                        std::unordered_map<int, int*> shortestPath = makeTree(graph, a);
                        std::vector<int> path = findPath(b, shortestPath);
                
                int pCount = 0;
                for (std::vector<int>::const_iterator i = path.begin(); i != path.end(); ++i){
                    pCount = pCount+1;
                }
                      
                 
                if(pCount==1){
                    std::cerr<<"Error: No path exist\n"<< std::endl;
                    break;
                }        
                for (std::vector<int>::const_iterator i = path.begin(); i != path.end(); ++i){
                        if(i==path.end()-1){
                            std::cout<<*i;
                        }
                        else std::cout << *i << '-';
                }
                std::cout << std::endl;
                break;
                }
                    
            }	
        }
        else if(std::cin.eof()){
            return 0;
        }
    	else{
    	    std::cerr<<"Error: Invalid command"<<"\n";
    	    return NULL;
    	}
    return NULL;
}

int main(int argc, char** argv)
{

    while(std::getline(cin,line))
    {
    		pthread_t t1;
    		pthread_create(&t1,NULL,&parsing_func,NULL);
    		pthread_join(t1,NULL);
       		 

     }

    return 0;
}
