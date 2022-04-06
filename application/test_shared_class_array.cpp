#include <iostream>
#include "queue/shared_array.h"

using namespace S4;



class base_t
{
public:
    virtual ~base_t(){
        std::cout << "base destructed id=" << id << std::endl;
    }
    int id = -1;
};

class derive_t : public base_t
{
public:
    virtual ~derive_t(){
        std::cout << "derive destructed derive_id=" << derive_id << std::endl;
    }
    int derive_id = -1;
};




int main(int argc, char** argv)
{
    shared_class_array<base_t> array;
    array.init<derive_t>(2);
    base_t* base = array.get_base(0);
    base->id = 0;
    ((derive_t*)base)->derive_id = 11;

    derive_t* derive = (derive_t*)array.get_base(1);
	derive->derive_id = 22;
	derive->id = 1;

    return 0;
}