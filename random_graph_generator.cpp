#include <unistd.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;
namespace stripe
{
    template < typename L > std::string inString( const L &n )
    {
        std::ostringstream s ;
        s << n ;
        return s.str();
    }
}

int m_xy( int c1, int c2, int m_flag)
{

    int m;
    
    if (m_flag == 1){
	    if ( c1 > c2)
	    {
		m = c1;
	    }else{
	    	m = c2;
	    }
    }else{ 
    	    if ( c1 > c2)
	    {
		m = c1;

	    }else{
	    	m = c1;
	    }
    }
    return m;
}



unsigned int waitNumFunction (unsigned int c)
{
    
    std::ifstream urandom("/dev/urandom");

    
    if (urandom.fail()) {
        std::cerr << "Error: cant access /dev/urandom\n";
        return 1;
    }
    unsigned int time = 3;
    urandom.read((char*)&time, sizeof(unsigned int));
    time = time % (c-5+1) + 5;
    urandom.close();
    return (unsigned int) time;


}

unsigned int stNumFunction ( unsigned int c )
{
    std::ifstream urandom("/dev/urandom");
    if (urandom.fail()) {
        std::cerr << "Error: cant access /dev/urandom\n";
        return 1;
    }
    unsigned int random = 3;
    urandom.read((char*)&random, sizeof(unsigned int));
    random = random % (c-2+1) +2;
    urandom.close();
    return (unsigned int) random;

}




unsigned int lineSegmentNumFunction(unsigned int c)
{
    
    std::ifstream urandom("/dev/urandom");

    
    if (urandom.fail()) {
        std::cerr << "Error: cant access /dev/urandom\n";
        return 1;
    }
    unsigned int random = 3;
    urandom.read((char*)&random, sizeof(unsigned int));
    random = random % (c-1+1) + 1;
    urandom.close();
    return (unsigned int) random;


}




void createSt( vector<string> &st , unsigned int st_num)
{
    string n = "s";
    for (unsigned int i=0; i < st_num; i++)
    {
        st.push_back(n);
        n = n +'s';
    }

}

int crdValue (unsigned int c)
{
    
    std::ifstream urandom("/dev/urandom");

    
    if (urandom.fail()) {
        std::cerr << "Error: cant access /dev/urandom\n";
        return 1;
    }
    unsigned int random = 3;
    urandom.read((char*)&random, sizeof(int));
    random = random % (c+c);
    int x = random - c ;
    urandom.close();
    return x;


}



bool linesIntersectPoints( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    float xn;
    float xd;
    float xCoor;
    float yn;
    float yd;
    float yCoor;
    int min_x1, min_x2, max_x1, max_x2, min_y1, min_y2, max_y1, max_y2;

    yn = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4);
    yd = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    yCoor = yn / yd;

    xn = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4));
    xd = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));
    xCoor =  xn / xd;

    min_x1 = m_xy(x1,x2,0); 
    min_x2 = m_xy(x3,x4,0);
    min_y1 = m_xy(y1,y2,0);
    min_y2 = m_xy(y3,y4,0);
    max_x1 = m_xy(x1,x2,1);
    max_x2 = m_xy(x3,x4,1);
    max_y1 = m_xy(y1,y2,1);
    max_y2 = m_xy(y3,y4,1);

    if ((min_x1<= xCoor && xCoor <= max_x1) && (min_x2 <= xCoor && xCoor <= max_x2))
            if ((min_y1 <= yCoor && yCoor <= max_y1) && ( min_y2 <= yCoor && yCoor <= max_y2))
                    return true;
    return false;

}



bool addingAPoint( vector<int> c, int xt, int yt)
{
    double m1;
    double m2;
    int s = c.size();
    if ( c.empty())
    {
        return true;
    }if ( s == 2)
    {
        if (( xt == c[0]) && (yt == c[1]) )
        {

            return false;
        }

        return true;
    }
    else
    {
	 
        //checking for the duplicate co-Ordinates
        for (unsigned int i = 0; i < c.size(); i = i + 2){
            if (xt == c[i] && yt == c[i+1]){
                return false;
            }
        }

        if ( xt == c[s-2] && xt == c[s-4])
            return false;

        //finding the intersection points
        if ( (xt == c[s-2]) && (xt != c[s-4]))
        {
            if ( s == 4)
                return true;
            else
            {
                for ( int i = s-3; i >=2; i = i-2 )
                {
                    if ( linesIntersectPoints(xt,yt,c[s-2],c[s-1],c[i-1],c[i],c[i-3],c[i-2]))
                        return false;

                }

                return true;
            }
        }

        if ( (c[s-4] == c[s-2]) && (xt != c[s-2]))
        {
            if ( s == 4)
                return true;
            else
            {
                for ( int i = s-3; i >=0; i = i-2 )
                {
                    if ( linesIntersectPoints(xt,yt,c[s-2],c[s-1],c[i-1],c[i],c[i-3],c[i-2]))
                        return false;

                }

                return true;
            }
        }

        

        m1 = ((yt - c[s-1]) / (xt-c[s-2]));
        m2 = ((c[s-1]) - c[s-3]) / (c[s-2]-c[s-4]);

        if (m1 == m2)
            return false;

        else
        {

            if ( s == 4)
                return true;
            else
            {
                for ( int i = s-3; i >=0; i = i-2 )
                {
                    if ( linesIntersectPoints(xt,yt,c[s-2],c[s-1],c[i-1],c[i],c[i-3],c[i-2]))
                        return false;

                }

                return true;
            }

        }


    }

}





