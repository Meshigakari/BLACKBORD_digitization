#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <opencv2/opencv.hpp>




class MyClass {
public:
    cv::Mat image;
    cv::Mat lab_image;
    int score;
    int rows;
    int cols;
    std::vector<std::vector<cv::Vec3b>> Values;
    double average[3];
    int dispersion;
    int nine_inout[14];
    int twenty_five_inout[14];

    // コンストラクタ
    MyClass(std::string str,int number) : image(cv::imread(str)), score(number) {
        if (image.empty()) {
            std::cerr << "Failed to read image file." << std::endl;
            exit(1);
        }
        
        cv::cvtColor(image, lab_image, cv::COLOR_BGR2Lab);
        rows = lab_image.rows;
        cols = lab_image.cols;
        
        // Valuesを初期化する
        Values.resize(rows, std::vector<cv::Vec3b>(cols));
        
        // 画像から値を取得してValuesに設定する
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                Values[y][x] = lab_image.at<cv::Vec3b>(y, x);
            }
        }
        dispersion  = 0;
        std::fill(std::begin(average), std::end(average), 0.0);
        std::fill(std::begin(nine_inout), std::end(nine_inout), 0);
        std::fill(std::begin(twenty_five_inout), std::end(twenty_five_inout), 0);
        
        std::vector<cv::Vec3b> dispersion_v;
        int count=0;
        // すべてのピクセルの値を合計する
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                if(Values[y][x][0])
                {
                    count++;
                    dispersion_v.push_back(Values[y][x]);
                    average[0] += Values[y][x][0]; // Lチャンネルの値を加算
                    average[1] += Values[y][x][1]; // aチャンネルの値を加算
                    average[2] += Values[y][x][2]; // bチャンネルの値を加算
                }
            }
        }

        // ピクセル数で割って平均を計算
        average[0] /= (count);
        average[1] /= (count);
        average[2] /= (count);
        
        std::cout << "averageL: " <<average[0] << " averageA: " << average[1] << "averageB: " << average[2] << std::endl;
        
        for(auto dispersion_vfor : dispersion_v)
        {
            dispersion += (pow((dispersion_vfor[0]-average[0]),2))+(pow((dispersion_vfor[1]-average[1]),2))+(pow((dispersion_vfor[2]-average[2]),2));
        }
        dispersion /= dispersion_v.size();
        std::cout <<dispersion<<std::endl;
        
        for (int y = 0; y < (rows-2); y++) {
            for (int x = 0; x < (cols-2); x++)
            {
                //nineboxにピクセル値を入れる
                std::vector<cv::Vec3b> ninebox;
                for(int i = 0;i<3;i++)
                {
                    for(int j = 0;j<3;j++)
                    {
                        if(Values[y+i][x+j][0])
                        {
                            ninebox.push_back(Values[y+i][x+j]);
                        }
                    }
                }
                //nine_Zに色差を入れる
                std::vector<int> nine_Z;
                for(size_t i=0;i<ninebox.size();i++)
                {
                    for (size_t j=i+1;j<ninebox.size();j++)
                    {
                        int squared = (pow((ninebox[i][0]-ninebox[j][0]),2))+(pow((ninebox[i][1]-ninebox[j][1]),2))+(pow((ninebox[i][2]-ninebox[j][2]),2));
                        nine_Z.push_back(squared);
                    }
                }
                int Max;
                if(!(nine_Z.empty()))
                {
                    auto max = std::max_element(nine_Z.begin(), nine_Z.end());
                    Max = static_cast<int>(*max); // 最大値を int にキャスト
                }
                if(Max > 10)   {nine_inout[0]++;}else{nine_inout[7]++;}
                if(Max > 50)   {nine_inout[1]++;}else{nine_inout[8]++;}
                if(Max > 100)  {nine_inout[2]++;}else{nine_inout[9]++;}
                if(Max > 500)  {nine_inout[3]++;}else{nine_inout[10]++;}
                if(Max > 1000) {nine_inout[4]++;}else{nine_inout[11]++;}
                if(Max > 5000) {nine_inout[5]++;}else{nine_inout[12]++;}
                if(Max > 10000){nine_inout[6]++;}else{nine_inout[13]++;}
            }
        }
        std::cout << "パイナップル  " <<nine_inout[0]<<" "<<nine_inout[7]<< std::endl;
        
        for (int y = 0; y < (rows-4); y++) {
            for (int x = 0; x < (cols-4); x++)
            {
                //nineboxにピクセル値を入れる
                std::vector<cv::Vec3b> ninebox;
                for(int i = 0;i<5;i++)
                {
                    for(int j = 0;j<5;j++)
                    {
                        if(Values[y+i][x+j][0])
                        {
                            ninebox.push_back(Values[y+i][x+j]);
                        }
                    }
                }
                //nine_Zに色差を入れる
                std::vector<int> nine_Z;
                for(size_t i=0;i<ninebox.size();i++)
                {
                    for (size_t j=i+1;j<ninebox.size();j++)
                    {
                        int squared = (pow((ninebox[i][0]-ninebox[j][0]),2))+(pow((ninebox[i][1]-ninebox[j][1]),2))+(pow((ninebox[i][2]-ninebox[j][2]),2));
                        nine_Z.push_back(squared);
                    }
                }
                int Max;
                if(!(nine_Z.empty()))
                {
                    auto max = std::max_element(nine_Z.begin(), nine_Z.end());
                    Max = static_cast<int>(*max); // 最大値を int にキャスト
                }
                if(Max > 10)   {twenty_five_inout[0]++;}else{twenty_five_inout[7]++;}
                if(Max > 50)   {twenty_five_inout[1]++;}else{twenty_five_inout[8]++;}
                if(Max > 100)  {twenty_five_inout[2]++;}else{twenty_five_inout[9]++;}
                if(Max > 500)  {twenty_five_inout[3]++;}else{twenty_five_inout[10]++;}
                if(Max > 1000) {twenty_five_inout[4]++;}else{twenty_five_inout[11]++;}
                if(Max > 5000) {twenty_five_inout[5]++;}else{twenty_five_inout[12]++;}
                if(Max > 10000){twenty_five_inout[6]++;}else{twenty_five_inout[13]++;}
            }
        }
        std::cout << "桃  " << twenty_five_inout[0] <<" "<< twenty_five_inout[7] << std::endl;
         
    }
    
    
    
    int GETscore() const {
        return score;
    }
    
    int GETdispersion() const {
        return dispersion;
    }

    const double* GETaverage() const {
        return average;
    }

    const int* GETnine_inout() const {
        return nine_inout;
    }

    const int* GETtwenty_five_inout() const {
        return twenty_five_inout;
    }
    
    
};



