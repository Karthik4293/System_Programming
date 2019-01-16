#include<iostream>
#include<memory>
#include<string>
#include<vector>

using namespace std;

class smart_class{
      string str;
      int token;
public:
      smart_class(string s, int t): str(s), token(t) {}
      void smart_print(){
           cout << "current smart msg is " << str << endl;
      }
};

int main(){
   // shared_ptr<smart_class> p1(new smart_class("I'm smart"), [](smart_class* p){cout << "custom delete" << endl;});
    
    unique_ptr<smart_class> u(new smart_class("Unique smart", 20));

    u->smart_print();
    //u.release();
    // sets u to nullptr
    u.reset(new smart_class("Unique resmart", 4));
    if(!u) cout << "pointer released" << endl;
    else u->smart_print();
	
    shared_ptr<smart_class> p1 = make_shared<smart_class>("I'm smart", 10);
    shared_ptr<smart_class> p3 = p1;
    {
	   //Manual sharing is necessary even when using make_shared 
	    shared_ptr<smart_class> p2 = make_shared<smart_class>("I'm smart", 5);
           // shared_ptr<smart_class> p2 = p1;
	    p2 = move(p1);
	    cout << p2.use_count() << endl;
            p2->smart_print();
    }
    cout << p3.use_count() << endl;

    return 0;
}
