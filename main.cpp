#include "init.h"
//#include "key.h"
//#include "user.h"

using namespace std;

int main(int argc, char **argv) {  //todo 输入的参数依次为：key.txt image.txt pixel_x pixel_y
    string keyinput = argv[1], imageinput = argv[2];
    int sizex = atoi(argv[3]), sizey = atoi(argv[4]);
    int th_num = atoi(argv[5]);

    /*声明key agent*/
    KA ka(keyinput);

    /*用户对图像加密*/
    //USER user(ka, imageinput, sizex, sizey);
    userInit(ka, imageinput, sizex, sizey, th_num);
    auto stime = chrono::system_clock::now();
    //auto encryptImage = user.encryption();
    auto encryptimage = userEncryption();
    auto etime = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = etime - stime;
    cout << "USER对图像加密的时间是: " << elapsed_seconds.count() << "s\n";

    /*用户解密图像*/
    stime = chrono::system_clock::now();
    //user.decryption(encryptImage);
    userDecryption(encryptimage);
    etime = chrono::system_clock::now();
    elapsed_seconds = etime - stime;
    cout << "USER对图像解密的时间是: " << elapsed_seconds.count() << "s\n";

    //user.testendecry();
    //usertestendecry();


    /**/


    std::cout << "Hello, World!" << std::endl;
    return 0;
}