int main (int argc, char **argv) {

    std::string stValue, nv, lv, cv;
    unsigned int si = 10, ni = 5, li = 5, ci = 20;
    int c;
    
    int sNum, lNum, xCo, yCo;
    unsigned int wSed;

    vector<string> strt; 
    vector<int> strtLineNum;
    vector<int> crd; 

    opterr = 0;
    while ((c = getopt(argc, argv, "s:n:l:c:")) != -1) {
        switch (c) {
            case 'n':
                nv = optarg;
                ni = std::atoi(nv.c_str());
                if (ni < 1)
                {
                    cerr << "Error: the number is too small (after n)"<<endl;
                    exit(1);
                }

                break;
            
            case 'c':
                cv = optarg;
                ci = std::atoi(cv.c_str());
                if (ci < 1)
                {
                    cerr << "Error: the number is too small (after c)"<<endl;
                    exit(1);
                }

                break;
	    case 'l':
                lv = optarg;
                li = std::atoi(lv.c_str());
                if (li < 5)
                {
                    cerr << "Error: the number is too small (after l)"<<endl;
                    exit(1);
                }

                break;
	    case 's':
                stValue = optarg;
                si = std::atoi(stValue.c_str());
                if (si < 2)
                {
                    cerr << "Error: the number is too small (after s)"<<endl;
                    exit(1);
                }

                break;

            case '?':
                if (optopt == 's') {
                    std::cerr << "Error: option -" << optopt
                              << " requires an argument." << std::endl;
                    break;
                }
		else if (optopt == 'c') {
                    std::cerr << "Error: option -" << optopt
                              << " requires an argument." << std::endl;
                    break;
                }
                
                else if (optopt == 'l') {
                    std::cerr << "Error: option -" << optopt
                              << " requires an argument." << std::endl;
                    break;
                }
		else if (optopt == 'n') {
                    std::cerr << "Error: option -" << optopt
                              << " requires an argument." << std::endl;
                    break;
                }
                

                else
                    std::cerr << "Error: unknown option: " << optopt << std::endl;
                    break;

        }


    }

    while (true)
    {
	//Removing all the existing st having r command 
        if ( !strt.empty())
        {
            for(std::vector<string>::iterator it = strt.begin(); it != strt.end(); it++)
            {
                cout<<"r "<<'"'<<*it<<'"'<<endl;

            }

            strt.clear();
            strtLineNum.clear();

        }

	//generation of the number of st
        sNum = stNumFunction(si);
	//the st names generation
        createSt(strt, sNum);

	//generation of the line segments
        for (int j = 0; j < sNum; j++)
        {
            lNum = lineSegmentNumFunction(ni);
            strtLineNum.push_back(lNum);

        }

        int index = 0;

        for (unsigned int z = 0; z < strtLineNum.size(); z++)
        {
            crd.clear();     
            int num = 0;
            int count = 0;
            while ( num < strtLineNum[z]+1 )
            {
                xCo = crdValue(ci);
                yCo = crdValue(ci);
                if (addingAPoint(crd, xCo, yCo))
                {
                    crd.push_back(xCo);
                    crd.push_back(yCo);
                    num++;
                    count = 0;
                }

                else
                {

                    if ( count == 25)
                    {
                        cerr<<"Error: failed to generate valid input for 25 simultaneous attempts"<<endl;
                        exit(1);   
                    }

                    count++;
                }


            }


            
            string gen_a;
            gen_a = string("a \"") + strt[index] + "\" ";


            for (unsigned int z = 0; z < crd.size(); z = z+2)
            {
                string S1,S2;
                if (crd[z]<0){
                  S1 = "-"+stripe::inString(crd[z]*(-1));
                } else {
                  S1 = stripe::inString(crd[z]);
                }
                if (crd[z+1]<0){
                  S2 = "-"+stripe::inString(crd[z+1]*(-1));
                } else {
                  S2 = stripe::inString(crd[z+1]);
                }
                gen_a = gen_a + '('+ S1 + ','+ S2 +')';
            }

            cout<<gen_a<<endl;

            index++;
        }


            cout<<'g'<<endl;
            wSed = waitNumFunction(li);
            usleep(wSed*1000000);

        }

return 0;
}

