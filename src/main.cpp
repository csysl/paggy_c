#include "init.h"
//#include "key.h"
//#include "user.h"
#include "cs.h"

using namespace std;

int main(int argc, char **argv) {  //todo 输入的参数依次为：key.txt image.txt pixel_x pixel_y  thread_num
    string keyinput = argv[1], imageinput = argv[2];

    int sizex = stoi(argv[3], nullptr), sizey = stoi(argv[4], nullptr);
    int th_num = stoi(argv[5], nullptr);

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



    /*cs对用户图像二次加密*/
    CS cs(ka, sizex, sizey, th_num);
    cs.CS1encryptImage(encryptimage);



    /*用户解密图像*/
    stime = chrono::system_clock::now();
    //user.decryption(encryptImage);
    userDecryption(encryptimage);
    etime = chrono::system_clock::now();
    elapsed_seconds = etime - stime;
    cout << "USER对图像解密的时间是: " << elapsed_seconds.count() << "s\n";
    usertestendecry();

    /**/

    /*释放内存*/
    userErasememory();


    std::cout << "Hello, World!" << std::endl;
    return 0;
}