int main()
{
    // 画像の読み込み
    std::vector<MyClass> BLACKBORD;
    //適切なファイル名と主観評価の値を書く
    BLACKBORD.emplace_back("ページ1.png",78);
    
    // 出力するCSVファイル名
    std::string filename = "output.csv";

    // CSVファイルを開く
    std::ofstream outputFile(filename);
    outputFile << "\xEF\xBB\xBF"; // UTF-8のBOM (Byte Order Mark) を書き込む
    // ヘッダーを書き込む
    outputFile << "主観評価,分散,平均L,平均A,平均B,9マス10以上,9マス50以上,9マス100以上,9マス500以上,9マス1000以上,9マス5000以上,9マス10000以上,9マス10以下,9マス50以下,9マス100以下,9マス500以下,9マス1000以下,9マス5000以下,9マス10000以下,25マス10以上,25マス50以上,25マス100以上,25マス500以上,25マス1000以上,25マス5000以上,25マス10000以上,25マス10以下,25マス50以下,25マス100以下,25マス500以下,25マス1000以下,25マス5000以下,25マス10000以下\n";
    for(auto blackbord:BLACKBORD)
    {
        MyClass* csv = &blackbord;
        int Score = csv -> GETscore();
        int Dispersion = csv -> GETdispersion();
        const double* Average = csv -> GETaverage();
        const int* Nine_inout = csv -> GETnine_inout();
        const int* Twenty_five_inout = csv -> GETtwenty_five_inout();
        
        outputFile << Score << "," << Dispersion << "," << Average[0] << "," << Average[1] << "," << Average[2] << "," << Nine_inout[0] << "," << Nine_inout[1] << "," << Nine_inout[2] << "," << Nine_inout[3] << "," << Nine_inout[4] << "," << Nine_inout[5] << "," << Nine_inout[6] << "," << Nine_inout[7] << "," << Nine_inout[8] << "," << Nine_inout[9] << "," << Nine_inout[10] << "," << Nine_inout[11] << "," << Nine_inout[12] << "," << Nine_inout[13] << "," << Twenty_five_inout[0] << "," << Twenty_five_inout[1] << "," << Twenty_five_inout[2] << "," << Twenty_five_inout[3] << "," << Twenty_five_inout[4] << "," << Twenty_five_inout[5] << "," << Twenty_five_inout[6] << "," << Twenty_five_inout[7] << "," << Twenty_five_inout[8] << "," << Twenty_five_inout[9] << "," << Twenty_five_inout[10] << "," << Twenty_five_inout[11] << "," << Twenty_five_inout[12] << "," << Twenty_five_inout[13] << "\n";
        
        std::cout
        << Score << ","
        << Dispersion << ","
        << Average[0] << ","
        << Average[1] << ","
        << Average[2] << ","
        << Nine_inout[0] << ","
        << Nine_inout[1] << ","
        << Nine_inout[2] << ","
        << Nine_inout[3] << ","
        << Nine_inout[4] << ","
        << Nine_inout[5] << ","
        << Nine_inout[6] << ","
        << Nine_inout[7] << ","
        << Nine_inout[8] << ","
        << Nine_inout[9] << ","
        << Nine_inout[10] << ","
        << Nine_inout[11] << ","
        << Nine_inout[12] << ","
        << Nine_inout[13] << ","
        << Twenty_five_inout[0] << ","
        << Twenty_five_inout[1] << ","
        << Twenty_five_inout[2] << ","
        << Twenty_five_inout[3] << ","
        << Twenty_five_inout[4] << ","
        << Twenty_five_inout[5] << ","
        << Twenty_five_inout[6] << ","
        << Twenty_five_inout[7] << ","
        << Twenty_five_inout[8] << ","
        << Twenty_five_inout[9] << ","
        << Twenty_five_inout[10] << ","
        << Twenty_five_inout[11] << ","
        << Twenty_five_inout[12] << ","
        << Twenty_five_inout[13] << "\n";
    }
    return 0;
}
