#include <iostream>
#include <cstring>
#include "Image.hpp"

int main() {
    const Image c_img;
    const unsigned char* c_tmp = c_img.data();

    unsigned char* upload_1 = new unsigned char[100]; 
    std::memcpy(upload_1, "123123321321231231", std::strlen("123123321321231231"));

    unsigned char* upload_2 = new unsigned char[18];
    std::memcpy(upload_2, (unsigned char*)("123123321321231231"), 18);

    unsigned char* upload_3 = new unsigned char[6];
    std::memcpy(upload_3, "123456", 6);

    int size = std::strlen("123123321321231231");
    unsigned char* upload_4 = new unsigned char[size];
    std::memcpy(upload_4, "123123321321231231", size);

    unsigned char* upload_5 = new unsigned char[18];
    for (unsigned char i = 0; i<18; i++) {
        upload_5[i] = i;
        
    }
    //Image img{3, 2, 3, upload_1};
    //Image img{3, 2, 3, upload_2};
    //Image img{3, 2, 1, upload_3};
    Image img{3, 2, 3, upload_4};
    //Image img{3, 2, 3, upload_5};
    std::cout<<img.countRef()<<std::endl;


    /*
    std::cout<<img.data()<<"opp"<<std::endl;  
    std::cout<<img.col(0).data()<<std::endl;  
    std::cout<<img.col(1).data()<<std::endl;  
    std::cout<<img.row(0).data()<<std::endl; 
    std::cout<<img.row(1).data()<<std::endl; 
    std::cout<<img.row(2).data()<<std::endl; 
    img.Rotate(270); 

    unsigned char* rotate_1 = new unsigned char[std::strlen("231321123231321123")];
    std::memcpy(rotate_1, "231321123231321123", std::strlen("231321123231321123"));
    std::cout<<std::memcmp(rotate_1, img.data(), std::strlen("231321123231321123"))<<std::endl;
    std::cout<<rotate_1<<std::endl;
    
    std::cout<<img.data()<<std::endl;  
    std::cout<<"cols:"<<std::endl;
    for (int i = 0; i < img.cols(); i++) {
        std::cout<<img.col(i).data()<<std::endl;
    }
    std::cout<<"rows:"<<std::endl;
    for (int i = 0; i < img.rows(); i++) {
        std::cout<<img.row(i).data()<<std::endl;
    }  
    */
    Image img2 = img.clone();
    std::cout<<"1 "<<img2.data()<<std::endl;
    std::cout<<"2 "<<img.data()<<std::endl;  
    img2.Rotate(90);
    std::cout<<"2 "<<img.data()<<std::endl; 
    std::cout<<"3 "<<img2.data()<<std::endl;

    img.release();
    img2.release();

    Image img_1c{3, 2, 1, upload_3};

    std::cout<<"4 "<<img_1c.data()<<std::endl;
    img_1c.Mirror(MirrorType::Horizontal);
    std::cout<<"5 "<<img_1c.data()<<std::endl;

    std::cout<<"6 "<<img.countRef()<<std::endl;
    std::cout<<"7 "<<img2.countRef()<<std::endl;
    std::cout<<"8 "<<img_1c.countRef()<<std::endl;

    Image img_cl = img.clone();
    std::cout<<"9 "<<img.countRef()<<std::endl;
    
    
    std::cout<<"10 "<<"img del"<<std::endl;
    img.release();
    std::cout<<"11 "<<"img_1c del"<<std::endl;
    img_1c.release();
    std::cout<<"12 "<<"img2 del"<<std::endl;
    img2.release();
    std::cout<<"end"<<std::endl;/*
*/
    
    //desructors are mass called by the end of the main and something breaks here
    //fixed

    return 0;
}





//very old rant, everything was fixed

// i begin to think that it should be done with 2d array of m_channels length 1d arrays
// using special methods to transform it into 1d array and the other way around
// because the hell is the problem with typing 123123 and getting it stored as 123456?
// literally the output of

//unsigned char* upload_2 = new unsigned char[18];
//memcpy(upload_2, (unsigned char*)("123123321321231231"), 18);
//Image img{3, 2, 3, upload_2};
//std::cout<<img.data()<<std::endl;

// is

//123456321321231231¤¤¤¤r

// it is beyond me how this happens
// tested with upload_4, problem seems to be in cast