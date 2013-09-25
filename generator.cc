#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <math.h>
using namespace std;


struct Pt{
    double x;
    double y;

    Pt(){}
    Pt(double x, double y):
        x(x), y(y)
    {}
};

struct Bezier{
        Pt st;
        Pt ctr1;
        Pt ctr2;
        Pt end;
	
	Bezier(){}
        Bezier(Pt st, Pt ctr1, Pt ctr2, Pt end):
            st(st), ctr1(ctr1), ctr2(ctr2), end(end)
        {}
};

template<class T>
void tokenize_vec(const string &s, vector<T> &v)
{
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tok_t;
    tok_t tok(s);
    for(tok_t::iterator i (tok.begin()); i != tok.end(); ++i){
        std::string f(*i);
        boost::trim(f);
        v.push_back(boost::lexical_cast<T>(f));
    }
}

Bezier connection(Bezier b1, Bezier b2)
{
    Pt ctr1(b1.end.x + (b1.end.x - b1.ctr2.x), b1.end.y + (b1.end.y - b1.ctr2.y));
    Pt ctr2(b2.st.x - (b2.ctr1.x - b2.st.x), b2.st.y - (b2.ctr1.y - b2.st.y));

    return Bezier(b1.end, ctr1, ctr2, b2.st);
}


void connect_curves(list<Bezier> &bezier_coordinates)
{
    list<Bezier>::iterator i;
    Bezier bp1;

    for( i = bezier_coordinates.begin() ; i != bezier_coordinates.end() ; i++) {
	bp1 = *i;
        if(i++ != bezier_coordinates.end()){
            bezier_coordinates.insert(i, connection(bp1, *i));
        }
    }
}

Pt bezier_eqn(double t, Bezier bp)
{
    double x, y;
    x = pow((1-t), 3) * bp.st.x + 3 * pow((1-t), 2) * t * bp.ctr1.x
        + 3 * pow(t, 2) * (1-t) * bp.ctr2.x + pow(t, 3) * bp.end.x;

    y = pow((1-t), 3) * bp.st.y + 3 * pow((1-t), 2) * t * bp.ctr1.y
        + 3 * pow(t, 2) * (1-t) * bp.ctr2.y + pow(t, 3) * bp.end.y;
    
    return(Pt(x,y));
}

void bezier_gen(Bezier bp, list<Pt> &coordinates_list, int steps)
{
    double stsize = (1/(double)steps);
    double t;
    int i = 0;
    for(i = 0; i<20; i++){
        t = stsize * i;
       	coordinates_list.push_back(bezier_eqn(t, bp));
    }
}


void bezier_insert_coord(vector<double> vec_cord, list<Bezier>& bezier_coordinates)
{
    Pt start(vec_cord[0], vec_cord[1]);
    Pt control1(vec_cord[2], vec_cord[3]);
    Pt control2(vec_cord[4], vec_cord[5]);
    Pt end(vec_cord[6], vec_cord[7]);

    bezier_coordinates.push_back(Bezier(start, control1, control2, end));
}

void read_file(const char *file_name, list<Bezier> &bezier_coordinates)
{
    ifstream file;
    string str;
    vector<double> vec_cord;

    file.open(file_name);
    while(getline(file, str)){
        vector<double> vec_cord;
        tokenize_vec<double>(str, vec_cord);
        bezier_insert_coord(vec_cord, bezier_coordinates);
    }
    file.close();
}




int main (int argc, char *argv[])
{
    //Input could be from a file or from the console
    char *file_name;
    int steps = 20;

    if(argc >= 2) {
       strcpy(argv[1], file_name);
    }
   
    list <Bezier> bezier_coordinates;
    list <Pt> coordinates_list;
    //read file
    read_file("points.txt", bezier_coordinates);
    connect_curves(bezier_coordinates);
    list<Bezier>::iterator l;
    for(l = bezier_coordinates.begin(); l != bezier_coordinates.end(); ++l){
	//cout<<l->st.x<<","<<l->st.y<<","<<l->ctr1.x<<","<<l->ctr1.y<<","<<l->ctr2.x<<","<<l->ctr2.y<<","<<l->end.x<<","<<l->end.y<<endl;
        bezier_gen(*l, coordinates_list, steps);
    }
    
    list<Pt>::iterator i;
    for(i = coordinates_list.begin(); i != coordinates_list.end(); i++){
	cout<<i->x<<","<<i->y<<endl;
    }
    return 0;
}


