#include <iostream>

/* 
        {
            // A = static_cast<float>(myMember)
            std::cout << " Base::foo " << std::endl;
        }
*/

class Base 
{
public:
    virtual void foo(double& A) = 0;
    virtual void foo(float& A) = 0;
    virtual void foo(int& A) = 0;
};


template<class T>
class Derived : public Base
{
public:
    T myMember;
        
    Derived(T aMember): myMember(aMember)
        {};

    virtual void foo(double& A)
        {
            A  = static_cast<double>(myMember);
            std::cout << " Derivedd::foo " << std::endl;
        }
    virtual void foo(float& A)
        {
            A  = static_cast<float>(myMember);
            std::cout << " Derivedf::foo " << std::endl;
        }
    virtual void foo(int& A)
        {
            A = static_cast<int>(myMember);
            std::cout << " Derivedi::foo " << std::endl;
        }
};


int main()
{
    

    Derived<int> bill(10);
    double oh = 2.0;
    std::cout <<" oh  : " <<  oh  << std::endl;

    bill.foo(oh);
    std::cout <<" oh  : " <<  oh  << std::endl;

    return 0;
}
