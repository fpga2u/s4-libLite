
#include "common/s4signalhandle.h"
#include "queue/sharedCharArray_ptr.h"
#include "common/s4logger.h"

using namespace S4;



int main(int argc, char** argv)
{
    signalhandler_t::HookupHandler();

    char* p;
    void* exscope;
    {
        {
            auto shared_array = make_sharedCharArray_ptr(64);
            exscope = &shared_array;

            p = shared_array->get();

            std::string s("test a sharedCharArray");
            memcpy(p, s.data(), s.size());
            p[s.size()] = 0;
            INFO("{}", p);

            unsigned int ref0 = shared_array->curRef();
            INFO("curRef:{}", ref0);

            shared_array->addRef();
            unsigned int ref1 = shared_array->curRef();
            INFO("after addRef:{}", ref1);
        }

        sharedCharArray_ptr after_scope = *(sharedCharArray_ptr*)exscope;
        unsigned int ref2 = (after_scope)->curRef();

        INFO("after_scope:{}", ref2);

        char* p2 = (after_scope)->get();

        INFO("{}", p2);
        after_scope->release();
        unsigned int ref3 = (after_scope)->curRef();
        INFO("after release:{}", ref3);
    }

	INFO("test done!");

	return 0;
}