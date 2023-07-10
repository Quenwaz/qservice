#ifndef _h_utils_included__
#define _h_utils_included__

namespace utils{


template <typename ClassType,  void(ClassType::*)(void*), typename fnMember = void(ClassType::*)(void*)>
void * thread_rounter(void * param)
{
    ClassType *p=(ClassType*)param;
    // std::invoke(fnMember,p,nullptr);
    // p->fnMember();
    return NULL;
}
}


#endif // _h_utils_included__