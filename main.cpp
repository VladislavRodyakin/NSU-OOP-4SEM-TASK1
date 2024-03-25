#include <iostream>
#include "Image.hpp"

int main(){
    std::cout<<"helo"<<std::endl;
    const Image c_img;
    const unsigned char* c_tmp = c_img.data();
    unsigned char upload_1[]{"123123321321231231"};
    unsigned char upload_2[]{"321231231123123321"};
    Image img{3, 2, 3, upload_1};
    //std::cout<<img.col(0).data()<<std::endl; //doesn't work (no output)
    std::cout<<img.data()<<std::endl;
    if(img.rows()!= 3){
        std::cout<<"fail"<<std::endl;
    }
    std::cout<<img.rows()<<std::endl;
    std::cout<<img.cols()<<std::endl;
    std::cout<<img.total()<<std::endl; //works on 3,2, but doesn't on 2,3
    //added & deleted breakpoint, it works now - WHY???? HOW?????
    //if 2,3 changes to 3,2 and back, then total doesn't compute on 2,3 again. the hell is it's problem
    //sometimes it works, sometimes it doesn't
    //it's not consistent, i don't know what is happening
    //removed memcpy, everything stabilized... still no clue why this was happening 
    img.Rotate(90); //seems to not work...
    std::cout<<"rotated successfully"<<std::endl;
    std::cout<<img.data()<<std::endl;
    if(img.rows()!= 3){
        std::cout<<"fail"<<std::endl;
    }
    std::cout<<img.rows()<<std::endl;
    std::cout<<img.cols()<<std::endl;
    std::cout<<img.total()<<std::endl;
    return 0;
}