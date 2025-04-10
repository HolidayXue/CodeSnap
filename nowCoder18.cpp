// nowCoder18.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include <sstream>

#include <cmath>
#define _USE_MATH_DEFINES 


//1.只有输出
int firstTestfun()
{
    std::cout << "Hello Nowcoder!" << std::endl;
    return 0;
}
//2.单组_A+B
int a_add_b_Testfun()
{
    int a, b;
    while (std::cin >> a >> b) { // 注意 while 处理多个 case
        std::cout << a + b << std::endl;
    }
    return 0;
}

//3.多组_A + B_EOF形式
int multi_a_add_b_Testfun()
{
    int a, b;
    while (std::cin >> a >> b) { // 注意 while 处理多个 case
        std::cout << a + b << std::endl;
    }
    return 0;
}

//4.多组_A + B_T组形式
int fixed_a_add_b_testFun() {
    int a, b;
    int lines;
    std::cin >> lines;
    for (int i = 0; i < lines; i++) {
        std::cin >> a >> b;
        std::cout << a + b << std::endl;
    }

    return 0;
}

//5.多组_A + B_零尾模式
int zero_end_a_add_b_testFun() {
    int a, b;
    while (std::cin >> a >> b) { // 注意 while 处理多个 case
        if (a == 0 && b == 0) {
            break;
        }
        std::cout << a + b << std::endl;
    }

    return 0;
}

//6.单组_一维数组
int sumArray() {
    int a, b;
    long sum = 0;
    std::cin >> a;
    for (int i = 0; i < a; i++) {
        std::cin >> b;
        sum += b;
    }
    std::cout << sum << std::endl;
    return 0;
}

//7.多组_一维数组_T组形式
int multi_sumArray() {
    int a, b;
    int inputLines;
    int lines = 0;
    std::cin >> inputLines;
    while (lines < inputLines) { // 注意 while 处理多个 case
        std::cin >> a;
        long sum = 0;
        for (int i = 0; i < a; i++) {
            std::cin >> b;
            sum += b;
        }
        std::cout << sum << std::endl;
        lines++;
    }
    return 0;
}


//8.单组_二维数组
int two_dim_sum_array() {
    int a, b;
    std::cin >> a >> b;
    long sum = 0;
    int inputValue;
    for (int i = 0; i < a * b; i++) {
        std::cin >> inputValue;
        sum += inputValue;
    }

    std::cout << sum << std::endl;
    return 0;
}

//9.多组_二维数组_T组形式
int multi_two_dim_sum_array()
{
    int lines;
    std::cin >> lines;
    for (int j = 0; j < lines; j++)
    {
        int a, b;
        std::cin >> a >> b;
        long sum = 0;
        int inputValue;
        for (int i = 0; i < a * b; i++) {
            std::cin >> inputValue;
            sum += inputValue;
        }
        std::cout << sum << std::endl;
    }
    return 0;
}

//10.单组_字符串 倒置
int reverse_string() {
    int input_chars;
    std::cin >> input_chars;
    std::string a;
    std::cin >> a;
    for (int i = a.size() - 1; i >= 0; i--) {
        std::cout << a[i];
    }

    return 0;
}

//11.多组_字符串_T组形式 倒置
int multi_reverse_string() {
    int lines;
    std::cin >> lines;
    for (int j = 0; j < lines; j++) {
        int input_chars;
        std::cin >> input_chars;
        std::string a;
        std::cin >> a;
        for (int i = a.size() - 1; i >= 0; i--) {
            std::cout << a[i];
        }
        std::cout << std::endl;
    }
    return 0;
}

//12.单组_二维字符数组 倒置
int reverse_two_dims_string() {
    int lines, chars_count;
    std::cin >> lines;
    std::cin >> chars_count;
    std::vector<std::string> inputValues;
    inputValues.resize(lines);
    for (int j = 0; j < lines; j++) {
        std::cin >> inputValues[j];
    }

    for (int j = lines - 1; j >= 0; j--) {
        for (int i = inputValues[j].size() - 1; i >= 0; i--) {
            std::cout << inputValues[j][i];
        }
        std::cout << std::endl;
    }
    return 0;
}

//13.多组_带空格的字符串_T组形式

//字符串去空格
int trimAllSapace(std::string& input_str) {
    if (!input_str.empty()) {
        int index = 0;
        while ((index = input_str.find(' ', index)) != std::string::npos) {
            input_str.erase(index, 1);
        }
    }
    return 0;
}

//13.多组_带空格的字符串_T组形式
int reverse_multi_string_trimSpaces() {
    int lines, count;
    std::cin >> lines;

    std::vector<std::string> inputValues;
    inputValues.resize(lines);
    for (int j = 0; j < lines; j++) {
        int count;
        std::cin >> count;
        std::cin >> std::ws;
        std::getline(std::cin, inputValues[j]);
        trimAllSapace(inputValues[j]);
    }
    for (int j = 0; j < lines; j++) {
        for (int i = inputValues[j].length() - 1; i >= 0; i--) {
            std::cout << inputValues[j][i];
        }
        std::cout << std::endl;
    }
    return 0;
}

//14.单组_保留小数位数
int setPrecisionTest()
{
    double a;
    while (std::cin >> a) { // 注意 while 处理多个 case
        std::cout << std::fixed << std::setprecision(3) << a << std::endl;
    }
    return 0;
}

//15.单组_补充前导零 前面填充0
int fillzeroPre()
{
    int a;
    std::cin >> a;

    std::ostringstream oss;
    int width = 9; // 想要的总宽度

    oss << std::setw(width) << std::setfill('0') << a;
    std::string result = oss.str();
    std::cout << result << std::endl;
    return 0;
}

//16.单组_spj判断YES与NO
int use_if()
{
    int a;
    while (std::cin >> a) { // 注意 while 处理多个 case
        if (a % 2 == 1)
        {
            std::cout << "YES" << std::endl;
        }
        else
        {
            std::cout << "NO" << std::endl;
        }
    }
    return 0;
}

// 17.单组_spj判断浮点误差 计算圆形的面积
int testSetPrecision2()
{
    int a;
    double tmp_pi = M_PI;
    while (std::cin >> a) { // 注意 while 处理多个 case

        double result = tmp_pi * a * a;
        std::cout << std::fixed << std::setprecision(8) << result << std::endl;
    }
    return 0;
}

//18.单组_spj判断数组之和 构造符合要求的数组
int constructArray() {
    int a, b;
    while (std::cin >> a >> b) { // 注意 while 处理多个 case
        int count = a;
        for (int i = 1; i < count; i++) {
            int value = (b - i >= a) ? i : 1;
            b = b - value;
            std::cout << value << " ";
            a--;
        }
        std::cout << b << std::endl;
    }
    return 0;
}

int main()
{
    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
